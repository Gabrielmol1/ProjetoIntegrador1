#include <iostream>  //entrada e saida
#include <fstream>   // manipulação de arquivo txt
#include <string>    // utilzacao de string, manipulcao de texto
#include <cstdlib>   // Para usar a função system
#include <algorithm> // sort e find
#include <vector>    // usar vetores
#include <ctime>     // cronometro n4a tela do jogo
#include <unistd.h>  // Para a função sleep
#include <unordered_map>

using namespace std;

time_t tempoInicial;

// inicializacao dos metodos em ordem antes da main

void salvarJogador_txt(string nome, string senha);
void salvarDadosPartida_txt(int numeroPartida, string dataHora, vector<pair<string, string>> jogadoresCores, vector<pair<string, pair<string, int>>> ranking);
void solicitarDadosJogador(string &nome, string &senha, string &perguntaSeguranca, string &respostaPergunta);
void salvarEditarPerfil(string nomeAtual, string senhaAtual);
void fecharJogo();
void validarLogin(string nome, string senha);
bool verificarNomeExistente(const string &nome);
bool verificarTamanhoSenha(const string &senha);
bool verificarSenhasIguais(const string &senha, const string &confirmarSenha);
bool verificarRespostasIguais(const string &resposta, const string &confirmarResposta);
void excluirPerfil(const string &nome, const string &senha, const string &respostaPergunta);
int selecionarQuantidadeJogParaPartida();
void selecionarJogadoresECoresParaPartida(int num_jogadores);
void imprimirTabuleiro(const vector<vector<string>> &tabuleiro);

void limparTela();
string trim(const string &str);
int rolarDado();
void moverPeca(int &posicaoAtual, int numRolado);

// inicializacao das telas em ordem antes da main

void tela_Login();
void tela_Menu();
void tela_Cadastrar();
void Tela_Jogar_MostrarTempoNaTela();
void tela_Jogar(const std::vector<std::string> &nomesJogadores, const std::vector<std::string> &coresJogadores);
void tela_Ranking();
void tela_HistoricoPartidas();
void tela_EditarPerfil();
void tela_RecuperarSenha();
void tela_Regras();
void tela_ExcluirPerfil();

int main()
{
    tela_Login();
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

void salvarDadosPartida_txt(int numeroPartida, string dataHora, vector<pair<string, string>> jogadoresCores, vector<pair<string, pair<string, int>>> ranking)
{
    ofstream arquivo("dadosPartida.txt", ios::app);
    if (arquivo.is_open())
    {
        arquivo << "Partida " << numeroPartida << " - " << dataHora << " - ";
        for (const auto &jogadorCor : jogadoresCores)
        {
            arquivo << jogadorCor.first << " (" << jogadorCor.second << "), ";
        }
        arquivo << "- ";
        for (const auto &jogadorPontos : ranking)
        {
            arquivo << jogadorPontos.first << " (" << jogadorPontos.second.first << " - " << jogadorPontos.second.second << " pontos), ";
        }
        arquivo << endl;
        arquivo.close();
        system("cls");
        cout << "Dados da partida salvos com sucesso." << endl;
        system("pause");
        tela_Menu();
    }
    else
    {
        system("cls");
        cout << "Erro ao abrir o arquivo para salvar os dados da partida." << endl;
        system("pause");
        tela_Menu();
    }
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
        cout << "\033[1;32mInforme a sua senha (mínimo de 6 caracteres):\033[0m " << endl;
        cin >> senha;
        if (!verificarTamanhoSenha(senha))
        {
            cout << "\033[1;31mA senha deve ter no mínimo 6 caracteres.\033[0m" << endl;
        }
        else
        {
            cout << "\033[1;32mConfirme sua senha:\033[0m " << endl;
            cin >> confirmarSenha;
            if (!verificarSenhasIguais(senha, confirmarSenha))
            {
                cout << "\033[1;31mAs senhas não coincidem.\033[0m" << endl;
            }
        }
    } while (!verificarTamanhoSenha(senha) || !verificarSenhasIguais(senha, confirmarSenha));

    cout << "\033[1;32mDigite uma pergunta que apenas você saiba responder:\033[0m" << endl;
    cin.ignore();                    // Ignora o restante da linha anterior
    getline(cin, perguntaSeguranca); // Permite espaços na pergunta
    cout << "\033[1;32mInforme a resposta da pergunta:\033[0m" << endl;
    getline(cin, respostaPergunta); // Permite espaços na resposta

    string confirmarRespostaSenha;
    cout << "\033[1;32mConfirme a resposta da pergunta:\033[0m" << endl;
    getline(cin, confirmarRespostaSenha); // Permite espaços na confirmação

    while (!verificarRespostasIguais(respostaPergunta, confirmarRespostaSenha))
    {
        cout << "\033[1;31mAs respostas da pergunta de segurança não coincidem.\033[0m" << endl;
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
            cout << "Este nome já existe. Por favor, escolha outro nome." << endl;
            return true;
        }
    }
    return false;
}

bool verificarTamanhoSenha(const string &senha)
{
    return senha.length() >= 6;
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

int selecionarQuantidadeJogParaPartida()
{
    int num_jogadores;
    do
    {
        cout << "Digite a quantidade de jogadores (entre 2 e 4): ";
        cin >> num_jogadores;
        if (num_jogadores < 2 || num_jogadores > 4)
        {
            cout << "Quantidade de jogadores invalida. Por favor, escolha entre 2 e 4 jogadores.\n";
        }
    } while (num_jogadores < 2 || num_jogadores > 4);
    // Chamando o método selecionarJogadoresECoresParaPartida() com o número de jogadores selecionado
    selecionarJogadoresECoresParaPartida(num_jogadores);
}

void selecionarJogadoresECoresParaPartida(int num_jogadores)
{
    limparTela();

    if (num_jogadores < 2 || num_jogadores > 4)
    {
        cout << "Numero de jogadores invalido. Por favor, escolha entre 2 e 4 jogadores.\n";
        return;
    }

    ifstream arquivo_jogadores("jogadores.txt", ios::in);
    vector<pair<string, string>> jogadores;

    if (arquivo_jogadores.is_open())
    {
        string linha;
        string nome, senha;

        while (getline(arquivo_jogadores, linha))
        {
            if (linha.find("Nome: ") != string::npos)
            {
                nome = trim(linha.substr(6));
                if (getline(arquivo_jogadores, linha) && linha.find("Senha: ") != string::npos)
                {
                    senha = trim(linha.substr(7));
                    jogadores.push_back(make_pair(nome, senha));
                }
            }
        }

        arquivo_jogadores.close();

        sort(jogadores.begin(), jogadores.end());

        cout << "Lista de Jogadores:\n";
        for (int i = 0; i < jogadores.size(); ++i)
        {
            cout << i + 1 << ". " << jogadores[i].first << "\n";
        }

        vector<string> coresDisponiveis = {
            "\033[31mVermelho\033[0m",
            "\033[32mVerde\033[0m",
            "\033[34mAzul\033[0m",
            "\033[33mAmarelo\033[0m"};

        vector<string> jogadoresSelecionados;
        vector<string> coresSelecionadas;

        for (int i = 0; i < num_jogadores; ++i)
        {
            string nomePesquisado;
            cout << "\nDigite o nome do jogador " << i + 1 << ": ";
            cin >> nomePesquisado;

            auto it = find_if(jogadores.begin(), jogadores.end(), [&nomePesquisado](const pair<string, string> &jogador)
                              { return jogador.first == nomePesquisado; });

            if (it != jogadores.end())
            {
                string senha;
                cout << "Digite a senha do jogador " << nomePesquisado << ": ";
                cin >> senha;

                if (senha == it->second)
                {
                    cout << "Jogador " << it->first << " adicionado." << endl;
                    jogadoresSelecionados.push_back(it->first);
                    jogadores.erase(it);
                }
                else
                {
                    cout << "Senha incorreta.\n";
                    --i; // Repetir a iteração para permitir que o usuário insira o nome e a senha novamente
                }
            }
            else
            {
                cout << "Jogador nao encontrado.\n";
                --i; // Repetir a iteração para permitir que o usuário insira o nome novamente
            }
        }

        cout << "\nAssocie as cores aos jogadores:\n";
        for (int i = 0; i < jogadoresSelecionados.size(); ++i)
        {
            limparTela();

            cout << "Escolha a cor para o jogador " << jogadoresSelecionados[i] << ":\n";

            // Mostrar cores disponíveis
            cout << "Cores disponiveis:\n";
            for (int j = 0; j < coresDisponiveis.size(); ++j)
            {
                cout << j + 1 << ". " << coresDisponiveis[j] << "\n";
            }

            int corEscolhida;
            do
            {
                cout << "Selecione o numero da sua cor: ";
                cin >> corEscolhida;

                // Verificar se a cor escolhida está dentro do intervalo válido
                if (corEscolhida < 1 || corEscolhida > coresDisponiveis.size())
                {
                    cout << "Opção invalida. Por favor, escolha uma cor valida.\n";
                }
            } while (corEscolhida < 1 || corEscolhida > coresDisponiveis.size());

            // Adicionar a cor selecionada à lista de cores selecionadas
            coresSelecionadas.push_back(coresDisponiveis[corEscolhida - 1]);

            // Remover a cor escolhida das cores disponíveis
            coresDisponiveis.erase(coresDisponiveis.begin() + corEscolhida - 1);
        }

        // Chamando tela_Jogar() passando os vetores preenchidos
        tela_Jogar(jogadoresSelecionados, coresSelecionadas);
    }
    else
    {
        cout << "Erro ao abrir o arquivo de jogadores.\n";
        return;
    }
}

void imprimirTabuleiro(const vector<vector<string>> &tabuleiro)
{
    for (const auto &linha : tabuleiro)
    {
        for (const auto &celula : linha)
        {
            std::cout << celula << " ";
        }
        std::cout << "\n";
    }
}

void limparTela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string trim(const string &str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int rolarDado()
{                          // Função para rolar o dado
    return rand() % 6 + 1; // Gera um número aleatório de 1 a 6
}

void moverPeca(int &posicaoAtual, int numRolado)
{                              // Função para mover a peça do jogador
    posicaoAtual += numRolado; // Move a peça para frente
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

void tela_Menu()
{
    system("cls");

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

    if (opcao_Menu == 1)
    {
        selecionarQuantidadeJogParaPartida();
        // selecionarJogadoresEcores(quantosJogadores);
        // TelaJogadores(jogdaoresecores);
        // selecionarJogadoresECoresParaPartida();
        // tela_Jogar();
    }
    else if (opcao_Menu == 2)
    {
        tela_Ranking();
    }
    else if (opcao_Menu == 3)
    {
        tela_HistoricoPartidas();
    }
    else if (opcao_Menu == 4)
    {
        tela_EditarPerfil();
    }
    else if (opcao_Menu == 5)
    {
        tela_Regras();
    }
    else if (opcao_Menu == 6)
    {
        tela_Login();
    }
    else
    {
        system("cls");
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

void tela_Jogar(const vector<string> &nomesJogadores, const vector<string> &coresJogadores)
{
    system("cls"); // Limpa o console antes de exibir a tela de cadastro

    tempoInicial = time(nullptr);

    cout << "\033[1;31m===========================\033[0m TELA DO JOGO \033[1;31m===========================\033[0m" << endl;

    cout << "*" << endl;
    cout << "**             |V| | |             **" << endl;
    cout << "**             | |\033[31m#\033[0m|\033[31m#\033[0m|             **" << endl;
    cout << "**     |\033[32m#\033[0m|\033[32m#\033[0m|   | |\033[31m#\033[0m| |   |\033[31m#\033[0m|\033[31m#\033[0m|     **" << endl;
    cout << "**     |\033[32m#\033[0m|\033[32m#\033[0m|   |C|\033[31m#\033[0m| |   |\033[31m#\033[0m|\033[31m#\033[0m|     **" << endl;
    cout << "**             | |\033[31m#\033[0m| |             **" << endl;
    cout << "**             |_|\033[31m#\033[0m|V|             **" << endl;
    cout << "** | |\033[32m#\033[0m| | | |V|  \033[31m#\033[0m  | | |C| | |V| **" << endl;
    cout << "** | |\033[32m#\033[0m|\033[32m#\033[0m|\033[32m#\033[0m|\033[32m#\033[0m|\033[32m#\033[0m|\033[32m#\033[0m W \033[34m#\033[0m|\033[34m#\033[0m|\033[34m#\033[0m|\033[34m#\033[0m|\033[34m#\033[0m|\033[34m#\033[0m| | **" << endl;
    cout << "** |V| | |C| | |_ \033[33m#\033[0m _|V| | | |\033[34m#\033[0m| | **" << endl;
    cout << "**             |V|\033[33m#\033[0m| |             **" << endl;
    cout << "**             | |\033[33m#\033[0m| |             **" << endl;
    cout << "**     |\033[33m#\033[0m|\033[33m#\033[0m|   | |\033[33m#\033[0m|C|   |\033[34m#\033[0m|\033[34m#\033[0m|     **" << endl;
    cout << "**     |\033[33m#\033[0m|\033[33m#\033[0m|   | |\033[33m#\033[0m| |   |\033[34m#\033[0m|\033[34m#\033[0m|     **" << endl;
    cout << "**             |\033[33m#\033[0m|\033[33m#\033[0m| |             **" << endl;
    cout << "**             | | |V|             **" << endl;
    cout << "*" << endl;

    // Inicialização das posições das peças dos jogadores
    int posicaoJogador1 = 0;
    int posicaoJogador2 = 0;
    // Adicione mais posições para mais jogadores, se necessário

    int partidaFinalizada = 0;

    do
    {
        // Mostrar o tempo na tela
        Tela_Jogar_MostrarTempoNaTela();

        // Simulação do lançamento do dado
        int resultadoDado = rolarDado();
        cout << "Resultado do dado: " << resultadoDado << endl;

        // Lógica para mover as peças dos jogadores com base no resultado do dado
        if (resultadoDado == 1 || resultadoDado == 6)
        {
            // Se o jogador tirar 1 ou 6, pode mover uma peça do início do tabuleiro
            posicaoJogador1++;
        }

        // Verificar se o jogador 1 venceu
        if (posicaoJogador1 >= 100)
        {
            cout << "Jogador 1 venceu!" << endl;
            partidaFinalizada = 1;
            break;
        }

        // Simular movimento do jogador 2
        if (resultadoDado == 1 || resultadoDado == 6)
        {
            // Se o jogador tirar 1 ou 6, pode mover uma peça do início do tabuleiro
            posicaoJogador2++;
        }

        // Verificar se o jogador 2 venceu
        if (posicaoJogador2 >= 100)
        {
            cout << "Jogador 2 venceu!" << endl;
            partidaFinalizada = 1;
            break;
        }

        // Mostrar o estado atual do tabuleiro, com as posições das peças dos jogadores

        cout << "1- Jogar novamente" << endl;
        cout << "2- Voltar para o Menu" << endl;
        cout << "3- Sair" << endl;
        cin >> partidaFinalizada;

        if (partidaFinalizada == 1)
        {
            // O jogo continua
        }
        else if (partidaFinalizada == 2)
        {
            tela_Menu();
            break;
        }
        else if (partidaFinalizada == 3)
        {
            tela_Login();
            break;
        }
        else
        {
            cout << "Opcao Invalida" << endl;
        }

    } while (partidaFinalizada != 3);
}

void tela_Ranking()
{
    system("cls"); // Limpa o console antes de exibir a tela de ranking

    cout << "\033[1;31m==============\033[0m RANKING \033[1;31m==============\033[0m" << endl;
    cout << " AINDA NAO FOI FEITO MANE " << endl;

    //  ifstream arquivo_estatisticas("estatisticas.txt");
    //  if (arquivo_estatisticas.is_open()) {
    //      string linha;
    //      while (getline(arquivo_estatisticas, linha)) {
    //          cout << linha << endl;
    //      }
    //      arquivo_estatisticas.close();
    //  } else {
    //       cout << "Erro ao abrir o arquivo de ranking." << endl;
    //  }

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
        cout << "opcao invalida";
        system("pause");
        tela_Ranking();
    }
}

void tela_HistoricoPartidas()
{
    system("cls"); // Limpa o console antes de exibir o histórico de partidas

    ifstream arquivo_dadosPartida("dadosPartida.txt");
    if (arquivo_dadosPartida.is_open())
    {
        cout << "\033[1;31m==============\033[0m HISTORICO DA PARTIDA \033[1;31m==============\033[0m" << endl;
        cout << "Número da Partida\tData e Hora\tJogadores e Cores\tRanking" << endl;

        string linha;
        while (getline(arquivo_dadosPartida, linha))
        {
            vector<string> partes;
            size_t pos = 0;
            string token;
            while ((pos = linha.find("-")) != string::npos)
            {
                token = linha.substr(0, pos);
                partes.push_back(token);
                linha.erase(0, pos + 1);
            }
            partes.push_back(linha); // Adiciona a última parte

            // Extrair informações da linha
            string numPartida, dataHora, jogadoresCores, ranking;
            if (partes.size() >= 4)
            {
                numPartida = partes[0];
                dataHora = partes[1];
                jogadoresCores = partes[2];
                ranking = partes[3];
            }

            // Exibir as informações formatadas na tabela
            cout << numPartida << "\t" << dataHora << "\t" << jogadoresCores << "\t" << ranking << endl;
        }
        arquivo_dadosPartida.close();
    }
    else
    {
        system("cls");
        cout << "Erro ao abrir o arquivo de histórico de partidas.";
        system("pause");
        tela_Menu();
    }

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
        cout << "opcao invalida" << endl;
        system("pause");
        tela_HistoricoPartidas();
    }
}

void tela_EditarPerfil()
{
    system("cls");

    int opcao;
    cout << "1 - Editar o perfil" << endl;
    cout << "2 - Excluir Perfil" << endl;
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
    system("cls");

    string nome, senha, respostaPergunta;

    cout << "Digite seu nome de usuario: ";
    cin >> nome;
    cout << "Digite sua senha: ";
    cin >> senha;
    cout << "Digite a resposta da sua pergunta de seguranca: ";
    cin.ignore(); // Ignora o newline da entrada anterior
    getline(cin, respostaPergunta);

    excluirPerfil(nome, senha, respostaPergunta);
    // }
    // else if (opcao == 2)
    // {
    //     tela_Menu();
    // }
    // else
    // {
    //     system("cls");
    //     cout << "Opcao invalida. Tente novamente.";
    //     system("pause");
    //     tela_ExcluirPerfil();
    // }
}
