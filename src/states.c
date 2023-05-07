// Feito por Nico Ramos -  GRR20210574

#include <stdio.h>

#include "../header/global.h"
#include "../header/draw.h"
#include "../header/states.h"
#include "../header/init.h"
#include "../header/play.h"
#include "../header/close.h"

// Executa o estagio de inicializacao
void state_init()
{
  addons_init(); 
  fonts_init();
  keyboard_init();
  mouse_init();
  timer_init();
  queue_init();
  disp_init();
  bg_init();
  sprites_init();
  board_init();
  popup_init();
  add_event_source();
  record_init();
  for ( int i = 0; i < 4; i++ ) { Move[i] = -1; }
  State = SERVINDO;
}

// Executa o estagio de servir
void state_serve()
{
  Board->spritesCnt = SPRITES_CNT - 3;
  Board->nivel = 1;

  // Garante que o jogo inicia com alguma jogada possivel
  do
  {
    gen_rand_matriz(L_INI, C_INI, 0, Board->spritesCnt);
    replace_seq();
  } while ( game_over(0, 0) );

  gen_new_pieces(C_INI, 0, Board->spritesCnt);
  
  // Estado do tabuleiro
  Board->state = PLAYER;
  Board->mission_cnt = 0;
  Board->nivel = 1;
  Board->mult = 1;
  Board->new_level  = false;
  Board->new_record = false;
  Board->score = 0;
  Board->score_next_level = 1000;
  Board->mission = alet(0, SPRITES_CNT - 3); 
  Board->next_level_frame_cnt = 0;
  Board->new_record_frame_cnt = 0;
  Board->record_already_updated = false;
  
  Key[ALLEGRO_KEY_R] = 0;
  disp_redraw(0);
  State = JOGANDO;  
}

// Laco principal, executa o jogo
void state_play()
{
  ALLEGRO_EVENT event;

  bool done = false;
  bool redraw = true;
  bool help = false;
  bool playAgain = false;
  int clickCnt = 0;
  int moves = 0;

  Key[ALLEGRO_KEY_H]  = 0;
  Key[ALLEGRO_KEY_F1] = 0;

  while(1)
  {
    al_wait_for_event(Queue, &event);

    switch(event.type)
    {
      case ALLEGRO_EVENT_TIMER:
        play_game(&clickCnt, &moves);
        redraw = true;
        if ( Key[ALLEGRO_KEY_ESCAPE] )                   { done = true;      }
        if ( Key[ALLEGRO_KEY_R] )                        { playAgain = true; }
        if ( Key[ALLEGRO_KEY_H] || Key[ALLEGRO_KEY_F1] ) { help = true;      }
        if ( Key[ALLEGRO_KEY_P] )                        { easter_egg();     }
        break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
    }

    if ( playAgain )       { State = SERVINDO;   break; }
    if ( done )            { State = FIMJOGO;    break; }
    if ( help )            { State = HELP;       break; }

    if( redraw && al_is_event_queue_empty(Queue) ) 
      redraw = disp_redraw(clickCnt);

    if( Board->state == PLAYER )
    {
      if ( game_over(0, 0) ) { State = FIMPART; break; }
      input_update(&event, &clickCnt);
    }
  } 
}

// Fim de uma partida, apresenta o score e pergunta se quer jogar novamente
void state_over()
{
  bool done = false;
  bool redraw = false;
  bool playAgain = false;

  while (1)
  {
    ALLEGRO_EVENT event;
    al_wait_for_event(Queue, &event);
    
    switch (event.type)
    {
      case ALLEGRO_EVENT_TIMER:
        if (Key[ALLEGRO_KEY_R])      { playAgain = true; }
        if (Key[ALLEGRO_KEY_ESCAPE]) { done = true; }
        redraw = true;
        break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
    }

    if ( playAgain )  { State = SERVINDO;  break; }
    else if ( done )  { State = FIMJOGO; break; }

    if(redraw && al_is_event_queue_empty(Queue)) { redraw = disp_redraw(0); }

    keyboard_update(&event);
  }
}

// Pausa o jogo e imprime a help page
void state_help()
{
  bool help = true;
  bool done = false;
  bool redraw = false;
  bool playAgain = false;

  Key[ALLEGRO_KEY_H]  = 0;
  Key[ALLEGRO_KEY_F1] = 0;

  while (1)
  {
    ALLEGRO_EVENT event;
    al_wait_for_event(Queue, &event);

    switch (event.type)
    {
      case ALLEGRO_EVENT_TIMER:
        if ( Key[ALLEGRO_KEY_R] )                        { playAgain = true;  }
        if ( Key[ALLEGRO_KEY_ESCAPE] )                   { done      = true;  }
        if ( Key[ALLEGRO_KEY_H] || Key[ALLEGRO_KEY_F1] ) { help      = false; }
        redraw = true;
        break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
    }

    if ( !help )           { State = JOGANDO;   break; }
    else if ( playAgain )  { State = SERVINDO;  break; }
    else if ( done )       { State = FIMJOGO;   break; }

    if(redraw && al_is_event_queue_empty(Queue)) { redraw = disp_redraw(0); }

    keyboard_update(&event);
  }
}

// Encerra o jogo
void state_close()
{
  update_new_record();
  board_deinit();
  font_deinit();
  sprites_deinit();
  popup_deinit();
  disp_deinit();
  bg_deinit();
  al_destroy_timer(Timer);
  al_destroy_event_queue(Queue);
  State = CLOSE;
}


