#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo a estrutura para representar um produto
struct Produto {
    int codigo;
    char nome[50];
    float preco;
};

// Função para adicionar um novo registro ao arquivo
void adicionarRegistro(FILE *arquivo) {
    struct Produto novoProduto;

    // Solicitando informações ao usuário para o novo produto
    printf("Digite o código do produto: ");
    scanf("%d", &novoProduto.codigo);
    printf("Digite o nome do produto: ");
    scanf(" %[^\n]", novoProduto.nome);
    printf("Digite o preço do produto: ");
    scanf("%f", &novoProduto.preco);

    int opcao;
    printf("Deseja adicionar o registro no início (1) ou no final (2) do arquivo? ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        // Abrindo um arquivo temporário para escrever os registros existentes
        FILE *temp = fopen("temp.txt", "w");

        // Escrevendo o novo produto no início do arquivo temporário
        fprintf(temp, "%d,%s,%.2f\n", novoProduto.codigo, novoProduto.nome, novoProduto.preco);

        // Lendo os registros existentes no arquivo original e escrevendo no arquivo temporário
        rewind(arquivo);
        struct Produto produto;
        while (fscanf(arquivo, "%d,%49[^,],%f\n", &produto.codigo, produto.nome, &produto.preco) == 3) {
            fprintf(temp, "%d,%s,%.2f\n", produto.codigo, produto.nome, produto.preco);
        }

        // Fechando os arquivos
        fclose(arquivo);
        fclose(temp);

        // Removendo o arquivo original e renomeando o temporário
        remove("produtos.txt");
        rename("temp.txt", "produtos.txt");
    } else if (opcao == 2) {
        // Escrevendo o novo produto no final do arquivo
        fprintf(arquivo, "%d,%s,%.2f\n", novoProduto.codigo, novoProduto.nome, novoProduto.preco);
    } else {
        printf("Opção inválida. O registro será adicionado no final do arquivo.\n");
        // Escrevendo o novo produto no final do arquivo
        fprintf(arquivo, "%d,%s,%.2f\n", novoProduto.codigo, novoProduto.nome, novoProduto.preco);
    }
}

// Função para listar todos os registros contidos no arquivo
void listarRegistros(FILE *arquivo) {
    struct Produto produto;

    // Voltando para o início do arquivo
    rewind(arquivo);

    // Lendo e exibindo cada registro
    printf("Todos os registros:\n");
    while (fscanf(arquivo, "%d,%49[^,],%f\n", &produto.codigo, produto.nome, &produto.preco) == 3) {
        printf("Código: %d, Nome: %s, Preço: %.2f\n", produto.codigo, produto.nome, produto.preco);
    }
}

// Função para buscar um registro pelo código
void buscarRegistro(FILE *arquivo, int codigo) {
    struct Produto produto;
    int encontrado = 0;

    // Voltando para o início do arquivo
    rewind(arquivo);

    // Buscando o registro pelo código
    while (fscanf(arquivo, "%d,%49[^,],%f\n", &produto.codigo, produto.nome, &produto.preco) == 3) {
        if (produto.codigo == codigo) {
            printf("Registro encontrado:\n");
            printf("Código: %d, Nome: %s, Preço: %.2f\n", produto.codigo, produto.nome, produto.preco);
            encontrado = 1;
            break;
        }
    }

    // Verificando se o registro foi encontrado
    if (!encontrado) {
        printf("Registro não encontrado.\n");
    }
}

// Função para remover um registro pelo código
void removerRegistro(FILE *arquivo, int codigo) {
    FILE *temp;
    struct Produto produto;
    int encontrado = 0;

    // Abrindo um arquivo temporário para escrever os registros exceto o que será removido
    temp = fopen("temp.txt", "w");

    // Voltando para o início do arquivo
    rewind(arquivo);

    // Escrevendo no arquivo temporário todos os registros exceto o que será removido
    while (fscanf(arquivo, "%d,%49[^,],%f\n", &produto.codigo, produto.nome, &produto.preco) == 3) {
        if (produto.codigo != codigo) {
            fprintf(temp, "%d,%s,%.2f\n", produto.codigo, produto.nome, produto.preco);
        } else {
            encontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Removendo o arquivo original e renomeando o temporário
    remove("produtos.txt");
    rename("temp.txt", "produtos.txt");

    // Verificando se o registro foi removido
    if (encontrado) {
        printf("Registro removido com sucesso.\n");
    } else {
        printf("Registro não encontrado.\n");
    }
}

int main() {
    FILE *arquivo;
    int opcao, codigo;

    // Abrindo o arquivo de produtos (ou criando se não existir)
    arquivo = fopen("produtos.txt", "a+");

    // Verificando se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    do {
        // Menu de opções
        printf("\n1. Adicionar um novo registro\n");
        printf("2. Listar registros\n");
        printf("3. Buscar um registro\n");
        printf("4. Remover um registro\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Executando a opção escolhida pelo usuário
        switch (opcao) {
            case 1:
                adicionarRegistro(arquivo);
                break;
            case 2:
                listarRegistros(arquivo);
                break;
            case 3:
                printf("Digite o código do produto a ser buscado: ");
                scanf("%d", &codigo);
                buscarRegistro(arquivo, codigo);
                break;
            case 4:
                printf("Digite o código do produto a ser removido: ");
                scanf("%d", &codigo);
                removerRegistro(arquivo, codigo);
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    // Fechando o arquivo
    fclose(arquivo);

    return 0;
}
