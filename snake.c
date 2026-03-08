#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include "snake.h"

void desenhar_borda(tabuleiro *T){
    int x = 0, y = 0;
    while(x < MAX){
        y = 0;
        while(y < MAX){
            if(x == 0) //em casos especificos ele marca a msm coordenada duas vzs, mas como eh o msm numero, nn me importei com isso
                T->matriz[0][y] = 1;
            if(y == 0)
                T->matriz[x][0] = 1;
            if(x == MAX - 1)
                T->matriz[MAX - 1][y] = 1;
            if(y == MAX - 1)
                T->matriz[x][MAX - 1] = 1;
            y++;
        }
        x++;
    }
}

void zerar_tabuleiro(tabuleiro *T){
    int x, y;
    for(x = 0; x < MAX; x++){
        for(y = 0; y < MAX; y++)
            T->matriz[x][y] = 0;
    }
}

void desenhar_snake(tabuleiro *T, snake *S){
    struct pos_nodo_snake *nodo = S->cabeca;
    while(nodo != NULL){
        T->matriz[nodo->pos_nodo_y][nodo->pos_nodo_x] = 2;
        nodo = nodo->prox;
    }
}

//desenha as bordas, o snake e a comida
void desenhar_tabuleiro(tabuleiro *T, snake *S, pos_food *F){
    zerar_tabuleiro(T);
    desenhar_borda(T);
    desenhar_snake(T, S);
    T->matriz[F->pos_comida_y][F->pos_comida_x] = 3; //spawna a comida
}

void imprimir_tabuleiro(tabuleiro *T){
    for(int y = 0; y < MAX; y++){
        for(int x = 0; x < MAX; x++){
            int tela_y = MAX - 1 - y; 
            switch(T->matriz[y][x]){
                case 1: 
                    mvaddch(tela_y, x, '#'); 
                    break; // simbolo para a borda
                case 2: 
                    mvaddch(tela_y, x, 'O'); 
                    break; // simbolo para o snake
                case 3: 
                    mvaddch(tela_y, x, '&'); 
                    break; // simbolo para a comida
                default: 
                    mvaddch(tela_y, x, ' '); 
                    break;
            }
        }
    }
    refresh();
}

void inicializar_snake(tabuleiro *T, snake *S){
    int x = MAX, y = MAX;

    S->cabeca = malloc(sizeof(struct pos_nodo_snake));
    S->nodo   = malloc(sizeof(struct pos_nodo_snake));
    S->rabo   = malloc(sizeof(struct pos_nodo_snake));

    S->cabeca->pos_nodo_x = x/2;
    S->cabeca->pos_nodo_y = y/2;

    S->nodo->pos_nodo_x = S->cabeca->pos_nodo_x - 1;
    S->nodo->pos_nodo_y = S->cabeca->pos_nodo_y;

    S->rabo->pos_nodo_x = S->cabeca->pos_nodo_x - 2;
    S->rabo->pos_nodo_y = S->cabeca->pos_nodo_y;

    S->cabeca->prox = S->nodo;
    S->nodo->prox = S->rabo;
    S->rabo->prox = NULL;

    S->direcao_atual = 2;
    S->tam = 3;
    S->morto = 0;

    T->matriz[S->cabeca->pos_nodo_y][S->cabeca->pos_nodo_x] = 2;
    T->matriz[S->nodo->pos_nodo_y][S->nodo->pos_nodo_x] = 2;
    T->matriz[S->rabo->pos_nodo_y][S->rabo->pos_nodo_x] = 2;
}

int verificar_colisao(snake *S){
    if(S->cabeca->pos_nodo_x == 0 || S->cabeca->pos_nodo_x == MAX - 1)
        return(1);
    if(S->cabeca->pos_nodo_y == 0 || S->cabeca->pos_nodo_y == MAX - 1)
        return(1);
    return(0);
}

void mover_nodo_snake(snake *S, int x_ant, int y_ant){
    struct pos_nodo_snake *nodo;
    nodo = S->cabeca->prox;
    int x_temp, y_temp;
    while(nodo != NULL){
        x_temp = nodo->pos_nodo_x;
        y_temp = nodo->pos_nodo_y;

        nodo->pos_nodo_x = x_ant;
        nodo->pos_nodo_y = y_ant;

        x_ant = x_temp;
        y_ant = y_temp;

        nodo = nodo->prox;
    }
}

void mover_snake(snake *S, int direcao){
    int x_ant = S->cabeca->pos_nodo_x, y_ant = S->cabeca->pos_nodo_y;
    switch (direcao){
        case 1:
            y_ant++;
            S->cabeca->pos_nodo_y = y_ant;
            break;
        case 2:
            x_ant++;
            S->cabeca->pos_nodo_x = x_ant;
            break;
        case 3:
            y_ant--;
            S->cabeca->pos_nodo_y = y_ant;
            break;
        case 4:
            x_ant--;
            S->cabeca->pos_nodo_x = x_ant;
            break;
    }
    mover_nodo_snake(S, x_ant, y_ant);
}

void gerar_comida(snake *S, pos_food *F){
    struct pos_nodo_snake *nodo;
    while(1){
        F->pos_comida_x = rand() % (MAX - 2) + 1; 
        F->pos_comida_y = rand() % (MAX - 2) + 1; 
        nodo = S->cabeca; 
        int colisao = 0;
        while(nodo != NULL){
            if(F->pos_comida_x == nodo->pos_nodo_x && F->pos_comida_y == nodo->pos_nodo_y){
                colisao = 1;
                break;
            }
            nodo = nodo->prox;
        }
        if(!colisao)
            break;
    }
}

int verificar_comida(snake *S, pos_food *F){
    if(S->cabeca->pos_nodo_x == F->pos_comida_x && S->cabeca->pos_nodo_y == F->pos_comida_y){
        struct pos_nodo_snake *temp = malloc(sizeof(struct pos_nodo_snake));
        temp->pos_nodo_x = S->rabo->pos_nodo_x;
        temp->pos_nodo_y = S->rabo->pos_nodo_y;
        temp->prox = NULL;
        S->rabo->prox = temp;
        S->rabo = temp;
        return(1);
    }
    return(0);
}

void destruir_snake(snake *S){
    struct pos_nodo_snake *nodo = S->cabeca->prox;
    struct pos_nodo_snake *nodo_ant = S->cabeca;
    while(nodo != NULL){
        free(nodo_ant);
        nodo_ant = nodo;
        nodo = nodo->prox;
    }
    free(nodo_ant);
}