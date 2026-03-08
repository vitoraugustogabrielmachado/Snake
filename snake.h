#ifndef SNAKE
#define SNAKE

#define MAX 20

struct pos_nodo_snake{
    int pos_nodo_x;
    int pos_nodo_y;
    struct pos_nodo_snake *prox;
};


typedef struct{
    int pos_comida_x;
    int pos_comida_y;
}pos_food;


typedef struct{
    struct pos_nodo_snake *cabeca;
    struct pos_nodo_snake *rabo;
    struct pos_nodo_snake *nodo;
    int morto;
    int direcao_atual;
    int tam;
}snake;

typedef struct{
    int matriz[MAX][MAX];
    snake pos_cobra;
    pos_food pos_comida;
}tabuleiro;

void desenhar_tabuleiro(tabuleiro *T, snake *S, pos_food *F);

void imprimir_tabuleiro(tabuleiro *T);

void inicializar_snake(tabuleiro *T, snake *S);

int verificar_colisao(snake *S);

void gerar_comida(snake *S, pos_food *F);

int verificar_comida(snake *S, pos_food *F);

void mover_snake(snake *S, int direcao);

void destruir_snake(snake *S);
#endif