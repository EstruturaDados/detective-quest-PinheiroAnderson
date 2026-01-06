#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// --- ESTRUTURAS ---

// Nó para a Tabela Hash (Encadeamento para tratar colisões)
typedef struct HashNode {
    char pista[50];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

// Nó para a BST de Pistas coletadas
typedef struct PistaNode {
    char texto[50];
    struct PistaNode *esquerda, *direita;
} PistaNode;

// Nó para o Mapa da Mansão
typedef struct Sala {
    char nome[50];
    char pista[50];
    struct Sala *esquerda, *direita;
} Sala;

// --- FUNÇÕES DE HASH (Mapeamento Pista -> Suspeito) ---

// Função Hash simples: soma dos valores ASCII
int funcaoHash(char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++) soma += str[i];
    return soma % TAM_HASH;
}

// Inserir associação na Tabela Hash
void inserirNaHash(HashNode *tabela[], char *pista, char *suspeito) {
    int indice = funcaoHash(pista);
    HashNode *novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[indice];
    tabela[indice] = novo;
}

// Buscar suspeito associado a uma pista
char* encontrarSuspeito(HashNode *tabela[], char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

// --- FUNÇÕES DE ÁRVORE (Mapa e BST) ---

PistaNode* inserirBST(PistaNode *raiz, char *texto) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->texto, texto);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(texto, raiz->texto) < 0) raiz->esquerda = inserirBST(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0) raiz->direita = inserirBST(raiz->direita, texto);
    return raiz;
}

Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

// --- LÓGICA DE JULGAMENTO ---

// Conta quantas pistas na BST apontam para o suspeito acusado
int contarProvas(PistaNode *raiz, HashNode *tabela[], char *acusado) {
    if (raiz == NULL) return 0;
    int conta = 0;
    char *suspeitoDaPista = encontrarSuspeito(tabela, raiz->texto);
    if (suspeitoDaPista && strcmp(suspeitoDaPista, acusado) == 0) conta = 1;
    return conta + contarProvas(raiz->esquerda, tabela, acusado) + contarProvas(raiz->direita, tabela, acusado);
}

// --- JOGO PRINCIPAL ---

void explorarESolucionar(Sala *inicio, HashNode *tabela[]) {
    Sala *atual = inicio;
    PistaNode *inventario = NULL;
    char escolha, acusado[50];

    while (atual != NULL) {
        printf("\n[SALA]: %s", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("\n[!] Pista encontrada: %s", atual->pista);
            inventario = inserirBST(inventario, atual->pista);
        }
        
        printf("\nIr para: ");
        if (atual->esquerda) printf("[e] %s  ", atual->esquerda->nome);
        if (atual->direita) printf("[d] %s  ", atual->direita->nome);
        printf("[s] Finalizar Investigação\nEscolha: ");
        scanf(" %c", &escolha);

        if (escolha == 's') break;
        if (escolha == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita) atual = atual->direita;
    }

    printf("\n--- O JULGAMENTO ---");
    printf("\nQuem você deseja acusar? ");
    scanf(" %[^\n]s", acusado);

    int provas = contarProvas(inventario, tabela, acusado);
    if (provas >= 2) {
        printf("\nSUCESSO! Você encontrou %d provas contra %s. O culpado foi preso!\n", provas, acusado);
    } else {
        printf("\nFRACASSO. Apenas %d prova(s) não são suficientes. %s escapou!\n", provas, acusado);
    }
}

int main() {
    HashNode *tabela[TAM_HASH] = {NULL};

    // Configurando Tabela Hash (Pista -> Suspeito)
    inserirNaHash(tabela, "Veneno", "Mordomo");
    inserirNaHash(tabela, "Luva Branca", "Mordomo");
    inserirNaHash(tabela, "Relogio de Ouro", "Cozinheiro");
    inserirNaHash(tabela, "Avental Sujo", "Cozinheiro");

    // Configurando Mapa
    Sala *hall = criarSala("Hall", "");
    Sala *biblioteca = criarSala("Biblioteca", "Veneno");
    Sala *cozinha = criarSala("Cozinha", "Avental Sujo");
    Sala *quarto = criarSala("Quarto", "Luva Branca");
    
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = quarto;

    explorarESolucionar(hall, tabela);

    return 0;
}