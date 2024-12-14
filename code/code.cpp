#include <vector>
#include <sstream>
using namespace std;

struct Ligacao; 

struct Estacao{
    int id;
    int numLinhas; //quantas linhas tem a estacao
    vector<Ligacao*> listaLigacoes; //vetor com todas as ligacoes desta estacao
    vector<int> listaLinhas; //vetor com todos os id's das linhas desta estacao
};

struct Ligacao{
    Estacao* estacaoX; 
    Estacao* estacaoY;
    int linha; //id da linha
};

int numEstacoesTotal, numLigacoesTotal, numLinhas;
vector <Estacao*> listaEstacoes;
vector <Ligacao*> listaLigacoes;

//adiciona a linha à estação caso esta ainda não pertencesse
void adicionarLinhaEstacao(Estacao* estacao, int id_linha){
    for(int id_linhaAtual : estacao->listaLinhas){
        if(id_linhaAtual==id_linha){
            return; //a estacao já tinha esta linha
        }
    }
    //a estacao ainda não tinha esta linha
    estacao->listaLinhas.push_back(id_linha);
    estacao->numLinhas++;
}

//adiciona uma nova ligacao à estação
void addLigacao(Estacao* estacao, Ligacao* novaLigacao){
    estacao->listaLigacoes.push_back(novaLigacao);
}

//cria uma nova ligacao
Ligacao* criarLigacao(Estacao* estacaoX, Estacao* estacaoY, int id_linha){
    Ligacao* novaLigacao = new Ligacao{estacaoX,estacaoY, id_linha};
    listaLigacoes.push_back(novaLigacao);
    addLigacao(estacaoX,novaLigacao); //adiciona a ligacao às ligacoes da estacao X
    addLigacao(estacaoY,novaLigacao); //adiciona a ligacao às liigacoes da estacao Y
    return novaLigacao;
}

//cria uma nova estacao e adiciona à lista de estacoes existentes
Estacao* criarEstacao(int id_novaEstacao, int id_linha){
    Estacao* novaEstacao = new Estacao{id_novaEstacao,1,{},{id_linha}};
    listaEstacoes.push_back(novaEstacao);
    return novaEstacao;
}

//retorna a estacao a partir do id
//retorna NULL caso a estacao ainda não existe
Estacao* getEstacao(int id_estacao){
    for(Estacao* estacaoAtual : listaEstacoes){
        if(estacaoAtual->id==id_estacao){
            return estacaoAtual;
        }
    }
    return NULL;
}

void getInput(){
    scanf("%d %d %d", &numEstacoesTotal, &numLigacoesTotal, &numLinhas);
    vector<bool> estacoesCriadas(numEstacoesTotal, false); //vetor de bools para verificar se a estacao ja foi ou nao criada 
    int numEstacoesCriadas=0; //para verificar se todas têm ligação

    for(int i=0;i<numLigacoesTotal;i++){
        int id_estacaoX, id_estacaoY, id_linha;
        scanf("%d %d %d",&id_estacaoX, &id_estacaoY, &id_linha);
        Estacao* estacaoX;
        Estacao* estacaoY;
        if(!estacoesCriadas[id_estacaoX-1]){
            //estacao X ainda não foi criada
            estacaoX = criarEstacao(id_estacaoX,id_linha);
            estacoesCriadas[id_estacaoX-1]=true;
            numEstacoesCriadas++;
        }else{
            //estacao X já tinha sido criada
            estacaoX = getEstacao(id_estacaoX);
            adicionarLinhaEstacao(estacaoX,id_linha); //adiciona a linha à estacao caso ainda não a tenha
        }
        if(!estacoesCriadas[id_estacaoY-1]){
            //estacao Y ainda não foi criada
            estacaoY = criarEstacao(id_estacaoY,id_linha);
            estacoesCriadas[id_estacaoY-1]=true;
            numEstacoesCriadas++;
        }else{
            //estacao Y já tinha sido criada
            estacaoY = getEstacao(id_estacaoY);
            adicionarLinhaEstacao(estacaoY,id_linha); //adiciona a linha à estacao caso ainda não a tenha
        }
        Ligacao* ligacaoNova = criarLigacao(estacaoX,estacaoY,id_linha); //cria a ligacao e adiciona às estações e à lista de ligações
    }

    if(numEstacoesCriadas!=numEstacoesTotal){
        //há uma estção sem ligação
        printf("%d\n",-1);
        return;
    }
}

int main(){
    getInput();
    return 0;
}