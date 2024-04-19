#include <iostream> //entrada e saida
#include <fstream> // manipulação de arquivo txt
#include <string> // utilzacao de string, manipulcao de texto
#include <cstdlib> // Para usar a função system
#include <algorithm> // sort e find
#include <vector> // usar vetores

using namespace std;

//inicializacao dos metodos antes da main 

void salvarJogador_txt(string nome, string senha);
void salvarDadosPartida_txt(int numeroPartida, string dataHora, vector<pair<string, string>> jogadoresCores, vector<pair<string, pair<string, int>>> ranking); 
void salvarEditarPerfil(string nomeAtual, string senhaAtual);
void fecharJogo();
void validarLogin(string nome, string senha);
void selecionarJogadoresECoresParaPartida();

//inicializacao das telas antes da main 

void tela_Login();
void tela_Menu();
void tela_Cadastrar();
void tela_Jogar();
void tela_Ranking();
void tela_HistoricoPartidas();
void tela_EditarPerfil();
void tela_Regras();

int main() {
	
    tela_Login();
    return 0;    
}

void salvarJogador_txt(string nome, string senha) {
	
    ofstream arquivo("jogadores.txt", ios::app);
    
    if (arquivo.is_open()) {
        arquivo << "Nome: " << nome << endl;
        arquivo << "Senha: " << senha << endl;
        arquivo.close();
        cout << "Dados salvos com sucesso." << endl;
		tela_Menu();//abre o menu se os dados forem salvos corretanente
			
} 		else {
       	 cout << "Erro ao abrir o arquivo para salvar os dados." << endl;
    }
}

void salvarDadosPartida_txt(int numeroPartida, string dataHora, vector<pair<string, string>> jogadoresCores, vector<pair<string, pair<string, int>>> ranking) {
    ofstream arquivo("dadosPartida.txt", ios::app);
    if (arquivo.is_open()) {
        arquivo << "Partida " << numeroPartida << " - " << dataHora << " - ";
        for (const auto& jogadorCor : jogadoresCores) {
            arquivo << jogadorCor.first << " (" << jogadorCor.second << "), ";
        }
        arquivo << "- ";
        for (const auto& jogadorPontos : ranking) {
            arquivo << jogadorPontos.first << " (" << jogadorPontos.second.first << " - " << jogadorPontos.second.second << " pontos), ";
        }
        arquivo << endl;
        arquivo.close();
        cout << "Dados da partida salvos com sucesso." << endl;
    } else {
        cout << "Erro ao abrir o arquivo para salvar os dados da partida." << endl;
    }
}

void validarLogin(string nome, string senha) {

    ifstream arquivo_jogadores("jogadores.txt", ios::in);
    if (arquivo_jogadores.is_open()) {
        string linha;
        string nomeSalvo, senhaSalva;
        bool loginValido = false;
        while (getline(arquivo_jogadores, linha)) {
            if (linha.find("Nome: ") != string::npos) {
                nomeSalvo = linha.substr(6); // Removendo "Nome: " da linha
            } else if (linha.find("Senha: ") != string::npos) {
                senhaSalva = linha.substr(7); // Removendo "Senha: " da linha
                // Verifica se o nome e a senha correspondem aos dados salvos
                if (nome == nomeSalvo && senha == senhaSalva) {
                    loginValido = true;
                    break;
                }
            }
        }
        arquivo_jogadores.close();
        if (loginValido) {
            cout << "Login realizado com sucesso!" << endl;
            tela_Menu();
        } else {
            cout << "Nome de usuario ou senha incorretos." << endl;
            tela_Login(); 
        }
    }else {
        cout << "Erro ao abrir o arquivo de jogadores." << endl;
        fecharJogo();
    }
}

void fecharJogo() {
  
   exit(0); // Encerrar o programa 
  
}

void salvarEditarPerfil(string nomeAtual, string senhaAtual) {

    ifstream arquivo_jogadores("jogadores.txt", ios::app);
    ofstream arquivo_temporario("temporario_jogadores.txt");

    if (arquivo_jogadores.is_open() && arquivo_temporario.is_open()) {
        string linha;
        bool jogadorEncontrado = false;

        while (getline(arquivo_jogadores, linha)) {
            if (linha.find("Nome: ") != string::npos && linha.find(nomeAtual) != string::npos) {
                string senhaSalva = ""; // Variável para armazenar a senha salva do usuário
                // Encontrou o jogador a ser editado, verificar a senha
                while (getline(arquivo_jogadores, linha)) {
                    if (linha.find("Senha: ") != string::npos) {
                        senhaSalva = linha.substr(7); // Obter a senha salva do arquivo
                        break;
                    }
                }
                if (senhaAtual == senhaSalva) { // Verificar se a senha atual coincide
                    // Se coincide, solicitar as novas informações e editar o perfil
                    string novoNome, novaSenha;
                    cout << "Digite seu novo nome ou o mesmo nome: ";
                    cin >> novoNome;
                    cout << "Digite sua nova senha ou a mesma senha: ";
                    cin >> novaSenha;

                    arquivo_temporario << "Nome: " << novoNome << endl;
                    arquivo_temporario << "Senha: " << novaSenha << endl;

                    jogadorEncontrado = true;
                    break;
                } else {
                    // Senha atual incorreta, manter os dados originais
                    arquivo_temporario << linha << endl;
                    cout << " Senha incorreta, por favor tente novamente";
                    tela_Menu();
                }
            } else {
                // Copiar outras linhas do arquivo original para o arquivo temporário
                arquivo_temporario << linha << endl;
            }
        }

        arquivo_jogadores.close();
        arquivo_temporario.close();

        if (jogadorEncontrado) {
            remove("jogadores.txt");
            rename("temporario_jogadores.txt", "jogadores.txt");
            cout << "Perfil editado com sucesso!" << endl;
        } else {
            cout << "Nome de usuário ou senha atual incorretos." << endl;
        }
    } else {
        cout << "Erro ao abrir os arquivos." << endl;
    }
}

void selecionarJogadoresECoresParaPartida(){

    int num_jogadores;

    do {
        cout << "Quantos jogadores deseja na partida? (Entre 2 e 4): ";
        cin >> num_jogadores;
        if (num_jogadores < 2 || num_jogadores > 4) {
            cout << "Número de jogadores inválido. Por favor, escolha entre 2 e 4 jogadores." << endl;
        }
    } while (num_jogadores < 2 || num_jogadores > 4);

    ifstream arquivo_jogadores("jogadores.txt", ios::in);
    vector<string> nomes; // Declarar a variável nomes fora do bloco condicional
    vector<string> JogadoresSelecionados;
    if (arquivo_jogadores.is_open()) {
        string linha;
        while (getline(arquivo_jogadores, linha)) {
            if (linha.find("Nome: ") != string::npos) {
                nomes.push_back(linha.substr(6)); // Adiciona o nome à lista
            }
        }
        arquivo_jogadores.close();

        // Ordenar os nomes em ordem alfabética
        sort(nomes.begin(), nomes.end());

        cout << "Lista de Jogadores:" << endl;
        for (int i = 0; i < nomes.size(); ++i) {
            cout << i + 1 << ". " << nomes[i] << endl;
        }

        // Permitir pesquisa por nome
        string nomePesquisado;
        cout << "\nDigite o nome do jogador que deseja selecionar: ";
        cin >> nomePesquisado;

        // Verificar se o nome está na lista
        auto it = find(nomes.begin(), nomes.end(), nomePesquisado);
        if (it != nomes.end()) {

            cout << "Jogador " << *it << "encontrado " << endl;
            
        } else {
            cout << "Jogador não encontrado." << endl;
        }
    } else {
        cout << "Erro ao abrir o arquivo de jogadores." << endl;
        return; // Sai do método se houver um erro ao abrir o arquivo
    }

    // O código abaixo será executado somente se o arquivo for aberto com sucesso
    int escolha;
    cout << "\nSelecione o número correspondente ao jogador que deseja adicionar à partida: ";
    cin >> escolha;

    if (escolha >= 1 && escolha <= nomes.size()) {
        string jogadorSelecionado = nomes[escolha - 1];
        cout << "Jogador selecionado: " << jogadorSelecionado << endl;
        // Aqui você pode realizar alguma ação com o jogador selecionado
    } else {
        cout << "Opção inválida. Selecione um número válido da lista." << endl;
    }
}

// ABAIXO DESTE PONTO ESTAO AS TELAS  XDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDX

void tela_Login() {

    system("cls"); // Limpa o console antes de exibir a tela de login
	
	string nome, senha;
	
	cout << "\033[1;31m==============================================\033[0m" << endl;
    cout << "\033[1;37m             BEM-VINDO AO LUDO PLUS         \033[0m" << endl;
    cout << "\033[1;31m==============================================\033[0m" << endl;
    cout << "\033[1;37m                                              \033[0m" << endl;
    cout << "\033[1;37m             [1] - FAZER LOGIN              \033[0m" << endl;
    cout << "\033[1;37m             [2] - CADASTRAR                \033[0m" << endl;
    cout << "\033[1;37m             [3] - SAIR                     \033[0m" << endl;
    cout << "\033[1;37m                                              \033[0m" << endl;
    cout << "\033[1;31m==============================================\033[0m" << endl;

    int opcao;
    cin >> opcao;

    if (opcao == 1) {
        cout << "Digite seu nome: ";
        cin >> nome;
        cout << "Digite sua senha: ";
        cin >> senha;
        validarLogin(nome, senha); // Chama o metodo para validar o login
       
    } else if (opcao == 2) {
    	
	tela_Cadastrar();
        
    } else {
    	
	 fecharJogo();

    }
}

void tela_Menu() { 

    system("cls"); // Limpa o console antes de exibir a tela de menu

	int opcao_Menu;
	
	cout << "  ****************** BEM VINDO AO LUDO PLUS² ******************  " << endl;
    cout << "            _____________________________________   " << endl;
    cout << "           |                                     |  " << endl;
    cout << "           | Digite 1 para jogar                 |  " << endl;
    cout << "           | Digite 2 para ver o Ranking         |  " << endl;
    cout << "           | Digite 3 para ver o historico       |  " << endl;
    cout << "           | Digite 4 para editar seu perfil     |  " << endl;
    cout << "           | Digite 5 para ver as regras do jogo |  " << endl;
    cout << "           | Digite 6 voltar ao login            |  " << endl;
    cout << "           | Digite 7 para fechar do jogo        |  " << endl;
    cout << "           |_____________________________________|  " << endl;

    cin >> opcao_Menu;
    
    if(opcao_Menu == 1){
    	
	void selecionarJogadoresECoresParaPartida();	
	}
	else if (opcao_Menu == 2){
		
	 tela_Ranking();
	}
    else if (opcao_Menu == 3){
		
 	 tela_HistoricoPartidas();
	}
	else if (opcao_Menu == 4){
		
 	 tela_EditarPerfil();
	}
	else if (opcao_Menu == 5){
		
	 tela_Regras();
	}
	else if (opcao_Menu == 6){
		
	 tela_Login();
	}
	else if (opcao_Menu == 7){
		
	 fecharJogo();
	}
    else {
    cout << "opção invalida, digite novamente";
    tela_Menu();
    }
}

void tela_Cadastrar() {
	
    system("cls"); // Limpa o console antes de exibir a tela de cadastro

	string nome, senha;
	
		cout << "\033[1;31m===========================\033[0m" << endl;
		cout << "      CADASTRAR JOGADOR     " << endl;
		cout << "\033[1;31m===========================\033[0m" << endl << endl;

    cout << "\033[1;32mInforme o Seu nome:\033[0m" << endl;
    cin >> nome;
    cout << "\033[1;32mInforme a sua senha:\033[0m " << endl;
    cin >> senha;
    
    salvarJogador_txt(nome, senha);
  
}

void tela_Jogar() {

    system("cls"); // Limpa o console antes de exibir a tela de cadastro
	
		cout << "         ****************** TELA DO JOGO ******************" << endl;
        cout << "   AINDA NAO FOI FEITO MANe " << endl;

    cout << "*************************************" << endl;
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
    cout << "*************************************" << endl;



}

void tela_Ranking() {

    system("cls"); // Limpa o console antes de exibir a tela de ranking
 	
    cout << "         ****************** RANKING ******************" << endl;
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
 
void tela_HistoricoPartidas() {
    system("cls"); // Limpa o console antes de exibir o histórico de partidas

    ifstream arquivo_dadosPartida("dadosPartida.txt");
    if (arquivo_dadosPartida.is_open()) {
        cout << "****************** HISTÓRICO DE PARTIDAS ******************" << endl;
        cout << "Número da Partida\tData e Hora\tJogadores e Cores\tRanking" << endl;

        string linha;
        while (getline(arquivo_dadosPartida, linha)) {
            vector<string> partes;
            size_t pos = 0;
            string token;
            while ((pos = linha.find("-")) != string::npos) {
                token = linha.substr(0, pos);
                partes.push_back(token);
                linha.erase(0, pos + 1);
            }
            partes.push_back(linha); // Adiciona a última parte

            // Extrair informações da linha
            string numPartida, dataHora, jogadoresCores, ranking;
            if (partes.size() >= 4) {
                numPartida = partes[0];
                dataHora = partes[1];
                jogadoresCores = partes[2];
                ranking = partes[3];
            }

            // Exibir as informações formatadas na tabela
            cout << numPartida << "\t" << dataHora << "\t" << jogadoresCores << "\t" << ranking << endl;
        }
        arquivo_dadosPartida.close();
    } else {
        cout << "Erro ao abrir o arquivo de histórico de partidas." << endl;
    }
}

void tela_EditarPerfil() {

    system("cls"); // Limpa o console antes de exibir a tela de editarperfil

	
    string nomeAtual, senhaAtual;
    
    cout << "****************** EDITAR PERFIL ******************" << endl;
    cout << "Digite seu nome de usuario atual: ";
    cin >> nomeAtual;
    cout << "Digite sua senha de usuario atual: ";
    cin >> senhaAtual;

    salvarEditarPerfil(nomeAtual, senhaAtual);

} 
    
void tela_Regras() {

    system("cls"); // Limpa o console antes de exibir a tela das regras

	
    cout << "         ****************** REGRAS DO JOGO ******************" << endl;
    
    cout << "        1. Regra 1: A primeira pessoa so pode sair da sua casinha se o numero que cair no dado for igual a 1 ou 6.\n" << endl;  
	cout << "        2. Regra 2: Enquando nao cair 1 ou 6 o jogador nao pode sair de sua casinha com a peca.  \n" << endl;
	cout << "        3. Regra 3: Quando a pessoa sair da casinha ela deve jogar novamente.  \n" << endl;
    cout << "        4. Regra 4: Se durante a partida o jogador tirar o numero 6 ao rodar o dado ele podera rodar novamente o dado limitado a 3 lances seguidos fora lance o original.  \n" << endl;

    // Implemente as regras do jogo conforme necess�rio
}

