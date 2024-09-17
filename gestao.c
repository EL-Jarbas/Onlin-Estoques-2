#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestao.h"

// Funções de ordenação Merge Sort
void intercala(int e, int m, int d, Produto v[])
{
  int n1 = (m - e) + 1;
  int n2 = (d - m);
  Produto E[n1];
  Produto D[n2];
  int i, j;

  for (i = 0; i < n1; i++)
  {
    E[i] = v[e + i];
  }
  for (j = 0; j < n2; j++)
  {
    D[j] = v[m + 1 + j];
  }

  i = 0;
  j = 0;
  int k = e;
  while (i < n1 && j < n2)
  {
    if (E[i].codigo < D[j].codigo)
    {
      v[k] = E[i];
      i++;
    }
    else
    {
      v[k] = D[j];
      j++;
    }
    k++;
  }

  while (i < n1)
  {
    v[k] = E[i];
    i++;
    k++;
  }
  while (j < n2)
  {
    v[k] = D[j];
    j++;
    k++;
  }
}

void mergeSort(int i, int f, Produto v[])
{
  if (i < f)
  {
    int meio = (i + f) / 2;
    mergeSort(i, meio, v);
    mergeSort(meio + 1, f, v);
    intercala(i, meio, f, v);
  }
}

// Função responsável por contar a quantidade de produtos cadastrados
int quantidadeDeProdutos(char *company)
{
  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");
  Produto a;
  int cont = 0;
  while (fread(&a, sizeof(Produto), 1, file))
  {
    cont++;
  }
  fclose(file);
  return cont;
}

// Função para ordenar e atualizar o arquivo binário após cadastrar um novo produto void atualizarArquivoOrdenado(char *company, Produto novoProduto)
void atualizarArquivoOrdenado(char *company, Produto novoProduto)
{
  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);

  // Carrega todos os produtos existentes
  int n = quantidadeDeProdutos(company);
  Produto *produtos = (Produto *)malloc((n + 1) * sizeof(Produto));

  FILE *file = fopen(filename, "rb");
  for (int i = 0; i < n; i++)
  {
    fread(&produtos[i], sizeof(Produto), 1, file);
  }
  fclose(file);

  // Adiciona o novo produto
  produtos[n] = novoProduto;

  // Ordena os produtos pelo código
  mergeSort(0, n, produtos);

  // Reescreve o arquivo binário com os produtos ordenados
  file = fopen(filename, "wb");
  for (int i = 0; i <= n; i++)
  {
    fwrite(&produtos[i], sizeof(Produto), 1, file);
  }
  fclose(file);

  free(produtos);
}

// Função responsável por cadastrar produtos
void cadastrar(char *company)
{
  Produto novoProduto;
  printf("Informe os dados do produto na ordem: Codigo Nome Preco e Estoque\n");
  scanf("%d %s %f %d", &novoProduto.codigo, novoProduto.nome, &novoProduto.preco, &novoProduto.quantidade);

  // Atualiza o arquivo binário com o novo produto de forma ordenada
  atualizarArquivoOrdenado(company, novoProduto);
}

// Função responsável por buscar produtos (usando busca binária)
int buscaBinariaRecursiva(int cod, Produto v[], int inicio, int final)
{
  if (inicio > final)
  {
    return -1;
  }
  int meio = (inicio + final) / 2;
  if (v[meio].codigo == cod)
  {
    return meio;
  }
  if (v[meio].codigo < cod)
  {
    return buscaBinariaRecursiva(cod, v, meio + 1, final);
  }
  return buscaBinariaRecursiva(cod, v, inicio, meio - 1);
}

void buscar(int cod, char *company)
{
  int n = quantidadeDeProdutos(company);
  Produto v[n];

  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");
  int i = 0;
  while (fread(&v[i], sizeof(Produto), 1, file))
  {
    i++;
  }
  fclose(file);

  mergeSort(0, n - 1, v);

  int pos = buscaBinariaRecursiva(cod, v, 0, n - 1);

  if (pos == -1)
  {
    printf("Produto nao encontrado\n");
  }
  else
  {
    printf("\nNome: %s\n", v[pos].nome);
    printf("- Codigo: %d\n", v[pos].codigo);
    printf("- Preco: %.2f\n", v[pos].preco);
    printf("- Quantidade em estoque: %d\n", v[pos].quantidade);
  }
}


// Função responsável por listar os produtos
void listar(char *company)
{
  int n = quantidadeDeProdutos(company);
  Produto v[n];

  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");

  int i = 0;
  while (fread(&v[i], sizeof(Produto), 1, file))
  {
    i++;
  }
  fclose(file);

  // Listar produtos já ordenados no arquivo
  for (i = 0; i < n; i++)
  {
    printf("\nNome: %s\n", v[i].nome);
    printf("- Codigo: %d\n", v[i].codigo);
    printf("- Preco: %.2f\n", v[i].preco);
    printf("- Quantidade em estoque: %d\n", v[i].quantidade);
  }
}

// Função que filtra todos os produtos com um estoque menor do que o escolhido pelo o usuário
void filtrandoPeloEstoque(char *company)
{
  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");
  Produto a;
  int entrou = 0;
  int filtro;
  printf("Informe o filtro do estoque que você deseja:");
  scanf("%d", &filtro);
  printf("Produtos com o estoque menor que %d: \n", filtro);
  while (fread(&a, sizeof(Produto), 1, file))
  {
    if (a.quantidade < filtro + 1)
    {
      printf("\nNome: %s\n", a.nome);
      printf("- Codigo: %d\n", a.codigo);
      printf("- Quantidade em estoque: %d\n", a.quantidade);
      entrou++;
    }
  }
  if (entrou == 0)
  {
    printf("Nenhum produto com o estoque menor ou igual a %d.\n", filtro);
  }
  fclose(file);
}

// Função que checa os produtos com o estoque zerado
void estoqueZerado(char *company)
{
  int n = quantidadeDeProdutos(company);
  Produto v[n];

  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");

  int i = 0;
  while (fread(&v[i], sizeof(Produto), 1, file))
  {
    i++;
  }
  fclose(file);

  // Listar produtos com estoque zerado
  int entrou = 0;
  printf("Produtos com o estoque zerado: \n");
  for (i = 0; i < n; i++)
  {
    if (v[i].quantidade == 0)
    {
      printf("\nNome: %s\n", v[i].nome);
      printf("- Codigo: %d\n", v[i].codigo);
      printf("- Quantidade em estoque: %d\n", v[i].quantidade);
      entrou++;
    }
  }

  if (entrou == 0)
  {
    printf("Estoque cheio, nenhum produto com o estoque zerado.\n");
  }
}

// Função responsável por excluir um produto, através do seu código
void excluir(int cod, char *company)
{
  int n = quantidadeDeProdutos(company);
  Produto v[n];
  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");
  int i = 0;
  while (fread(&v[i], sizeof(Produto), 1, file))
  {
    i++;
  }
  fclose(file);
  file = fopen(filename, "wb");
  for (i = 0; i < n; i++)
  {
    if (v[i].codigo != cod)
    {
      fwrite(&v[i], sizeof(Produto), 1, file);
    }
  }
  fclose(file);
}

// Função responsável por editar um produto, através do seu código
void editar(int cod, char *company)
{
  int n = quantidadeDeProdutos(company);
  Produto v[n];
  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");
  int i = 0;
  while (fread(&v[i], sizeof(Produto), 1, file))
  {
    i++;
  }
  fclose(file);
  file = fopen(filename, "wb");
  for (i = 0; i < n; i++)
  {
    if (v[i].codigo == cod)
    {
      int option;
      printf("Escolha uma opcao:\n");
      printf("1 - Editar preco\n");
      printf("2 - Editar estoque\n");
      scanf("%d", &option);
      if (option == 1)
      {
        float p;
        printf("Informe o novo preco:\n");
        scanf("%f", &p);
        v[i].preco = p;
        printf("Preco atualizado: %.2f\n", v[i].preco);
      }
      else if (option == 2)
      {
        int stock;
        printf("Informe o novo estoque:\n");
        scanf("%d", &stock);
        v[i].quantidade = stock;
        printf("Estoque atualizado: %d\n", v[i].quantidade);
      }
      else
      {
        printf("Opcao inválida, tente novamente!");
        editar(cod, company);
      }
    }
    fwrite(&v[i], sizeof(Produto), 1, file);
  }
  fclose(file);
}
