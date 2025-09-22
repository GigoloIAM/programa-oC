#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAM_FILA 5
#define TAM_PILHA 3

//---------------------- Estruturas -----------------------
typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int frente, tras, qtd;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

//---------------------- Variáveis globais -----------------
int contadorID = 0; // ID único para cada peça

//---------------------- Funções de Utilidade --------------
char gerarNome() {
    char tipos[] = {'I','O','T','L'};
    return tipos[rand() % 4];
}

Peca gerarPeca() {
    Peca p;
    p.nome = gerarNome();
    p.id = contadorID++;
    return p;
}

//---------------------- Fila ------------------------------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->tras = (f->tras + 1) % TAM_FILA;
        f->itens[f->tras] = p;
        f->qtd++;
    }
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (!filaVazia(f)) {
        removida = f->itens[f->frente];
        f->frente = (f->frente + 1) % TAM_FILA;
        f->qtd--;
    }
    return removida;
}

//---------------------- Pilha -----------------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void push(Pilha *p, Peca x) {
    if (!pilhaCheia(p)) {
        p->itens[++p->topo] = x;
    }
}

Peca pop(Pilha *p) {
    Peca removida = {'-', -1};
    if (!pilhaVazia(p)) {
        removida = p->itens[p->topo--];
    }
    return removida;
}

Peca topoPilha(Pilha *p) {
    if (!pilhaVazia(p)) return p->itens[p->topo];
    Peca vazio = {'-', -1};
    return vazio;
}

//---------------------- Visualização ----------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n---------------- ESTADO ATUAL ----------------\n");
    printf("Fila de peças:\t");
    int idx = f->frente;
    for (int i = 0; i < f->qtd; i++) {
        Peca atual = f->itens[idx];
        printf("[%c %d] ", atual.nome, atual.id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\nPilha de reserva (Topo -> Base):\t");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n----------------------------------------------\n");
}

//---------------------- Operações -------------------------
void jogarPeca(Fila *fila) {
    if (!filaVazia(fila)) {
        Peca jogada = desenfileirar(fila);
        printf("Ação: Peça [%c %d] jogada da fila.\n", jogada.nome, jogada.id);
        if (!filaCheia(fila)) enfileirar(fila, gerarPeca());
    } else {
        printf("Fila vazia!\n");
    }
}

void reservarPeca(Fila *fila, Pilha *pilha) {
    if (pilhaCheia(pilha)) {
        printf("Pilha cheia! Não é possível reservar.\n");
        return;
    }
    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }
    Peca movida = desenfileirar(fila);
    push(pilha, movida);
    printf("Ação: Peça [%c %d] movida para a pilha.\n", movida.nome, movida.id);
    if (!filaCheia(fila)) enfileirar(fila, gerarPeca());
}

void usarReservada(Pilha *pilha) {
    if (!pilhaVazia(pilha)) {
        Peca usada = pop(pilha);
        printf("Ação: Peça [%c %d] usada (removida da pilha).\n", usada.nome, usada.id);
    } else {
        printf("Pilha vazia!\n");
    }
}

void trocarFrenteTopo(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("Troca não possível (fila ou pilha vazia).\n");
        return;
    }
    Peca temp = fila->itens[fila->frente];
    fila->itens[fila->frente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;
    printf("Ação: Frente da fila e topo da pilha trocados.\n");
}

void trocaMultipla(Fila *fila, Pilha *pilha) {
    if (fila->qtd < 3 || pilha->topo + 1 < 3) {
        printf("Troca múltipla não possível (faltam peças).\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int posFila = (fila->frente + i) % TAM_FILA;
        Peca temp = fila->itens[posFila];
        fila->itens[posFila] = pilha->itens[pilha->topo - i];
        pilha->itens[pilha->topo - i] = temp;
    }
    printf("Ação: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

//---------------------- Programa Principal -----------------
int main() {
    srand((unsigned)time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila cheia
    for (int i = 0; i < TAM_FILA; i++)
        enfileirar(&fila, gerarPeca());

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogarPeca(&fila); break;
            case 2: reservarPeca(&fila, &pilha); break;
            case 3: usarReservada(&pilha); break;
            case 4: trocarFrenteTopo(&fila, &pilha); break;
            case 5: trocaMultipla(&fila, &pilha); break;
            case 0: printf("Encerrando programa...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
