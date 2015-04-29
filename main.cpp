#include <iostream>

#include "sudoku.h"

using namespace std;

Sudoku sudoku;

class Pilha
{
public:
    Pilha();
    void empilha(Celula& cel);
    Celula* desempilha();
    int topo();
    bool vazia();

protected:
    int _topo;
    Celula* _valores[100];

private:
    void underflow();
    void overflow();
};

Pilha::Pilha()
{
    _topo = -1;
}

void Pilha::empilha(Celula& cel)
{
    _topo++;
    _valores[_topo] = &cel;
}

Celula* Pilha::desempilha()
{
    Celula* temp = _valores[_topo];
    _topo--;
    return temp;
}

int Pilha::topo()
{
    return _topo;
}

bool Pilha::vazia()
{
    if(_topo == -1) return true;

    else return false;
}

void Pilha::underflow()
{
    std::cout << "Underflow!" << std::endl;
    return;
}

void Pilha::overflow()
{
    std::cout << "Overflow!" << std::endl;
    return;
}

Pilha P; //Armazena as células não fixas
Pilha pAux; //Armazena as células já postas no sudoku


//IDÉIA: Empilhar as células não fixas na pilha P.
//       Desempilhar célula a célula e setar o primeiro número disponível a ela.
//       Caso seja possível setar algum número, esta célula é posta na pilha pAux
//       Caso não seja possível setar nenhum número, esta célula é devolvida a pilha P, e
//       a última célula é pega da pilha pAux, para setar o seguinte número disponível.
//       Quando a pilha P estiver vazia, todas as células foram devidamente testadas e setadas.
void resolveSudoku (Sudoku& sudoku)
{
    //Torna fixa todas aquelas células cuja opção de valor a ser preenchida é única
    for(int aux = 0; aux<10; aux++){ //é feito o loop mais de uma vez pois surgem novas células com uma única opção de valor
    int cont;    //conta quantas opções de números há
    unsigned int nfixo; //caso só haja uma opção de número na célula (nfixo) este número será setado na célula

    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
            if(sudoku.pegaCelula(i, j)->testaFixo() == false){
                    cont = 0;
                    for(unsigned int n = 1; n<10; n++)
                    if(n == sudoku.verificaValoresPossiveis(sudoku, sudoku.pegaEndereco(i, j), n)){
                    cont++;
                    nfixo = n;}
            if(cont == 1){
                sudoku.pegaCelula(i, j)->setaValor(nfixo);
                sudoku.pegaCelula(i, j)->tornaFixo();}}
    }


    //Todas as células não fixas são postas na pilha
    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
            if(sudoku.pegaCelula(i, j)->testaFixo() == false)
            P.empilha(sudoku.pegaEndereco(i, j));

    int auxl; //Grava o número da linha da matriz abaixo
    unsigned int Mat[81][11]; //Matriz para armazenar todos os valores possíveis para cada célula
                             //Linhas: todas as células possiveis Colunas:todos os valores possíveis
                             //todos os nós Mat[k][0], 0<=k<81, guardam o número de elementos nessa linha
                             //todos os nós Mat[k][10], 0<=k<81, guardam o número do contador

    //Inicializando contagem
    for(int k = 0; k<81; k++)
        Mat[k][0] = 0;

    //Inicializando contador
    //O contador conta quais dos valores possíveis já foram testados
    for(int k = 0; k<81; k++)
        Mat[k][10] = 1;

    for(auxl = 0; !P.vazia(); auxl++){
        int auxc = 1;
        Celula* temp = P.desempilha();
        for(unsigned int n = 1; n<10; n++){
                if(n == sudoku.verificaValoresPossiveis(sudoku, *temp, n)){ //Ou seja, se for possível pôr aquele valor na célula
                Mat[auxl][0]++;
                Mat[auxl][auxc] = n;
                auxc++;}
    }
    for(int i = Mat[auxl][0]+1; i<10; i++) //Zerar os nós não utilizados
        Mat[auxl][i] = 0;
    }

    //Novammente empilhadas as células não fixas
    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
            if(sudoku.pegaCelula(i, j)->testaFixo() == false)
            P.empilha(sudoku.pegaEndereco(i, j));

    //A seguir são feitos os testes recursivos dos valores

    Celula* temp = P.desempilha();
    bool continua = false; //Auxilia na escolha da pilha.
                           //Se a célula anterior foi setada, ela é posta na pilha pAux e continua=true;
                           //Se a célula anterior não tinha nenhum valor possível de ser setado, ela é devolvida a pilha P, e
                           //a célula anterior a ela é pega da pilha pAux. Portanto não é necessário desempilhar P, ou seja, continua=false.
    unsigned int k; //valor no qual é associodado o cantador Mat[auxl][10];
    auxl = 0; //Lê as linhas da matriz nas quais estão escritas os valores possíveis de serem postos e o contador
    while(!P.vazia()){
        if(continua) temp = P.desempilha();

        k = Mat[auxl][10];
        for(k = Mat[auxl][10]; k<=Mat[auxl][0]; k++)
            if(Mat[auxl][k] == sudoku.verificaValoresPossiveis(sudoku, *sudoku.pegaCelula(temp->pegaLinha(), temp->pegaColuna()), Mat[auxl][k]))
            //Ou seja, if(Mat[auxl][k] é um número válido de ser posta na célula)
            {
                sudoku.pegaCelula(temp->pegaLinha(), temp->pegaColuna())->setaValor(Mat[auxl][k]);
                k=1000; //Somente para pegar o primeiro valor disonível e sair do loop
                continua = true;
                pAux.empilha(*sudoku.pegaCelula(temp->pegaLinha(), temp->pegaColuna()));
                auxl++;
            }
        if(k == Mat[auxl][0]+1) //Significa que nenhum valor foi possivel, portanto é preciso mudar a célula anterior
        {
            sudoku.pegaCelula(temp->pegaLinha(), temp->pegaColuna())->setaValor(0);
            Mat[auxl][10] = 1; //Inicializa contador
            P.empilha(*sudoku.pegaCelula(temp->pegaLinha(), temp->pegaColuna()));

            temp = pAux.desempilha();
            continua = false;
            auxl--;
            Mat[auxl][10]++; //incrementa contador
        }
    }
    //------------Imprime------------------------------/

    cout << "+---+---+---+---+---+---+---+---+---+" << endl << "|";

    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++)
        cout << " " << sudoku.pegaCelula(i, j)->pegaValor() << " |";
    cout << endl;
    cout << "+---+---+---+---+---+---+---+---+---+" << endl << "|";
    }

   atualizaGUI();
}

/* Esta função retorna true caso o jogo esteja resolvido. false caso contrário */
bool ganhou (Sudoku& sudoku)
{
    for(int i = 0; i<8; i++)
        for(int j = 0; j<8; j++)
            if(sudoku.pegaCelula(i, j)->pegaValor() == 0)
            return false;
    return true;
}

/* Esta função verifica se o número "valor" pode ser colocado na célula "cel".
 * Retorna true caso seja possível, false caso contrário. */
bool verificaSePodeColocarNumero(Sudoku& sudoku, Celula& cel, unsigned int valor)
{
    //Se a celula for fixa, nao podera ser posto valor
    if(cel.testaFixo()) return false;

    bool linhaOk = true;
    bool colunaOk = true;
    bool regiaoOk = true; //verifica os numeros do quadrado 3x3

    //Primeiro: verificar a linha
    int l = cel.pegaLinha();
    for(int j = 0; j<9; j++)
        if(sudoku.pegaCelula(l, j)->pegaValor() == valor)
            linhaOk = false;

    //Segundo: verificar a coluna
    int c = cel.pegaColuna();
    for(int i = 0; i<9; i++)
        if(sudoku.pegaCelula(i, c)->pegaValor() == valor)
            colunaOk = false;

    //Terceiro: verificar regiao - 9 casos possiveis
    if(c%3 == 0 && l%3 == 0)
        for(int i = l; i<l+3; i++)
        for(int j = c; j<c+3; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 0 && l%3 == 1)
        for(int i = l-1; i<l+2; i++)
        for(int j = c; j<c+3; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 0 && l%3 == 2)
        for(int i = l-2; i<l+1; i++)
        for(int j = c; j<c+3; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 1 && l%3 == 0)
        for(int i = l; i<l+3; i++)
        for(int j = c-1; j<c+2; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 1 && l%3 == 1)
        for(int i = l-1; i<l+2; i++)
        for(int j = c-1; j<c+2; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 1 && l%3 == 2)
        for(int i = l-2; i<l+1; i++)
        for(int j = c-1; j<c+2; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 2 && l%3 == 0)
        for(int i = l; i<l+3; i++)
        for(int j = c-2; j<c+1; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 2 && l%3 == 1)
        for(int i = l-1; i<l+2; i++)
        for(int j = c-2; j<c+1; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 2 && l%3 == 2)
        for(int i = l-2; i<l+1; i++)
        for(int j = c-2; j<c+1; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(linhaOk==true && colunaOk==true && regiaoOk==true)
        return true;

    else return false;
}

inline unsigned int Celula::pegaValor() const
{
    return valor;
}

inline bool Celula::setaValor(unsigned int numero)
{
    if(testaFixo()) return false;

    else
        valor = numero;
    return true;
}

inline bool Celula::testaFixo() const
{
    if(fixo) return true;

    else return false;
}

inline unsigned int Celula::pegaLinha() const
{
    return linha;
}

inline unsigned int Celula::pegaColuna() const
{
    return coluna;
}

inline void Celula::tornaFixo()
{
    fixo = true;
}

inline unsigned int Sudoku::verificaValoresPossiveis(Sudoku& sudoku, Celula& cel,unsigned int valor)
{
    bool linhaOk = true;
    bool colunaOk = true;
    bool regiaoOk = true; //verifica os numeros do quadrado 3x3

    //Primeiro: verificar a linha
    int l = cel.pegaLinha();
    for(int j = 0; j<9; j++)
        if(sudoku.pegaCelula(l, j)->pegaValor() == valor)
            linhaOk = false;

    //Segundo: verificar a coluna
    int c = cel.pegaColuna();
    for(int i = 0; i<9; i++)
        if(sudoku.pegaCelula(i, c)->pegaValor() == valor)
            colunaOk = false;

    //Terceiro: verificar regiao - 9 casos possiveis
    if(c%3 == 0 && l%3 == 0)
        for(int i = l; i<l+3; i++)
        for(int j = c; j<c+3; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 0 && l%3 == 1)
        for(int i = l-1; i<l+2; i++)
        for(int j = c; j<c+3; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 0 && l%3 == 2)
        for(int i = l-2; i<l+1; i++)
        for(int j = c; j<c+3; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 1 && l%3 == 0)
        for(int i = l; i<l+3; i++)
        for(int j = c-1; j<c+2; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 1 && l%3 == 1)
        for(int i = l-1; i<l+2; i++)
        for(int j = c-1; j<c+2; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 1 && l%3 == 2)
        for(int i = l-2; i<l+1; i++)
        for(int j = c-1; j<c+2; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 2 && l%3 == 0)
        for(int i = l; i<l+3; i++)
        for(int j = c-2; j<c+1; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 2 && l%3 == 1)
        for(int i = l-1; i<l+2; i++)
        for(int j = c-2; j<c+1; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(c%3 == 2 && l%3 == 2)
        for(int i = l-2; i<l+1; i++)
        for(int j = c-2; j<c+1; j++)
        if(sudoku.pegaCelula(i,j)->pegaValor() == valor)
        regiaoOk = false;

    if(linhaOk==true && colunaOk==true && regiaoOk==true)
        return valor;

    else return 0;
}

inline Celula* Sudoku::pegaCelula(unsigned int linha, unsigned int coluna)
{
    return &_numeros[linha][coluna];
}

inline Celula& Sudoku::pegaEndereco(unsigned int linha, unsigned int coluna)
{
    return _numeros[linha][coluna];
}


/* Aqui no main iniciamos a interface gráfica. Evite editar esta função */
int main()
{
    setaFuncaoResolve(resolveSudoku);
    setaFuncaoVerifica(verificaSePodeColocarNumero);
    setaFuncaoVitoria(ganhou);
    iniciaGUI();
    return  0;
}

