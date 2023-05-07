// Feito por Nico Ramos - GRR20210574

#include <stdio.h>

#include "../header/global.h"
#include "../header/draw.h"

#define BLACK        al_map_rgb(0, 0, 0)
#define LIGHT_BLACK  al_map_rgb(30, 30, 30)
#define DARK_GREY    al_map_rgb(1, 105, 102)
#define LIGHTER_GREY al_map_rgb(100, 105, 102)
#define LIGHT_GREY   al_map_rgb(1, 50, 50)
#define GREY_GREEN   al_map_rgb(54, 165, 161)

// Desenho o background
void draw_bg()
{
  al_draw_bitmap(Bg, 0, 0, 0);
  al_draw_bitmap(BgEffectFlare, 0, 0, 0);
}

void draw_overlay()
{

  int oriW = al_get_bitmap_width(BgOverlay);
  int oriH = al_get_bitmap_height(BgOverlay);
  int newSizeW = al_get_bitmap_width(Bg);
  int newSizeH = al_get_bitmap_height(Bg);

  al_draw_scaled_bitmap(BgOverlay,          // Bitmap orig
                        0, 0,               // Src bitmap
                        oriW, oriH,         // Orig size
                        0, 0,               // Desti X e Y
                        newSizeW, newSizeH, // Tamanho
                        0 );                // Flag
}

void draw_click(int clickCnt)
{
  if ( (clickCnt == 0) || (Move[0] == -1) || (Move[1] == -1) )
      return;

  int tamMax = Board->tamMax;
  int offset = Board->offset;

  draw_piece(tamMax * 1.05, tamMax, offset, Move[0], Move[1]);
}

// Desenha uma peca nova
void draw_new_piece(int newSize, int tamMax, int offset, int j)
{
  piece *p = Board->newPieces[j];
  if ( abs(p->dy) <= tamMax/2 ) { return; }

  int i = p->i;
  int type = p->type;
  int oriW = al_get_bitmap_width(Sprites[type]);
  int oriH = al_get_bitmap_height(Sprites[type]);
  int desY = (i-1) * tamMax + offset + p->dy;
  int desX = j * tamMax + offset + p->dx;
  int newSizeX = newSize;
  int newSizeY = newSize;

  al_draw_scaled_bitmap(Sprites[type],      // Bitmap orig
                        0, 0,               // Src bitmap
                        oriW, oriH,         // Orig size
                        desX, desY,         // Desti X e Y
                        newSizeX, newSizeY, // Tamanho
                        0 );                // Flag

}

// Desenha uma peca da matriz
void draw_piece(int newSize, int tamMax, int offset, int i, int j)
{
  piece *p = Board->pieces[i][j];
  int type = p->type;

  if ( (type == -1) ) { return; }

  int oriW = al_get_bitmap_width(Sprites[type]);
  int oriH = al_get_bitmap_height(Sprites[type]);
  int desY = i * tamMax + offset + p->dy;
  int desX = j * tamMax + offset + p->dx;
  int newSizeX = newSize;
  int newSizeY = newSize;

  al_draw_scaled_bitmap(Sprites[type],      // Bitmap orig
                        0, 0,               // Src bitmap
                        oriW, oriH,         // Orig size
                        desX, desY,         // Desti X e Y
                        newSizeX, newSizeY, // Tamanho
                        0 );                // Flag
}

// Verifica se uma peca esta selecionada
bool is_click(int clickCnt, int i, int j)
{
  if ( (clickCnt != 0) && (i == Move[0]) && (j == Move[1]) ) { return true; }

  return false;
}

// Desenha as novas pecas
void draw_vector_new_pieces()
{
  int tamMax = Board->tamMax;
  int offset = Board->offset;
  int newSize = Board->spriteSize;
  for ( int j = 0; j < C_INI; j++ )
    draw_new_piece(newSize, tamMax, offset, j);

}

// Desenha a matriz de pecas
void draw_matriz(int clickCnt)
{
  int tamMax = Board->tamMax;
  int offset = Board->offset;
  int newSize = Board->spriteSize;

  for ( int i = 0; i < L_INI; i++ )
    for ( int j = 0; j < C_INI; j++ )
    {
      if ( is_click(clickCnt, i, j) ) { continue; }
      draw_piece(newSize, tamMax, offset, i, j);
    }
}

// Desenha o popup com a missao
void draw_popup(int newSizeX, int newSizeY, int desX, int desY)
{
  int baseW = al_get_bitmap_width(PopUpBase);
  int baseH = al_get_bitmap_height(PopUpBase);

  int headerW = al_get_bitmap_width(PopUpHeader);
  int headerH = al_get_bitmap_height(PopUpHeader);
  int headerY = (desY - baseH/2) * 1.05;
  int headerX = desX + headerW * 0.1;
  int headerS = newSizeY * 0.80;

  // Desenha a base
  al_draw_scaled_bitmap(PopUpBase,          // Bitmap orig
                        0, 0,               // Src bitmap
                        baseW, baseH,       // Orig size
                        desX, desY,         // Desti X e Y
                        newSizeX, newSizeY, // Tamanho
                        0 );                // Flag

  // Desenha o header
  al_draw_scaled_bitmap(PopUpHeader,        // Bitmap orig
                        0, 0,               // Src bitmap
                        headerW, headerH,   // Orig size
                        headerX, headerY,   // Desti X e Y
                        headerS, headerS,   // Tamanho
                        0 );                // Flag
}

void draw_footer()
{
  int maxW     = Board->maxW;
  int offset   = Board->offset;
  int newSizeX = (maxW - offset)/2;
  int newSizeY = newSizeX * 0.95;
  int desY     = (BUF_H - offset - newSizeY) * 0.97;
  
  // Mission
  draw_popup(newSizeX, newSizeY, offset, desY);

  // Score
  int scoreDesX = BUF_W - newSizeX - offset;
  draw_popup(newSizeX, newSizeY, scoreDesX, desY);
}

void draw_score_title()
{
  int desY     = 5*BUF_H/7;
  int desX     = BUF_W - BUF_W/4 - BUF_W/55;

  al_draw_text(Font45, DARK_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, "Score");
}

void draw_score_text()
{
  int desY     = 21*BUF_H/28 + 5;
  int desX     = BUF_W - BUF_W/4 - BUF_W/55;

  char str[200];
  sprintf(str, "%ld/%ld", Board->score, Board->score_next_level);
  al_draw_text(Font35, LIGHT_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, str);
}

void draw_record_title()
{
  int desY     = 5*BUF_H/6;
  int desX     = BUF_W - BUF_W/4 - BUF_W/55;

  al_draw_text(Font45, DARK_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, "Record");
}

void draw_record_text()
{
  int desY     = 7*BUF_H/8;
  int desX     = BUF_W - BUF_W/4 - BUF_W/55;

  char str[200];
  sprintf(str, "%ld", Board->record);
  al_draw_text(Font35, LIGHT_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, str);

}

void draw_mult_text()
{
  int desY     = 22*BUF_H/28 + 5;
  int desX     = BUF_W - BUF_W/4 - BUF_W/55;

  char str[200];
  sprintf(str, "x%d", Board->mult);
  al_draw_text(Font35, GREY_GREEN, desX, desY, ALLEGRO_ALIGN_CENTER, str);
}

void draw_score()
{
  draw_score_title();
  draw_score_text();
  draw_record_title();
  draw_record_text();
  draw_mult_text();
}

void draw_mission_title()
{
  int desY     = 5*BUF_H/7;
  int desX     = BUF_W/4 + BUF_W/55;

  al_draw_text(Font45, DARK_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, "Mission");
}

void draw_mission_sprite()
{
  int type = Board->mission;

  int oriW = al_get_bitmap_width(Sprites[type]);
  int oriH = al_get_bitmap_height(Sprites[type]);

  int tamMax = Board->tamMax;

  int desX = 1.3 * tamMax;
  int desY = 21*BUF_H/28 + 5;

  int newSize = Board->spriteSize;
  int newSizeX = newSize;
  int newSizeY = newSize;

  al_draw_scaled_bitmap(Sprites[type],      // Bitmap orig
                        0, 0,               // Src bitmap
                        oriW, oriH,         // Orig size
                        desX, desY,         // Desti X e Y
                        newSizeX, newSizeY, // Tamanho
                        0 );                // Flag


}

void draw_mission_progress()
{
  int desY     = 22*BUF_H/28 - 10;
  int desX     = BUF_W/4 + 2*BUF_W/19;

  char str[15];
  sprintf(str, "%d/%d", Board->mission_cnt, 7 + 2*Board->nivel);
  al_draw_text(Font35, LIGHT_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, str);
}

void draw_level_title()
{
  int desY     = 5*BUF_H/6;
  int desX     = BUF_W/4 + BUF_W/55;

  al_draw_text(Font45, DARK_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, "Level");
}

void draw_level_text()
{
  int desY     = 7*BUF_H/8;
  int desX     = BUF_W/4 + BUF_W/55;

  char str[100];
  sprintf(str, "%d", Board->nivel);
  al_draw_text(Font35, LIGHT_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, str);
}

void draw_new_mission()
{
  if ( Board->mission_cnt < (7 + 2*Board->nivel) ) { return; }

  int desY     = Board->maxW/2.6;
  int desX     = BUF_W/2;

  char str[100];
  sprintf(str, "New Mission!");
  al_draw_text(FontNewMission,DARK_GREY,desX,desY,ALLEGRO_ALIGN_CENTER,str);
}

void draw_mission()
{
  draw_mission_title();
  draw_mission_sprite();
  draw_mission_progress();
  draw_level_title();
  draw_level_text();
  draw_new_mission();
}

void draw_new_level()
{
  if ( ! Board->new_level ) { return; }

  int desY     = Board->maxW/2;
  int desX     = BUF_W/2;

  char str[100];
  sprintf(str, "Level %d", Board->nivel);
  al_draw_text(FontNewLevel, DARK_GREY, desX, desY, ALLEGRO_ALIGN_CENTER, str);
}

void draw_record()
{
  if ( !Board->new_record || Board->record_already_updated) { return; }

  int desY     = Board->maxW/4;
  int desX     = BUF_W/2;

  char str[100];
  sprintf(str, "New Record!");
  al_draw_text(FontNewMission,DARK_GREY,desX,desY,ALLEGRO_ALIGN_CENTER,str);
  
  if ( Board->new_record_frame_cnt == FRAME_RATE/2 )
  {
    Board->new_record_frame_cnt = 0;
    Board->record_already_updated = true;
  }

  else
    Board->new_record_frame_cnt++;
}

void draw_how_to_play()
{
  int desY     = Board->maxW/8;
  int desX     = BUF_W/2;

  char comoJogar[] = "Como jogar:"; 
  al_draw_text(Font45,BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,comoJogar);

  char clique[]    = "Clique em duas peças adjacentes" ; 
  desY += 50;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,clique);

  char troca[]    = "para troca-las de posição."; 
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,troca);

  char obj[]      = "O objetivo é criar sequencias de"; 
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,obj);

  char obj1[]     = "três ou mais. Quando não houver"; 
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,obj1);
                    
  char acaba[]    = "mais nenhum movimento possível";
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,acaba);

  char rosa[]     = "o jogo acaba. As peças rosas";
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,rosa);

  char nivel[]    = "(estrelas) não fazem combinação,";
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,nivel);

  char missao[]   = "mas são removidas a cada novo";
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,missao);

  char ponto[]    = "nível. As missões valem pontos.";
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,ponto);
}

void draw_keyboard()
{
  int desY     = 7*Board->maxW/8;
  int desX     = BUF_W/2;

  char teclas[] = "Teclas:"; 
  al_draw_text(Font45,BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,teclas);

  char r[]      = "\"R\": reinicia o jogo" ;
  desY += 50;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,r);

  char h[]     = "\"H\" ou \"F1\": menu de ajuda";
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,h);

  char esc[]  = "\"ESC\": encerra o jogo";
  desY += 35;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,esc);
}

void draw_autoria()
{
  int desY     = 5*Board->maxW/4;
  int desX     = BUF_W/2;

  char made[] = "Desenvolvido por:";
  al_draw_text(Font45,BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,made);

  char nome[]         = "Nico Ramos";
  desY += 50;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,nome);
}

// Desenha um menu de ajuda
void draw_help()
{
  draw_overlay();
  draw_how_to_play();
  draw_keyboard();
  draw_autoria();
}

void draw_game_over()
{
  draw_overlay();
  
  int desY     = 3*Board->maxW/8;
  int desX     = BUF_W/2;
  
  char go[]  = "GAME OVER";
  al_draw_text(FontNewMission,BLACK,desX,desY,ALLEGRO_ALIGN_CENTER,go);

  char scoreT[] = "Sua pontuacao: ";
  desY += 80;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_RIGHT,scoreT);

  char score[200];
  sprintf(score, "%ld", Board->score);
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_LEFT,score);

  char recordT[] = "Record: ";
  desY += 40;
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_RIGHT,recordT);

  char record[200];
  sprintf(record, "%ld", Board->record);
  al_draw_text(Font35,LIGHT_BLACK,desX,desY,ALLEGRO_ALIGN_LEFT,record);
}

// Desenha primeiro no buffer
void disp_pre_draw() { al_set_target_bitmap(Buf); }

// Desenha os elementos da tela
bool disp_redraw(int clickCnt)
{
  disp_pre_draw();
  draw_bg();
  draw_vector_new_pieces();
  draw_matriz(clickCnt);
  draw_footer();
  draw_score();
  draw_mission();
  draw_new_level();
  draw_record();
  
  if ( State == JOGANDO )
    draw_click(clickCnt);
  
  else if ( State == FIMPART )
    draw_game_over();
  
  else if ( State == HELP )
    draw_help();

  disp_post_draw();
      
  return false;
}

// Desenha no display o que estava no buffer
void disp_post_draw()
{
    al_set_target_backbuffer(Disp);
    al_draw_scaled_bitmap(Buf, 0, 0, BUF_W, BUF_H, 0, 0, DISP_W, DISP_H, 0);
    al_flip_display();
}





