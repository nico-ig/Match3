// Feito por Nico Ramos - GRR20210574
#ifndef _DRAW_
#define _DRAW_

// Desenho o background
void draw_bg();

// Desenha uma peca da matriz
void draw_piece(int newSize, int tamMax, int offset, int i, int j);
  
// Desenha a matriz de pecas
void draw_matriz(int clickCnt);

// Desenha primeiro no buffer 
void disp_pre_draw();

// Desenha os elementos da tela
bool disp_redraw(int clickCnt);

// Desenha no display o que estava no buffer
void disp_post_draw();

#endif
