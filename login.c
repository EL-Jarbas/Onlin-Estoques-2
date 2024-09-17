#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "login.h"


// Função para verificar se o email já existe no arquivo login.txt
int verificarEmailExistente(const char *email)
{
  FILE *arquivo = fopen("login.txt", "r");
  if (arquivo == NULL)
  {
    return 0; // Arquivo não existe, então email não está cadastrado
  }

  char linha[200];
  char email_lido[MAX_EMAIL_LENGTH];

  // Lendo o arquivo linha por linha
  while (fgets(linha, sizeof(linha), arquivo) != NULL)
  {
    sscanf(linha, "%*s %*d %*s %s", email_lido); // Pula o nome, CNPJ, senha e captura o email
    if (strcmp(email_lido, email) == 0)
    {
      fclose(arquivo);
      return 1; // Email já cadastrado
    }
  }

  fclose(arquivo);
  return 0; // Email não encontrado no arquivo
}
// Função para verificar a validade do email
int verificarEmail(const char *email)
{
  if (strlen(email) >= MAX_EMAIL_LENGTH)
  {
    return 0; // Email muito longo
  }
  char *posArroba = strchr(email, '@');
  if (posArroba == NULL)
  {
    return 0; // Email não contém @
  }
  if (verificarEmailExistente(email))
  {
    return 0; // Email já cadastrado
  }
  return 1; // Email válido
}

int email()
{
  char email[MAX_EMAIL_LENGTH];
  int valido = 0;

  while (!valido)
  {
    printf("Digite seu email: ");
    fgets(email, MAX_EMAIL_LENGTH, stdin);
    // Remover o caractere de nova linha de fgets
    email[strcspn(email, "\n")] = 0;

    if (verificarEmail(email))
    {
      valido = 1;
    }
    else
    {
      printf("Email invalido ou ja cadastrado. Por favor, tente novamente.\n");
    }
  }

  // Agora que temos um email válido, vamos adicioná-lo ao arquivo
  FILE *arquivo = fopen("emails.txt", "a");
  if (arquivo == NULL)
  {
    printf("Erro ao abrir o arquivo para escrita.\n");
    return 1;
  }
  fprintf(arquivo, "%s\n", email);
  fclose(arquivo);

  printf("Email cadastrado com sucesso!\n");

  return 0;
}

// Função para verificar se a senha é forte
int verificar_senha_forte(const char *senha)
{
  int tem_maiuscula = 0, tem_minuscula = 0, tem_digito = 0;

  for (int i = 0; senha[i] != '\0'; i++)
  {
    if (isupper(senha[i]))
      tem_maiuscula = 1;
    else if (islower(senha[i]))
      tem_minuscula = 1;
    else if (isdigit(senha[i]))
      tem_digito = 1;
  }

  return tem_maiuscula && tem_minuscula && tem_digito;
}

// Função para verificar se a senha é forte
int senha_forte(const char *senha)
{
  if (strlen(senha) < MIN_PASSWORD_LENGTH || strlen(senha) > MAX_PASSWORD_LENGTH)
  {
    return 0;
  }

  int tem_numero = 0, tem_maiuscula = 0;
  for (size_t i = 0; i < strlen(senha); i++)
  {
    if (isdigit(senha[i]))
    {
      tem_numero = 1;
    }
    if (isupper(senha[i]))
    {
      tem_maiuscula = 1;
    }
  }

  return tem_numero && tem_maiuscula;
}

// Função para cadastrar uma empresa (modificada para salvar o email)
void cadastrar_empresa()
{
  struct empresa emp;
  char senha[MAX_PASSWORD_LENGTH + 1];
  char email[MAX_EMAIL_LENGTH];

  printf("Digite o nome da empresa: ");
  scanf(" %[^\n]%*c", emp.nome);

  printf("Digite o CNPJ (apenas numeros): ");
  scanf("%d", &emp.cnpj);
  fflush(stdin);

  // Verificação e cadastro de email
  while (1)
  {
    printf("Digite seu email: ");
    scanf("%s", email);

    if (verificarEmail(email))
    {
      break;
    }
    else
    {
      printf("Email inválido ou já cadastrado. Por favor, tente novamente.\n");
    }
  }

  // Verificação de senha forte
  while (1)
  {
    printf("Digite a senha (deve ter entre 8 e 32 caracteres, incluir um número e uma letra maiúscula): ");
    scanf("%s", senha);

    if (senha_forte(senha))
    {
      strcpy(emp.senha, senha);
      break;
    }
    else
    {
      printf("Senha fraca. Tente novamente.\n");
    }
  }

  // Salvando as informações no arquivo login.txt
  FILE *file = fopen("login.txt", "a");
  if (file == NULL)
  {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  fprintf(file, "%s %d %s %s\n", emp.nome, emp.cnpj, emp.senha, email); // Salvando o nome, CNPJ, senha e email
  fclose(file);

  printf("Empresa cadastrada com sucesso!\n");
}

// Função para login de uma empresa
int login_empresa()
{
  char nome[100];
  char senha[MAX_PASSWORD_LENGTH + 1];
  int cnpj;
  // char email[MAX_EMAIL_LENGTH];

  printf("Digite o nome da empresa: ");
  scanf(" %[^\n]%*c", nome);

  printf("Digite o CNPJ (apenas números): ");
  scanf("%d", &cnpj);

  printf("Digite a senha: ");
  scanf("%s", senha);

  FILE *file = fopen("login.txt", "r");
  if (file == NULL)
  {
    printf("Erro ao abrir o arquivo!\n");
    return 0;
  }

  struct empresa emp;
  char email_lido[MAX_EMAIL_LENGTH];
  int nome_valido = 0, cnpj_valido = 0, senha_valida = 0;

  while (fscanf(file, "%s %d %s %s", emp.nome, &emp.cnpj, emp.senha, email_lido) != EOF)
  {
    if (strcmp(emp.nome, nome) == 0)
    {
      nome_valido = 1;
      if (emp.cnpj == cnpj)
      {
        cnpj_valido = 1;
        if (strcmp(emp.senha, senha) == 0)
        {
          senha_valida = 1;
          break;
        }
      }
    }
  }

  fclose(file);

  if (!nome_valido)
  {
    printf("Nome da empresa incorreto.\n");
  }
  else if (!cnpj_valido)
  {
    printf("CNPJ incorreto.\n");
  }
  else if (!senha_valida)
  {
    printf("Senha inválida.\n");
  }

  return nome_valido && cnpj_valido && senha_valida;
}
