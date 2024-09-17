#ifndef GESTAO_H
#define GESTAO_H

#define MAX_PASSWORD_LENGTH 32

// Definições de estruturas
typedef struct produto
{
    char nome[100];
    int codigo;
    int quantidade;
    float preco;
} Produto;

// Funções relacionadas à gestão de produtos
void mergeSort(int i, int f, Produto v[]);
void cadastrar(char *company);
void listar(char *company);
void excluir(int cod, char *company);
void editar(int cod, char *company);
void estoqueZerado(char *company);
void filtrandoPeloEstoque(char *company);
void buscar(int cod, char *company);
int quantidadeDeProdutos(char *company);
int buscaBinariaRecursiva(int cod, Produto v[], int inicio, int final);
void atualizarArquivoOrdenado(char *company, Produto novoProduto);

#endif
