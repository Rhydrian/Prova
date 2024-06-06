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
        // Movendo o cursor para o início do arquivo e escrevendo o novo produto
        rewind(arquivo);
        fwrite(&novoProduto, sizeof(struct Produto), 1, arquivo);
    } else if (opcao == 2) {
        // Movendo o cursor para o final do arquivo e escrevendo o novo produto
        fseek(arquivo, 0, SEEK_END);
        fwrite(&novoProduto, sizeof(struct Produto), 1, arquivo);
    } else {
        printf("Opção inválida. O registro será adicionado no final do arquivo.\n");
        fseek(arquivo, 0, SEEK_END);
        fwrite(&novoProduto, sizeof(struct Produto), 1, arquivo);
    }
}

// Função para listar todos os registros contidos no arquivo
void listarRegistros(FILE *arquivo) {
    struct Produto produto;
    int opcao, totalRegistros = 0;

    // Verificando o número total de registros no arquivo
    fseek(arquivo, 0, SEEK_END);
    totalRegistros = ftell(arquivo) / sizeof(struct Produto);

    // Voltando para o início do arquivo
    rewind(arquivo);

    // Solicitando ao usuário o tipo de visualização desejada
    printf("Escolha o tipo de visualização:\n");
    printf("1. Os 5 primeiros registros\n");
    printf("2. Os 5 últimos registros\n");
    printf("3. Todos os registros\n");
    printf("Opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Os 5 primeiros registros:\n");
            for (int i = 0; i < 5 && fread(&produto, sizeof(struct Produto), 1, arquivo); i++) {
                printf("Código: %d, Nome: %s, Preço: %.2f\n", produto.codigo, produto.nome, produto.preco);
            }
            break;
        case 2:
            printf("Os 5 últimos registros:\n");
            fseek(arquivo, -sizeof(struct Produto) * 5, SEEK_END);
            for (int i = 0; i < 5 && fread(&produto, sizeof(struct Produto), 1, arquivo); i++) {
                printf("Código: %d, Nome: %s, Preço: %.2f\n", produto.codigo, produto.nome, produto.preco);
            }
            break;
        case 3:
            printf("Todos os registros:\n");
            while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
                printf("Código: %d, Nome: %s, Preço: %.2f\n", produto.codigo, produto.nome, produto.preco);
            }
            break;
        default:
            printf("Opção inválida.\n");
    }
}

// Função para buscar um registro pelo código
void buscarRegistro(FILE *arquivo, int codigo) {
    struct Produto produto;
    int encontrado = 0;

    // Voltando para o início do arquivo
    rewind(arquivo);

    // Buscando o registro pelo código
    while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
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

// Função para atualizar um registro pelo código
void atualizarRegistro(FILE *arquivo, int codigo) {
    struct Produto produto;
    int encontrado = 0;

    // Voltando para o início do arquivo
    rewind(arquivo);

    // Buscando o registro pelo código
    while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
        if (produto.codigo == codigo) {
            // Solicitando novas informações ao usuário
            printf("Digite o novo nome do produto: ");
            scanf(" %[^\n]", produto.nome);
            printf("Digite o novo preço do produto: ");
            scanf("%f", &produto.preco);

            // Movendo o cursor de volta e atualizando o registro no arquivo
            fseek(arquivo, -sizeof(struct Produto), SEEK_CUR);
            fwrite(&produto, sizeof(struct Produto), 1, arquivo);
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
    temp = fopen("temp.dat", "wb");

    // Voltando para o início do arquivo
    rewind(arquivo);

    // Escrevendo no arquivo temporário todos os registros exceto o que será removido
    while (fread(&produto, sizeof(struct Produto), 1, arquivo)) {
        if (produto.codigo != codigo) {
            fwrite(&produto, sizeof(struct Produto), 1, temp);
        } else {
            encontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Removendo o arquivo original e renomeando o temporário
    remove("produtos.dat");
    rename("temp.dat", "produtos.dat");

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
    arquivo = fopen("produtos.dat", "ab+");

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
        printf("4. Atualizar um registro\n");
        printf("5. Remover um registro\n");
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
                printf("Digite o código do produto a ser atualizado: ");
                scanf("%d", &codigo);
                atualizarRegistro(arquivo, codigo);
                break;
            case 5:
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