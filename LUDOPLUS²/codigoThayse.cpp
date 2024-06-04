#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <unistd.h> // Para a função sleep
#include <cstdlib> // Para a função system

using namespace std;

const int TAMANHO_TABULEIRO = 15;
const int CENTRO_TABULEIRO = 7;

// Estrutura para representar uma célula no tabuleiro
struct Celula {
    int linha;
    int coluna;

    Celula(int l = -1, int c = -1) : linha(l), coluna(c) {}
};

// Função para imprimir o tabuleiro com os caminhos dos peões
void imprimirTabuleiroColorido(const vector<vector<char>>& tabuleiro) {
    //percursos
    unordered_map<char, string> cores_min = {
        {'r', "\033[1;31m"},  // Vermelho
        {'b', "\033[1;34m"},  // Azul
        {'y', "\033[1;33m"},  // Amarelo
        {'g', "\033[1;32m"},  // Verde
        {';', "\033[1;35m"},  // Roxo
        {':', "\033[1;38;5;208m"}  // Laranja
    };
    //peões 
    unordered_map<char, string> cores_mai = {
        {'R', "\033[1;31m"},  // Vermelho
        {'B', "\033[1;34m"},  // Azul
        {'Y', "\033[1;33m"},  // Amarelo
        {'G', "\033[1;32m"},  // Verde
    };
    //tabuleiro
    for (const auto& linha : tabuleiro) {
        for (char celula : linha) {
            if (cores_min.find(celula) != cores_min.end()) {
                cout << cores_min[celula] << celula << "\033[0m ";
            } else if (cores_mai.find(celula) != cores_mai.end()) {
                cout << cores_mai[celula] << celula << "\033[0m ";
            } else {
                cout << celula << " ";
            }
        }
        cout << endl;
    }
}

// Função para criar e inicializar o tabuleiro com os caminhos dos peões e as casas de saída
vector<vector<char>> criarTabuleiro() {
    vector<vector<char>> tabuleiro(TAMANHO_TABULEIRO, vector<char>(TAMANHO_TABULEIRO, ' '));
    // bordas do tabuleiro 
    for (int i = 0; i < TAMANHO_TABULEIRO; ++i) {
        tabuleiro[0][i] = '*';  // Linha superior
        tabuleiro[TAMANHO_TABULEIRO - 1][i] = '*';  // Linha inferior
        tabuleiro[i][0] = '*';  // Coluna esquerda
        tabuleiro[i][TAMANHO_TABULEIRO - 1] = '*';  // Coluna direita
    }
    //onde andam os peões
    for (int i = 1; i < TAMANHO_TABULEIRO - 1; ++i) {
        if (i < TAMANHO_TABULEIRO - 1) {
            tabuleiro[i][6] = '.';
        }
        if (i == 2) {
            tabuleiro[i - 1][7] = '.';
        }
        if (i == 2) {
            tabuleiro[TAMANHO_TABULEIRO - i][7] = '.';
        }
        if (i < TAMANHO_TABULEIRO - 1) {
            tabuleiro[i][8] = '.';
        }
    }

    for (int i = 1; i < TAMANHO_TABULEIRO - 1; ++i) {
        if (i < TAMANHO_TABULEIRO - 1) {
            tabuleiro[6][i] = '.';
        }
        if (i == 1) {
            tabuleiro[7][i] = '.';
        }
        if (i == 2) {
            tabuleiro[7][TAMANHO_TABULEIRO - i] = '.';
        }
        if (i < TAMANHO_TABULEIRO - 1) {
            tabuleiro[8][i] = '.';
        }
    }
    //estrutura das bases 
    //vermelho 
    tabuleiro[2][3] = '|';
    tabuleiro[4][3] = '|';
    tabuleiro[3][2] = '-';
    tabuleiro[3][3] = '+';
    tabuleiro[3][4] = '-';
    //azul
    tabuleiro[2][11] = '|';
    tabuleiro[4][11] = '|';
    tabuleiro[3][10] = '-';
    tabuleiro[3][11] = '+';
    tabuleiro[3][12] = '-';
    //verde 
    tabuleiro[10][3] = '|';
    tabuleiro[12][3] = '|';
    tabuleiro[11][2] = '-';
    tabuleiro[11][3] = '+';
    tabuleiro[11][4] = '-';
    //amarelo
    tabuleiro[10][11] = '|';
    tabuleiro[12][11] = '|';
    tabuleiro[11][10] = '-';
    tabuleiro[11][11] = '+';
    tabuleiro[11][12] = '-';
    //casas iniciais
    tabuleiro[6][2] = 'r';  // Linha 6, Coluna 2
    tabuleiro[2][8] = 'b';  // Linha 2, Coluna 8
    tabuleiro[8][12] = 'y'; // Linha 8, Coluna 12
    tabuleiro[12][6] = 'g'; // Linha 12, Coluna 6
    //zonas seguras - reta final de cada cor
    for (int i = 2; i <= 6; ++i) {
        tabuleiro[7][i] = 'r'; // Linha 7, Colunas 2 a 6
        tabuleiro[i][7] = 'b'; // Coluna 7, Linhas 2 a 6
    }
    for (int i = 8; i <= 12; ++i) {
        tabuleiro[7][i] = 'y'; // Linha 7, Colunas 8 a 12
        tabuleiro[i][7] = 'g'; // Coluna 7, Linhas 8 a 12
    }
    //casa coroa
    tabuleiro[8][4] = ':';   // Linha 8, Coluna 4
    tabuleiro[4][6] = ':';   // Linha 4, Coluna 6
    tabuleiro[6][10] = ':';  // Linha 6, Coluna 10
    tabuleiro[10][8] = ':';  // Linha 10, Coluna 8
    //casa vingança 
    tabuleiro[8][1] = ';';   // Linha 8, Coluna 1
    tabuleiro[1][6] = ';';   // Linha 1, Coluna 6
    tabuleiro[6][13] = ';';  // Linha 6, Coluna 13
    tabuleiro[13][8] = ';';  // Linha 13, Coluna 8
    // Casas seguras - peão fica após chegar ao fim F
    // Vermelho
    tabuleiro[1][1] = '.';
    tabuleiro[1][2] = '.';
    tabuleiro[1][3] = '.';
    tabuleiro[1][4] = '.';
    // Azul
    tabuleiro[1][10] = '.';
    tabuleiro[1][11] = '.';
    tabuleiro[1][12] = '.';
    tabuleiro[1][13] = '.';
    // Amarelo
    tabuleiro[13][10] = '.';
    tabuleiro[13][11] = '.';
    tabuleiro[13][12] = '.';
    tabuleiro[13][13] = '.';
    // Verde
    tabuleiro[13][1] = '.';
    tabuleiro[13][2] = '.';
    tabuleiro[13][3] = '.';
    tabuleiro[13][4] = '.';
    //casa final
    tabuleiro[CENTRO_TABULEIRO][CENTRO_TABULEIRO] = 'F';
    // peões base vermelha
    tabuleiro[2][2] = 'R';   // Linha 2, Coluna 2
    tabuleiro[2][4] = 'R';   // Linha 2, Coluna 4
    tabuleiro[4][2] = 'R';   // Linha 4, Coluna 2
    tabuleiro[4][4] = 'R';   // Linha 4, Coluna 4
    // peões base azul 
    tabuleiro[2][10] = 'B';  // Linha 2, Coluna 10
    tabuleiro[2][12] = 'B';  // Linha 2, Coluna 12
    tabuleiro[4][10] = 'B';  // Linha 4, Coluna 10
    tabuleiro[4][12] = 'B';  // Linha 4, Coluna 12
    // peões base verde
    tabuleiro[10][2] = 'G';  // Linha 10, Coluna 2
    tabuleiro[10][4] = 'G';  // Linha 10, Coluna 4
    tabuleiro[12][2] = 'G';  // Linha 12, Coluna 2
    tabuleiro[12][4] = 'G';  // Linha 12, Coluna 4
    // peões base amarela 
    tabuleiro[10][10] = 'Y'; // Linha 10, Coluna 10
    tabuleiro[10][12] = 'Y'; // Linha 10, Coluna 12
    tabuleiro[12][10] = 'Y'; // Linha 12, Coluna 10
    tabuleiro[12][12] = 'Y'; // Linha 12, Coluna 12
    return tabuleiro;
}

// Função para lançar o dado
int lancarDado() {
    return rand() % 6 + 1;
}

// Função para mover um peão no tabuleiro
bool moverPeao(vector<vector<char>>& tabuleiro, char cor, int movimentos) {
    int direcao[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int dx = 0, dy = 0;
    Celula peao;
    // Encontrar a posição atual do peão
    for (int i = 0; i < TAMANHO_TABULEIRO; ++i) {
        for (int j = 0; j < TAMANHO_TABULEIRO; ++j) {
            if (tabuleiro[i][j] == cor) {
                peao.linha = i;
                peao.coluna = j;
                break;
            }
        }
    }
    // Mover o peão
    while (movimentos > 0) {
        // Verificar se o peão chegou à casa final
        if (peao.linha == CENTRO_TABULEIRO && peao.coluna == CENTRO_TABULEIRO) {
            return true;
        }
        // Verificar a direção do movimento
        if (peao.coluna == 6 && peao.linha > 0 && peao.linha < 6) {
            dy = -1;
        } else if (peao.coluna == 8 && peao.linha > 8 && peao.linha < TAMANHO_TABULEIRO) {
            dy = 1;
        } else if (peao.linha == 6 && peao.coluna > 0 && peao.coluna < 6) {
            dx = -1;
        } else if (peao.linha == 8 && peao.coluna > 8 && peao.coluna < TAMANHO_TABULEIRO) {
            dx = 1;
        }
        // Atualizar a posição do peão
        peao.linha += dx;
        peao.coluna += dy;
        // Atualizar o tabuleiro
        tabuleiro[peao.linha][peao.coluna] = cor;
        movimentos--;
    }
    return false;
}

// Função para realizar uma jogada
bool realizarJogada(vector<vector<char>>& tabuleiro, char cor) {
    // Lançar o dado
    int movimentos = lancarDado();
    cout << "Jogador " << cor << " tirou " << movimentos << " no dado." << endl;
    // Mover o peão
    return moverPeao(tabuleiro, cor, movimentos);
}

// Função para limpar a tela
void limparTela() {
    system("clear"); // Para sistemas Unix/Linux
}

// Função principal do jogo
int main() {
    srand(time(0));
    vector<vector<char>> tabuleiro = criarTabuleiro();
    vector<char> cores = {'R', 'B', 'Y', 'G'};
    bool jogoTerminado = false;
    int rodada = 0;

    while (!jogoTerminado) {
        // Limpar a tela e imprimir o tabuleiro
        limparTela();
        imprimirTabuleiroColorido(tabuleiro);
        // Jogar
        char corAtual = cores[rodada % 4];
        jogoTerminado = realizarJogada(tabuleiro, corAtual);
        // Aguardar um pouco antes de continuar
        sleep(1);
        rodada++;
    }

    cout << "Jogador " << cores[(rodada - 1) % 4] << " venceu o jogo!" << endl;

    return 0;
}