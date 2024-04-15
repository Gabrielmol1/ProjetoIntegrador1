#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // Para usar a função system()

using namespace std;

//inicializacao dos metodos antes da main 

void salvarJogador_txt(string nome, string senha);
void salvarEstatisticas_txt();
void fecharJogo();
void validarLogin(string nome, string senha);
void editarPerfil(string nomeAtual, string senhaAtual);

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

// ABAIXO DESTE PONTO ESTAO AS TELAS  XDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDXDX

void tela_Login() {

    system("cls"); // Limpa o console antes de exibir a tela de login
	
	string nome, senha;
	
	cout << "                   " << endl;
	cout << "         ****************** TELA DE LOGIN ******************" << endl << endl << endl << endl;

    cout << "            Digite 1 para fazer login\n            Digite 2 para cadastrar-se\n            Digite 3 para sair\n    " << endl;
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
	
		cout << "         ****************** CADASTRAR JOGADOR ******************" << endl;

    cout << "Digite seu nome: ";
    cin >> nome;
    cout << "Digite sua senha: ";
    cin >> senha;

    salvarJogador_txt(nome, senha);
  
}

void tela_Jogar() {

    system("cls"); // Limpa o console antes de exibir a tela de cadastro
	
		cout << "         ****************** TELA DO JOGO ******************" << endl;
        cout << "   AINDA NAO FOI FEITO MANe " << endl;

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

