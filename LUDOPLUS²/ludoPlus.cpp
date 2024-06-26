#include <iostream>  // entrada e saída
#include <fstream>   // manipulação de arquivo txt
#include <string>    // utilização de string, manipulação de texto
#include <cstdlib>   // Para usar a função system
#include <algorithm> // sort e find
#include <vector>    // usar vetores
#include <ctime>     // cronômetro na tela do jogo
#include <unistd.h>  // Para a função sleep
#include <unordered_map>
#include <conio.h>   // Para usar _kbhit() e _getch()
#include <windows.h> // Para usar Sleep() e SetConsoleTextAttribute()
#include <sstream> // para a classe stringstream
using namespace std;

time_t tempoInicial;

const int TAMANHO_TABULEIRO = 15;
const int CENTRO_TABULEIRO = 7;

// Definição das posições das tocas para cada cor de peão
const unordered_map<char, pair<int, int>> posicoesTocas = {
    {'R', {1, 1}},  // Toca para peões vermelhos
    {'B', {1, 14}}, // Toca para peões azuis
    {'G', {14, 1}}, // Toca para peões verdes
    {'Y', {14, 14}} // Toca para peões amarelos
};

// Estrutura para representar uma célula no tabuleiro
struct Celula {
    int linha;
    int coluna;

    Celula(int l = -1, int c = -1) : linha(l), coluna(c) {}

    bool operator==(const Celula &other) const {
        return linha == other.linha && coluna == other.coluna;
    }
};

// Declaração das funções em ordem sequecial 
void salvarJogador_txt(const string &nome, const string &senha, const string &perguntaSeguranca, const string &respostaPergunta);
void salvarDadosPartida_txt(int numeroPartida, string dataHora, vector<pair<string, string>> jogadoresCores, unordered_map<string, int> ranking);
void salvarVitoriasGeral(const unordered_map<string, int>& vitoriasGeral);
void salvarVitorias4Jogadores(const unordered_map<string, int>& vitorias4Jogadores);
void salvarVitorias3Jogadores(const unordered_map<string, int>& vitorias3Jogadores);
void salvarVitorias2Jogadores(const unordered_map<string, int>& vitorias2Jogadores);
vector<string> lerCsvDadosPartidatoHistorico();
void lerCsvDadosPartidatoRankingPontos(vector<pair<string, int>>& pontosJogadores);
void lerCsvDadosPartidatoRankingPartidas(unordered_map<string, int>& vitoriasGeral, unordered_map<string, int>& vitorias4Jogadores, unordered_map<string, int>& vitorias3Jogadores, unordered_map<string, int>& vitorias2Jogadores);
void salvarRankingPontuacao(const vector<pair<string, int>>& ranking);
void solicitarDadosJogador(string &nome, string &senha, string &perguntaSeguranca, string &respostaPergunta);
void salvarEditarPerfil(string nomeAtual, string senhaAtual);
void fecharJogo();
void validarLogin(string nome, string senha);
bool verificarNomeExistente(const string &nome);
bool verificarTamanhoSenha(const string &senha);
void tabelaRankingPontos();
void atualizarTabelaPontos();
void somarPontosDosJogadores(const vector<pair<string, int>>& pontosJogadores, vector<pair<string, int>>& ranking);
void tabelaRankingVitoriasGeral();
void TabelaRankingVitorias4Jogadores();
void TabelaRankingVitorias3Jogadores();
void TabelaRankingVitorias2Jogadores();
void atualizarTabelaVitorias();
void exibirHistorico(const string& nomeJogador, const vector<string>& linhasPartida);
bool verificarSenhasIguais(const string &senha, const string &confirmarSenha);
bool verificarRespostasIguais(const string &resposta, const string &confirmarResposta);
void excluirPerfil(const string &nome, const string &senha, const string &respostaPergunta);
int selecionarQuantidadeJogParaPartida();
void selecionarJogadoresECoresParaPartida(int num_jogadores);
void imprimirTabuleiroColorido(const vector<vector<char>> &tabuleiro);
void limparTela();
string trim(const string &str);
int lancarDado();
void atualizarTabuleiro(vector<vector<char>> &tabuleiro, const unordered_map<char, vector<Celula>> &posicoesPecas);
bool moverPeao(vector<vector<char>> &tabuleiro, char cor, int movimentos, const vector<Celula> &percurso, unordered_map<char, vector<Celula>> &posicoesPecas);
vector<vector<char>> criarTabuleiro();
bool realizarJogada(vector<vector<char>> &tabuleiro, char cor, unordered_map<char, vector<Celula>> &posicoesPecas);
void capturarPeoesAdversarios(vector<vector<char>> &tabuleiro, int linha, int coluna, char corAdversario, unordered_map<char, vector<Celula>> &posicoesPecas);
bool existePecaNoTabuleiro(const vector<vector<char>> &tabuleiro, char cor);
bool verificarPosicao(vector<vector<char>> &tabuleiro, int linha, int coluna, char cor, unordered_map<char, vector<Celula>> &posicoesPecas);
void retirarPecaDaToca(vector<vector<char>> &tabuleiro, char cor, unordered_map<char, vector<Celula>> &posicoesPecas);
vector<Celula> definirPercursoVermelho(); //defini percurso cor vermelha
vector<Celula> definirPercursoAzul(); //defini percurso do azul
vector<Celula> definirPercursoAmarelo(); //defini percurso amarelo
vector<Celula> definirPercursoVerde(); //defini percurso verde
Celula calcularNovaPosicao(const Celula &peao, int movimentos, const vector<Celula> &percurso);
// telas em ordem sequencial  

void tela_Login();
void tela_Menu();
void tela_Cadastrar();
void Tela_Jogar_MostrarTempoNaTela();
void tela_Jogar(const vector<string> &nomesJogadores, const vector<string> &coresJogadores);
void tela_Ranking();
void tela_Historico();
void tela_EditarPerfil();
void tela_RecuperarSenha();
void tela_Regras();
void tela_ExcluirPerfil();

int main() {

    //tela_Login();

    tela_Menu();

    //selecionarQuantidadeJogParaPartida();

    return 0;
}

void salvarJogador_txt(const string &nome, const string &senha, const string &perguntaSeguranca, const string &respostaPergunta)
{
    ofstream arquivo_jogadores("jogadores.txt", ios::app);
    if (arquivo_jogadores.is_open())
    {
        arquivo_jogadores << "Nome: " << nome << endl;
        arquivo_jogadores << "Senha: " << senha << endl;
        arquivo_jogadores << "Pergunta: " << perguntaSeguranca << endl;
        arquivo_jogadores << "Resposta: " << respostaPergunta << endl;
        arquivo_jogadores.close();
        system("cls");
        cout << "Jogador cadastrado com sucesso!" << endl;
        system("pause");
        tela_Login();
    }
    else
    {
        system("cls");
        cout << "Erro ao abrir o arquivo de jogadores." << endl;
        system("pause");
        tela_Login();
    }
}

void salvarDadosPartida_txt(int numeroPartida, string dataHora, vector<pair<string, string>> jogadoresCores, unordered_map<string, int> ranking) {
    ofstream arquivo("dadosPartida.txt", ios::app);
    if (arquivo.is_open()) {
        arquivo << "Partida " << numeroPartida << " - " << dataHora << " - ";
        for (const auto &jogadorCor : jogadoresCores) {
            arquivo << jogadorCor.first << " (" << jogadorCor.second << "), ";
        }
        arquivo << "- ";
        for (const auto &jogadorPontos : ranking) {
            arquivo << jogadorPontos.first << " - " << jogadorPontos.second << " pontos, ";
        }
        arquivo << endl;
        arquivo.close();
        system("cls");
        cout << "Dados da partida salvos com sucesso." << endl;
        system("pause");
        tela_Menu();
    } else {
        system("cls");
        cout << "Erro ao abrir o arquivo para salvar os dados da partida." << endl;
        system("pause");
        tela_Menu();
    }
}

void salvarVitoriasGeral(const unordered_map<string, int>& vitoriasGeral) {
    ofstream arquivo("rankingVitoriasPartidas.csv", ios::out);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de vitórias." << endl;
        return;
    }

    for (const auto& par : vitoriasGeral) {
        arquivo << par.first << "," << par.second << endl;
    }

    arquivo.close();
}

void salvarVitorias4Jogadores(const unordered_map<string, int>& vitorias4Jogadores) {
    ofstream arquivo("rankingVitorias4Jogadores.csv", ios::out);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de vitórias de 4 jogadores." << endl;
        return;
    }

    for (const auto& par : vitorias4Jogadores) {
        arquivo << par.first << "," << par.second << endl;
    }

    arquivo.close();
}

void salvarVitorias3Jogadores(const unordered_map<string, int>& vitorias3Jogadores) {
    ofstream arquivo("rankingVitorias3Jogadores.csv", ios::out);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de vitórias de 3 jogadores." << endl;
        return;
    }

    for (const auto& par : vitorias3Jogadores) {
        arquivo << par.first << "," << par.second << endl;
    }

    arquivo.close();
}

void salvarVitorias2Jogadores(const unordered_map<string, int>& vitorias2Jogadores) {
    ofstream arquivo("rankingVitorias2Jogadores.csv", ios::out);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de vitórias de 2 jogadores." << endl;
        return;
    }

    for (const auto& par : vitorias2Jogadores) {
        arquivo << par.first << "," << par.second << endl;
    }

    arquivo.close();
}

vector<string> lerCsvDadosPartidatoHistorico() {
    vector<string> linhasPartida;
    ifstream arquivo("DadosPartida.csv", ios::in);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de dados da partida." << endl;
        return linhasPartida;
    }

    string linha;
    while (getline(arquivo, linha)) {
        linhasPartida.push_back(linha);
    }

    arquivo.close();
    return linhasPartida;
}

void lerCsvDadosPartidatoRankingPontos(vector<pair<string, int>>& pontosJogadores) {
    ifstream arquivo("dadosPartida.csv", ios::in);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de dados da partida." << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha)) {
        // Verificar se a linha está vazia
        if (linha.empty()) {
            continue;
        }

        stringstream ss(linha);
        string campo;
        vector<string> dadosLinha;

        // Separar os campos da linha
        while (getline(ss, campo, '-')) {
            dadosLinha.push_back(campo);
        }

        if (dadosLinha.size() >= 3) {  // Verifica se a linha tem pelo menos 3 campos após a separação por '-'
            string jogadoresPontos = dadosLinha[2];
            stringstream ssJogadoresPontos(jogadoresPontos);
            string jogadorPonto;

            // Separar os jogadores e pontos
            while (getline(ssJogadoresPontos, jogadorPonto, ',')) {
                stringstream ssJogadorPonto(jogadorPonto);
                string nome;
                string pontosStr;

                getline(ssJogadorPonto, nome, '-');
                getline(ssJogadorPonto, pontosStr);

                try {
                    int pontos = stoi(pontosStr);
                    pontosJogadores.push_back(make_pair(nome, pontos));
                } catch (const exception& e) {
                    cout << "Erro ao processar a linha: " << linha << endl;
                }
            }
        } else {
            cout << "Erro: linha malformada no arquivo CSV: " << linha << endl;
        }
    }

    arquivo.close();
}

void lerCsvDadosPartidatoRankingPartidas(unordered_map<string, int>& vitoriasGeral, unordered_map<string, int>& vitorias4Jogadores, unordered_map<string, int>& vitorias3Jogadores, unordered_map<string, int>& vitorias2Jogadores) {
    ifstream arquivo("DadosPartida.csv", ios::in);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de dados da partida." << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string campo;
        vector<string> dadosLinha;

        while (getline(ss, campo, ',')) {
            dadosLinha.push_back(campo);
        }

        if (dadosLinha.size() >= 4) {  // Verifica se a linha tem pelo menos 4 campos
            int numJogadores;
            string vencedor;

            try {
                numJogadores = stoi(dadosLinha[0]);
                vencedor = dadosLinha[1];
            } catch (const invalid_argument& e) {
                cout << "Erro: valor inválido no campo de número de jogadores ou vencedor: " << linha << endl;
                continue;
            } catch (const out_of_range& e) {
                cout << "Erro: valor fora do alcance no campo de número de jogadores ou vencedor: " << linha << endl;
                continue;
            }

            // Atualiza o número de vitórias
            vitoriasGeral[vencedor]++;
            if (numJogadores == 4) {
                vitorias4Jogadores[vencedor]++;
            } else if (numJogadores == 3) {
                vitorias3Jogadores[vencedor]++;
            } else if (numJogadores == 2) {
                vitorias2Jogadores[vencedor]++;
            } else {
                cout << "Erro: número de jogadores inválido na linha: " << linha << endl;
            }
        } else {
            cout << "Erro: linha malformada no arquivo CSV: " << linha << endl;
        }
    }

    arquivo.close();
}

void salvarRankingPontuacao(const vector<pair<string, int>>& ranking) {
    vector<pair<string, int>> rankingOrdenado = ranking;
    
    sort(rankingOrdenado.begin(), rankingOrdenado.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return b.second < a.second;
    });

    ofstream arquivo("rankingPontuacao.csv", ios::out);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de pontuação." << endl;
        return;
    }

    for (const auto& par : rankingOrdenado) {
        arquivo << par.first << "," << par.second << endl;
    }
    
    arquivo.close();
}

void solicitarDadosJogador(string &nome, string &senha, string &perguntaSeguranca, string &respostaPergunta)
{
    cout << "\033[1;32mInforme um nome:\033[0m" << endl;
    cin >> nome;

    while (verificarNomeExistente(nome))
    {
        cout << "\033[1;32mInforme outro nome:\033[0m" << endl;
        cin >> nome;
    }

    string confirmarSenha;
    do
    {
        cout << "\033[1;32mInforme a sua senha (minimo de 6 caracteres):\033[0m " << endl;
        cin >> senha;
        if (!verificarTamanhoSenha(senha))
        {
            cout << "\033[1;31mA senha deve ter no minimo 6 caracteres.\033[0m" << endl;
        }
        else
        {
            cout << "\033[1;32mConfirme sua senha:\033[0m " << endl;
            cin >> confirmarSenha;
            if (!verificarSenhasIguais(senha, confirmarSenha))
            {
                cout << "\033[1;31mAs senhas nao coincidem.\033[0m" << endl;
            }
        }
    } while (!verificarTamanhoSenha(senha) || !verificarSenhasIguais(senha, confirmarSenha));

    cout << "\033[1;32mDigite uma pergunta que apenas voce saiba responder:\033[0m" << endl;
    cin.ignore();                    // Ignora o restante da linha anterior
    getline(cin, perguntaSeguranca); // Permite espaços na pergunta
    cout << "\033[1;32mInforme a resposta da pergunta:\033[0m" << endl;
    getline(cin, respostaPergunta); // Permite espaços na resposta

    string confirmarRespostaSenha;
    cout << "\033[1;32mConfirme a resposta da pergunta:\033[0m" << endl;
    getline(cin, confirmarRespostaSenha); // Permite espaços na confirmação

    while (!verificarRespostasIguais(respostaPergunta, confirmarRespostaSenha))
    {
        cout << "\033[1;31mAs respostas da pergunta de segurança nao coincidem.\033[0m" << endl;
        cout << "\033[1;32mInforme a resposta da pergunta:\033[0m" << endl;
        getline(cin, respostaPergunta); // Permite espaços na resposta
        cout << "\033[1;32mConfirme a resposta da pergunta:\033[0m" << endl;
        getline(cin, confirmarRespostaSenha); // Permite espaços na confirmação
    }
}

void salvarEditarPerfil(string nomeAtual, string senhaAtual)
{
    system("cls");

    ifstream arquivo_jogadores("jogadores.txt");
    ofstream arquivo_temporario("temporario_jogadores.txt");

    if (arquivo_jogadores.is_open() && arquivo_temporario.is_open())
    {
        string linha;
        bool jogadorEncontrado = false;

        while (getline(arquivo_jogadores, linha))
        {
            if (linha.find("Nome: " + nomeAtual) != string::npos)
            {
                string senhaSalva = ""; // Variável para armazenar a senha salva do usuário
                string perguntaSalva = "";
                string respostaSalva = "";
                // Encontrou o jogador a ser editado, verificar a senha
                while (getline(arquivo_jogadores, linha))
                {
                    if (linha.find("Senha: ") != string::npos)
                    {
                        senhaSalva = linha.substr(7); // Obter a senha salva do arquivo
                    }
                    else if (linha.find("Pergunta: ") != string::npos)
                    {
                        perguntaSalva = linha.substr(10); // Obter a pergunta salva do arquivo
                    }
                    else if (linha.find("Resposta: ") != string::npos)
                    {
                        respostaSalva = linha.substr(10); // Obter a resposta salva do arquivo
                        break;                            // Parar ao encontrar a resposta da pergunta
                    }
                }
                if (senhaAtual == senhaSalva)
                { // Verificar se a senha atual coincide
                    // Se coincide, solicitar as novas informações e editar o perfil
                    string novoNome, novaSenha, NovaPerguntaSeguranca, novaRespostaPergunta;
                    solicitarDadosJogador(novoNome, novaSenha, NovaPerguntaSeguranca, novaRespostaPergunta);

                    arquivo_temporario << "Nome: " << novoNome << endl;
                    arquivo_temporario << "Senha: " << novaSenha << endl;
                    arquivo_temporario << "Pergunta: " << NovaPerguntaSeguranca << endl;
                    arquivo_temporario << "Resposta: " << novaRespostaPergunta << endl;

                    jogadorEncontrado = true;
                }
                else
                {
                    // Senha atual incorreta, manter os dados originais
                    arquivo_temporario << linha << endl;
                    cout << "Senha incorreta, por favor tente novamente" << endl;
                    jogadorEncontrado = false;
                    break; // Saia do loop ao encontrar uma senha incorreta
                }
            }
            else
            {
                // Copiar outras linhas do arquivo original para o arquivo temporário
                arquivo_temporario << linha << endl;
            }
        }

        arquivo_jogadores.close();
        arquivo_temporario.close();

        if (jogadorEncontrado)
        {
            remove("jogadores.txt");
            rename("temporario_jogadores.txt", "jogadores.txt");
            system("cls");
            cout << "Perfil editado com sucesso!" << endl;
            system("pause");
            tela_Menu();
        }
        else
        {
            system("cls");
            cout << "Jogador nao encontrado" << endl;
            system("pause");
            tela_EditarPerfil();
        }
    }
    else
    {
        system("cls");
        cout << "Erro ao abrir os arquivos." << endl;
        system("pause");
        tela_Menu();
    }
}

void fecharJogo()
{
    exit(0); // Encerrar o programa
}

void validarLogin(string nome, string senha)
{
    ifstream arquivo_jogadores("jogadores.txt", ios::in);
    if (arquivo_jogadores.is_open())
    {
        string linha;
        string nomeSalvo, senhaSalva;

        bool loginValido = false;
        while (getline(arquivo_jogadores, linha))
        {
            if (linha.find("Nome: ") != string::npos)
            {
                nomeSalvo = linha.substr(6); // Removendo "Nome: " da linha
            }
            else if (linha.find("Senha: ") != string::npos)
            {
                senhaSalva = linha.substr(7); // Removendo "Senha: " da linha
                // Verifica se o nome e a senha correspondem aos dados salvos
                if (nome == nomeSalvo && senha == senhaSalva)
                {
                    loginValido = true;
                    break;
                }
            }
        }
        arquivo_jogadores.close();
        if (loginValido)
        {
            tela_Menu();
        }
        else
        {
            system("cls");
            cout << "Nome de usuario ou senha incorretos." << endl;
            system("pause");
            tela_Login();
        }
    }
    else
    {
        system("cls");
        cout << "Erro ao abrir o arquivo de jogadores." << endl;
        fecharJogo();
    }
}

bool verificarNomeExistente(const string &nome)
{
    ifstream arquivo("jogadores.txt");
    string linha;
    while (getline(arquivo, linha))
    {
        if (linha.find("Nome: " + nome) != string::npos)
        {
            cout << "Este nome ja existe. Por favor, escolha outro nome." << endl;
            return true;
        }
    }
    return false;
}

void tabelaRankingPontos() {

    atualizarTabelaPontos();

    ifstream arquivo("rankingPontuacao.csv", ios::in);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de pontuação." << endl;
        return;
    }

    string linha;
    int linhaNumero = 0;

    while (getline(arquivo, linha)) {
        if (linhaNumero == 0) {
            cout << "------- CAMPEAO DOS CAMPEOES -------" << endl;
            cout << linha << endl;
            cout << endl; // Pula uma linha após o primeiro jogador
        } else {
            cout << linhaNumero + 1 << " - " << linha << endl;
        }
        linhaNumero++;
    }

    arquivo.close();
    cout << endl << endl << "Pressione Enter para voltar ao Menu" << endl;
    cin.ignore(); // Ignora o caractere de nova linha restante no buffer
    cin.get();    // Espera pelo pressionamento de uma tecla
    tela_Menu();
}

void atualizarTabelaPontos() {
    // vector<pair<string, int>> pontosJogadores;
    // vector<pair<string, int>> ranking;

    // lerCsvDadosPartidatoRankingPontos(pontosJogadores);
    // somarPontosDosJogadores(pontosJogadores, ranking);
    // salvarRankingPontuacao(ranking);
}

void somarPontosDosJogadores(const vector<pair<string, int>>& pontosJogadores, vector<pair<string, int>>& ranking) {
    for (const auto& par : pontosJogadores) {
        auto it = find_if(ranking.begin(), ranking.end(), [&](const pair<string, int>& elemento) {
            return elemento.first == par.first;
        });
        
        if (it != ranking.end()) {
            it->second += par.second;
        } else {
            ranking.push_back(par);
        }
    }
}

void tabelaRankingVitoriasGeral() {
    atualizarTabelaVitorias();

    ifstream arquivo("rankingVitoriasPartidas.csv", ios::in);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de vitórias." << endl;
        return;
    }

    string linha;
    int linhaNumero = 0;

    while (getline(arquivo, linha)) {
        if (linhaNumero == 0) {
            cout << "------- REI GLADIADOR -------" << endl;
            cout << linha << endl;
            cout << endl; // Pula uma linha após o primeiro jogador
        } else {
            cout << linhaNumero + 1 << " - " << linha << endl;
        }
        linhaNumero++;
    }

    arquivo.close();

    arquivo.close();
    cout << endl << endl << "Pressione Enter para voltar ao Menu" << endl;
    cin.ignore(); // Ignora o caractere de nova linha restante no buffer
    cin.get();    // Espera pelo pressionamento de uma tecla
    tela_Menu();
}

void TabelaRankingVitorias4Jogadores() {
    atualizarTabelaVitorias();

    ifstream arquivo("rankingVitorias4Jogadores.csv", ios::in);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de vitórias de 4 jogadores." << endl;
        return;
    }

    string linha;
    int linhaNumero = 0;

    while (getline(arquivo, linha)) {
        if (linhaNumero == 0) {
            cout << "------- GRANDE GLADIADOR -------" << endl;
            cout << linha << endl;
            cout << endl; // Pula uma linha após o primeiro jogador
        } else {
            cout << linhaNumero + 1 << " - " << linha << endl;
        }
        linhaNumero++;
    }

    arquivo.close();

    arquivo.close();
    cout << endl << endl << "Pressione Enter para voltar ao Menu" << endl;
    cin.ignore(); // Ignora o caractere de nova linha restante no buffer
    cin.get();    // Espera pelo pressionamento de uma tecla
    tela_Menu();
}

void TabelaRankingVitorias3Jogadores() {
    atualizarTabelaVitorias();

    ifstream arquivo("rankingVitorias3Jogadores.csv", ios::in);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de vitórias de 3 jogadores." << endl;
        return;
    }

    string linha;
    int linhaNumero = 0;

    while (getline(arquivo, linha)) {
        if (linhaNumero == 0) {
            cout << "------- CHEFE GUERREIRO -------" << endl;
            cout << linha << endl;
            cout << endl; // Pula uma linha após o primeiro jogador
        } else {
            cout << linhaNumero + 1 << " - " << linha << endl;
        }
        linhaNumero++;
    }

    arquivo.close();

    arquivo.close();
    cout << endl << endl << "Pressione Enter para voltar ao Menu" << endl;
    cin.ignore(); // Ignora o caractere de nova linha restante no buffer
    cin.get();    // Espera pelo pressionamento de uma tecla
    tela_Menu();
}

void TabelaRankingVitorias2Jogadores() {
    atualizarTabelaVitorias();

    ifstream arquivo("rankingVitorias2Jogadores.csv", ios::in);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo de ranking de vitórias de 2 jogadores." << endl;
        return;
    }

    string linha;
    int linhaNumero = 0;

    while (getline(arquivo, linha)) {
        if (linhaNumero == 0) {
            cout << "------- MESTRE DO X1 -------" << endl;
            cout << linha << endl;
            cout << endl; // Pula uma linha após o primeiro jogador
        } else {
            cout << linhaNumero + 1 << " - " << linha << endl;
        }
        linhaNumero++;
    }

    arquivo.close();
        
    arquivo.close();
    cout << endl << endl << "Pressione Enter para voltar ao Menu" << endl;
    cin.ignore(); // Ignora o caractere de nova linha restante no buffer
    cin.get();    // Espera pelo pressionamento de uma tecla
    tela_Menu();
}

void atualizarTabelaVitorias() {

    // unordered_map<string, int> vitoriasGeral;
    // unordered_map<string, int> vitorias4Jogadores;
    // unordered_map<string, int> vitorias3Jogadores;
    // unordered_map<string, int> vitorias2Jogadores;

    // lerCsvDadosPartidatoRankingPartidas(vitoriasGeral, vitorias4Jogadores, vitorias3Jogadores, vitorias2Jogadores);

    // salvarVitoriasGeral(vitoriasGeral);
    // salvarVitorias4Jogadores(vitorias4Jogadores);
    // salvarVitorias3Jogadores(vitorias3Jogadores);
    // salvarVitorias2Jogadores(vitorias2Jogadores);
}

bool verificarTamanhoSenha(const string &senha)
{
    return senha.length() >= 6;
}

void exibirHistorico(const string& nomeJogador, const vector<string>& linhasPartida) {
    cout << "Historico de partidas para o jogador: " << nomeJogador << endl;

    for (const auto& linha : linhasPartida) {
        if (linha.find(nomeJogador) != string::npos) {
            cout << linha << endl;
        }
    }
}

bool verificarSenhasIguais(const string &senha, const string &confirmarSenha)
{
    return senha == confirmarSenha;
}

bool verificarRespostasIguais(const string &resposta, const string &confirmarResposta)
{
    return resposta == confirmarResposta;
}

void excluirPerfil(const string &nome, const string &senha, const string &respostaPergunta)
{
    ifstream arquivo_jogadores("jogadores.txt");
    ofstream arquivo_temporario("temporario_jogadores.txt");

    if (arquivo_jogadores.is_open() && arquivo_temporario.is_open())
    {
        string linha;
        bool jogadorEncontrado = false;

        while (getline(arquivo_jogadores, linha))
        {
            if (linha.find("Nome: " + nome) != string::npos)
            {
                string senhaSalva = "";
                string perguntaSalva = "";
                string respostaSalva = "";
                // Encontrou o jogador a ser excluído, verificar a senha e a resposta da pergunta
                while (getline(arquivo_jogadores, linha))
                {
                    if (linha.find("Senha: ") != string::npos)
                    {
                        senhaSalva = linha.substr(7); // Obter a senha salva do arquivo
                    }
                    else if (linha.find("Pergunta: ") != string::npos)
                    {
                        perguntaSalva = linha.substr(10); // Obter a pergunta salva do arquivo
                    }
                    else if (linha.find("Resposta: ") != string::npos)
                    {
                        respostaSalva = linha.substr(10); // Obter a resposta salva do arquivo
                        break;                            // Parar ao encontrar a resposta da pergunta
                    }
                }
                if (senha == senhaSalva && respostaPergunta == respostaSalva)
                { // Verificar se a senha e a resposta coincidem
                    jogadorEncontrado = true;
                    // Não copiar o perfil para o arquivo temporário, efetivamente excluindo-o
                }
                else
                {
                    // Dados incorretos, manter os dados originais
                    cout << "Senha ou resposta da pergunta de segurança incorretas, por favor tente novamente." << endl;
                    arquivo_temporario << "Nome: " << nome << endl;
                    arquivo_temporario << "Senha: " << senhaSalva << endl;
                    arquivo_temporario << "Pergunta: " << perguntaSalva << endl;
                    arquivo_temporario << "Resposta: " << respostaSalva << endl;
                }
            }
            else
            {
                // Copiar outras linhas do arquivo original para o arquivo temporário
                arquivo_temporario << linha << endl;
            }
        }

        arquivo_jogadores.close();
        arquivo_temporario.close();

        if (jogadorEncontrado)
        {
            remove("jogadores.txt");
            rename("temporario_jogadores.txt", "jogadores.txt");
            system("cls");
            cout << "Perfil excluído com sucesso!" << endl;
            system("pause");
            tela_Login();
        }
        else
        {
            remove("temporario_jogadores.txt"); // Remover o arquivo temporário se o jogador não foi encontrado
            system("cls");
            cout << "Perfil não encontrado ou dados incorretos." << endl;
            system("pause");
            tela_ExcluirPerfil();
        }
    }
    else
    {
        system("cls");
        cout << "Erro ao abrir os arquivos." << endl;
        system("pause");
        tela_Menu();
    }
}

int selecionarQuantidadeJogParaPartida() {
    int num_jogadores;
    do {
        cout << "Digite a quantidade de jogadores (entre 2 e 4): ";
        cin >> num_jogadores;
        if (num_jogadores < 2 || num_jogadores > 4) {
            cout << "Quantidade de jogadores invalida. Por favor, escolha entre 2 e 4 jogadores.\n";
        }
    } while (num_jogadores < 2 || num_jogadores > 4);
    selecionarJogadoresECoresParaPartida(num_jogadores);
    return num_jogadores; // Adicionado retorno do número de jogadores
}

void selecionarJogadoresECoresParaPartida(int num_jogadores) {
    limparTela();

    if (num_jogadores < 2 || num_jogadores > 4) {
        cout << "Numero de jogadores invalido. Por favor, escolha entre 2 e 4 jogadores.\n";
        return;
    }

    ifstream arquivo_jogadores("jogadores.txt", ios::in);
    vector<pair<string, string>> jogadores;

    if (arquivo_jogadores.is_open()) {
        string linha;
        string nome, senha;

        while (getline(arquivo_jogadores, linha)) {
            if (linha.find("Nome: ") != string::npos) {
                nome = trim(linha.substr(6));
                if (getline(arquivo_jogadores, linha) && linha.find("Senha: ") != string::npos) {
                    senha = trim(linha.substr(7));
                    jogadores.push_back(make_pair(nome, senha));
                }
            }
        }

        arquivo_jogadores.close();

        sort(jogadores.begin(), jogadores.end());

        cout << "Lista de Jogadores:\n";
        for (size_t i = 0; i < jogadores.size(); ++i) {
            cout << i + 1 << ". " << jogadores[i].first << "\n";
        }

        vector<pair<string, string>> coresDisponiveis = {
            {"\033[31mVermelho\033[0m", "Vermelho"},
            {"\033[32mVerde\033[0m", "Verde"},
            {"\033[34mAzul\033[0m", "Azul"},
            {"\033[33mAmarelo\033[0m", "Amarelo"}
        };

        vector<string> jogadoresSelecionados;
        vector<string> coresSelecionadas;

        for (int i = 0; i < num_jogadores; ++i) {
            string nomePesquisado;
            cout << "\nDigite o nome do jogador " << i + 1 << ": ";
            cin >> nomePesquisado;

            auto it = find_if(jogadores.begin(), jogadores.end(), [&nomePesquisado](const pair<string, string> &jogador) {
                return jogador.first == nomePesquisado;
            });

            if (it != jogadores.end()) {
                string senha;
                cout << "Digite a senha do jogador " << nomePesquisado << ": ";
                cin >> senha;

                if (senha == it->second) {
                    cout << "Jogador " << it->first << " adicionado." << endl;
                    jogadoresSelecionados.push_back(it->first);
                    jogadores.erase(it);
                } else {
                    cout << "Senha incorreta.\n";
                    --i; // Repetir a iteração para permitir que o usuário insira o nome e a senha novamente
                }
            } else {
                cout << "Jogador nao encontrado.\n";
                --i; // Repetir a iteração para permitir que o usuário insira o nome novamente
            }
        }

        cout << "\nAssocie as cores aos jogadores:\n";
        for (size_t i = 0; i < jogadoresSelecionados.size(); ++i) {
            limparTela();

            cout << "Escolha a cor para o jogador " << jogadoresSelecionados[i] << ":\n";

            // Mostrar cores disponíveis
            cout << "Cores disponiveis:\n";
            for (size_t j = 0; j < coresDisponiveis.size(); ++j) {
                cout << j + 1 << ". " << coresDisponiveis[j].first << "\n";
            }

            size_t corEscolhida;
            do {
                cout << "Selecione o numero da sua cor: ";
                cin >> corEscolhida;

                // Verificar se a cor escolhida está dentro do intervalo válido
                if (corEscolhida < 1 || corEscolhida > coresDisponiveis.size()) {
                    cout << "Opcao invalida. Por favor, escolha uma cor valida.\n";
                }
            } while (corEscolhida < 1 || corEscolhida > coresDisponiveis.size());

            // Adicionar a cor selecionada à lista de cores selecionadas
            coresSelecionadas.push_back(coresDisponiveis[corEscolhida - 1].second);

            // Remover a cor escolhida das cores disponíveis
            coresDisponiveis.erase(coresDisponiveis.begin() + corEscolhida - 1);
        }

        // Chamando tela_Jogar() passando os vetores preenchidos
        tela_Jogar(jogadoresSelecionados, coresSelecionadas);
    } else {
        cout << "Erro ao abrir o arquivo de jogadores.\n";
        return;
    }
}

void imprimirTabuleiroColorido(const vector<vector<char>> &tabuleiro){
    // percursos
    unordered_map<char, string> cores_min = {
        {'r', "\033[1;31m"},      // Vermelho
        {'b', "\033[1;34m"},      // Azul
        {'y', "\033[1;33m"},      // Amarelo
        {'g', "\033[1;32m"},      // Verde
        {';', "\033[1;35m"},      // Roxo
        {':', "\033[1;38;5;208m"} // Laranja
    };
    // peões
    unordered_map<char, string> cores_mai = {
        {'R', "\033[1;31m"}, // Vermelho
        {'B', "\033[1;34m"}, // Azul
        {'Y', "\033[1;33m"}, // Amarelo
        {'G', "\033[1;32m"}, // Verde
    };
    
    // Adicionando correspondência de cores para as letras A, B, C, D nas tocas
    unordered_map<char, string> cores_peoes_toca = {
        {'A', "\033[1;31m"}, // Vermelho
        {'B', "\033[1;31m"}, // Vermelho
        {'C', "\033[1;31m"}, // Vermelho
        {'D', "\033[1;31m"}, // Vermelho
        {'A', "\033[1;34m"}, // Azul
        {'B', "\033[1;34m"}, // Azul
        {'C', "\033[1;34m"}, // Azul
        {'D', "\033[1;34m"}, // Azul
        {'A', "\033[1;33m"}, // Amarelo
        {'B', "\033[1;33m"}, // Amarelo
        {'C', "\033[1;33m"}, // Amarelo
        {'D', "\033[1;33m"}, // Amarelo
        {'A', "\033[1;32m"}, // Verde
        {'B', "\033[1;32m"}, // Verde
        {'C', "\033[1;32m"}, // Verde
        {'D', "\033[1;32m"}  // Verde
    };

    // tabuleiro
    for (const auto &linha : tabuleiro)
    {
        for (char celula : linha)
        {
            if (cores_min.find(celula) != cores_min.end())
            {
                cout << cores_min[celula] << celula << "\033[0m ";
            }
            else if (cores_mai.find(celula) != cores_mai.end())
            {
                cout << cores_mai[celula] << celula << "\033[0m ";
            }
            else if (cores_peoes_toca.find(celula) != cores_peoes_toca.end())
            {
                cout << cores_peoes_toca[celula] << celula << "\033[0m ";
            }
            else
            {
                cout << celula << " ";
            }
        }
        cout << endl;
    }
}

vector<Celula> definirPercursoVermelho() {
    return {
        {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {1, 7}, {1, 8},
        {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13}, {7, 13},
        {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9}, {8, 8}, {9, 8}, {10, 8}, {11, 8}, {12, 8}, {13, 8},
        {13, 7}, {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6}, {8, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2},
        {8, 1}, {7, 1}, {6, 1}, {6, 2}
    };
}

vector<Celula> definirPercursoAzul() {
    return {
        {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13}, {7, 13},
        {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9}, {8, 8}, {9, 8}, {10, 8}, {11, 8}, {12, 8}, {13, 8},
        {13, 7}, {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6}, {8, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2},
        {8, 1}, {7, 1}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6},
        {1, 6}, {1, 7}, {1, 8}, {2, 8}
    };
}

vector<Celula> definirPercursoAmarelo() {
    return {
        {8, 12}, {8, 11}, {8, 10}, {8, 9}, {8, 8}, {9, 8}, {10, 8}, {11, 8}, {12, 8}, {13, 8}, {13, 7},
        {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6}, {8, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2}, {8, 1},
        {7, 1}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6},
        {1, 7}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12},
        {6, 13}, {7, 13}, {8, 13}, {8, 12}
    };
}

vector<Celula> definirPercursoVerde() {
    return {
        {12, 6}, {11, 6}, {10, 6}, {9, 6}, {8, 6}, {8, 5}, {8, 4}, {8, 3}, {8, 2}, {8, 1}, {7, 1},
        {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6},
        {1, 7}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12},
        {6, 13}, {7, 13}, {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9}, {8, 8}, {9, 8}, {10, 8},
        {11, 8}, {12, 8}, {13, 8}, {13, 7}, {13, 6}, {12, 6}
    };
}

Celula calcularNovaPosicao(const Celula &peao, int movimentos, const vector<Celula> &percurso) {
    int posicaoAtual = 0;
    for (size_t i = 0; i < percurso.size(); ++i) {
        if (percurso[i].linha == peao.linha && percurso[i].coluna == peao.coluna) {
            posicaoAtual = i;
            break;
        }
    }

    int novaPosicao = (posicaoAtual + movimentos) % percurso.size();
    return percurso[novaPosicao];
}

string trim(const string &str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int lancarDado()
{                          // Função para rolar o dado
    return rand() % 6 + 1; // Gera um número aleatório de 1 a 6
}

void atualizarTabuleiro(vector<vector<char>> &tabuleiro, const unordered_map<char, vector<Celula>> &posicoesPecas) {
    // Limpa o tabuleiro, mantendo as casas especiais intactas
    for (int i = 1; i < TAMANHO_TABULEIRO - 1; ++i) {
        for (int j = 1; j < TAMANHO_TABULEIRO - 1; ++j) {
            if (tabuleiro[i][j] != ':' && tabuleiro[i][j] != ';' && tabuleiro[i][j] != 'F' &&
                tabuleiro[i][j] != 'r' && tabuleiro[i][j] != 'b' && tabuleiro[i][j] != 'y' && tabuleiro[i][j] != 'g') {
                tabuleiro[i][j] = ' ';
            }
        }
    }

    // Redesenha as peças nas novas posições
    for (const auto &par : posicoesPecas) {
        char cor = par.first;
        for (const Celula &celula : par.second) {
            tabuleiro[celula.linha][celula.coluna] = cor;
        }
    }
}

bool moverPeao(vector<vector<char>> &tabuleiro, char cor, int movimentos, const vector<Celula> &percurso, unordered_map<char, vector<Celula>> &posicoesPecas) {
    Celula peao;
    bool encontrado = false;

    for (auto &pos : posicoesPecas[cor]) {
        peao = pos;
        encontrado = true;
        break;
    }

    if (!encontrado) {
        cout << "Peao da cor " << cor << " nao encontrado no tabuleiro." << endl;
        return false;
    }

    Celula novaPosicao = calcularNovaPosicao(peao, movimentos, percurso);

    if (!verificarPosicao(tabuleiro, novaPosicao.linha, novaPosicao.coluna, cor, posicoesPecas)) {
        return false;
    }

    // Remover a peça da posição antiga
    posicoesPecas[cor].erase(remove(posicoesPecas[cor].begin(), posicoesPecas[cor].end(), peao), posicoesPecas[cor].end());

    // Adicionar a peça na nova posição
    posicoesPecas[cor].push_back(novaPosicao);

    // Atualizar o tabuleiro
    atualizarTabuleiro(tabuleiro, posicoesPecas);

    return true;
}

vector<vector<char>> criarTabuleiro()
{
    vector<vector<char>> tabuleiro(TAMANHO_TABULEIRO, vector<char>(TAMANHO_TABULEIRO, ' '));
    // bordas do tabuleiro
    for (int i = 0; i < TAMANHO_TABULEIRO; ++i)
    {
        tabuleiro[0][i] = '*';                     // Linha superior
        tabuleiro[TAMANHO_TABULEIRO - 1][i] = '*'; // Linha inferior
        tabuleiro[i][0] = '*';                     // Coluna esquerda
        tabuleiro[i][TAMANHO_TABULEIRO - 1] = '*'; // Coluna direita
    }
    // onde andam os peões
    for (int i = 1; i < TAMANHO_TABULEIRO - 1; ++i)
    {
        if (i < TAMANHO_TABULEIRO - 1)
        {
            tabuleiro[i][6] = '.';
        }
        if (i == 2)
        {
            tabuleiro[i - 1][7] = '.';
        }
        if (i == 2)
        {
            tabuleiro[TAMANHO_TABULEIRO - i][7] = '.';
        }
        if (i < TAMANHO_TABULEIRO - 1)
        {
            tabuleiro[i][8] = '.';
        }
    }

    for (int i = 1; i < TAMANHO_TABULEIRO - 1; ++i)
    {
        if (i < TAMANHO_TABULEIRO - 1)
        {
            tabuleiro[6][i] = '.';
        }
        if (i == 1)
        {
            tabuleiro[7][i] = '.';
        }
        if (i == 2)
        {
            tabuleiro[7][TAMANHO_TABULEIRO - i] = '.';
        }
        if (i < TAMANHO_TABULEIRO - 1)
        {
            tabuleiro[8][i] = '.';
        }
    }
    // estrutura das bases
    // vermelho
    tabuleiro[2][3] = '|';
    tabuleiro[4][3] = '|';
    tabuleiro[3][2] = '-';
    tabuleiro[3][3] = '+';
    tabuleiro[3][4] = '-';
    // azul
    tabuleiro[2][11] = '|';
    tabuleiro[4][11] = '|';
    tabuleiro[3][10] = '-';
    tabuleiro[3][11] = '+';
    tabuleiro[3][12] = '-';
    // verde
    tabuleiro[10][3] = '|';
    tabuleiro[12][3] = '|';
    tabuleiro[11][2] = '-';
    tabuleiro[11][3] = '+';
    tabuleiro[11][4] = '-';
    // amarelo
    tabuleiro[10][11] = '|';
    tabuleiro[12][11] = '|';
    tabuleiro[11][10] = '-';
    tabuleiro[11][11] = '+';
    tabuleiro[11][12] = '-';
    // casas iniciais
    tabuleiro[6][2] = 'r';  // Linha 6, Coluna 2
    tabuleiro[2][8] = 'b';  // Linha 2, Coluna 8
    tabuleiro[8][12] = 'y'; // Linha 8, Coluna 12
    tabuleiro[12][6] = 'g'; // Linha 12, Coluna 6
    // zonas seguras - reta final de cada cor
    for (int i = 2; i <= 6; ++i)
    {
        tabuleiro[7][i] = 'r'; // Linha 7, Colunas 2 a 6
        tabuleiro[i][7] = 'b'; // Coluna 7, Linhas 2 a 6
    }
    for (int i = 8; i <= 12; ++i)
    {
        tabuleiro[7][i] = 'y'; // Linha 7, Colunas 8 a 12
        tabuleiro[i][7] = 'g'; // Coluna 7, Linhas 8 a 12
    }
    // casa coroa
    tabuleiro[8][4] = ':';  // Linha 8, Coluna 4
    tabuleiro[4][6] = ':';  // Linha 4, Coluna 6
    tabuleiro[6][10] = ':'; // Linha 6, Coluna 10
    tabuleiro[10][8] = ':'; // Linha 10, Coluna 8
    // casa vingança
    tabuleiro[8][1] = ';';  // Linha 8, Coluna 1
    tabuleiro[1][6] = ';';  // Linha 1, Coluna 6
    tabuleiro[6][13] = ';'; // Linha 6, Coluna 13
    tabuleiro[13][8] = ';'; // Linha 13, Coluna 8
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
    // casa final
    tabuleiro[CENTRO_TABULEIRO][CENTRO_TABULEIRO] = 'F';
    // peões base vermelha
    tabuleiro[2][2] = 'A'; // Linha 2, Coluna 2
    tabuleiro[2][4] = 'B'; // Linha 2, Coluna 4
    tabuleiro[4][2] = 'C'; // Linha 4, Coluna 2
    tabuleiro[4][4] = 'D'; // Linha 4, Coluna 4
    // peões base azul
    tabuleiro[2][10] = 'A'; // Linha 2, Coluna 10
    tabuleiro[2][12] = 'B'; // Linha 2, Coluna 12
    tabuleiro[4][10] = 'C'; // Linha 4, Coluna 10
    tabuleiro[4][12] = 'D'; // Linha 4, Coluna 12
    // peões base verde
    tabuleiro[10][2] = 'A'; // Linha 10, Coluna 2
    tabuleiro[10][4] = 'B'; // Linha 10, Coluna 4
    tabuleiro[12][2] = 'C'; // Linha 12, Coluna 2
    tabuleiro[12][4] = 'D'; // Linha 12, Coluna 4
    // peões base amarela
    tabuleiro[10][10] = 'A'; // Linha 10, Coluna 10
    tabuleiro[10][12] = 'B'; // Linha 10, Coluna 12
    tabuleiro[12][10] = 'C'; // Linha 12, Coluna 10
    tabuleiro[12][12] = 'D'; // Linha 12, Coluna 12
    return tabuleiro;
}

bool realizarJogada(vector<vector<char>> &tabuleiro, char cor, unordered_map<char, vector<Celula>> &posicoesPecas) {
    int movimentos = lancarDado();
    cout << "Jogador " << cor << " tirou " << movimentos << " no dado." << endl;

    if (movimentos == 6) {
        if (!existePecaNoTabuleiro(tabuleiro, cor)) {
            cout << "Voce tirou 6, mas nao tem pecas no tabuleiro. Pressione Enter para retirar uma peca da toca." << endl;
            cin.ignore();
            cin.get();
            retirarPecaDaToca(tabuleiro, cor, posicoesPecas);
            posicoesPecas[cor].push_back(Celula(6, 2)); // Atualize a posição inicial conforme necessário
        } else {
            char opcao;
            cout << "Digite M (Mover peca) - R (Retirar peca da Casa)" << endl << "Opcao: ";
            cin >> opcao;

            if (opcao == 'r' || opcao == 'R') {
                retirarPecaDaToca(tabuleiro, cor, posicoesPecas);
                posicoesPecas[cor].push_back(Celula(6, 2)); // Atualize a posição inicial conforme necessário
            } else {
                if (cor == 'R') {
                    if (moverPeao(tabuleiro, cor, movimentos, definirPercursoVermelho(), posicoesPecas)) {
                        return true;
                    }
                } else if (cor == 'B') {
                    if (moverPeao(tabuleiro, cor, movimentos, definirPercursoAzul(), posicoesPecas)) {
                        return true;
                    }
                } else if (cor == 'Y') {
                    if (moverPeao(tabuleiro, cor, movimentos, definirPercursoAmarelo(), posicoesPecas)) {
                        return true;
                    }
                } else if (cor == 'G') {
                    if (moverPeao(tabuleiro, cor, movimentos, definirPercursoVerde(), posicoesPecas)) {
                        return true;
                    }
                }
            }
        }
    } else {
        if (cor == 'R') {
            if (moverPeao(tabuleiro, cor, movimentos, definirPercursoVermelho(), posicoesPecas)) {
                return true;
            }
        } else if (cor == 'B') {
            if (moverPeao(tabuleiro, cor, movimentos, definirPercursoAzul(), posicoesPecas)) {
                return true;
            }
        } else if (cor == 'Y') {
            if (moverPeao(tabuleiro, cor, movimentos, definirPercursoAmarelo(), posicoesPecas)) {
                return true;
            }
        } else if (cor == 'G') {
            if (moverPeao(tabuleiro, cor, movimentos, definirPercursoVerde(), posicoesPecas)) {
                return true;
            }
        }
    }

    // Atualize o tabuleiro com as novas posições das peças
    atualizarTabuleiro(tabuleiro, posicoesPecas);
    return false;
}

bool verificarPosicao(vector<vector<char>> &tabuleiro, int linha, int coluna, char cor, unordered_map<char, vector<Celula>> &posicoesPecas) {
    char celula = tabuleiro[linha][coluna];
    if (celula == ' ' || celula == '.') {
        return true;
    } else if (celula == cor) {
        return false;
    } else {
        capturarPeoesAdversarios(tabuleiro, linha, coluna, celula, posicoesPecas);
        return true;
    }
}

void capturarPeoesAdversarios(vector<vector<char>> &tabuleiro, int linha, int coluna, char corAdversario, unordered_map<char, vector<Celula>> &posicoesPecas) {
    auto toca = posicoesTocas.at(corAdversario);
    if (tabuleiro[linha][coluna] == corAdversario) {
        tabuleiro[linha][coluna] = '.';
        tabuleiro[toca.first][toca.second] = corAdversario;
        posicoesPecas[corAdversario].push_back(Celula(toca.first, toca.second));
    }
}

bool existePecaNoTabuleiro(const vector<vector<char>> &tabuleiro, char cor)
{
    for (const auto &linha : tabuleiro)
    {
        for (char celula : linha)
        {
            if (celula == cor)
            {
                return true;
            }
        }
    }
    return false;
}

void retirarPecaDaToca(vector<vector<char>> &tabuleiro, char cor, unordered_map<char, vector<Celula>> &posicoesPecas) {
    if (cor == 'R') {
        if (tabuleiro[2][2] == 'A') {
            tabuleiro[2][2] = ' ';
            tabuleiro[6][2] = 'A';
        } else if (tabuleiro[2][4] == 'B') {
            tabuleiro[2][4] = ' ';
            tabuleiro[6][2] = 'B';
        } else if (tabuleiro[4][2] == 'C') {
            tabuleiro[4][2] = ' ';
            tabuleiro[6][2] = 'C';
        } else if (tabuleiro[4][4] == 'D') {
            tabuleiro[4][4] = ' ';
            tabuleiro[6][2] = 'D';
        }
    } else if (cor == 'B') {
        if (tabuleiro[2][10] == 'A') {
            tabuleiro[2][10] = ' ';
            tabuleiro[2][8] = 'A';
        } else if (tabuleiro[2][12] == 'B') {
            tabuleiro[2][12] = ' ';
            tabuleiro[2][8] = 'B';
        } else if (tabuleiro[4][10] == 'C') {
            tabuleiro[4][10] = ' ';
            tabuleiro[2][8] = 'C';
        } else if (tabuleiro[4][12] == 'D') {
            tabuleiro[4][12] = ' ';
            tabuleiro[2][8] = 'D';
        }
    } else if (cor == 'Y') {
        if (tabuleiro[10][10] == 'A') {
            tabuleiro[10][10] = ' ';
            tabuleiro[8][12] = 'A';
        } else if (tabuleiro[10][12] == 'B') {
            tabuleiro[10][12] = ' ';
            tabuleiro[8][12] = 'B';
        } else if (tabuleiro[12][10] == 'C') {
            tabuleiro[12][10] = ' ';
            tabuleiro[8][12] = 'C';
        } else if (tabuleiro[12][12] == 'D') {
            tabuleiro[12][12] = ' ';
            tabuleiro[8][12] = 'D';
        }
    } else if (cor == 'G') {
        if (tabuleiro[10][2] == 'A') {
            tabuleiro[10][2] = ' ';
            tabuleiro[12][6] = 'A';
        } else if (tabuleiro[10][4] == 'B') {
            tabuleiro[10][4] = ' ';
            tabuleiro[12][6] = 'B';
        } else if (tabuleiro[12][2] == 'C') {
            tabuleiro[12][2] = ' ';
            tabuleiro[12][6] = 'C';
        } else if (tabuleiro[12][4] == 'D') {
            tabuleiro[12][4] = ' ';
            tabuleiro[12][6] = 'D';
        }
    }
}

void limparTela()
{
    system("cls");
}

void tela_Login()
{
    system("cls");

    string nome, senha;

    cout << "\033[1;31m==============================================\033[0m" << endl;
    cout << "             BEM-VINDO AO LUDO PLUS              " << endl;
    cout << "\033[1;31m==============================================\033[0m" << endl;
    cout << "\033[1;37m                                              \033[0m" << endl;
    cout << "\033[1;37m             [1] - FAZER LOGIN              \033[0m" << endl;
    cout << "\033[1;37m             [2] - CADASTRAR                \033[0m" << endl;
    cout << "\033[1;37m             [3] - SAIR                     \033[0m" << endl;
    cout << "\033[1;37m             [4] - RECUPERAR SENHA           \033[0m" << endl;
    cout << "\033[1;37m                                              \033[0m" << endl;
    cout << "\033[1;31m==============================================\033[0m" << endl;

    int opcao;
    cin >> opcao;

    if (opcao == 1)
    {
        cout << "Digite seu nome: ";
        cin >> nome;
        cout << "Digite sua senha: ";
        cin >> senha;
        validarLogin(nome, senha); // Chama o metodo para validar o login
    }
    else if (opcao == 2)
    {
        tela_Cadastrar();
    }
    else if (opcao == 3)
    {
        fecharJogo();
    }
    else if (opcao == 4)
    {
        tela_RecuperarSenha();
    }
    else
    {
        system("cls");
        cout << "Opcao invalida! \nTente Novamente ";
        system("pause");
        tela_Login();
    }
}

void tela_Menu() {
    limparTela();

    int opcao_Menu;

    cout << "\033[1;31m==============================================\033[0m" << endl;
    cout << "                  LUDO PLUS                  " << endl;
    cout << "\033[1;31m==============================================\033[0m" << endl;
    cout << "                                                 " << endl;
    cout << "             [1] - Jogar                   " << endl;
    cout << "             [2] - Ranking           " << endl;
    cout << "             [3] - Historico das Partidas         " << endl;
    cout << "             [4] - Editar/excluir perfil   " << endl;
    cout << "             [5] - Regras do Jogo   " << endl;
    cout << "             [6] - voltar ao login             " << endl;
    cout << "                                                 " << endl;
    cout << "\033[1;31m==============================================\033[0m" << endl;

    cin >> opcao_Menu;

    if (opcao_Menu == 1) {
        selecionarQuantidadeJogParaPartida();
    } else if (opcao_Menu == 2) {
        tela_Ranking();
    } else if (opcao_Menu == 3) {
        tela_Historico();
    } else if (opcao_Menu == 4) {
        tela_EditarPerfil();
    } else if (opcao_Menu == 5) {
        tela_Regras();
    } else if (opcao_Menu == 6) {
        tela_Login();
    } else {
        limparTela();
        cout << "opção invalida, tente novamente";
        system("pause");
        tela_Menu();
    }
}

void tela_Cadastrar()
{
    system("cls"); // Limpa o console antes de exibir a tela de cadastro

    string nome, senha, perguntaSeguranca, respostaPergunta;
    solicitarDadosJogador(nome, senha, perguntaSeguranca, respostaPergunta);

    // Salvar o jogador
    salvarJogador_txt(nome, senha, perguntaSeguranca, respostaPergunta);
}

void Tela_Jogar_MostrarTempoNaTela()
{

    time_t tempoAtual = time(nullptr);

    int segundosDecorridos = difftime(tempoAtual, tempoInicial);

    cout << endl
         << "TIMER DO JOGO: " << segundosDecorridos << endl;
}

void tela_Jogar(const vector<string> &nomesJogadores, const vector<string> &coresJogadores) {
    srand(time(0));
    vector<vector<char>> tabuleiro = criarTabuleiro();
    bool jogoTerminado = false;
    int rodada = 0;
    int numJogadores = nomesJogadores.size();
    vector<char> cores;

    // Mapear cores dos jogadores
    for (const auto &cor : coresJogadores) {
        if (cor == "Vermelho") {
            cores.push_back('R');
        } else if (cor == "Verde") {
            cores.push_back('G');
        } else if (cor == "Azul") {
            cores.push_back('B');
        } else if (cor == "Amarelo") {
            cores.push_back('Y');
        }
    }

    // Inicializar posicoesPecas conforme necessário
    unordered_map<char, vector<Celula>> posicoesPecas = {
        {'R', {Celula(2, 2), Celula(2, 4), Celula(4, 2), Celula(4, 4)}},
        {'B', {Celula(2, 10), Celula(2, 12), Celula(4, 10), Celula(4, 12)}},
        {'G', {Celula(10, 2), Celula(10, 4), Celula(12, 2), Celula(12, 4)}},
        {'Y', {Celula(10, 10), Celula(10, 12), Celula(12, 10), Celula(12, 12)}}
    };

    // Definir os percursos de cada cor
    vector<Celula> percursoVermelho = definirPercursoVermelho();
    vector<Celula> percursoAzul = definirPercursoAzul();
    vector<Celula> percursoAmarelo = definirPercursoAmarelo();
    vector<Celula> percursoVerde = definirPercursoVerde();

    while (!jogoTerminado) {
        // Limpar a tela e imprimir o tabuleiro
        limparTela();
        imprimirTabuleiroColorido(tabuleiro);

        // Jogar
        char corAtual = cores[rodada % numJogadores];
        cout << "Vez do jogador " << corAtual << " (" << nomesJogadores[rodada % numJogadores] << ")" << endl;
        cout << "Pressione qualquer tecla para rolar o dado..." << endl;
        _getch(); // Esperar pelo pressionamento de uma tecla

        int movimentos = lancarDado();
        cout << "Jogador " << corAtual << " tirou " << movimentos << " no dado." << endl;

        if (movimentos == 6) {
            if (!existePecaNoTabuleiro(tabuleiro, corAtual)) {
                cout << "Voce tirou 6, mas nao tem pecas no tabuleiro. Pressione Enter para retirar uma peca da toca." << endl;
                cin.ignore();
                cin.get();
                retirarPecaDaToca(tabuleiro, corAtual, posicoesPecas);
                posicoesPecas[corAtual].push_back(Celula(6, 2)); // Atualize a posição inicial conforme necessário
            } else {
                char opcao;
                cout << "M (Mover peca) - R (Retirar peca da Casa)" << endl << "Opcao: ";
                cin >> opcao;

                if (opcao == 'r' || opcao == 'R') {
                    retirarPecaDaToca(tabuleiro, corAtual, posicoesPecas);
                    posicoesPecas[corAtual].push_back(Celula(6, 2)); // Atualize a posição inicial conforme necessário
                } else {
                    if (corAtual == 'R') {
                        jogoTerminado = moverPeao(tabuleiro, corAtual, movimentos, percursoVermelho, posicoesPecas);
                    } else if (corAtual == 'B') {
                        jogoTerminado = moverPeao(tabuleiro, corAtual, movimentos, percursoAzul, posicoesPecas);
                    } else if (corAtual == 'Y') {
                        jogoTerminado = moverPeao(tabuleiro, corAtual, movimentos, percursoAmarelo, posicoesPecas);
                    } else if (corAtual == 'G') {
                        jogoTerminado = moverPeao(tabuleiro, corAtual, movimentos, percursoVerde, posicoesPecas);
                    }
                }
            }
        } else {
            if (corAtual == 'R') {
                jogoTerminado = moverPeao(tabuleiro, corAtual, movimentos, percursoVermelho, posicoesPecas);
            } else if (corAtual == 'B') {
                jogoTerminado = moverPeao(tabuleiro, corAtual, movimentos, percursoAzul, posicoesPecas);
            } else if (corAtual == 'Y') {
                jogoTerminado = moverPeao(tabuleiro, corAtual, movimentos, percursoAmarelo, posicoesPecas);
            } else if (corAtual == 'G') {
                jogoTerminado = moverPeao(tabuleiro, corAtual, movimentos, percursoVerde, posicoesPecas);
            }
        }
        jogoTerminado = realizarJogada(tabuleiro, corAtual, posicoesPecas);
        // Verificar se a jogada resultou em término de jogo
        if (jogoTerminado) {
            cout << "Jogador " << corAtual << " venceu o jogo!" << endl;
            break;
        }

        // Aguardar um pouco antes de continuar
        Sleep(1000); // No Windows, use Sleep(1000) para esperar 1 segundo

        rodada++;
    }
}


void tela_Ranking() {
    
    limparTela();

    int opcao;
    cout << "[1] - Digite 1 para ver o ranking de Pontos\n";
    cout << "[2] - Digite 2 para ver o ranking de Vitorias\n";
    cout << "[3] - Digite 3 para Voltar\nOpcao: ";
    cin >> opcao;

    if (opcao == 1) {
        tabelaRankingPontos();
    } else if (opcao == 2) {
        limparTela();

        int subOpcao;
        cout << "[1] - Digite 1 para ver o ranking Geral\n";
        cout << "[2] - Digite 2 para ver o ranking de Vitorias de partidas com 4 Jogadores\n";
        cout << "[3] - Digite 3 para ver o ranking de Vitorias de partidas com 3 Jogadores\n";
        cout << "[4] - Digite 4 para ver o ranking de Vitorias de partidas com 2 Jogadores\n";
        cout << "[5] - Digite 5 para Voltar\nOpcao: ";
        cin >> subOpcao;

        if (subOpcao == 1) {
            tabelaRankingVitoriasGeral();
        } else if (subOpcao == 2) {
            TabelaRankingVitorias4Jogadores();
        } else if (subOpcao == 3) {
            TabelaRankingVitorias3Jogadores();
        } else if (subOpcao == 4) {
            TabelaRankingVitorias2Jogadores();
        } else if (subOpcao == 5) {
            tela_Ranking();
        } else {
            limparTela();
            cout << "opcao invalida";
            system("pause");
            tela_Ranking();
        }
    } else if (opcao == 3) {
        tela_Menu();
    } else {
        limparTela();
        cout << "opcao invalida";
        system("pause");
        tela_Ranking();
    }
}

void tela_Historico() {
    system("cls"); // Limpa o console antes de exibir o histórico de partidas

    string nomeJogador;
    cout << "Digite o nome do jogador para ver o historico de partidas: ";
    cin >> nomeJogador;

    vector<string> linhasPartida = lerCsvDadosPartidatoHistorico();
    if (linhasPartida.empty()) {
        cout << "Nao foi possivel ler o arquivo de dados da partida ou nao ha partidas registradas." << endl;
    } else {
        exibirHistorico(nomeJogador, linhasPartida);
    }

    int opcao;
    cout << "\n[1] - Digite 1 para Voltar\nOpcao: ";
    cin >> opcao;

    if (opcao == 1) {
        tela_Menu();
    } else {
        system("cls");
        cout << "Opcao invalida. Tente novamente." << endl;
        system("pause");
        tela_Historico();
    }
}

void tela_EditarPerfil()
{
    system("cls");

    int opcao;
    cout << "1 - Editar o perfil" << endl;
    cout << "2 - Excluir Perfil" << endl;
    cout << "3 - voltar" << endl;
    cout << "opcao desejada: ";

    cin >> opcao;

    if (opcao == 1)
    {
        string nomeAtual, senhaAtual;

        cout << "\033[1;31m==============\033[0m EDITAR PERFIL \033[1;31m==============\033[0m" << endl;
        cout << "Usuario: ";
        cin >> nomeAtual;
        cout << "Senha: ";
        cin >> senhaAtual;

        salvarEditarPerfil(nomeAtual, senhaAtual);
    }
    else if (opcao == 2)
    {
        tela_ExcluirPerfil();
    }
    else if (opcao == 3)
    {
        tela_Menu();
    }
    else
    {
        system("cls");
        cout << "Opcao invalida. Tente novamente.";
        system("pause");
        tela_EditarPerfil();
    }
}

void tela_RecuperarSenha()
{
    system("cls");
    int opcao;

    cout << "1 - Recuperar senha" << endl;
    cout << "2 - Login" << endl;
    cout << "opcao desejada: ";
    cin >> opcao;

    if (opcao == 1)
    {
        string nome;

        cout << "Usuario: " << endl;
        cin >> nome;

        ifstream arquivo_jogadores("jogadores.txt", ios::in);
        if (arquivo_jogadores.is_open())
        {
            string linha;
            bool nomeEncontrado = false;

            while (getline(arquivo_jogadores, linha))
            {
                if (linha.find("Nome: " + nome) != string::npos)
                {
                    // Encontrou o nome de usuário, agora lê as linhas seguintes
                    getline(arquivo_jogadores, linha);   // Lê a linha da senha
                    string senhaSalva = linha.substr(7); // Extrai a senha

                    getline(arquivo_jogadores, linha);       // Lê a linha da pergunta
                    string perguntaSalva = linha.substr(10); // Extrai a pergunta
                    cout << "Pergunta de seguranca: " << perguntaSalva << endl;

                    getline(arquivo_jogadores, linha);       // Lê a linha da resposta
                    string respostaSalva = linha.substr(10); // Extrai a resposta

                    // Lê a resposta do usuário para a pergunta de segurança
                    string resposta;
                    cout << "Resposta pergunta de seguranca: ";
                    cin >> resposta;

                    // Verifica se a resposta está correta
                    if (resposta == respostaSalva)
                    {
                        cout << "Sua senha e: " << senhaSalva << endl;
                        nomeEncontrado = true;
                    }
                    else
                    {
                        system("cls");
                        cout << "Resposta incorreta.";
                        system("pause");
                        tela_RecuperarSenha();
                    }
                    break; // Sai do loop ao encontrar o nome de usuário
                }
            }

            if (!nomeEncontrado)
            {
                system("cls");
                cout << "Nome de usuario nao encontrado.";
                system("pause");
                tela_RecuperarSenha();
            }

            arquivo_jogadores.close();
        }
        else
        {
            cout << "Nao foi possivel abrir o arquivo de jogadores." << endl;
        }
    }
    else if (opcao == 2)
    {
        tela_Login();
    }
    else
    {
        system("cls");
        cout << "Opcao invalida. Tente novamente.";
        system("pause");
        tela_RecuperarSenha();
    }
}

void tela_Regras()
{
    system("cls"); // Limpa o console antes de exibir a tela das regras

    cout << "\033[1;31m==============\033[0m  \033[32m REGRAS DO JOGO \033[0m \033[1;31m==============\033[0m" << endl
         << endl;
    cout << "\033[34m 1. A primeira pessoa so pode sair da sua casinha se o numero que cair no dado for igual a 1 ou 6.\033[0m" << endl;
    cout << "\033[34m 2. Enquanto nao cair 1 ou 6 o jogador não pode sair de sua casinha com a peca.\033[0m" << endl;
    cout << "\033[34m 3. Quando a pessoa sair da casinha ela deve jogar novamente.\033[0m" << endl;
    cout << "\033[34m 4. Se durante a partida o jogador tirar o numero 6 ao rodar o dado ele podera rodar novamente o dado limitado a 3 lances seguidos fora lance o original.\033[0m" << endl;
    cout << "\033[34m 5. Se o jogador cair em uma casinha que possua o simbolo:# ele podera avançar 6 casinhas para frente ou esolher reitar mais umas peça da sua toca.\033[0m" << endl;

    int opcao;
    cout << "\033[1;31m[1] - Digite 1 para Voltar\nOpcao: \033[0m";
    cin >> opcao;

    if (opcao == 1)
    {
        tela_Menu();
    }
    else
    {
        system("cls");
        cout << "Opcao invalida! tente novamente " << endl;
        system("pause");
        tela_Regras();
    }
}

void tela_ExcluirPerfil()
{
    limparTela();

    int opcao;

    cout << "\0331 - Excluir perfil \033" << endl;
    cout << "\0332 - voltar\033" << endl;
    cout << "\033Opcao: \033" << endl;
    cin >> opcao;

    if (opcao == 1)
    {
    string nome, senha, respostaPergunta;

    cout << "Digite seu nome de usuario: ";
    cin >> nome;
    cout << "Digite sua senha: ";
    cin >> senha;
    cout << "Digite a resposta da sua pergunta de seguranca: ";
    cin.ignore(); // Ignora o newline da entrada anterior
    getline(cin, respostaPergunta);

    excluirPerfil(nome, senha, respostaPergunta);
}
else if (opcao == 2)
{
    tela_EditarPerfil();
}
else
{
    limparTela();
    cout << "Opcao invalida. Tente novamente.";
    system("pause");
    tela_ExcluirPerfil();
}
}
