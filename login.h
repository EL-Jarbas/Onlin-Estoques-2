#ifndef LOGIN_H
#define LOGIN_H

#define MAX_EMAIL_LENGTH 80
#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 32

typedef struct empresa {
    int cnpj;
    char senha[MAX_PASSWORD_LENGTH + 1];
    char nome[100];
} Empresa;

// Funções relacionadas ao login e cadastro de empresas
int verificarEmailExistente(const char *email);
int verificarEmail(const char *email);
int email();
int verificar_senha_forte(const char *senha);
int senha_forte(const char *senha);
void cadastrar_empresa();
int login_empresa();

#endif
