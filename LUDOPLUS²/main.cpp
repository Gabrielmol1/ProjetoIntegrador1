#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // Para usar a função system()
#include <cstdlib> // rand 
#include <ctime>// time
#include <vector>
#include <iomanip> // Para formatar a saída

using namespace std;

//inicializacao dos metodos antes da main 

void salvarJogador_txt(string nome, string senha);
void salvarEstatisticas_txt();
void salvarDadosPartida(const Partida& partida);
void jogar_Ludo();
void fecharJogo();
void validarLogin(string nome, string senha);
void editarPerfil(string nomeAtual, string senhaAtual);
int rolarDado();
void inicializarSemente();
string gerarIDPartida(int incremento);
void turnoJogador(int jogador);
void listaDeJogadores();

//inicializacao das telas antes da main 

void tela_Login();
void tela_Menu();
void tela_Cadastrar();
void tela_Jogar();
void tela_Ranking();
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

void salvarEstatisticas_txt(){ // nao esquecer de passar os dados como parametros tipo partidas e pontos
	
 //ofstream arquivo("estatisticas.txt", ios::app);
	 
  //    if (arquivo.is_open()) {
  //       arquivo << "Nome: " << nome << endl;
  //       arquivo << "Senha: " << senha << endl;
  //       arquivo.close();
  //       cout << "Dados salvos com sucesso." << endl;
  //  } else {
  //       cout << "Erro ao salvar os dados." << endl;
  //   }
	
}

void salvarDadosPartida(const Partida& partida) {
    ofstream arquivo("partida.txt");

    if (arquivo.is_open()) {
        arquivo << "ID da Partida: " << partida.id << endl;
        arquivo << "Quantidade de Jogadores: " << partida.quantidadeJogadores << endl;
        for (const auto& jogador : partida.jogadores) {
            arquivo << "Jogador: " << jogador.first << " (Cor: " << jogador.second << ")" << endl;
        }
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

void editarPerfil(string nomeAtual, string senhaAtual) {

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
                    cout << "Digite seu novo nome: ";
                    cin >> novoNome;
                    cout << "Digite sua nova senha: ";
                    cin >> novaSenha;

                    arquivo_temporario << "Nome: " << novoNome << endl;
                    arquivo_temporario << "Senha: " << novaSenha << endl;

                    jogadorEncontrado = true;
                    // Não é necessário ler mais linhas do arquivo, saia do loop interno
                    break;
                } else {
                    // Senha atual incorreta, manter os dados originais
                    arquivo_temporario << linha << endl;
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

int rolarDado(){
     // Geração de número aleatório de 1 a 6
    return srand() % 6 + 1;
}

void inicializarSemente() {
    srand(time(NULL));
}

bool verificarUmOuSeis(int numero) {
    return (numero == 1 || numero == 6);
}

struct Peca {
    int posicao;
    bool naToca; // Indica se a peça está na "toca" ou em jogo
};

struct Jogador {
    string nome;
    string cor;
    vector<Peca> pecas; // Pode ser um vetor com as peças do jogador
};

struct Tabuleiro {
    vector<Jogador> jogadores;
    // Outros elementos do tabuleiro, como casas especiais, por exemplo
};

string gerarIDPartida(int incremento) {
    // Obter a data e hora atuais
    auto agora = std::chrono::system_clock::now();
    std::time_t tempo_agora = std::chrono::system_clock::to_time_t(agora);
    
    // Converter a data e hora para uma string formatada
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tempo_agora), "%Y%m%d%H%M%S"); // Formato: AAAAMMDDHHMMSS

    // Adicionar o valor de incremento ao final do ID da partida
    ss << "_" << incremento;

    return ss.str();
}

void turnoJogador(int jogador) {
    int numeroDado;
    // Realizando o lançamento do dado até obter 1 ou 6
    do {
        numeroDado = rolarDado();
        cout << "Jogador " << jogador << ", você rolou o dado! O número obtido é: " << numeroDado << endl;
        if (!verificarUmOuSeis(numeroDado)) {
            cout << "Você não pode sair da casinha ainda. Passando a vez para o próximo jogador." << endl;
            return; // Saindo do turno do jogador atual
        }
        cout << "Parabéns! Você pode sair da casinha e jogar novamente." << endl;
    } while (numeroDado == 6); // Jogar novamente se o número for 6

    // Aqui você pode adicionar a lógica para mover as peças do Ludo
    // conforme as regras do jogo.
}

void listaDeJogadores() {

    ifstream arquivo_jogadores("jogadores.txt");
    if (arquivo_jogadores.is_open()) {
        vector<string> nomes;
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
            cout << "Jogador encontrado: " << *it << endl;
            // Aqui você pode realizar alguma ação com o jogador selecionado
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

void jogar_Ludo() {
    int incremento_sequencial = 1;

    // Gerar o ID da partida
    string id_partida = gerarIDPartida(incremento_sequencial);

    int num_jogadores;
    cout << "Quantos jogadores deseja? (2, 3 ou 4): ";
    cin >> num_jogadores;

    // Mostrar lista de jogadores e permitir que cada jogador escolha sua cor
    vector<string> cores_disponiveis = {"Vermelho", "Azul", "Verde", "Amarelo"};
    vector<pair<string, string>> jogadores_e_cores; // Pares de jogador e cor escolhida

    for (int i = 1; i <= num_jogadores; ++i) {
        cout << "Escolha o jogador " << i << " da lista abaixo:" << endl;
        listaDeJogadores(); // Mostra a lista de jogadores disponíveis
        int escolha_jogador;
        cin >> escolha_jogador;
        // Adiciona o jogador escolhido com sua cor correspondente
        jogadores_e_cores.push_back(make_pair(nomes[escolha_jogador - 1], ""));

        cout << "Escolha a cor para o jogador " << jogadores_e_cores.back().first << ":" << endl;
        for (int j = 0; j < cores_disponiveis.size(); ++j) {
            cout << j + 1 << ". " << cores_disponiveis[j] << endl;
        }
        int escolha_cor;
        cin >> escolha_cor;
        if (escolha_cor >= 1 && escolha_cor <= cores_disponiveis.size()) {
            jogadores_e_cores.back().second = cores_disponiveis[escolha_cor - 1];
            cout << "O jogador " << jogadores_e_cores.back().first << " escolheu a cor " << jogadores_e_cores.back().second << endl;
            cores_disponiveis.erase(cores_disponiveis.begin() + escolha_cor - 1); // Remover a cor escolhida das opções disponíveis
        } else {
            cout << "Opção inválida. Escolha uma cor válida." << endl;
            --i; // Voltar para o jogador anterior
        }
    }

    // Agora você tem os jogadores e suas cores correspondentes em "jogadores_e_cores"
    // Inicialize o jogo com essas informações
    // ...
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
    cout << "           | Digite 3 para editar seu perfil     |  " << endl;
    cout << "           | Digite 4 para ver as regras do jogo |  " << endl;
    cout << "           | Digite 5 voltar ao login            |  " << endl;
    cout << "           | Digite 6 para sair do jogo          |  " << endl;
    cout << "           |_____________________________________|  " << endl;

    cin >> opcao_Menu;
    
    if(opcao_Menu == 1){
    	
	 tela_Jogar();
    	
	}
	else if (opcao_Menu == 2){
		
	 tela_Ranking();

	}
	else if (opcao_Menu == 3){
		
 	tela_EditarPerfil();

	}
	else if (opcao_Menu == 4){
		
	 tela_Regras();

	}
	else if (opcao_Menu == 5){
		
	 tela_Login();

	}
	else if (opcao_Menu == 6){
		
	 fecharJogo();

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

    inicializarSemente();

    jogar_Ludo();

    // Simulação de um jogo de Ludo com 4 jogadores
    for (int i = 1; i <= 4; ++i) {
        turnoJogador(i);
    }

	
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
 
void tela_EditarPerfil() {

    system("cls"); // Limpa o console antes de exibir a tela de editarperfil

	
    string nomeAtual, senhaAtual;
    
    cout << "****************** EDITAR PERFIL ******************" << endl;
    cout << "Digite seu nome de usuario atual: ";
    cin >> nomeAtual;
    cout << "Digite sua senha de usuario atual: ";
    cin >> senhaAtual;

    editarPerfil(nomeAtual, senhaAtual);

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

