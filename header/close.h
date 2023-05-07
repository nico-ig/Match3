// Feito por Nico Ramos - GRR20210574
#ifndef _CLOSE_
#define _CLOSE_

// Destroi as fontes
void font_deinit();

// Libera a memoria utilizada pelo board
void board_deinit();

// Destroi os bitmaps utilizados pelos sprites
void sprites_deinit();

// Destroi os bitmaps utilizados pelos popups
void popup_deinit();

// Destroi os bitmaps utilizados para o diplay 
void disp_deinit();

// Destroi os bitmaps utilizados para o background
void bg_deinit();

// Atualiza o arquivo de record
void update_new_record();

#endif
