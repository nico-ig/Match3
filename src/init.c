// Feito por Nico Ramos - GRR20210574

#include <stdio.h>
#include <sys/stat.h>

#include "../header/init.h"
#include "../header/global.h"

// Funcao para verificar se uma estrutura foi iniciada corretamente ou nao, em caso de erro encerra o programa com codigo de 
// erro 1
void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

// Inicializa os addons
void addons_init()
{
  must_init(al_init(), "allegro");
  must_init(al_init_image_addon(), "image");
  must_init(al_init_font_addon(), "font");
  must_init(al_init_ttf_addon(), "font ttf");
}

// Inicializa o display
void disp_init()
{
  Disp = al_create_display(DISP_W, DISP_H);
  must_init(Disp, "display");

  al_set_new_bitmap_flags(ALLEGRO_MIPMAP);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
  Buf = al_create_bitmap(BUF_W, BUF_H);
  must_init(Buf, "bitmap buffer");
}

// Inicializa as fontes
void fonts_init()
{
  Font45 = al_load_font("./resources/font/SnowDream.TTF", 45, 0);
  Font35 = al_load_font("./resources/font/HVD_Comic_Serif_Pro.otf", 35, 0);
  FontNewLevel = al_load_font("./resources/font/SnowDream.TTF", 100, 0);
  FontNewMission = al_load_font("./resources/font/SnowDream.TTF", 70, 0);
}

// Inicializa o teclado
void keyboard_init() 
{ 
  must_init(al_install_keyboard(), "keyboard");
  memset(Key, 0, sizeof(Key)); 
}

// Inicializa o mouse
void mouse_init()
{
  must_init(al_install_mouse(), "mouse");
}

// Inicializa o background
void bg_init()
{
  al_set_new_bitmap_flags(ALLEGRO_MIPMAP);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
  Bg = al_load_bitmap("./resources/ui_assets/bg.png");
  must_init(Bg, "background");

  al_set_new_bitmap_flags(ALLEGRO_MIPMAP);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
  BgEffectFlare = al_load_bitmap("./resources/ui_assets/bg_effect_flare.png");
  must_init(BgEffectFlare, "background flare effect");

  al_set_new_bitmap_flags(ALLEGRO_MIPMAP);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
  BgOverlay = al_load_bitmap("./resources/ui_assets/bg_overlay.png");
  must_init(BgOverlay, "background overlay");
}

// Inicializa o popup
void popup_init()
{
  al_set_new_bitmap_flags(ALLEGRO_MIPMAP);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
  PopUpBase = al_load_bitmap("./resources/ui_assets/popup_base.png");

  al_set_new_bitmap_flags(ALLEGRO_MIPMAP);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
  PopUpHeader = al_load_bitmap("./resources/ui_assets/popup_header.png");
}

// Inicializa o timer
void timer_init()
{
  Timer = al_create_timer(1.0/FRAME_RATE);
  must_init(Timer, "timer");
  al_start_timer(Timer);
}

// Inicializa a fila de eventos
void queue_init()
{
  Queue = al_create_event_queue();
  must_init(Queue, "queue");
}

// Registra as fontes de eventos
void add_event_source()
{
  al_register_event_source(Queue, al_get_keyboard_event_source());
  al_register_event_source(Queue, al_get_mouse_event_source());
  al_register_event_source(Queue, al_get_display_event_source(Disp));
  al_register_event_source(Queue, al_get_timer_event_source(Timer));
}

// Inicializa os sprites do jogo
void sprites_init()
{
  char nome[200], character[12];
  for ( int i = 0; i < SPRITES_CNT; i++ )
  {
    sprintf(nome, "./resources/characters/characters_000%d.png", i+1);
    sprintf(character, "character %d", i+1);
    
    al_set_new_bitmap_flags(ALLEGRO_MIPMAP);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
    Sprites[i] = al_load_bitmap(nome);

    must_init(Sprites[i], character);
  }
}

// Inicializa a matriz de pecas
void matriz_init()
{
  if ( ! (Board->pieces = malloc(sizeof(piece **) * L_INI)) ) { exit(1); }
  for ( int i = 0; i < L_INI; i++ )
  {
    if ( ! (Board->pieces[i] = malloc(sizeof(piece *) * C_INI)) ) { exit(1); }
    for ( int j = 0; j < C_INI; j++ )
      if ( ! (Board->pieces[i][j] = malloc(sizeof(piece))) ) { exit(1); }
  }
}

// Aloca o vetor de novas pecas
void vector_init()
{
  if ( ! (Board->newPieces = malloc(sizeof(piece *) * C_INI)) ) { exit(1); }
  for ( int j = 0; j < C_INI; j++ )
    if ( ! (Board->newPieces[j] = malloc(sizeof(piece))) ) { exit(1); }
}

// Inicializa o tabuleiro
void board_init()
{
  Board = malloc(sizeof(board));

  // Aloca a matriz de pecas
  matriz_init();

  // Aloca o vetor de pecas novas
  vector_init();

  // Matriz ocupa no maximo 90% da tela
  Board->maxW = BUF_W * 0.90;

  // Tamanho maximo de uma celula da matriz
  Board->tamMax = Board->maxW / C_INI;
  Board->tamMaxScaled = Board->tamMax * DISP_SCALE;

  // Offset para centralizar a matriz
  Board->offset = (BUF_W - Board->maxW)/2;
  Board->offsetScaled = Board->offset * DISP_SCALE;

  // Tamanho desejado do sprite
  Board->spriteSize = Board->tamMax * 0.97;
}

// Inicializa o arquivo com a pontuacao mais alta
void record_init()
{
  char *arqName = "./record.txt";
  struct stat buffer;

  // Arquivo nao exite
  if ( stat(arqName, &buffer) )
  {
    if ( ! (Board->arqRecord = fopen(arqName, "w+")) ) { exit(1); }
    Board->record = 0;
  }

  // Arquivo existe
  else  
  {
    if ( ! (Board->arqRecord = fopen(arqName, "r+")) ) { exit(1); }
    fscanf(Board->arqRecord, "%ld", &Board->record);
  } 

  rewind(Board->arqRecord);
}

