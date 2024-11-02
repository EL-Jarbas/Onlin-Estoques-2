#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestao.h"

// Funções adicionadas para lidar com listas
struct no
{
  Produto dado;
  struct no *prox;
};

struct lista
{
  No *inicio;
};

void adicionaFim(Lista *lista, Produto e)
{
  No *novo = (No *)malloc(sizeof(No));
  novo->dado = e;
  novo->prox = NULL;
  if (lista->inicio == NULL)
  {
    lista->inicio = novo;
  }
  else
  {
    No *pi;
    for (pi = lista->inicio; pi->prox != NULL; pi = pi->prox)
      ;
    pi->prox = novo;
  }
}

void editarLista(Lista *lista, int cod)
{
  if (lista->inicio == NULL)
  {
    printf("Lista vazia\n");
  }
  else
  {
    No *pi;
    for (pi = lista->inicio; pi != NULL && pi->dado.codigo != cod; pi = pi->prox)
      ;
    if (pi == NULL)
    {
      printf("Produto nao encontrado\n");
    }
    else
    {
      int option;
      printf("Escolha uma opcao:\n");
      printf("1 - Editar preco\n");
      printf("2 - Editar estoque\n");
      scanf("%d", &option);
      if (option == 1)
      {
        float price;
        printf("Informe o novo preco:\n");
        scanf("%f", &price);
        pi->dado.preco = price;
        printf("Preco atualizado: %.2f\n", pi->dado.preco);
      }
      else if (option == 2)
      {
        int stock;
        printf("Informe o novo estoque:\n");
        scanf("%i", &stock);
        pi->dado.quantidade = stock;
        printf("Estoque atualizado: %d\n", pi->dado.quantidade);
      }
      else
      {
        printf("Opcao inválida, tente novamente!");
        editarLista(lista, cod);
      }
    }
  }
}

void deletar(Lista *lista, int cod)
{
  if (lista->inicio == NULL)
  {
    printf("Lista vazia\n");
  }
  else
  {
    No *pa = NULL;
    No *pi;
    for (pi = lista->inicio; pi->prox != NULL && pi->dado.codigo != cod; pi = pi->prox)
    {
      pa = pi;
    }
    if (pa == NULL)
    {
      if (pi->dado.codigo == cod)
      {
        lista->inicio = pi->prox;
        free(pi);
      }
      else
      {
        printf("Elemento nao encontrado\n");
      }
    }
    else
    {
      if (pi->dado.codigo == cod)
      {
        pa->prox = pi->prox;
        free(pi);
      }
      else
      {
        printf("Elemento nao encontrado\n");
      }
    }
  }
}

void lerLista(Lista *lista, char *company)
{
  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");
  Produto e;
  while (fread(&e, sizeof(Produto), 1, file))
  {
    adicionaFim(lista, e);
  }
  fclose(file);
}

void salvaLista(Lista *lista, char *company)
{
  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "wb");
  No *pi;
  for (pi = lista->inicio; pi != NULL; pi = pi->prox)
  {
    fwrite(&pi->dado, sizeof(Produto), 1, file);
  }
  fclose(file);
}

// Função para dividir a lista encadeada em duas metades
void dividirLista(No *origem, No **frente, No **tras)
{
  No *rapido;
  No *devagar;

  if (origem == NULL || origem->prox == NULL)
  {
    *frente = origem;
    *tras = NULL;
  }
  else
  {
    devagar = origem;
    rapido = origem->prox;

    // O rápido se move duas vezes mais rápido que o devagar
    while (rapido != NULL)
    {
      rapido = rapido->prox;
      if (rapido != NULL)
      {
        devagar = devagar->prox;
        rapido = rapido->prox;
      }
    }

    *frente = origem;
    *tras = devagar->prox;
    devagar->prox = NULL;
  }
}

// Função para mesclar duas sublistas ordenadas
No *mergeListas(No *a, No *b)
{
  No *resultado = NULL;

  if (a == NULL)
    return b;
  else if (b == NULL)
    return a;

  // Compara os códigos dos produtos e mescla as listas
  if (a->dado.codigo <= b->dado.codigo)
  {
    resultado = a;
    resultado->prox = mergeListas(a->prox, b);
  }
  else
  {
    resultado = b;
    resultado->prox = mergeListas(a, b->prox);
  }

  return resultado;
}

// Função principal do Merge Sort para listas encadeadas
void mergeSort(No **cabeca)
{
  No *h = *cabeca;
  No *a;
  No *b;

  // Caso base: lista com 0 ou 1 nó
  if ((h == NULL) || (h->prox == NULL))
  {
    return;
  }

  // Divide a lista em duas metades
  dividirLista(h, &a, &b);

  // Aplica o Merge Sort recursivamente em cada metade
  mergeSort(&a);
  mergeSort(&b);

  // Mescla as duas metades ordenadas
  *cabeca = mergeListas(a, b);
}

// Função para ordenar e atualizar o arquivo binário após cadastrar um novo produto
void atualizarArquivoOrdenado(char *company, Produto novoProduto)
{
  // Cria uma lista de produtos
  Lista lista;
  lista.inicio = NULL;

  // Carrega todos os produtos existentes na lista
  lerLista(&lista, company);

  // Adiciona o novo produto à lista
  adicionaFim(&lista, novoProduto);
  // Ordena a lista usando Merge Sort
  mergeSort(&(lista.inicio));

  // Reescreve o arquivo binário com os produtos ordenados
  salvaLista(&lista, company);
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

// Função para encontrar o tamanho da lista
int tamanhoLista(No *cabeca)
{
  int tamanho = 0;
  while (cabeca != NULL)
  {
    tamanho++;
    cabeca = cabeca->prox;
  }
  return tamanho;
}

// Função para obter o enésimo nó da lista
No *obterNo(No *cabeca, int indice)
{
  int contador = 0;
  while (cabeca != NULL && contador < indice)
  {
    cabeca = cabeca->prox;
    contador++;
  }
  return cabeca;
}

// Função recursiva para realizar busca binária em uma lista encadeada
No *buscaBinariaLista(No *cabeca, int esquerda, int direita, int chave)
{
  if (direita >= esquerda)
  {
    int meio = esquerda + (direita - esquerda) / 2;

    // Obtém o nó do meio
    No *meioNo = obterNo(cabeca, meio);

    // Verifica se a chave está no nó do meio
    if (meioNo != NULL && meioNo->dado.codigo == chave)
    {
      return meioNo;
    }

    // Se a chave é menor que o código no meio, busca na metade esquerda
    if (meioNo != NULL && meioNo->dado.codigo > chave)
    {
      return buscaBinariaLista(cabeca, esquerda, meio - 1, chave);
    }

    // Senão, busca na metade direita
    return buscaBinariaLista(cabeca, meio + 1, direita, chave);
  }

  // Se a chave não for encontrada, retorna NULL
  return NULL;
}

void buscar(int cod, char *company)
{
  Lista lista;
  lista.inicio = NULL;

  // Carrega todos os produtos existentes na lista
  lerLista(&lista, company);

  // Calcula o tamanho da lista
  int tamanho = tamanhoLista(lista.inicio); // Passa o ponteiro para o início da lista

  // Realiza a busca binária
  No *resultado = buscaBinariaLista(lista.inicio, 0, tamanho - 1, cod); // Passa o ponteiro para o início da lista

  if (resultado != NULL)
  {
    printf("\nNome: %s\n", resultado->dado.nome);
    printf("- Codigo: %d\n", resultado->dado.codigo);
    printf("- Preco: %.2f\n", resultado->dado.preco);
    printf("- Quantidade em estoque: %d\n", resultado->dado.quantidade);
  }
  else
  {
    printf("Produto com codigo %d nao encontrado.\n", cod);
  }
}

// Função responsável por listar os produtos
void listar(char *company)
{
  char filename[105];
  snprintf(filename, sizeof(filename), "%s.b", company);
  FILE *file = fopen(filename, "rb");
  Produto x;
  while (fread(&x, sizeof(Produto), 1, file))
  {
    printf("\nNome: %s\n", x.nome);
    printf("- Codigo: %d\n", x.codigo);
    printf("- Preco: %.2f\n", x.preco);
    printf("- Quantidade em estoque: %d\n", x.quantidade);
  }
  fclose(file);
}

// Função que filtra todos os produtos com um estoque menor do que o escolhido pelo usuário
void filtrandoPeloEstoque(char *company)
{
  Lista lista;
  lista.inicio = NULL;
  lerLista(&lista, company); // Carrega produtos da lista

  No *pi = lista.inicio;
  int filtro;
  printf("Informe o filtro do estoque que voce deseja: ");
  scanf("%d", &filtro);
  printf("Produtos com o estoque menor que %d: \n", filtro);

  int entrou = 0;
  while (pi != NULL)
  {
    if (pi->dado.quantidade < filtro + 1)
    {
      printf("\nNome: %s\n", pi->dado.nome);
      printf("- Codigo: %d\n", pi->dado.codigo);
      printf("- Quantidade em estoque: %d\n", pi->dado.quantidade);
      entrou++;
    }
    pi = pi->prox;
  }

  if (entrou == 0)
  {
    printf("Nenhum produto com o estoque menor ou igual a %d.\n", filtro);
  }
}

// Função que checa os produtos com o estoque zerado
void estoqueZerado(char *company)
{

  Lista lista;
  lista.inicio = NULL;
  lerLista(&lista, company); // Carrega produtos da lista

  No *pi = lista.inicio;
  int entrou = 0;
  printf("Produtos com o estoque zerado: \n");

  while (pi != NULL)
  {
    if (pi->dado.quantidade == 0)
    {
      printf("\nNome: %s\n", pi->dado.nome);
      printf("- Codigo: %d\n", pi->dado.codigo);
      printf("- Quantidade em estoque: %d\n", pi->dado.quantidade);
      entrou++;
    }
    pi = pi->prox;
  }

  if (entrou == 0)
  {
    printf("Estoque cheio, nenhum produto com o estoque zerado.\n");
  }
}

// Função responsável por excluir um produto, através do seu código
void excluir(int cod, char *company)
{

  Lista lista;
  lista.inicio = NULL;
  lerLista(&lista, company);
  deletar(&lista, cod);
  salvaLista(&lista, company);
}

// Função responsável por editar um produto, através do seu código
void editar(int cod, char *company)
{
  Lista lista;
  lista.inicio = NULL;
  lerLista(&lista, company);
  editarLista(&lista, cod);
  salvaLista(&lista, company);
}
