#include <stdio.h>
#include "tela.h"

void limpar_tela() {
    printf("\033[2J"); 
}

void mover_cursor(int x, int y) {
   
    printf("\033[%d;%dH", y + 1, x + 1);
}

void esconder_cursor() {
    printf("\033[?25l");
}

void mostrar_cursor() {
    printf("\033[?25h");
}