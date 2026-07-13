// main.c
#include <stdio.h>
#include <windows.h> 
#include "jogo.h"
#include "tela.h"

int main() {
    Submarino player;
    Entidade entidades[10]; 
    Tiro tiros[5]; 

    esconder_cursor();
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 
    // animação do start
    limpar_tela();

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    
    char msg_titulo[] = "S E A Q U E S T";
    mover_cursor(22, 3); 
    
    for(int i = 0; msg_titulo[i] != '\0'; i++) {
        printf("%c", msg_titulo[i]);
        fflush(stdout);
        Sleep(100); 
    }

    int largura_caixa = 50;
    int posX = (LARGURA - largura_caixa) / 2; 
    int posY = 6;

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); 
    mover_cursor(posX, posY++);
    printf("==================================================");
    mover_cursor(posX, posY++);
    printf("              CONTROLES DO SUBMARINO              ");
    mover_cursor(posX, posY++);
    printf("==================================================");
    mover_cursor(posX, posY++);
    printf("   W / S  : Subir e Descer                        ");
    mover_cursor(posX, posY++);
    printf("   A / D  : Mover para os Lados                   ");
    mover_cursor(posX, posY++);
    printf("   ESPACO : Disparar Torpedos                     ");
    mover_cursor(posX, posY++);
    printf("   Q      : Sair do Jogo                          ");
    mover_cursor(posX, posY++);
    printf("==================================================");

    int frame_pisca = 0;
    while (1) {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) break; 
        if (GetAsyncKeyState('Q') & 0x8000) return 0;

        mover_cursor(14, 16); 
        
        if (frame_pisca % 2 == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf(" PRESSIONE ESPACO PARA INICIAR ");
        } else {
            printf("                               ");
        }

        fflush(stdout);
        Sleep(350); 
        frame_pisca++;
    }

    limpar_tela();
    Sleep(300);

    // inicialização e loop principal 

    inicializar_jogo(&player, entidades, tiros); 

    int cooldown_tiro = 0; 
    int cooldown_y = 0; 

    while (player.vidas > 0) {
        if (cooldown_tiro > 0) cooldown_tiro--;
        if (cooldown_y > 0) cooldown_y--;

        // Movimentação Vertical
        if (cooldown_y == 0) {
            if (GetAsyncKeyState('W') & 0x8000 && player.y > 0) {
                player.y--;
                cooldown_y = 3; 
            }
            else if (GetAsyncKeyState('S') & 0x8000 && player.y < ALTURA - 1) {
                player.y++;
                cooldown_y = 3; 
            }
        }
        
        // Movimentação Horizontal
        if (GetAsyncKeyState('A') & 0x8000 && player.x > 0) { 
            player.x--; 
            player.direcao = -1; 
        }
        if (GetAsyncKeyState('D') & 0x8000 && player.x < LARGURA - 16) { 
            player.x++; 
            player.direcao = 1; 
        }
        
        if (GetAsyncKeyState('Q') & 0x8000) break; 
        
        if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && cooldown_tiro == 0) {
            atirar(&player, tiros); 
            cooldown_tiro = 5; 
        }

        atualizar_jogo(&player, entidades, tiros); 
        desenhar_jogo(&player, entidades, tiros);  

        Sleep(20); 
    }

    
    // animção do game over, basicamente a mesma coisa do start 
    limpar_tela();
    mostrar_cursor();
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    
    char msg_gameover[] = "G A M E   O V E R";
    mover_cursor(21, 4); 
    
    for(int i = 0; msg_gameover[i] != '\0'; i++) {
        printf("%c", msg_gameover[i]);
        fflush(stdout);
        Sleep(90); 
    }

    for(int f = 0; f < 6; f++) {
        mover_cursor(21, 4);
        if (f % 2 == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        }
        printf("%s", msg_gameover);
        fflush(stdout);
        Sleep(250); 
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    posX = (LARGURA - largura_caixa) / 2; 
    posY = 8;                                     

    mover_cursor(posX, posY++);
    printf("==================================================");
    mover_cursor(posX, posY++);
    
    if (player.oxigenio <= 0) {
        printf("         MOTIVO: ASFIXIA NAS PROFUNDEZAS!         ");
        mover_cursor(posX, posY++);
        printf("==================================================");
        mover_cursor(posX, posY++);
        printf("   O tanque de oxigenio esgotou completamente!   ");
        mover_cursor(posX, posY++);
        printf("   A tripulacao ficou sem ar antes de emergir.   ");
        mover_cursor(posX, posY++);
        printf("   Dica: Fique de olho no topo da tela (~)        ");
    } else {
        printf("           MOTIVO: SUBMARINO DESTRUIDO!           ");
        mover_cursor(posX, posY++);
        printf("==================================================");
        mover_cursor(posX, posY++);
        printf("   O casco do seu submarino nao aguentou os danos.");
        mover_cursor(posX, posY++);
        printf("   Os tubaroes venceram a batalha desta vez!      ");
        mover_cursor(posX, posY++);
        printf("   Dica: Use o ESPACO para se defender com tiros. ");
    }
    
    mover_cursor(posX, posY++);
    printf("--------------------------------------------------");
    mover_cursor(posX, posY++);
    printf("           PONTUACAO FINAL: %3d PONTOS            ", player.pontos);
    mover_cursor(posX, posY++);
    printf("==================================================");

    printf("\n\n\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return 0;
}