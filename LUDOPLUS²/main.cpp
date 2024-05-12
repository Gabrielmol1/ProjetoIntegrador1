#include <iostream>  //entrada e saida
#include <fstream>   // manipulação de arquivo txt
#include <string>    // utilzacao de string, manipulcao de texto
#include <cstdlib>   // Para usar a função system
#include <algorithm> // sort e find
#include <vector>    // usar vetores
#include <ctime>     // cronometro n4a tela do jogo

using namespace std;

time_t tempoInicial;

// inicializacao dos metodos em ordem antes da main

void salvarJogador_txt(string nome, string senha);
void salvarDadosPartida_txt(int numeroPartida, string dataHora, vector<pair<string, string>> jogadoresCores, vector<pair<string, pair<string, int>>> ranking);
void salvarEditarPerfil(string nomeAtual, string senhaAtual);
void fecharJogo();
bool verificarNomeExistente(const string &nome);
void validarLogin(string nome, string senha);
void selecionarJogadoresECoresParaPartida();
int rolarDado();
void moverPeca(int &posicaoAtual, int numRolado);

// inicializacao das telas em ordem antes da main

void tela_Login();
void tela_Menu();
void tela_Cadastrar();
void Tela_Jogar_MostrarTempoNaTela();
void tela_Jogar();
void tela_Ranking();
void tela_HistoricoPartidas();
void tela_EditarPerfil();
void tela_RecuperarSenha();
void tela_Regras();

int main()
{

    tela_Login();
    return 0;
}

void salvarJogador_txt(string nome, string senha, string perguntaSeguranca, string respostaPergunta)
{

    ofstream arquivo("jogadores.txt", ios::app);

    if (arquivo.is_open())
    {
        arquivo << "Nome: " << nome << endl;
        arquivo << "Senha: " << senha << endl;
        arquivo << "pergunta segurança: " << perguntaSeguranca << endl;
        arquivo << "resposta pergunta: " << respostaPergunta << endl;
        arquivo.close();
        cout << "Dados salvos com sucesso." << endl;
        system("cls"); // Limpa o console antes de exibir a tela de menu
        tela_Login();  // abre o login se os dados forem salvos corretanente
    }
    else
    {
        cout << "Erro ao abrir o arquivo para salvar os dados." << endl;
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
        cout << "Dados da partida salvos com sucesso." << endl;
    }
    else
    {
        cout << "Erro ao abrir o arquivo para salvar os dados da partida." << endl;
    }
}

void salvarEditarPerfil(string nomeAtual, string senhaAtual)
{

    ifstream arquivo_jogadores("jogadores.txt", ios::app);
    ofstream arquivo_temporario("temporario_jogadores.txt");

    if (arquivo_jogadores.is_open() && arquivo_temporario.is_open())
    {
        string linha;
        bool jogadorEncontrado = false;

        while (getline(arquivo_jogadores, linha))
        {
            if (linha.find("Nome: ") != string::npos && linha.find(nomeAtual) != string::npos)
            {
                string senhaSalva = ""; // Variável para armazenar a senha salva do usuário
                // Encontrou o jogador a ser editado, verificar a senha
                while (getline(arquivo_jogadores, linha))
                {
                    if (linha.find("Senha: ") != string::npos)
                    {
                        senhaSalva = linha.substr(7); // Obter a senha salva do arquivo
                        break;
                    }
                }
                if (senhaAtual == senhaSalva)
                { // Verificar se a senha atual coincide
                    // Se coincide, solicitar as novas informações e editar o perfil
                    string novoNome, novaSenha, NovaPerguntaSeguranca, novaRespostaPergunta;
                    cout << "Digite seu novo nome ou o mesmo nome: ";
                    cin >> novoNome;
                    cout << "Digite sua nova senha ou a mesma senha: ";
                    cin >> novaSenha;
                    cout << "Digite sua nova pergunta de segurança ou a mesma nova pergunta de seguranca: ";
                    cin >> NovaPerguntaSeguranca;
                    cout << "Digite sua nova nova resposta de segurança ou a mesma resposta de seguranca: ";
                    cin >> novaRespostaPergunta;

                    arquivo_temporario << "Nome: " << novoNome << endl;
                    arquivo_temporario << "Senha: " << novaSenha << endl;
                    arquivo_temporario << "pergunta: " << NovaPerguntaSeguranca << endl;
                    arquivo_temporario << "resposta: " << novaRespostaPergunta << endl;

                    jogadorEncontrado = true;
                    break;
                }
                else
                {
                    // Senha atual incorreta, manter os dados originais
                    arquivo_temporario << linha << endl;
                    cout << " Senha incorreta, por favor tente novamente";
                    tela_Menu();
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
            cout << "Perfil editado com sucesso!" << endl;
            tela_Menu();
        }
        else
        {
            cout << "Nome de usuário ou senha atual incorretos." << endl;
        }
    }
    else
    {
        cout << "Erro ao abrir os arquivos." << endl;
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
            cout << "Nome de usuario ou senha incorretos." << endl;
            system("pause");
            tela_Login();
        }
    }
    else
    {
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
        size_t pos = linha.find(':');
        if (pos != string::npos)
        {
            string nomeExistente = linha.substr(0, pos);
            if (nomeExistente == nome)
            {
                cout << "Este nome já existe. Por favor, escolha outro nome." << endl;
                return true;
            }
        }
    }
    return false;
}
// void selecionarJogadoresECoresParaPartida(){

//     int num_jogadores;

//     do {
//         cout << "Quantos jogadores deseja na partida? (Entre 2 e 4): ";
//         cin >> num_jogadores;
//         if (num_jogadores < 2 || num_jogadores > 4) {
//             cout << "Número de jogadores inválido. Por favor, escolha entre 2 e 4 jogadores." << endl;
//         }
//     } while (num_jogadores < 2 || num_jogadores > 4);

//     ifstream arquivo_jogadores("jogadores.txt", ios::in);
//     vector<string> nomes; // Declarar a variável nomes fora do bloco condicional
//     vector<string> JogadoresSelecionados;
//     if (arquivo_jogadores.is_open()) {
//         string linha;
//         while (getline(arquivo_jogadores, linha)) {
//             if (linha.find("Nome: ") != string::npos) {
//                 nomes.push_back(linha.substr(6)); // Adiciona o nome à lista
//             }
//         }
//         arquivo_jogadores.close();

//         // Ordenar os nomes em ordem alfabética
//         sort(nomes.begin(), nomes.end());

//         cout << "Lista de Jogadores:" << endl;
//         for (int i = 0; i < nomes.size(); ++i) {
//             cout << i + 1 << ". " << nomes[i] << endl;
//         }

//         // Permitir pesquisa por nome
//         string nomePesquisado;
//         cout << "\nDigite o nome do jogador que deseja selecionar: ";
//         cin >> nomePesquisado;

//         // Verificar se o nome está na lista
//         auto it = find(nomes.begin(), nomes.end(), nomePesquisado);
//         if (it != nomes.end()) {

//             cout << "Jogador " << *it << "encontrado " << endl;

//         } else {
//             cout << "Jogador não encontrado." << endl;
//         }
//     } else {
//         cout << "Erro ao abrir o arquivo de jogadores." << endl;
//         return; // Sai do método se houver um erro ao abrir o arquivo
//     }

//     // O código abaixo será executado somente se o arquivo for aberto com sucesso
//     int escolha;
//     cout << "\nSelecione o número correspondente ao jogador que deseja adicionar à partida: ";
//     cin >> escolha;

//     if (escolha >= 1 && escolha <= nomes.size()) {
//         string jogadorSelecionado = nomes[escolha - 1];
//         cout << "Jogador selecionado: " << jogadorSelecionado << endl;
//         // Aqui você pode realizar alguma ação com o jogador selecionado
//     } else {
//         cout << "Opção inválida. Selecione um número válido da lista." << endl;
//     }
// }

// Função para rolar o dado
int rolarDado()
{
    return rand() % 6 + 1; // Gera um número aleatório de 1 a 6
}

// Função para mover a peça do jogador
void moverPeca(int &posicaoAtual, int numRolado)
{
    posicaoAtual += numRolado; // Move a peça para frente
}

// ABAIXO DESTE PONTO ESTAO AS TELAS

void tela_Login()
{

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
    else if (opcao == 4)
    {

        tela_RecuperarSenha();
    }
    else
    {

        fecharJogo();
    }
}

void tela_Menu()
{

    system("cls");

    int opcao_Menu;

    cout << "\033[1;31m==============================================\033[0m" << endl;
    cout << "\033[1;37m                  LUDO PLUS                  \033[0m" << endl;
    cout << "\033[1;31m==============================================\033[0m" << endl;
    cout << "                                                 " << endl;
    cout << "             [1] - Jogar                   " << endl;
    cout << "             [2] - Ranking           " << endl;
    cout << "             [3] - Historico das Partidas         " << endl;
    cout << "             [4] - Editar/excluir perfil   " << endl;
    cout << "             [5] - Regras do Jogo   " << endl;
    cout << "             [6] - Sair             " << endl;
    cout << "                                                 " << endl;
    cout << "\033[1;31m==============================================\033[0m" << endl;

    cin >> opcao_Menu;

    if (opcao_Menu == 1)
    {
        // void selecionarJogadoresECoresParaPartida();
        tela_Jogar();
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
        cout << "opção invalida, digite novamente";
        tela_Menu();
    }
}

void tela_Cadastrar()
{

    system("cls"); // Limpa o console antes de exibir a tela de cadastro

    string nome, senha;
    string perguntaSeguranca, respostaPergunta;

    cout << "\033[1;31m===========================\033[0m" << endl;
    cout << "      CADASTRAR JOGADOR     " << endl;
    cout << "\033[1;31m===========================\033[0m" << endl
         << endl;

    cout << "\033[1;32mInforme um nome:\033[0m" << endl;
    cin >> nome;

    while (verificarNomeExistente(nome))
    {
        cout << "\033[1;32mInforme outro nome:\033[0m" << endl;
        cin >> nome;
    }

    do
    {
        cout << "\033[1;32mInforme a sua senha (minimo de 6 caracteres):\033[0m " << endl;
        cin >> senha;
        if (senha.length() <= 5)
        {
            cout << "\033[1;31mA senha deve ter no minimo 6 caracteres.\033[0m" << endl;
        }
    } while (senha.length() <= 5);

    cout << "\033[1;32mDigite uma pergunta que apenas voce saiba responder:\033[0m" << endl;
    cin >> perguntaSeguranca;
    cout << "\033[1;32mInforme a resposta da pergunta:\033[0m" << endl;
    cin >> respostaPergunta;

    // Salvar o jogador apenas se o nome for único e a senha atender aos requisitos
    salvarJogador_txt(nome, senha, perguntaSeguranca, respostaPergunta);
}

void Tela_Jogar_MostrarTempoNaTela()
{

    time_t tempoAtual = time(nullptr);

    int segundosDecorridos = difftime(tempoAtual, tempoInicial);

    cout << endl
         << "TIMER DO JOGO: " << segundosDecorridos << endl;
}

void tela_Jogar()
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
        cout << "Erro ao abrir o arquivo de histórico de partidas." << endl;
    }
}

void tela_EditarPerfil()
{

    system("cls"); // Limpa o console antes de exibir a tela de editarperfil
    int opcao;
    cout << "Digite 1 para editar o perfil\n Digite 2 para voltar ao menu";
    cin >> opcao;

    if (opcao == 1)
    {
        string nomeAtual, senhaAtual;

        cout << "\033[1;31m==============\033[0m EDITAR PERFIL \033[1;31m==============\033[0m" << endl;
        cout << "Digite seu nome de usuario atual: ";
        cin >> nomeAtual;
        cout << "Digite sua senha de usuario atual: ";
        cin >> senhaAtual;

        salvarEditarPerfil(nomeAtual, senhaAtual);
    }
    else if (opcao == 2)
    {
        tela_Menu();
    }
    else
    {
        tela_EditarPerfil();
        cout << "Opcao invalida! \nTente novamente.";
    }
}

void tela_RecuperarSenha()
{

    int opcao;

    cout << "Digite 1 para reuperar sua senha \nDigite 2 para voltar a tela de login";
    cin >> opcao;

    if (opcao == 1)
    {

        string nome;

        cout << "Digite seu nome de usuario para recuperar a senha: ";
        cin >> nome;

        ifstream arquivo_jogadores("jogadores.txt", ios::in);
        if (arquivo_jogadores.is_open())
        {
            string linha;
            while (getline(arquivo_jogadores, linha))
            {
                if (linha.find("Nome: " + nome) != string::npos)
                {
                    // Encontrou o nome de usuário, agora exibe a pergunta correspondente
                    getline(arquivo_jogadores, linha);                             // pula linha
                    getline(arquivo_jogadores, linha);                             // pula linha
                    cout << "Pergunta de seguranca: " << linha.substr(11) << endl; // Exibe a pergunta
                    string perguntaSalva = linha.substr(11);                       // Salva a pergunta para comparação posterior

                    // Lê a resposta da pergunta
                    string resposta;
                    cout << "Digite a resposta da pergunta de seguranca: ";
                    cin >> resposta;

                    // Verifica resposta
                    size_t pos_senha = linha.find("Senha: ");
                    if (pos_senha != string::npos)
                    {
                        cout << "Sua senha e: " << linha.substr(pos_senha + 7) << endl;
                        arquivo_jogadores.close();
                        tela_Login();
                        return;
                    }
                    else
                    {
                        cout << "Erro ao recuperar a senha." << endl;
                        arquivo_jogadores.close();
                        tela_RecuperarSenha();
                        return;
                    }
                }
            }
            arquivo_jogadores.close();
            cout << "Nome de usuario não encontrado." << endl;
            tela_Menu();
        }
        else
        {
            cout << "Erro ao abrir o arquivo de jogadores." << endl;
        }
    }
    else if (opcao == 2)
    {
        tela_Login();
    }
    else
    {
        system("cls");
        cout << "Opcao invalida tente novamente ";
        tela_RecuperarSenha();
    }
}

void tela_Regras()
{

    int voltar;

    system("cls"); // Limpa o console antes de exibir a tela das regras

    cout << "\033[1;31m==============\033[0m  \033[32m REGRAS DO JOGO \033[0m \033[1;31m==============\033[0m" << endl;

    cout << endl << "\033[34m 1. A primeira pessoa so pode sair da sua casinha se o numero que cair no dado for igual a 1 ou 6.\033[0m" << endl;
    cout << "\033[34m 2. Enquanto nao cair 1 ou 6 o jogador não pode sair de sua casinha com a peca.\033[0m" << endl;
    cout << "\033[34m 3. Quando a pessoa sair da casinha ela deve jogar novamente.\033[0m" << endl;
    cout << "\033[34m 4. Se durante a partida o jogador tirar o numero 6 ao rodar o dado ele podera rodar novamente o dado limitado a 3 lances seguidos fora lance o original.\033[0m" << endl
         << endl;

    cout << "\033[1;31m[1] - Voltar\033[0m" << endl;
    cin >> voltar;

    if (voltar == 1)
    {

        tela_Menu();
    }
}