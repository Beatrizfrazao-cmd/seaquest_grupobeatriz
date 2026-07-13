#ifndef JOGO_H
#define JOGO_H

#define LARGURA 60
#define ALTURA 20

typedef struct {
    int x, y;
    int direcao; // 1 = Direita, -1 = Esquerda
    int oxigenio;
    int vidas;
    int pontos;
} Submarino;

typedef struct {
    int x, y;
    int ativo;
    int tipo;          // 1: Tubarão, 2: Mergulhador
    int direcao;       // 1 = Direita, -1 = Esquerda
    int velocidade;     //quantos frames demora pra mover um bloco
    int contador_frame; // contador para gerenciar a velocidade individual
} Entidade;

typedef struct {
    int x, y;
    int ativo;
    int direcao;
} Tiro;

void inicializar_jogo(Submarino *player, Entidade inimigos[], Tiro tiros[]);
void atualizar_jogo(Submarino *player, Entidade inimigos[], Tiro tiros[]);
void desenhar_jogo(Submarino *player, Entidade inimigos[], Tiro tiros[]);
void atirar(Submarino *player, Tiro tiros[]);

#endif