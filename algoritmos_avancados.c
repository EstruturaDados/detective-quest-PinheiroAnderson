#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura da Sala
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função para criar uma nova sala dinamicamente
Sala* criarSala(char *nome) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Função para realizar a exploração interativa da mansão
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Verifica se é um nó-folha (fim da linha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou ao fim do caminho. Não há mais salas para explorar aqui.\n");
            break;
        }

        printf("Opções de caminho: ");
        if (atual->esquerda != NULL) printf("[e] Esquerda: %s  ", atual->esquerda->nome);
        if (atual->direita != NULL) printf("[d] Direita: %s  ", atual->direita->nome);
        printf("[s] Sair\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 's') {
            printf("Saindo da exploração...\n");
            break;
        } else if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Caminho inválido ou inexistente! Tente novamente.\n");
        }
    }
}

int main() {
    // --- MONTAGEM DO MAPA (Árvore Binária) ---
    // Nível 0: Raiz
    Sala *hall = criarSala("Hall de Entrada");

    // Nível 1
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");

    // Nível 2
    Sala *jardimInverno = criarSala("Jardim de Inverno");
    Sala *sotao = criarSala("Sótão");
    Sala *despensa = criarSala("Despensa");
    Sala *porao = criarSala("Porão");

    // Conectando as salas (Construção manual da hierarquia)
    hall->esquerda = biblioteca;
    hall->direita = cozinha;

    biblioteca->esquerda = jardimInverno;
    biblioteca->direita = sotao;

    cozinha->esquerda = despensa;
    cozinha->direita = porao;

    // --- INÍCIO DO JOGO ---
    printf("--- BEM-VINDO AO DETECTIVE QUEST ---\n");
    printf("Explore a mansão para encontrar o culpado!\n");

    explorarSalas(hall);

    printf("\nObrigado por jogar Detective Quest!\n");

    
    return 0;
}