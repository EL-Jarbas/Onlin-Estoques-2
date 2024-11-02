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

typedef struct lista Lista;
typedef struct no No;

// Funções relacionadas à gestão de produtos
void cadastrar(char *company);
void listar(char *company);
void excluir(int cod, char *company);
void editar(int cod, char *company);
void estoqueZerado(char *company);
void filtrandoPeloEstoque(char *company);
void buscar(int cod, char *company);
void atualizarArquivoOrdenado(char *company, Produto novoProduto);

#endif
