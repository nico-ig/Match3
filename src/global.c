// Feito por Nico Ramos - GRR20210574

#include <stdio.h>

#include "../header/global.h"

#define KEY_SEEN     1
#define KEY_RELEASED 2

// ---------------------------- VARIAVEIS GLOBAIS ------------------------- //
state               State;
board               *Board;
int                 Move[4];
long                Frame;
unsigned char       Key[ALLEGRO_KEY_MAX];

// -------------- ALLEGRO ------------- //
ALLEGRO_DISPLAY     *Disp;
ALLEGRO_BITMAP      *Buf;
ALLEGRO_FONT        *Font45;
ALLEGRO_FONT        *Font35;
ALLEGRO_FONT        *FontNewLevel;
ALLEGRO_FONT        *FontNewMission;
ALLEGRO_BITMAP      *Bg;
ALLEGRO_BITMAP      *BgEffectFlare;
ALLEGRO_BITMAP      *BgOverlay;
ALLEGRO_BITMAP      *PopUpBase;
ALLEGRO_BITMAP      *PopUpHeader;
ALLEGRO_BITMAP      *Sprites[SPRITES_CNT];
ALLEGRO_TIMER       *Timer;
ALLEGRO_EVENT_QUEUE *Queue;

// ----------------------------- FUNCOES ---------------------------------- //
// Retorna um inteiro aleatorio entre lo e hi
int alet(int lo, int hi)
{
  return lo + (rand() % (hi - lo + 1));
}

// Cria uma peca do tabuleiro
void make_piece(int i, int j, int lo, int hi)
{
  Board->pieces[i][j]->i = i;
  Board->pieces[i][j]->j = j;
  Board->pieces[i][j]->dx = 0;
  Board->pieces[i][j]->dy = 0;
  Board->pieces[i][j]->seq = false;
  Board->pieces[i][j]->type = alet(lo, hi); 
}

// Gera uma matriz de pecas de l linhas e c colunas com
// os tipos de peca variando entre lo e hi
void gen_rand_matriz(int l, int c, int lo, int hi)
{
  for ( int i = 0; i < l; i++ )
    for ( int j = 0; j < c; j++ )
      make_piece(i, j, lo, hi);
}

// Gera um vetor de pecas de c colunas com os tipo das
// pecas variando entre lo e hi
void gen_new_pieces(int c, int lo, int hi)
{
  for ( int j = 0; j < c; j++ )
  {
    Board->newPieces[j]->type = alet(lo, hi);
    Board->newPieces[j]->i = 0;
    Board->newPieces[j]->j = j;
    Board->newPieces[j]->dx = 0;
    Board->newPieces[j]->dy = 0;
  }
}

// Incrementa o contador de clicks somente se o click foi em uma regiao do
// tabuleiro, se nao, volta para 0
int cnt_click(int i, int j)
{ 
  if ( (Move[i] != -1) && (Move[j] != -1) )
    return j + 1;
    
  return 0;
}

// Registra o evento de teclado
void keyboard_update(ALLEGRO_EVENT *event)
{
  switch(event->type)
  {
    case ALLEGRO_EVENT_TIMER:
      for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
        Key[i] &= KEY_SEEN;
      break;

    case ALLEGRO_EVENT_KEY_DOWN:
      Key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
      break;

    case ALLEGRO_EVENT_KEY_UP:
      Key[event->keyboard.keycode] &= KEY_RELEASED;
      break;
  }
}

// Calcula o indice na matriz de pecas correspondente a posicao (x, y)
void get_piece_mouse(int x, int y, int *l, int *c)
{
  *l = -1; *c = -1;

  // o clique nao foi na area do tabuleiro
  if ( (x <= Board->offsetScaled) || (y <= Board->offsetScaled) ||
       (x >= Board->offsetScaled + Board->tamMaxScaled * C_INI) ||
       (y >= Board->offsetScaled + Board->tamMaxScaled * L_INI) )
  {
    return;
  }

  // Calcula o indice referente a posicao
  for ( int lim=Board->offsetScaled; x>lim; (*c)++, lim+=Board->tamMaxScaled);
  for ( int lim=Board->offsetScaled; y>lim; (*l)++, lim+=Board->tamMaxScaled);
}

// Registra o evento de mouse
void mouse_update(ALLEGRO_EVENT *event, int *clickCnt)
{
  int i, j;
  switch(event->type)
  {
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      i = *clickCnt;
      j = *clickCnt + 1;
      get_piece_mouse(event->mouse.x, event->mouse.y, &Move[i], &Move[j]);
      *clickCnt = cnt_click(*clickCnt, *clickCnt+1);
      break;
  }
}


