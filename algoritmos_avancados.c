#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ESTRUTURAS DE DADOS ---

// Estrutura para a Árvore Binária de Busca (BST) de Pistas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura para o Mapa da Mansão (Árvore Binária)
typedef struct Sala {
    char nome[50];
    char pista[100]; // Pista contida nesta sala
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- FUNÇÕES PARA A ÁRVORE DE PISTAS (BST) ---

// Função para inserir uma nova pista na BST (Mantém ordem alfabética)
PistaNode* inserirPista(PistaNode* raiz, char* textoPista) {
    if (textoPista == NULL || strlen(textoPista) == 0) return raiz;

    // Se chegamos em um nó vazio, alocamos a nova pista aqui
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->texto, textoPista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    // Comparação alfabética para decidir o lado da inserção
    if (strcmp(textoPista, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, textoPista);
    } else if (strcmp(textoPista, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, textoPista);
    }
    // Se a pista for idêntica, não inserimos duplicatas
    return raiz;
}

// Função para exibir pistas em ordem alfabética (Percurso Em-Ordem)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->direita);
    }
}

// --- FUNÇÕES PARA O MAPA DA MANSÃO ---

// Função para criar uma sala com uma pista opcional
Sala* criarSala(char* nome, char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    if (pista) strcpy(novaSala->pista, pista);
    else strcpy(novaSala->pista, ""); // Sala sem pista
    
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para controlar a navegação e coleta automática de pistas
void explorarSalasComPistas(Sala* atual) {
    char escolha;
    PistaNode* inventarioPistas = NULL;

    printf("\n--- Iniciando Investigação ---\n");

    while (atual != NULL) {
        printf("\n[LOCALIZAÇÃO]: %s\n", atual->nome);
        
        // Coleta automática de pista se existir
        if (strlen(atual->pista) > 0) {
            printf("[PISTA ENCONTRADA]: %s\n", atual->pista);
            inventarioPistas = inserirPista(inventarioPistas, atual->pista);
        } else {
            printf("Não há pistas óbvias nesta sala.\n");
        }

        printf("Para onde deseja ir? ");
        if (atual->esquerda) printf("[e] %s  ", atual->esquerda->nome);
        if (atual->direita) printf("[d] %s  ", atual->direita->nome);
        printf("[s] Sair e Relatar\nSua escolha: ");
        
        scanf(" %c", &escolha);

        if (escolha == 's') break;
        
        if (escolha == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita) {
            atual = atual->direita;
        } else {
            printf("\nCaminho inválido!\n");
        }
    }

    // Relatório Final
    printf("\n====================================\n");
    printf("RELATÓRIO FINAL DO DETETIVE\n");
    printf("Pistas coletadas (em ordem alfabética):\n");
    if (inventarioPistas == NULL) {
        printf("Nenhuma pista encontrada.\n");
    } else {
        exibirPistas(inventarioPistas);
    }
    printf("====================================\n");
}

// --- FUNÇÃO PRINCIPAL ---

int main() {
    // Montagem do Mapa (Fixa conforme o nível aventureiro)
    Sala* hall = criarSala("Hall de Entrada", "Chave enferrujada");
    Sala* biblioteca = criarSala("Biblioteca", "Diário rasgado");
    Sala* cozinha = criarSala("Cozinha", "Faca de prata");
    Sala* sotao = criarSala("Sótão", "Bilhete anônimo");
    Sala* jardim = criarSala("Jardim", "Pegadas de lama");
    Sala* porao = criarSala("Porão", "Cofre entreaberto");

    // Construção da hierarquia do mapa
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    
    biblioteca->esquerda = jardim;
    biblioteca->direita = sotao;
    
    cozinha->direita = porao;

    // Início do jogo
    explorarSalasComPistas(hall);

    return 0;
}