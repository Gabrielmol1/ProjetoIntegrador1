#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <ctime>
#include <limits>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

const int TAMANHO_TABULEIRO = 15;

// Cores para o tabuleiro
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLDBLACK   "\033[1m\033[30m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"

// Função para limpar a tela
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Função para ler uma tecla sem bloquear a execução
int lerTecla() {
#ifdef _WIN32
    if (kbhit()) {
        return getch();
    }
    return 0;
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

// Função para inicializar o tabuleiro do Ludo
std::vector<std::vector<char>> inicializarTabuleiroLudo() {
    std::vector<std::vector<char>> tabuleiro(TAMANHO_TABULEIRO, std::vector<char>(TAMANHO_TABULEIRO, ' '));

    // Caminhos neutros verticais e horizontais
    for (int i = 1; i < TAMANHO_TABULEIRO - 1; ++i) {
        if (i != 7) { // Evita o centro
            tabuleiro[7][i] = '-';
            tabuleiro[i][7] = '|';
        }
    }

    // Caminhos especiais para cada jogador
    for (int i = 0; i < 6; ++i) {
        tabuleiro[6 - i][6] = '.'; // Caminho do jogador 1
        tabuleiro[6][6 - i] = '.';
        tabuleiro[8 + i][8] = '.'; // Caminho do jogador 3
        tabuleiro[8][8 + i] = '.';
        tabuleiro[8 + i][6] = '.'; 
        tabuleiro[8][6 - i] = '.'; // Caminho do jogador 2
        tabuleiro[6 - i][8] = '.'; 
        tabuleiro[6][8 + i] = '.'; // Caminho do jogador 4
        
        // Correções
        tabuleiro[1][7] = '.';
        tabuleiro[7][1] = '.';
        tabuleiro[7][13] = '.';
        tabuleiro[13][7] = '.';
    }

    // Define as peças de cada jogador com cores diferentes
    tabuleiro[3][3] = '1';  // Jogador 1 (Vermelho)
    tabuleiro[4][3] = '1';
    tabuleiro[4][4] = '1';
    tabuleiro[3][4] = '1';

    tabuleiro[3][10] = '2'; // Jogador 2 (Azul)
    tabuleiro[3][11] = '2';
    tabuleiro[4][10] = '2';
    tabuleiro[4][11] = '2';

    tabuleiro[10][3] = '3'; // Jogador 3 (Amarelo)
    tabuleiro[11][3] = '3';
    tabuleiro[10][4] = '3';
    tabuleiro[11][4] = '3';

    tabuleiro[10][10] = '4'; // Jogador 4 (Verde)
    tabuleiro[10][11] = '4';
    tabuleiro[11][10] = '4';
    tabuleiro[11][11] = '4';

    // Definindo o centro e areas de inicio
    tabuleiro[7][7] = '*'; // Centro

    // Areas de inicio para cada jogador
    tabuleiro[1][1] = 'A';       // Area de inicio do Jogador 1
    tabuleiro[1][13] = 'B';      // Area de inicio do Jogador 2
    tabuleiro[13][1] = 'C';      // Area de inicio do Jogador 3
    tabuleiro[13][13] = 'D';     // Area de inicio do Jogador 4

    return tabuleiro;
}

// Função para imprimir o tabuleiro do Ludo com cores para cada jogador
void imprimirTabuleiro(const std::vector<std::vector<char>>& tabuleiro) {
    limparTela(); 
    for (int i = 0; i < TAMANHO_TABULEIRO; ++i) {
        for (int j = 0; j < TAMANHO_TABULEIRO; ++j) {
            // Define a cor da casa do tabuleiro
            if ((i == 0 && j == 0) || (i == 14 && j == 14) || (i == 0 && j == 14) || (i == 14 && j == 0) ||
                (i == 1 && j == 1) || (i == 13 && j == 13) || (i == 1 && j == 13) || (i == 13 && j == 1)) {
                std::cout << BOLDGREEN << std::setw(2) << tabuleiro[i][j] << RESET; 
            } else if ((i >= 1 && i <= 6) && (j >= 1 && j <= 6) ||
                       (i >= 9 && i <= 14) && (j >= 9 && j <= 14) ||
                       (i >= 1 && i <= 6) && (j >= 9 && j <= 14) ||
                       (i >= 9 && i <= 14) && (j >= 1 && j <= 6)) {
                std::cout << BOLDYELLOW << std::setw(2) << tabuleiro[i][j] << RESET; 
            } else if (tabuleiro[i][j] == '1') {
                std::cout << BOLDRED << std::setw(2) << tabuleiro[i][j] << RESET; // Jogador 1 - Vermelho
            } else if (tabuleiro[i][j] == '2') {
                std::cout << BOLDBLUE << std::setw(2) << tabuleiro[i][j] << RESET;  // Jogador 2 - Azul
            } else if (tabuleiro[i][j] == '3') {
                std::cout << BOLDYELLOW << std::setw(2) << tabuleiro[i][j] << RESET; // Jogador 3 - Amarelo
            } else if (tabuleiro[i][j] == '4') {
                std::cout << BOLDGREEN << std::setw(2) << tabuleiro[i][j] << RESET;  // Jogador 4 - Verde
            } else {
                std::cout << BOLDBLACK << std::setw(2) << tabuleiro[i][j] << RESET; 
            }
        }
        std::cout << std::endl;
    }
}

// Função para rolar o dado
int rolarDado() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 6);

    return dis(gen);
}

// Função para mudar a vez do jogador
void mudarVez(int& jogadorAtual) {
    jogadorAtual = (jogadorAtual % 4) + 1; // Muda para o próximo jogador (de 1 a 4)
}

// Função para exibir a vez do jogador atual
void exibirJogadorAtual(int jogadorAtual) {
    std::cout << "Jogador " << jogadorAtual << ", é sua vez de jogar." << std::endl;
}

// Função para mover uma peça da base para o tabuleiro
bool moverPecaDaBase(int jogadorAtual, std::vector<std::vector<char>>& tabuleiro, int resultadoDado) {
    int linhaAlvo, colunaAlvo;
    bool pecaMovida = false;

    // Determinar as posições alvo com base no jogador atual
    switch (jogadorAtual) {
        case 1:
            linhaAlvo = 6;
            colunaAlvo = 2;
            break;
        case 2:
            linhaAlvo = 2;
            colunaAlvo = 8;
            break;
        case 3:
            linhaAlvo = 12;
            colunaAlvo = 6;
            break;
        case 4:
            linhaAlvo = 8;
            colunaAlvo = 12;
            break;
        default:
            std::cerr << "Erro: Jogador inválido!" << std::endl;
            return false;
    }

    // Verificar se o resultado do dado é igual a 6
    if (resultadoDado != 6) {
        std::cout << "Você precisa tirar 6 para mover uma peça da base para o tabuleiro." << std::endl;
        return false;
    }

    // Verificar se a posição alvo está livre
    if (tabuleiro[linhaAlvo][colunaAlvo] != '.') {
        std::cout << "A posição alvo já está ocupada." << std::endl;
        return false;
    }

    // Encontrar uma peça na base do jogador e movê-la
    for (int i = 0; i < TAMANHO_TABULEIRO && !pecaMovida; ++i) {
        for (int j = 0; j < TAMANHO_TABULEIRO && !pecaMovida; ++j) {
            if (tabuleiro[i][j] == '0' + jogadorAtual) {
                tabuleiro[i][j] = ' '; // Remover a peça da base
                tabuleiro[linhaAlvo][colunaAlvo] = '0' + jogadorAtual; // Mover a peça para o tabuleiro
                pecaMovida = true;
                std::cout << "Jogador " << jogadorAtual << " moveu uma peça da base para o tabuleiro." << std::endl;
                break;
            }
        }
    }

    return pecaMovida;
}

// Função para mover uma peça no tabuleiro
void moverPecaNoTabuleiro(int jogadorAtual, std::vector<std::vector<char>>& tabuleiro, int resultadoDado) {
    int linhaAtual, colunaAtual, linhaAlvo, colunaAlvo;
    bool pecaEncontrada = false;

    // Encontrar a peça do jogador no tabuleiro
    for (int i = 0; i < TAMANHO_TABULEIRO && !pecaEncontrada; ++i) {
        for (int j = 0; j < TAMANHO_TABULEIRO && !pecaEncontrada; ++j) {
            if (tabuleiro[i][j] == '0' + jogadorAtual) {
                linhaAtual = i;
                colunaAtual = j;
                pecaEncontrada = true;
                break;
            }
        }
    }

    // Verificar se uma peça foi encontrada
    if (!pecaEncontrada) {
        std::cout << "O jogador " << jogadorAtual << " não possui peças no tabuleiro para mover." << std::endl;
        return;
    }

    // Calcular a posição alvo com base no resultado do dado
    linhaAlvo = linhaAtual;
    colunaAlvo = colunaAtual;
    for (int i = 0; i < resultadoDado; ++i) {
        // Lógica para mover a peça dependendo do caminho do tabuleiro
        // (neste exemplo, está apenas movendo a peça para a direita)
        if (colunaAlvo + 1 < TAMANHO_TABULEIRO) {
            colunaAlvo++;
        } else {
            // Se a peça chegou ao final do tabuleiro, retorna à área inicial
            colunaAlvo = 0;
        }
    }

    // Verificar se a posição alvo está livre
    if (tabuleiro[linhaAlvo][colunaAlvo] != '.' && tabuleiro[linhaAlvo][colunaAlvo] != '0' + jogadorAtual) {
        std::cout << "A posição alvo está ocupada." << std::endl;
        return;
    }

    // Mover a peça para a posição alvo
    tabuleiro[linhaAtual][colunaAtual] = ' ';
    tabuleiro[linhaAlvo][colunaAlvo] = '0' + jogadorAtual;
    std::cout << "Jogador " << jogadorAtual << " moveu uma peça para [" << linhaAlvo << "][" << colunaAlvo << "]." << std::endl;
}

int main() {
    int jogadorAtual = 1;
    std::vector<std::vector<char>> tabuleiroLudo = inicializarTabuleiroLudo();
    int resultadoDado; 

    while (true) {
        imprimirTabuleiro(tabuleiroLudo);
        exibirJogadorAtual(jogadorAtual);

        std::cout << "Pressione Enter para rolar o dado..." << std::endl;
        while (lerTecla() != 10) {} // Aguarda o pressionamento da tecla Enter (código ASCII 10)
        
        resultadoDado = rolarDado();
        std::cout << "O dado rolou: " << resultadoDado << std::endl;

        bool jogarNovamente = false;

        if (resultadoDado == 6) {
            jogarNovamente = moverPecaDaBase(jogadorAtual, tabuleiroLudo, resultadoDado);
            imprimirTabuleiro(tabuleiroLudo);
            if (jogarNovamente){
                std::cout << "Jogue novamente!" << std::endl; 
            }
        }
        else {
            moverPecaNoTabuleiro(jogadorAtual, tabuleiroLudo, resultadoDado);
            imprimirTabuleiro(tabuleiroLudo); 
        }

        if (!jogarNovamente) {
            mudarVez(jogadorAtual); 
        }
        
        // Pequena pausa para visualização
        usleep(1000000); 
    }

    return 0;
}