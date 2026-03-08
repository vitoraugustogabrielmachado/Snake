#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "snake.h"

void inicializar_jogo(tabuleiro *T, snake *S, pos_food *F){
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    srand(time(NULL));
    inicializar_snake(T, S);
    gerar_comida(S, F);
    desenhar_tabuleiro(T, S, F);
    imprimir_tabuleiro(T);
}

void carregar_input(snake *S, int input){
    if(input != ERR){
            switch(input){
                case 'w':
                    //mover para cima 
                    if(S->direcao_atual != 3)
                        S->direcao_atual = 1;
                    break;
                case 'd': 
                    //mover para a direita
                    if(S->direcao_atual != 4)
                        S->direcao_atual = 2;
                    break;
                case 's':
                    //mover para baixo
                    if(S->direcao_atual != 1)
                        S->direcao_atual = 3;
                    break;
                case 'a':
                    //mover para a esquerda
                    if(S->direcao_atual != 2)
                        S->direcao_atual = 4;
                    break;
                default:
                    //segue a direçao q ja tava
                    break;
            }
        }
}

void execucao_jogo(tabuleiro *T, snake *S, pos_food *F){
    int input;
    do{ 
        input = getch();
        carregar_input(S, input);
        mover_snake(S, S->direcao_atual);
        S->morto = verificar_colisao(S);
        if(verificar_comida(S, F))
            gerar_comida(S, F);
        desenhar_tabuleiro(T, S, F);
        clear();
        imprimir_tabuleiro(T);
        napms(160);
    }while(S->morto != 1);
}

//cima 1; direita 2; baixo 3; esquerda 4
int main(){
    tabuleiro T;
    snake S;
    pos_food F;
    inicializar_jogo(&T, &S, &F);
    execucao_jogo(&T, &S, &F);
    destruir_snake(&S);
    endwin();
}