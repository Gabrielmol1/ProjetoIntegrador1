#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <ctime>
#include <limits>

const int TAMANHO_TABULEIRO = 15;

// Funcao para inicializar o tabuleiro do Ludo
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
        tabuleiro[1][7] = '.';
        tabuleiro[7][1] = '.';
        tabuleiro[7][13] = '.';
        tabuleiro[13][7] = '.';
    }

    // Pecas do jogador 1
    tabuleiro[3][3] = '1';
    tabuleiro[4][3] = '1';
    tabuleiro[4][4] = '1';
    tabuleiro[3][4] = '1';

    // Pecas do jogador 2
    tabuleiro[3][10] = '2';
    tabuleiro[3][11] = '2';
    tabuleiro[4][10] = '2';
    tabuleiro[4][11] = '2';

    // Pecas do jogador 3
    tabuleiro[10][3] = '3';
    tabuleiro[11][3] = '3';
    tabuleiro[10][4] = '3';
    tabuleiro[11][4] = '3';

    // Pecas do jogador 4
    tabuleiro[10][10] = '4';
    tabuleiro[10][11] = '4';
    tabuleiro[11][10] = '4';
    tabuleiro[11][11] = '4';

    // Definindo o centro e areas de inicio
    tabuleiro[7][7] = '*'; // Centro

    // Areas de inicio para cada jogador corrigidas
    tabuleiro[1][1] = '1';       // Area de inicio do Jogador 1
    tabuleiro[1][13] = '2';      // Area de inicio do Jogador 2
    tabuleiro[13][1] = '3';      // Area de inicio do Jogador 3
    tabuleiro[13][13] = '4';     // Area de inicio do Jogador 4

    return tabuleiro;
}

// Funcao para imprimir o tabuleiro do Ludo
void imprimirTabuleiro(const std::vector<std::vector<char>>& tabuleiro) {
    for (int i = 0; i < TAMANHO_TABULEIRO; ++i) {
        for (int j = 0; j < TAMANHO_TABULEIRO; ++j) {
            std::cout << std::setw(2) << tabuleiro[i][j];
        }
        std::cout << std::endl;
    }
}

// Funcao para rolar o dado
int rolarDado() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 6);

    return dis(gen);
}

// Funcao para mudar a vez do jogador
void mudarVez(int& jogadorAtual) {
    jogadorAtual = (jogadorAtual % 4) + 1; // Muda para o proximo jogador (de 1 a 4)
}

// Funcao para exibir a vez do jogador atual
void exibirJogadorAtual(int jogadorAtual) {
    std::cout << "Jogador " << jogadorAtual << ", e sua vez de jogar." << std::endl;
}

// Funcao para mover uma peca da base para o tabuleiro
bool moverPecaDaBase(int jogadorAtual, std::vector<std::vector<char>>& tabuleiro, int resultadoDado) {
    int linhaAlvo, colunaAlvo;
    bool pecaMovida = false;

    // Determinar as posicoes alvo com base no jogador atual
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
        std::cerr << "Erro: Jogador invalido!" << std::endl;
        return false;
    }

    // Verificar se o resultado do dado e igual a 6
    if (resultadoDado != 6) {
        std::cerr << "Voce precisa tirar 6 para mover uma peca da base para o tabuleiro." << std::endl;
        return false;
    }

    // Verificar a posicao da peca na base e mover uma peca
    for (int i = 0; i < TAMANHO_TABULEIRO && !pecaMovida; ++i) {
        for (int j = 0; j < TAMANHO_TABULEIRO && !pecaMovida; ++j) {
            // Condicoes especificas para encontrar pecas na base inicial de cada jogador
            if (tabuleiro[i][j] == '0' + jogadorAtual && (
                (jogadorAtual == 1 && (i == 3 || i == 4) && (j == 3 || j == 4)) ||
                (jogadorAtual == 2 && (i == 3 || i == 4) && (j == 10 || j == 11)) ||
                (jogadorAtual == 3 && (i == 10 || i == 11) && (j == 3 || j == 4)) ||
                (jogadorAtual == 4 && (i == 10 || i == 11) && (j == 10 || j == 11))
                )) {
                tabuleiro[i][j] = ' '; // Remover a peca da posicao anterior
                tabuleiro[linhaAlvo][colunaAlvo] = '0' + jogadorAtual; // Mover a peca da base para a posicao alvo
                pecaMovida = true;
                std::cout << "Jogador " << jogadorAtual << " moveu uma peca para [" << linhaAlvo << "][" << colunaAlvo << "]." << std::endl;
                break;
            }
        }
    }

    if (!pecaMovida) {
        std::cerr << "Nao ha pecas na base para mover." << std::endl;
    }

    return pecaMovida;
}

void moverPecaNoTabuleiro(int jogadorAtual, std::vector<std::vector<char>>& tabuleiro, int resultadoDado) {
    std::vector<std::pair<int, int>> posicoesValidas;

    // Percorre o tabuleiro e encontra todas as posições válidas para mover uma peça
    for (int i = 0; i < TAMANHO_TABULEIRO; ++i) {
        for (int j = 0; j < TAMANHO_TABULEIRO; ++j) {
            if (tabuleiro[i][j] == '0' + jogadorAtual) {
                posicoesValidas.push_back(std::make_pair(i, j));
            }
        }
    }

    // Verifica se há pelo menos uma peça para mover
    if (posicoesValidas.empty()) {
        std::cerr << "Erro: O jogador " << jogadorAtual << " nao tem pecas no tabuleiro para mover." << std::endl;
        return;
    }

    // Escolhe aleatoriamente uma das posições válidas
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, posicoesValidas.size() - 1);
    int indice = dis(gen);
    int linha = posicoesValidas[indice].first;
    int coluna = posicoesValidas[indice].second;

    // Move a peça para frente de acordo com o valor do dado
    for (int i = 0; i < resultadoDado; ++i) {
        // Lógica para mover a peça dependendo do caminho do tabuleiro
        // (neste exemplo, está apenas movendo a peça para a direita)
        if (coluna + 1 < TAMANHO_TABULEIRO) {
            tabuleiro[linha][coluna + 1] = tabuleiro[linha][coluna];
            tabuleiro[linha][coluna] = ' ';
            coluna++;
        } else {
            // Se a peça chegou ao final do tabuleiro, retorna à área inicial
            tabuleiro[linha][coluna] = '0' + jogadorAtual;
            break;
        }
    }

    // Atualiza o tabuleiro após mover a peça
    imprimirTabuleiro(tabuleiro);
}

int main() {
    int jogadorAtual = 1;
    std::vector<std::vector<char>> tabuleiroLudo = inicializarTabuleiroLudo();

    while (true) {
        imprimirTabuleiro(tabuleiroLudo);
        exibirJogadorAtual(jogadorAtual);

        std::cout << "Pressione Enter para rolar o dado..." << std::endl;
        std::cin.get();

        int resultadoDado = rolarDado();
        std::cout << "O dado rolou: " << resultadoDado << std::endl;

        bool jogarNovamente = false;

        if (resultadoDado == 6) {
            jogarNovamente = moverPecaDaBase(jogadorAtual, tabuleiroLudo, resultadoDado);
        }
        else {
            std::cout << "Voce nao tirou 6, movendo uma peca no tabuleiro..." << std::endl;
            moverPecaNoTabuleiro(jogadorAtual, tabuleiroLudo, resultadoDado);
        }

        // Se o jogador tirou um 6 e moveu uma peça da base para o tabuleiro, ele joga novamente
        if (resultadoDado == 6 && jogarNovamente) {
            imprimirTabuleiro(tabuleiroLudo);
            std::cout << "Pressione Enter para rolar o dado novamente..." << std::endl;
            std::cin.get();
            resultadoDado = rolarDado();
            std::cout << "O dado rolou: " << resultadoDado << std::endl;
        }

        // Mudar para o proximo jogador
        mudarVez(jogadorAtual);
    }

    return 0;
}