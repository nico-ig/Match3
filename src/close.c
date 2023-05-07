// Feito por Nico Ramos - GRR20210574

#include <stdio.h>

#include "../header/close.h"
#include "../header/global.h"

// Destroui as fontes
void font_deinit()
{
  al_destroy_font(Font45);
  al_destroy_font(Font35);
  al_destroy_font(FontNewLevel);
  al_destroy_font(FontNewMission);
}

// Destroi os bitmaps utilizados pelos sprites
void sprites_deinit()
{
  for ( int i = 0; i < SPRITES_CNT; i++ )
    al_destroy_bitmap(Sprites[i]); 
}

// Libera a memoria utilizada pelo board
void board_deinit()
{
  fclose(Board->arqRecord);

  for ( int i = 0; i < L_INI; i++ )
  {
    for ( int j = 0; j < C_INI; j++ )
      free(Board->pieces[i][j]);

    free(Board->pieces[i]);
  }
  free(Board->pieces);

  for ( int i = 0; i < C_INI; i++ )
    free(Board->newPieces[i]);
  free(Board->newPieces);

  free(Board);
}

// Destroi os bitmaps utilizados pelos popups
void popup_deinit()
{
  al_destroy_bitmap(PopUpBase);
  al_destroy_bitmap(PopUpHeader);
}

// Destroi os bitmaps utilizados para o diplay 
void disp_deinit()
{
    al_destroy_bitmap(Buf);
    al_destroy_display(Disp);
}

// Destroi os bitmaps utilizados para o background
void bg_deinit()
{
  al_destroy_bitmap(Bg);
  al_destroy_bitmap(BgEffectFlare);
  al_destroy_bitmap(BgOverlay);
}

// Atualiza o arquivo de record
void update_new_record()
{
  fprintf(Board->arqRecord, "%ld\n", Board->record);
}
