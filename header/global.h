// Feito por Nico Ramos - GRR20210574

#ifndef _GLOBAL_
#define _GLOBAL_

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

// -------------------------------- MACROS -------------------------------- //
// ------------ FUNCOES ----------- //
#define MIN(a, b) ( a < b ? a : b )
#define DISP_W (BUF_W * DISP_SCALE)
#define DISP_H (BUF_H * DISP_SCALE)

// ----------- VARIAVEIS --------- //
#define BUF_W 640
#define BUF_H 960
#define L_INI 8
#define C_INI 8
#define DISP_SCALE 1
#define FRAME_RATE 60
#define SPRITES_CNT 7

// ---------------------------- TIPOS DE DADOS ---------------------------- //
// Define o estado da execucao do jogo
typedef enum {CLOSE, INICIO, SERVINDO, JOGANDO, FIMPART, FIMJOGO, HELP} state;

// Estatos do tabuleiro
typedef enum 
{
  PLAYER, 
  CHANGE_GO, 
  MOVE, 
  CHANGE_BACK, 
  FALL, 
  VERIFY, 
  LEVEL,
  NEXT_LEVEL
} state_board;

// Define uma peca do jogo
typedef struct s_pieces
{
 int type; 
 int i;
 int j;
 int dx;
 int dy;
 bool seq;
} piece;

typedef struct s_board
{
  int   maxW;
  int   tamMax;
  int   offset;
  int   spriteSize;
  int   mission;
  int   mission_cnt;
  int   nivel;
  int   next_level_frame_cnt;
  int   new_record_frame_cnt;
  int   mult;
  int   spritesCnt;

  int   tamMaxScaled;
  int   offsetScaled;
  piece **newPieces;
  piece ***pieces;

  bool new_level;
  bool new_record;
  bool record_already_updated;

  long int score;
  long int score_next_level;
  long int record;

  state_board state;
  FILE *arqRecord;

} board;

// ---------------------------- VARIAVEIS GLOBAIS ------------------------- //
extern state               State;
extern board               *Board;
extern int                 Move[4]; 
extern unsigned char       Key[ALLEGRO_KEY_MAX];

// -------------- ALLEGRO ------------- //
extern ALLEGRO_DISPLAY     *Disp;
extern ALLEGRO_BITMAP      *Buf;
extern ALLEGRO_FONT        *Font45;
extern ALLEGRO_FONT        *Font35;
extern ALLEGRO_FONT        *FontNewLevel;
extern ALLEGRO_FONT        *FontNewMission;
extern ALLEGRO_BITMAP      *Bg;
extern ALLEGRO_BITMAP      *BgEffectFlare;
extern ALLEGRO_BITMAP      *BgOverlay;
extern ALLEGRO_BITMAP      *PopUpBase;
extern ALLEGRO_BITMAP      *PopUpHeader;
extern ALLEGRO_BITMAP      *Sprites[SPRITES_CNT];
extern ALLEGRO_TIMER       *Timer;
extern ALLEGRO_EVENT_QUEUE *Queue;

// ----------------------------- FUNCOES ---------------------------------- //
// Retorna um inteiro aleatorio entre lo e hi
int alet(int lo, int hi);

// Cria uma peca do tabuleiro
void make_piece(int i, int j, int lo, int hi);

// Gera uma matriz de pecas de l linhas e c colunas com
// os tipos de peca variando entre lo e hi
void gen_rand_matriz(int l, int c, int lo, int hi);

// Gera um vetor de pecas de c colunas com os tipo das
// pecas variando entre lo e hi
void gen_new_pieces(int c, int lo, int hi);

// Registra o evento de teclado
void keyboard_update(ALLEGRO_EVENT *event);

// Registra o evento de mouse
void mouse_update(ALLEGRO_EVENT *event, int *clickCnt);

#endif

