#include <stdio.h>
#include "gestao.h"
#include "login.h"

// Esse será o menu de opções
void menu()
{
  printf("\n===================\n");
  printf("Escolha uma opcao:\n");
  printf("1 - Cadastrar produtos\n");
  printf("2 - Remover produtos\n");
  printf("3 - Atualizar produto\n");
  printf("4 - Listar produtos cadastrados\n");
  printf("5 - Filtrar produtos pelo estoque\n");
  printf("6 - Listar produtos sem estoque\n");
  printf("7 - Buscar produto\n");
  printf("0 - Sair\n");
  printf("===================\n");
}

// Função principal
int main()
{
  int login_sucesso = 0;
  int opcao;

  while (1)
  {
    printf("\n===================\n");
    printf("Bem Vindos a Onlin-Estoques:\n");
    printf("1 - Cadastrar empresa\n");
    printf("2 - Login empresa\n");
    printf("0 - Sair\n");
    printf("===================\n");
    scanf("%d", &opcao);

    if (opcao == 1)
    {
      cadastrar_empresa();
    }
    else if (opcao == 2)
    {
      login_sucesso = login_empresa();
      if (login_sucesso)
      {
        break;
      }
    }
    else if (opcao == 0)
    {
      return 0;
    }
    else
    {
      printf("Opcao invalida. Tente novamente.\n");
    }
  }

  // Aqui começará o menu de opções após o login
  while (1)
  {
    menu();
    scanf("%d", &opcao);
    char nome_empresa[100];

    if (opcao == 0)
    {
      printf("Saindo...\n");
      break;
    }
    else if (opcao >= 1 && opcao <= 7)
    {
      printf("Informe o nome da empresa: ");
      scanf(" %[^\n]%*c", nome_empresa);
    }

    switch (opcao)
    {
    case 1:
      cadastrar(nome_empresa);
      break;
    case 2:
      printf("Informe o codigo do produto a ser removido: ");
      int cod_rem;
      scanf("%d", &cod_rem);
      excluir(cod_rem, nome_empresa);
      break;
    case 3:
      printf("Informe o codigo do produto a ser atualizado: ");
      int cod_edit;
      scanf("%d", &cod_edit);
      editar(cod_edit, nome_empresa);
      break;
    case 4:
      listar(nome_empresa);
      break;
    case 5:
      filtrandoPeloEstoque(nome_empresa);
      break;
    case 6:
      estoqueZerado(nome_empresa);
      break;
    case 7:
      printf("Informe o codigo do produto a ser buscado: ");
      int cod_busca;
      scanf("%d", &cod_busca);
      buscar(cod_busca, nome_empresa);
      break;
    default:
      printf("Opcao invalida. Tente novamente.\n");
    }
  }
  return 0;
}
