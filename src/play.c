// Feito por Nico Ramos - GRR20210574

#include <stdio.h>

#include "../header/play.h"
#include "../header/draw.h"
#include "../header/init.h"
#include "../header/global.h"

#define SPEED_CHANGE  4
#define SPEED_FALLING 5
#define PIECE_VALUE   10

// Registra o evento de input
void input_update(ALLEGRO_EVENT *event, int *clickCnt)
{
  keyboard_update(event);
  mouse_update(event, clickCnt);
}

// Executa o movimento do jogador 
void update_move(int *clickCnt, int *moves)
{
  if ( *clickCnt == 4 ) 
  {
    (*moves)++;
    *clickCnt = 0;

    piece *p1 = get_piece(Move[0], Move[1]);
    piece *p2 = get_piece(Move[2], Move[3]);
    
    if ( valid_move(p1, p2) ) { Board->state = CHANGE_GO; }
  }
}

// Verifica se terminou de trocar as pecas
bool finished_changing(piece *p1, int speed)
{
  int dx = abs(p1->dx);
  int dy = abs(p1->dy);
  int dis = Board->tamMax - speed + 1;
  if ( (dx >= dis) || (dy >= dis) )
    return true;

  return false;
}

// Verifica se e uma troca para a esquerda 
bool change_left(piece *p1, piece *p2)
{
  if ( p1->j > p2->j ) { return true; }
  return false;
}

// Verifica se e uma troca para a direita
bool change_right(piece *p1, piece *p2)
{
  if ( p1->j < p2->j ) { return true; }
  return false;
}

// Verifica se e uma troca para cima
bool change_up(piece *p1, piece *p2)
{
  if ( p1->i > p2->i ) { return true; }
  return false;
}

// Incrementa o deslocamento das pecas
void move_pieces()
{
  piece *p1 = get_piece(Move[0], Move[1]);
  piece *p2 = get_piece(Move[2], Move[3]);

  if ( change_left(p1, p2) )  
  { (p1->dx) -= SPEED_CHANGE; (p2->dx) += SPEED_CHANGE; }

  else if ( change_right(p1, p2) ) 
  { (p1->dx) += SPEED_CHANGE; (p2->dx) -= SPEED_CHANGE; }
  
  else if ( change_up(p1, p2) )    
  { (p1->dy) -= SPEED_CHANGE; (p2->dy) += SPEED_CHANGE; }

  else                            
  { (p1->dy) += SPEED_CHANGE; (p2->dy) -= SPEED_CHANGE; }
}

// Reseta os deslocamentos da peca para 0
void reset_piece(piece *p)
{
  p->dx = 0;
  p->dy = 0;
}

// Controla a primeira troca das pecas
void change_go()
{
  piece *p1 = get_piece(Move[0], Move[1]);
  piece *p2 = get_piece(Move[2], Move[3]);

  if ( finished_changing(p1, SPEED_CHANGE) ) 
  { 
    // Reseta os deslocamentos
    reset_piece(p1);
    reset_piece(p2);

    change_piece(p1, p2); 
    Board->state = MOVE; 
    return; 
  }
  
  move_pieces();
}

// Destroca as pecas caso nao seja sequencia
void change_back()
{
  piece *p1 = get_piece(Move[0], Move[1]);
  piece *p2 = get_piece(Move[2], Move[3]);

  if ( finished_changing(p1, SPEED_CHANGE ) ) 
  { 
    // Reseta os deslocamentos
    reset_piece(p1);
    reset_piece(p2);
  
    change_piece(p1, p2); 
    Board->state = PLAYER; 
    return; 
  }
  
  move_pieces();
}

// Acha todas as sequencias do tabuleiro
void find_seq()
{
  for ( int i = 0; i < L_INI; i++ )
    for ( int j = 0; j < C_INI; j++ )
     {
        piece *p = get_piece(i, j);
        if ( is_seq(p) )
          p->seq = true;
     }
}

// Atualiza a pontuacao mais alta
void update_record()
{
  if ( Board->score > Board->record )
  {
    Board->new_record = true;
    Board->record = Board->score;
  }
}

// Calcula o score de uma peca removida
void calc_score(piece *p)
{
  if ( p->type == Board->mission ) 
  { 
    Board->mission_cnt++; 
    Board->score += (PIECE_VALUE * Board->mult * 2);
  }

  else
    Board->score += (PIECE_VALUE * Board->mult);

  update_record();
}   

// Remove todas as sequencias do tabuleiro
bool rm_seq()
{
  bool seq = false;

  find_seq();
  for ( int i = 0; i < L_INI; i++ )
    for ( int j = 0; j < C_INI; j++ )
    {
      piece *p = get_piece(i, j);
      if ( p->seq )
      {
        calc_score(p);
        p->type = -1;
        p->seq = false;
        seq = true;
      }

      else if ( p->type == -1 )
        seq = true;
    }
  
  return seq;
}

void make_move()
{
  piece *p1 = get_piece(Move[0], Move[1]);
  piece *p2 = get_piece(Move[2], Move[3]);

  if ( is_seq(p1) || is_seq(p2) ) { rm_seq(); Board->state = FALL; }
  else                            { Board->state = CHANGE_BACK; }
}

// Troca todas as sequencias do tabuleiro
void replace_seq()
{
  int spritesCnt;
  if ( Board->nivel < 3 ) { spritesCnt = Board->spritesCnt; }
  else                    { spritesCnt = SPRITES_CNT - 2;   }

  for ( int i = 0; i < L_INI; i++ )
    for ( int j = 0; j < C_INI; j++ )
    {
      piece *p = get_piece(i, j);
      while ( is_seq(p) )
        make_piece(i, j, 0, spritesCnt);
    }
}

// Cria uma nova peca
void make_new_piece(int j)
{
  int spritesCnt;
  if ( Board->nivel < 3 ) { spritesCnt = Board->spritesCnt; }
  else                    { spritesCnt = SPRITES_CNT - 1;   }

  Board->newPieces[j]->type = alet(0, spritesCnt);
  Board->newPieces[j]->dy = 0;
}

// Joga o p[i][j] para a primeira linha
void change_above(int i, int j)
{
  while ( i > 0 )
  {
    piece *p = get_piece(i, j);
    piece *pAnt = get_piece(i-1, j);

    reset_piece(pAnt); 
    change_piece(p, pAnt);

    i--;
  }

  Board->pieces[0][j]->type = Board->newPieces[j]->type;
  make_new_piece(j);
}

// As pecas na coluna j e acima do indice i caem
void fall_above(int i, int j)
{
  for ( int aux = i-1; aux >= 0; aux-- )
  {
    piece *pAux = get_piece(aux, j);
    (pAux->dy) += SPEED_FALLING;
  } 

  (Board->newPieces[j]->dy) += SPEED_FALLING;
}

// Faz as pecas de uma coluna cairem
void fall_pieces_collum(int j)
{
  for ( int i = L_INI - 1; i >= 0; i-- )
  {
    piece *p = get_piece(i, j);

    if ( p->type == -1 )
    {
      if ( finished_changing(p, SPEED_FALLING) ) 
      {
        p->dy = 0; 
        change_above(i, j);
      }

      else
      {
        fall_above(i, j); 
        (p->dy) -= SPEED_FALLING; 
        break;
      }
    }
  }
}

// Verifica se já terminou de cair todas as pecas
bool finished_falling()
{
  for ( int j = 0; j < C_INI; j++ )
    for ( int i = 1; i < L_INI; i++ )
    {
      piece *p = get_piece(i, j);
      piece *pAnt = get_piece(i-1, j);
      if ( (p->type == -1) && (pAnt->type != -1) ) { return false; }
    }

  return true;
}

// Faz as pecas do tabuleiro cairem
void fall_pieces()
{
  for ( int j = 0; j < C_INI; j++ )
    fall_pieces_collum(j);

  if ( finished_falling() ) { Board->state = VERIFY; }
}

// Verifica se ainda existe alguma jogada possivel
bool game_over(int i, int j)
{
  if ( (i == L_INI - 1) && (j == C_INI - 1) ) { return true; }

  piece *p = get_piece(i, j);

  // Testa trocar pela direita
  if ( j < C_INI - 1 )
  {
    piece *right = get_piece(i, j+1);
    change_piece(p, right);
    if ( is_seq(p) || is_seq(right) ) {change_piece(p, right); return false;}
    change_piece(p, right);
  }

  // Testa trocar pela de baixo
  if ( i < (L_INI - 1 ) )
  {
    piece *bottom = get_piece(i+1, j);
    change_piece(p, bottom);
    if ( is_seq(p) || is_seq(bottom) ) {change_piece(p, bottom); return false;}
    change_piece(p, bottom);
  }

  // Chegou na ultima coluna, testa na proxima linha
  if ( j == (C_INI - 1) ) { return game_over(i+1, 0); }
  
  // Testa na proxima coluna
  return game_over(i, j+1);
}

// Verifica se nao tem mais pontos no tabuleiro
void verify_board()
{
  if ( rm_seq() ) { Board->state = FALL; }
  else            { Board->state = LEVEL; }
}

// Atualiza o nivel do jogo
void verify_level()
{
  if ( Board->mission_cnt >= (7 + 2 * Board->nivel) )
  {
    Board->score += (PIECE_VALUE * Board->mission_cnt);
    Board->mission_cnt = 0;

    int spritesCnt;
    if ( Board->nivel >= 3 ) { spritesCnt = SPRITES_CNT - 2;   }
    else                     { spritesCnt = Board->spritesCnt; }

    int newMission = alet(0, spritesCnt);
    while ( Board->mission == newMission )
      newMission = alet(0, spritesCnt);

    Board->mission = newMission;
  }

  if ( Board->score >= Board->score_next_level )
  {
    Board->mult++;
    Board->nivel++;
    Board->score_next_level += (1000 * Board->nivel * 1.8);
    Board->new_level = true;

    if ( Board->spritesCnt < SPRITES_CNT )
      Board->spritesCnt++;

   Board->state = NEXT_LEVEL;
  }

  else
    Board->state = PLAYER;

  update_record();
}

void next_level()
{
  int spritesCnt;
  if ( Board->nivel < 3 ) { spritesCnt = Board->spritesCnt; }
  else                    { spritesCnt = SPRITES_CNT - 2;   }

  if ( ! (Board->next_level_frame_cnt % 5) )
  {
    for ( int i = 0; i < L_INI; i++ )
      for ( int j = 0; j < C_INI; j++ )
      {
        piece *p = get_piece(i, j);
        p->type = alet(0, spritesCnt); 
      }
  }

  if ( Board->next_level_frame_cnt == FRAME_RATE )
  {
    Board->next_level_frame_cnt = 0;

    do
    {
      replace_seq();
    } while (game_over(0, 0)) ;

    Board->new_level = false;
    Board->state = PLAYER;
  }

  else
    Board->next_level_frame_cnt++;
}

// Executa um evento de acordo com o tipo dele
void play_game(int *clickCnt, int *moves)
{
  switch (Board->state)
  {
    case PLAYER:        update_move(clickCnt, moves); break;
    case CHANGE_GO:     change_go();                  break;
    case MOVE:          make_move();                  break;  
    case CHANGE_BACK:   change_back();                break;
    case FALL:          fall_pieces();                break;
    case VERIFY:        verify_board();               break;
    case LEVEL:         verify_level();               break;
    case NEXT_LEVEL:    next_level();                 break;
  }
}

// Pega uma peca do tabuleiro
piece *get_piece(int i, int j)
{
  if ( (i == -1) || (j == -1) ) return NULL; 
  return Board->pieces[i][j];
}

// Verifica se um movimento eh valido ou nao
bool valid_move(piece *p1, piece *p2)
{
  int i1 = p1->i;
  int j1 = p1->j;

  int i2 = p2->i;
  int j2 = p2->j;

  return ( ((abs(i1 - i2) == 1) && (abs(j1 - j2) == 0)) ||
           ((abs(i1 - i2) == 0) && (abs(j1 - j2) == 1)) );
}

// Copia uma peca
piece *cpy_piece(piece *p)
{ 
  piece *cpy;
  if ( ! (cpy = malloc(sizeof(piece))) ) { exit(1); }
  
  cpy->i = p->i;
  cpy->j = p->j;
  cpy->type = p->type;

  return cpy;
}

// Acha o elemento do msm tipo de p mais a esquerda
int leftmost(piece *p)
{
  piece *pAux = cpy_piece(p);

  int j = pAux->j;
  while ( j > 0 )
  { 
    free(pAux);
    pAux = cpy_piece(get_piece(p->i, --j)); 
    if ( pAux->type != p->type ) { j++; break; }
  }

  free(pAux);
  return j;
}

// Acha o elemento do msm tipo de p mais a direita
int rightmost(piece *p)
{
  piece *pAux = cpy_piece(p);

  int j = pAux->j;
  while ( j < (C_INI - 1) )
  { 
    free(pAux);
    pAux = cpy_piece(get_piece(p->i, ++j)); 
    if ( pAux->type != p->type ) { j--; break; }
  }

  free(pAux);
  return j;
}

// Acha o elemento do msm tipo de p mais a cima
int uppermost(piece *p)
{
  piece *pAux = cpy_piece(p);

  int i = pAux->i;
  while ( i > 0 )
  { 
    free(pAux);
    pAux = cpy_piece(get_piece(--i, p->j)); 
    if ( pAux->type != p->type ) { i++; break; }
  }

  free(pAux);
  return i;
}

// Acha o elemento do msm tipo de p mais em baixo
int bottommost(piece *p)
{
  piece *pAux = cpy_piece(p);

  int i = pAux->i;
  while ( i < (L_INI - 1) )
  { 
    free(pAux);
    pAux = cpy_piece(get_piece(++i, p->j)); 
    if ( pAux->type != p->type ) { i--; break; }
  }

  free(pAux);
  return i;
}

// Verifica se uma peca p esta em uma sequencia de 3
bool is_seq(piece *p)
{
  if ( (p->type == -1) || (p->type == SPRITES_CNT - 1) ) { return false; }

  int left = leftmost(p);
  if ( (p->j - left) >= 2 ) { return true; }
  
  int right = rightmost(p);
  if ( (right - left) >= 2 ) { return true; }

  int up = uppermost(p);
  if ( (p->i - up) >= 2 ) { return true; }

  int bottom = bottommost(p);
  if ( (bottom - up) >= 2 ) { return true; }

  return false;
}

// Troca duas pecas do tabuleiro
void change_piece(piece *p1, piece *p2 )
{
  int i1 = p1->i;
  int j1 = p1->j;

  int i2 = p2->i;
  int j2 = p2->j;

  // Troca os indices
  p1->i = i2;
  p1->j = j2;

  p2->i = i1;
  p2->j = j1;

  Board->pieces[i1][j1] = p2;
  Board->pieces[i2][j2] = p1;
}

// Troca os sprites para sprites do pokemon
void easter_egg()
{   
  char nome[200], pokemon[12];
  for ( int i = 0; i < SPRITES_CNT; i++ )
  {
    sprintf(nome, "./resources/characters/pokemon_000%d.png", i+1);
    sprintf(pokemon, "pokemon %d", i+1);
    
    al_set_new_bitmap_flags(ALLEGRO_MIPMAP);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
    Sprites[i] = al_load_bitmap(nome);

    must_init(Sprites[i], pokemon);
  }
         
}        
         
         
         
         
         
         
        
         
         
