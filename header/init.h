// Feito por Nico Ramos - GRR20210574
#ifndef _INIT_
#define _INIT_

#include <allegro5/allegro5.h>

// Funcao para verificar se uma estrutura foi iniciada corretamente ou nao, em caso de erro encerra o programa com codigo de
// erro 1
void must_init(bool test, const char *description);

// Inicializa os addons
void addons_init();

// Inicializa o display
void disp_init();

// Inicializa as fontes
void fonts_init();

// Inicializa o teclado
void keyboard_init();

// Inicializa o mouse
void mouse_init();

// Inicializa o background
void bg_init();

// Inicializa o popup
void popup_init();

// Inicializa o timer
void timer_init();

// Inicializa a fila de eventos
void queue_init();

// Registra as fontes de eventos
void add_event_source();

// Inicializa os sprites do jogo
void sprites_init();

// Inicializa o tabuleiro
void board_init();

// Inicializa o arquivo com as pontuacoes mais altas
void record_init();

#endif
