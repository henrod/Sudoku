#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

// Utilize as marcas abaixo para marcar as funções que podem ser acessadas por programas utilizando essa DLL
#if defined _WIN32 || defined __CYGWIN__
    #define DLL_IMPORT __declspec(dllimport)
#else
    #define DLL_IMPORT __attribute__ ((visibility ("default")))
#endif

#define LIN_COL_SUDOKU 9

// Representa uma célula do sudoku
class DLL_IMPORT Celula
{
public:
    /* Retorna o valor da celula atual. */
    inline unsigned int pegaValor() const;
    /* Seta o valor atual de um número. Retorna FALHA caso não consiga.
    * Esta função só testa se o número é fixo. Não testa por inconsistências. */
    inline bool setaValor(unsigned int numero);
    /* Retorna true se o número for fixo do problema. */
    inline bool testaFixo() const;
    /* Retorna qual posição do problema esta celula representa. */
    inline unsigned int pegaLinha() const;
    inline unsigned int pegaColuna() const;
    inline void tornaFixo(); //Torna fixa aquelas celulas que somente tem uma opção de valor a ser preenchida
protected:
    unsigned int valor;
    bool fixo;
    unsigned int linha;
    unsigned int coluna;

    friend class Sudoku;
};

// Representa o sudoku inteiro
class DLL_IMPORT Sudoku
{
public:
    /* Pega a Celula que representa a posição pedida */
    inline Celula& pegaEndereco(unsigned int linha, unsigned int coluna);
    inline Celula* pegaCelula(unsigned int linha, unsigned int coluna);
    inline unsigned int verificaValoresPossiveis(Sudoku& sudoku, Celula& cel, unsigned int valor);
private:
    Celula _numeros[LIN_COL_SUDOKU][LIN_COL_SUDOKU];

    friend class Celula;

};


/* Inicia a interface gráfica */
extern "C" DLL_IMPORT int iniciaGUI();

/* Setam as funções que serão chamadas pela interface gráfica */
extern "C" DLL_IMPORT void setaFuncaoVitoria(bool (*ganhou)(Sudoku&));
extern "C" DLL_IMPORT void setaFuncaoResolve(void (*resolveSudoku)(Sudoku&));
extern "C" DLL_IMPORT void setaFuncaoVerifica(bool (*verificaSePodeColocarNumero)(Sudoku&, Celula&, unsigned int));

/* Atualiza a tela.
 * Como é uma operação razoavelmente demorada, ela não é chamada sempre
 * que alguma coisa muda durante uma função demorada (como o resolveSudoku)
 * Você pode chama-lá se quiser ver o que está acontecendo. */
extern "C" DLL_IMPORT void atualizaGUI();

#endif // SUDOKU_H_INCLUDED
