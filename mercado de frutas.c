#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Fruta {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
} Fruta;

typedef struct Nodo {
    Fruta *fruta;
    struct Nodo *prox;
} Nodo;

Nodo *listaFrutas = NULL;

void cadastrarFruta();
void listarFrutas();
void buscarFruta();
void alterarFruta();
void excluirFruta();
void venderFruta();
void salvarVenda(int codigo, char *nome, int quantidade, float preco);
Nodo *buscarFrutaPorCodigo(int codigo);
int codigoExiste(int codigo);

int main() {
    int opcao;

    do {
        printf("\n==== Mercado de Frutas ====\n");
        printf("1. Cadastrar fruta\n");
        printf("2. Listar frutas\n");
        printf("3. Buscar fruta\n");
        printf("4. Alterar fruta\n");
        printf("5. Excluir fruta\n");
        printf("6. Vender fruta\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do teclado

        switch (opcao) {
            case 1: cadastrarFruta(); break;
            case 2: listarFrutas(); break;
            case 3: buscarFruta(); break;
            case 4: alterarFruta(); break;
            case 5: excluirFruta(); break;
            case 6: venderFruta(); break;
            case 7: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 7);

    return 0;
}

void cadastrarFruta() {
    Fruta *novaFruta = (Fruta *) malloc(sizeof(Fruta));
    if (!novaFruta) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    printf("Digite o codigo da fruta: ");
    scanf("%d", &novaFruta->codigo);

    if (codigoExiste(novaFruta->codigo)) {
        printf("Codigo ja existe! Tente outro codigo.\n");
        free(novaFruta);
        return;
    }

    printf("Digite o nome da fruta: ");
    getchar(); // Limpar o buffer do teclado
    fgets(novaFruta->nome, 50, stdin);
    novaFruta->nome[strcspn(novaFruta->nome, "\n")] = 0; // Remover o \n do final

    printf("Digite a quantidade da fruta: ");
    scanf("%d", &novaFruta->quantidade);

    printf("Digite o preco da fruta: ");
    scanf("%f", &novaFruta->preco);

    Nodo *novoNodo = (Nodo *) malloc(sizeof(Nodo));
    novoNodo->fruta = novaFruta;
    novoNodo->prox = listaFrutas;
    listaFrutas = novoNodo;

    printf("Fruta cadastrada com sucesso!\n");
}

void listarFrutas() {
    if (listaFrutas == NULL) {
        printf("Nenhuma fruta cadastrada.\n");
        return;
    }

    Nodo *atual = listaFrutas;
    printf("\nLista de frutas:\n");
    while (atual != NULL) {
        printf("Codigo: %d | Nome: %s | Quantidade: %d | Preco: %.2f\n",
               atual->fruta->codigo, atual->fruta->nome, atual->fruta->quantidade, atual->fruta->preco);
        atual = atual->prox;
    }
}

void buscarFruta() {
    int codigo;
    printf("Digite o codigo da fruta: ");
    scanf("%d", &codigo);

    Nodo *nodo = buscarFrutaPorCodigo(codigo);
    if (nodo) {
        printf("Codigo: %d | Nome: %s | Quantidade: %d | Preco: %.2f\n",
               nodo->fruta->codigo, nodo->fruta->nome, nodo->fruta->quantidade, nodo->fruta->preco);
    } else {
        printf("Fruta nao encontrada.\n");
    }
}

void alterarFruta() {
    int codigo;
    printf("Digite o codigo da fruta para alterar: ");
    scanf("%d", &codigo);

    Nodo *nodo = buscarFrutaPorCodigo(codigo);
    if (nodo) {
        printf("Digite o novo nome da fruta: ");
        getchar();
        fgets(nodo->fruta->nome, 50, stdin);
        nodo->fruta->nome[strcspn(nodo->fruta->nome, "\n")] = 0;

        printf("Digite a nova quantidade: ");
        scanf("%d", &nodo->fruta->quantidade);

        printf("Digite o novo preco: ");
        scanf("%f", &nodo->fruta->preco);

        printf("Fruta alterada com sucesso!\n");
    } else {
        printf("Fruta nao encontrada.\n");
    }
}

void excluirFruta() {
    int codigo;
    printf("Digite o codigo da fruta para excluir: ");
    scanf("%d", &codigo);

    Nodo *anterior = NULL;
    Nodo *atual = listaFrutas;

    while (atual != NULL && atual->fruta->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Fruta nao encontrada.\n");
    } else if (atual->fruta->quantidade > 0) {
        printf("Fruta nao pode ser excluida pois ainda tem estoque.\n");
    } else {
        if (anterior == NULL) {
            listaFrutas = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }
        free(atual->fruta);
        free(atual);
        printf("Fruta excluida com sucesso!\n");
    }
}

void venderFruta() {
    int codigo, quantidade;
    printf("Digite o codigo da fruta para venda: ");
    scanf("%d", &codigo);

    Nodo *nodo = buscarFrutaPorCodigo(codigo);
    if (!nodo) {
        printf("Fruta nao encontrada.\n");
        return;
    }

    printf("Digite a quantidade a ser vendida: ");
    scanf("%d", &quantidade);

    if (quantidade > nodo->fruta->quantidade) {
        printf("Quantidade em estoque insuficiente!\n");
    } else {
        nodo->fruta->quantidade -= quantidade;
        salvarVenda(nodo->fruta->codigo, nodo->fruta->nome, quantidade, nodo->fruta->preco);
        printf("Venda realizada com sucesso!\n");
    }
}

void salvarVenda(int codigo, char *nome, int quantidade, float preco) {
    FILE *arquivo = fopen("vendas.txt", "a");
    if (!arquivo) {
        printf("Erro ao abrir arquivo de vendas.\n");
        return;
    }
    fprintf(arquivo, "Codigo: %d | Nome: %s | Quantidade: %d | Preco: %.2f | Total: %.2f\n",
            codigo, nome, quantidade, preco, quantidade * preco);
    fclose(arquivo);
}

Nodo *buscarFrutaPorCodigo(int codigo) {
    Nodo *atual = listaFrutas;
    while (atual != NULL) {
        if (atual->fruta->codigo == codigo) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

int codigoExiste(int codigo) {
    return buscarFrutaPorCodigo(codigo) != NULL;
}
