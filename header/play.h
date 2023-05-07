// Feito por Nico Ramos - GRR20210574
#ifndef _PLAY_
#define _PLAY_

#include <allegro5/allegro5.h>

#include "../header/global.h"
 
// Registra o evento de input
void input_update(ALLEGRO_EVENT *event, int *clickCnt);

// Logica do jogo
void play_game(int *clickCnt, int *moves);

// Troca duas pecas do tabuleiro
void change_piece(piece *p1, piece *p2);

// Verifica se um movimento eh valido ou nao
bool valid_move(piece *p1, piece *p2);

// Pega uma peca do tabuleiro
piece *get_piece(int i, int j);

// Verifica se uma peca p esta em uma sequencia de 3
bool is_seq(piece *p);

// Troca todas as sequencias do tabuleiro
void replace_seq();

// Verifica se ainda existe alguma jogada possivel
bool game_over(int i, int j);

// Troca os sprites para sprites do pokemon
void easter_egg();

#endif
