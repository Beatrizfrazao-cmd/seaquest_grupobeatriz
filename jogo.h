#ifndef JOGO_H
#define JOGO_H

#define LARGURA 60
#define ALTURA 20

typedef struct {
    int x, y;
    int direcao; 
    int oxigenio;
    int vidas;
    int pontos;
} Submarino;

typedef struct {
    int x, y;
    int ativo;
    int tipo;         
    int direcao;       
    int velocidade;     
    int contador_frame; 
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