#include <stdlib.h>
#include <time.h>

#include "../header/states.h"
#include "../header/global.h"

int main()
{
  srand(time(0));

  State = INICIO;
  while (State)
  {
    switch (State)
    {
      case INICIO:   state_init();  break;
      case SERVINDO: state_serve(); break;
      case JOGANDO:  state_play();  break;
      case FIMPART:  state_over();  break;
      case FIMJOGO:  state_close(); break;
      case HELP:     state_help();  break;
      default:                      break;
    }
  }

  return 0;
}


