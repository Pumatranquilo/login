#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <conio.h> // Necessário para ocultar a senha no Windows
#include <windows.h> // Necessário para configurar o console UTF-8

using namespace std;

// Configurar o console para UTF-8
void configurarConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

// Função para carregar dados do "banco de dados"
void carregarDados(unordered_map<string, string>& bancoDeDados) {
    ifstream arquivo("banco_de_dados.txt");
    if (!arquivo.is_open()) {
        return; // Arquivo ainda não existe
    }

    string usuario, senha;
    while (arquivo >> usuario >> senha) {
        bancoDeDados[usuario] = senha;
    }
    arquivo.close();
}

// Função para salvar dados no "banco de dados"
void salvarDados(const unordered_map<string, string>& bancoDeDados) {
    ofstream arquivo("banco_de_dados.txt", ios::trunc);
    for (const auto& par : bancoDeDados) {
        arquivo << par.first << " " << par.second << endl;
    }
    arquivo.close();
}

// Função para ocultar a senha durante a entrada do usuário
string digitarSenha() {
    string senha;
    char ch;
    cout << "Digite a senha (ela será escondida): ";
    while ((ch = _getch()) != '\r') { // '\r' é o Enter
        if (ch == '\b') { // Backspace
            if (!senha.empty()) {
                senha.pop_back();
                cout << "\b \b"; // Remove o último caractere visível
            }
        }
        else {
            senha.push_back(ch);
            cout << '*'; // Mostra um asterisco para cada caractere
        }
    }
    cout << endl;
    return senha;
}

// Função para cadastrar um novo usuário
void cadastrar(unordered_map<string, string>& bancoDeDados) {
    string usuario, senha;
    cout << "=== Cadastro ===\n";
    cout << "Digite o nome de usuário: ";
    cin >> usuario;

    if (bancoDeDados.find(usuario) != bancoDeDados.end()) {
        cout << "Usuário já existe. Tente novamente.\n";
        return;
    }

    senha = digitarSenha();
    bancoDeDados[usuario] = senha;
    salvarDados(bancoDeDados);
    cout << "Cadastro realizado com sucesso!\n";
}

// Função para realizar login
void login(const unordered_map<string, string>& bancoDeDados) {
    string usuario, senha;
    cout << "=== Login ===\n";
    cout << "Digite o nome de usuário: ";
    cin >> usuario;

    auto it = bancoDeDados.find(usuario);
    if (it == bancoDeDados.end()) {
        cout << "Usuário não encontrado. Por favor, cadastre-se primeiro.\n";
        return;
    }

    senha = digitarSenha();
    if (it->second == senha) {
        cout << "Login bem-sucedido! Bem-vindo, " << usuario << "!\n";
    }
    else {
        cout << "Senha incorreta. Tente novamente.\n";
    }
}

int main() {
    setlocale(LC_ALL, "pt-BR.UTF-8");
    configurarConsole();

    unordered_map<string, string> bancoDeDados;
    carregarDados(bancoDeDados);

    int opcao;
    do {
        cout << "\n=== Menu ===\n";
        cout << "1. Cadastrar\n";
        cout << "2. Login\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        switch (opcao) {
        case 1:
            cadastrar(bancoDeDados);
            break;
        case 2:
            login(bancoDeDados);
            break;
        case 0:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opção inválida. Tente novamente.\n";
        }
    } while (opcao != 0);

    return 0;
}
