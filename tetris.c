#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Struct que representa uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Fila circular de peças futuras
typedef struct {
    Peca elementos[TAM_FILA];
    int inicio, fim, total;
} Fila;

// Pilha linear de peças reservadas
typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

// Função para gerar uma nova peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int idx = rand() % 4;
    Peca nova;
    nova.nome = tipos[idx];
    nova.id = id;
    return nova;
}

// Inicializa a fila circular
void inicializarFila(Fila *f, int *idGlobal) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->elementos[i] = gerarPeca((*idGlobal)++);
        f->fim = (f->fim + 1) % TAM_FILA;
        f->total++;
    }
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Remove a peça da frente da fila (dequeue)
Peca jogarPeca(Fila *f, int *idGlobal) {
    Peca removida = f->elementos[f->inicio];
    f->elementos[f->inicio] = gerarPeca((*idGlobal)++); // Gera nova peça
    f->inicio = (f->inicio + 1) % TAM_FILA;
    // Não altera total, pois sempre mantém cheia
    return removida;
}

// Move a peça da frente da fila para o topo da pilha (push)
int reservarPeca(Fila *f, Pilha *p, int *idGlobal) {
    if (p->topo == TAM_PILHA - 1) return 0; // Pilha cheia
    p->topo++;
    p->elementos[p->topo] = f->elementos[f->inicio];
    f->elementos[f->inicio] = gerarPeca((*idGlobal)++); // Gera nova peça
    f->inicio = (f->inicio + 1) % TAM_FILA;
    return 1;
}

// Remove a peça do topo da pilha (pop)
int usarReserva(Pilha *p) {
    if (p->topo == -1) return 0; // Pilha vazia
    p->topo--;
    return 1;
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(Fila *f, Pilha *p) {
    printf("\nEstado atual:\n");
    printf("Fila de peças\t");
    for (int i = 0; i < TAM_FILA; i++) {
        int idx = (f->inicio + i) % TAM_FILA;
        printf("[%c %d] ", f->elementos[idx].nome, f->elementos[idx].id);
    }
    printf("\nPilha de reserva \t(Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->elementos[i].nome, p->elementos[i].id);
    }
    printf("\n------------------------------------------\n");
}

// Exibe o menu de opções
void exibirMenu() {
    printf("\nOpções de Ação:\n");
    printf("Código\tAção\n");
    printf("1\tJogar peça\n");
    printf("2\tReservar peça\n");
    printf("3\tUsar peça reservada\n");
    printf("0\tSair\n");
    printf("Opção: ");
}

int main() {
    srand((unsigned)time(NULL));
    Fila fila;
    Pilha pilha;
    int idGlobal = 0;
    int opcao;

    inicializarFila(&fila, &idGlobal);
    inicializarPilha(&pilha);

    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                jogarPeca(&fila, &idGlobal);
                printf("Peça jogada!\n");
                break;
            case 2:
                if (reservarPeca(&fila, &pilha, &idGlobal))
                    printf("Peça reservada!\n");
                else
                    printf("Reserva cheia!\n");
                break;
            case 3:
                if (usarReserva(&pilha))
                    printf("Peça da reserva usada!\n");
                else
                    printf("Reserva vazia!\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}