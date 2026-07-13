#include <stdio.h>
#include <stdlib.h> 
#include "jogo.h"
#include "tela.h"

// 
void inicializar_jogo(Submarino *player, Entidade inimigos[], Tiro tiros[]) {
    player->x = LARGURA / 2;
    player->y = ALTURA / 2;
    player->direcao = 1;
    player->oxigenio = 100;
    player->vidas = 3;
    player->pontos = 0;

    for(int i = 0; i < 10; i++) {
        inimigos[i].ativo = 0;
        inimigos[i].contador_frame = 0;
        inimigos[i].velocidade = 1;
    }
    for(int i = 0; i < 5; i++) tiros[i].ativo = 0; 
}

void atirar(Submarino *player, Tiro tiros[]) {
    for (int i = 0; i < 5; i++) {
        if (!tiros[i].ativo) { 
            tiros[i].ativo = 1;
            tiros[i].direcao = player->direcao;
            tiros[i].y = player->y;
            // Ajusta o ponto de saída do torpedo com base na frente do submarino
            tiros[i].x = (player->direcao == 1) ? player->x + 8 : player->x - 1; 
            break;
        }
    }
}

void atualizar_jogo(Submarino *player, Entidade inimigos[], Tiro tiros[]) {
    static int frames_jogados = 0;
    frames_jogados++;

    // 1. Oxigênio Dinâmico com Perda Acelerada
    static int oxigenio_timer = 0;
    int velocidad_oxigenio = 18 - (frames_jogados / 250);
    
    if (velocidad_oxigenio < 5) velocidad_oxigenio = 5;

    if (player->y == 0) {
        player->oxigenio = 100; // Recarrega ao emergir
        oxigenio_timer = 0;
    } else {
        oxigenio_timer++;
        if (oxigenio_timer >= velocidad_oxigenio) { 
            player->oxigenio--;
            oxigenio_timer = 0;
        }
        if (player->oxigenio <= 0) {
            player->vidas = 0; // Game Over por falta de ar
        }
    }

    // spawn de Tubarões Progressivo
    int chance_tubarao = 3 + (frames_jogados / 400);
    if (chance_tubarao > 12) chance_tubarao = 12;

    if (rand() % 100 < chance_tubarao) { 
        for (int i = 0; i < 10; i++) {
            if (!inimigos[i].ativo) { 
                inimigos[i].ativo = 1;
                inimigos[i].tipo = 1;  // 1 = Tubarão
                inimigos[i].y = (rand() % (ALTURA - 4)) + 2; 
                inimigos[i].contador_frame = 0;
                inimigos[i].velocidade = (rand() % 3) + 1; 
                
                if (rand() % 2 == 0) {
                    inimigos[i].x = 0;
                    inimigos[i].direcao = 1; 
                } else {
                    inimigos[i].x = LARGURA - 6; 
                    inimigos[i].direcao = -1; 
                }
                break; 
            }
        }
    }

    // Spawn de Mergulhadores 
    if (rand() % 400 < 1) {
        for (int i = 0; i < 10; i++) {
            if (!inimigos[i].ativo) {
                inimigos[i].ativo = 1;
                inimigos[i].tipo = 2;  // 2 = Mergulhador
                inimigos[i].y = (rand() % (ALTURA - 4)) + 2;
                inimigos[i].contador_frame = 0;
                inimigos[i].velocidade = (rand() % 2) + 2; 
                
                if (rand() % 2 == 0) {
                    inimigos[i].x = 0;
                    inimigos[i].direcao = 1;
                } else {
                    inimigos[i].x = LARGURA - 4; 
                    inimigos[i].direcao = -1;
                }
                break;
            }
        }
    }

    // movimentação dos npcs
    for(int i = 0; i < 10; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].contador_frame++;
            if (inimigos[i].contador_frame >= inimigos[i].velocidade) {
                inimigos[i].x += inimigos[i].direcao; 
                inimigos[i].contador_frame = 0;
            }
            if (inimigos[i].x >= LARGURA || inimigos[i].x < 0) {
                inimigos[i].ativo = 0; 
            }
        }
    }

    // lógica do tiro
    static int tiro_timer = 0;
    tiro_timer++;
    if (tiro_timer >= 2) { 
        for(int i = 0; i < 5; i++) {
            if (tiros[i].ativo) {
                tiros[i].x += tiros[i].direcao * 2; 
                if (tiros[i].x < 0 || tiros[i].x >= LARGURA) tiros[i].ativo = 0;
            }
        }
        tiro_timer = 0;
    }

    // colisão do tiro com o tubarão
    for(int i = 0; i < 5; i++) {
        if (tiros[i].ativo) {
            for(int j = 0; j < 10; j++) {
                if (inimigos[j].ativo && inimigos[j].tipo == 1) { 
                    if (tiros[i].y == inimigos[j].y && tiros[i].x >= inimigos[j].x && tiros[i].x <= inimigos[j].x + 5) {
                        tiros[i].ativo = 0;   
                        inimigos[j].ativo = 0; 
                        player->pontos += 10;  
                        break;
                    }
                }
            }
        }
    }

    //  Submarino x Entidades
    for(int i = 0; i < 10; i++) {
        if (inimigos[i].ativo) {
            if (player->y == inimigos[i].y) {
                if (player->x < inimigos[i].x + 5 && player->x + 8 > inimigos[i].x) {
                    
                    if (inimigos[i].tipo == 1) {
                        player->vidas--;        
                        inimigos[i].ativo = 0;   
                        player->x = LARGURA / 2;
                        player->y = ALTURA / 2;
                    } 
                    else if (inimigos[i].tipo == 2) {
                        player->vidas++; // Resgata o mergulhador e ganha vida
                        inimigos[i].ativo = 0; 
                    }
                    break;
                }
            }
        }
    }
}

void desenhar_jogo(Submarino *player, Entidade inimigos[], Tiro tiros[]) {
    limpar_tela();

    mover_cursor(0, 0);
    printf("VIDAS: %d | PONTOS: %d | OXIGENIO: %d%%", player->vidas, player->pontos, player->oxigenio);

    mover_cursor(0, 1);
    for(int i = 0; i < LARGURA; i++) printf("~");

    // Lógica da animação da hélice (+ ou x alternando usando a posição do submarino)
    char helice = (player->x % 2 == 0) ? '+' : 'x';

    // adiciona a hélice no desenho sdo submarino
    mover_cursor(player->x, player->y + 2); 
    if (player->direcao == 1) {
        printf("%c=[o o]>", helice);
    } else {
        printf("<[o o]=%c", helice);
    }

    // Desenho dos inimigos e dos mergulhadores
    for(int i = 0; i < 10; i++) {
        if (inimigos[i].ativo) {
            mover_cursor(inimigos[i].x, inimigos[i].y + 2);
            if (inimigos[i].tipo == 1) {
                if (inimigos[i].direcao == 1) printf("<^///"); 
                else printf("///^>");                          
            } 
            else if (inimigos[i].tipo == 2) {
                if (inimigos[i].direcao == 1) printf("~~o"); 
                else printf("o~~");                          
            }
        }
    }

    for(int i = 0; i < 5; i++) {
        if (tiros[i].ativo) {
            mover_cursor(tiros[i].x, tiros[i].y + 2);
            printf("-");
        }
    }
    
    fflush(stdout); 
}