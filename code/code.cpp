#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <unordered_map>
using namespace std;

int numEstacoesTotal, numLigacoesTotal, numLinhasTotal;

struct Linha{
    int id; //id da linha
    set <int> listaEstacoes; //lista de estacoes que pertencem à linha
    vector<bool> vetorEstacoesLinha; //vetor de booleans para verificar se uma estacao ja esta ou nao na linha
    vector<Linha*> vetorLinhasLigadas; //vetor de todas as linhas com ligacoes a esta linha

    //Constructor
    Linha(int id_linha, int numEstacoes) 
        :id(id_linha), vetorEstacoesLinha(numEstacoes, false) {}
};

vector <Linha*> listaLinhas; //vetor global com todas as linhas criadas

//adiciona uma estacao à lista de estacoes de uma linha
void adicionarEstacaoLinha(Linha* linha, int estacao){
    linha->listaEstacoes.insert(estacao);
}

//cria uma linha e adiciona-a à lista global de linhas
Linha* criarLinha(int id_linha, int numEstacoesTotal){
    Linha* linhaNova = new Linha{id_linha,numEstacoesTotal};
    listaLinhas.push_back(linhaNova);
    return linhaNova;
}

//retorna a linha a partir do id
Linha* getLinha(int id_linha){
    for(Linha* linha : listaLinhas){
        if(linha->id==id_linha){
            return linha;
        }
    }
    return NULL;
}

//obtem os inputs
int readInput(){
    scanf("%d %d %d", &numEstacoesTotal, &numLigacoesTotal, &numLinhasTotal);
    vector<bool> linhasCriadas(numLinhasTotal, false); //vetor de bools para verificar se a linha ja foi ou nao criada
    vector<bool> estacoesCriadas(numEstacoesTotal, false); //vetor de bools para verificar se a linha ja foi ou nao criada
    int numEstacoesCriadas=0; //para verificar se todas têm ligação

    for(int i=0;i<numLigacoesTotal;i++){
        int id_estacaoX, id_estacaoY, id_linha;
        scanf("%d %d %d",&id_estacaoX, &id_estacaoY, &id_linha);
        if(id_estacaoX!=id_estacaoY){
            Linha* linha;
            if(!linhasCriadas[id_linha-1]){
                //ainda nao foi criada
                linha=criarLinha(id_linha,numEstacoesTotal); //cria uma nova linha
                linhasCriadas[id_linha-1]=true; //mete true no vetor de bools para nao ser criada novamente
            }else{
                //a linha ja tinha sido criada
                linha=getLinha(id_linha); //obtem a linha
            }

            if(!linha->vetorEstacoesLinha[id_estacaoX-1]){
                //a estacao X ainda nao tinha sido adicionada à linha
                adicionarEstacaoLinha(linha, id_estacaoX); //adiciona a estacao X à lista de estacoes da linha
                linha->vetorEstacoesLinha[id_estacaoX-1]=true;
            }
            if(!linha->vetorEstacoesLinha[id_estacaoY-1]){
                //a estacao Y ainda nao tinha sido adicionada à linha
                adicionarEstacaoLinha(linha, id_estacaoY); //adiciona a estacao Y à lista de estacoes da linha
                linha->vetorEstacoesLinha[id_estacaoY-1]=true;
            }

            if(!estacoesCriadas[id_estacaoX-1]){
                //a estacao X ainda nao tinha sido criada
                numEstacoesCriadas++;
                estacoesCriadas[id_estacaoX-1]=true;
            }
            if(!estacoesCriadas[id_estacaoY-1]){
                //a estacao Y ainda nao tinha sido criada
                numEstacoesCriadas++;
                estacoesCriadas[id_estacaoY-1]=true;
            }
        }
    }
    if(numEstacoesCriadas!=numEstacoesTotal){
        //há pelo menos uma estação sem ligação
        printf("%d\n",-1);
        return -1;
    }
    return 0;
}

//retorna as estacoes em comum da linha1 e linha2. NULL caso não haja
set<int> getEstacoesComum(Linha* linhaX, Linha* linhaY){
    set<int> estacoesComum;
    set_intersection(                                                   //percorre os dois sets e faz a intersecao
        linhaX->listaEstacoes.begin(), linhaX->listaEstacoes.end(),
        linhaY->listaEstacoes.begin(), linhaY->listaEstacoes.end(),
    inserter(estacoesComum, estacoesComum.begin()));
    return estacoesComum;
}

//calcula as estacoes em comum de todos os pares de linhas
void estacoesComum(){
    for(int linha=0;linha<numLinhasTotal;linha++){
        Linha* linhaX = listaLinhas[linha]; 
        for(int coluna=linha+1;coluna<numLinhasTotal;coluna++){
            Linha* linhaY = listaLinhas[coluna]; 
            set<int> estacoesComum = getEstacoesComum(linhaX,linhaY); //vai buscar as estacoes em comum
            if (!estacoesComum.empty() && estacoesComum!=linhaX->listaEstacoes && estacoesComum!=linhaY->listaEstacoes) {
                //verifica se têm estacoes em comum e se a linha esta inserida noutra linha (caso esteja, podemos ignorar)
                linhaX->vetorLinhasLigadas.push_back(linhaY); 
                linhaY->vetorLinhasLigadas.push_back(linhaX);
            }
        }
    }
}

//devolve a distancia maximo da linhaInicial até uma estacao qualquer
int calcula(Linha* linhaInicial){
    queue<Linha*> filaLinhas; //fila de linhas
    filaLinhas.push(linhaInicial); //adiciona a linha inicial à fila

    unordered_map<int, int> distancias; //lista das distancias linked com o id da linha
    distancias[linhaInicial->id] = 0; //distancia a si mesmo é zero
    int distanciaMaxima = 0;

    while (!filaLinhas.empty()) {
        Linha* atual = filaLinhas.front(); //vai buscar a primeira linha da fila
        filaLinhas.pop(); //tira a linha

        int distanciaAtual = distancias[atual->id]; //distancia do nó atual

        //percorre todas as linhas ligadas à linha atual
        for (Linha* vizinho : atual->vetorLinhasLigadas) {
            if (distancias.find(vizinho->id) == distancias.end()) {
                //ainda nao tinha sido visitado
                distancias[vizinho->id] = distanciaAtual + 1; //atualizar a distância na lista de distancias
                filaLinhas.push(vizinho); //adiciona à fila
                distanciaMaxima = max(distanciaMaxima, distancias[vizinho->id]); //atualiza a distância máxima
            }
        }
    }
    return distanciaMaxima;
}

int main(){
    if(readInput()==-1){
        //programa ja acabou
        return 0;
    }
    int mudancasLinhaMax = 0;
    estacoesComum(); //guarda as estacoes em comum de todos os pares de linhas

    for(int linha=0; linha<numLinhasTotal;linha++){
        int mudanca= calcula(listaLinhas[linha]); //mudanca=num maximo de mudancas de linha para a linha dada
        if(mudancasLinhaMax<mudanca){
            //nova distancia maxima
            mudancasLinhaMax=mudanca; 
        }
    }
    printf("%d\n",mudancasLinhaMax);
    return 0;
}