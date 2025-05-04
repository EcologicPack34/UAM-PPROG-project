#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libscreen.h"

#pragma GCC diagnostic ignored "-Wpedantic"

/* Global variables */
int ROWS=23;
int COLUMNS=80;

#define TOTAL_DATA (ROWS * COLUMNS) + 1
#define BG_CHAR '~'
#define FG_CHAR ' '
#define ACCESS(d, x, y) (d + ((y) * COLUMNS) + (x))


typedef struct{
  char character;
  Frame_color background;
  Frame_color charColor;
}Cell;

struct _Area{
  int x, y, width, height;
  Cell *cursor;
  int cX, cY;
};


Cell *__data;



/****************************/
/*     Private functions    */
/****************************/
int  screen_area_cursor_is_out_of_bounds(Area* area);
void screen_area_scroll_up(Area* area);
void screen_utils_replaces_special_chars(char* str);
char *frame_color_to_string(Frame_color color);
const char* color_to_ansi(Frame_color frame, Frame_color chararacter);
Frame_color color_tag_to_color(const char *tag);


/****************************/
/* Functions implementation */
/****************************/
void screen_init(int rows, int columns){
  int i;

  screen_destroy(); /* Dispose if previously initialized */
  ROWS = rows;
  COLUMNS = columns;
  __data = (Cell *) malloc(sizeof(Cell) * (rows * columns + 1));

  if (__data){
    for (i = 0; i < TOTAL_DATA; i++)
    {
      __data[i].character = BG_CHAR;
      __data[i].charColor = WHITE;
      __data[i].background = BLUE;
    }
  }
}

void screen_destroy(){
  if (__data)
    free(__data);
}

void screen_paint(Frame_color color){
  char *src = NULL;
  char dest[COLUMNS + 1];
  int i=0;

  int x,y;
  Cell *cell;

  memset(dest, 0, COLUMNS + 1);
  printf("\033[2J");
  if (__data){
    /* puts(__data); */ /*Dump data directly to the terminal*/
    /*It works fine if the terminal window has the right size*/
    for (y = 0; y < ROWS; y++)
    {
      for ( x = 0; x < COLUMNS; x++)
      {
        cell = ACCESS(__data,x, y);
        if(cell->character == BG_CHAR){
          cell->charColor = color;
          cell->background = color;
        }else{
          cell->background = WHITE;
        }

        printf("%s%c\033[0m", color_to_ansi(cell->charColor, cell->background), cell->character);
      }
      printf("\n");
    }
    


//    puts("\033[2J"); /*Clear the terminal*/
//    for (src = __data; src < (__data + TOTAL_DATA - 1); src += COLUMNS)
//    {
//      memcpy(dest, src, COLUMNS);
//      /* printf("%s\n", dest); */
//      for (i = 0; i < COLUMNS; i++)
//      {
//        if (dest[i] == BG_CHAR)
//        {
//          printf("%s%c\033[0m", frame_color_to_string(color), dest[i]); /* fg:blue(34);bg:blue(44) */
//        }
//        else
//        {
//          printf("\033[0;30;47m%c\033[0m", dest[i]); /* fg:black(30);bg:white(47)*/
//        }
//      }
//      printf("\n");
//    }
  }
}

const char *color_to_ansi(Frame_color charC, Frame_color backC){
  static char str[32] = "";
  //str must be static so it isnt deleted after exiting function call. This way we save creating a string in the calling function

  snprintf(str, 32, "\033[0;%d;%dm", 30 + charC, 40 + backC);
  return str;
}

Frame_color color_tag_to_color(const char *tag){
  if(strcmp(tag, "BLACK") == 0) return BLACK;
  if(strcmp(tag, "RED") == 0) return RED;
  if(strcmp(tag, "GREEN") == 0) return GREEN;
  if(strcmp(tag, "YELLOW") == 0) return YELLOW;
  if(strcmp(tag, "BLUE") == 0) return BLUE;
  if(strcmp(tag, "PURPLE") == 0) return PURPLE;
  if(strcmp(tag, "CYAN") == 0) return CYAN;
  if(strcmp(tag, "WHITE") == 0) return WHITE;
  return NO_TAG;
}

Area* screen_area_init(int x, int y, int width, int height){
  int i = 0, j = 0;
  Area* area = NULL;
  Cell *cell = NULL;

  area = (Area *)malloc(sizeof(struct _Area));
  if(!area) return NULL;

  area->x = x;
  area->cX = 0;
  area->y = y;
  area->cY = 0;
  area->width = width;
  area->height = height;
  area->cursor =  ACCESS(__data, x, y);

  for (i = 0; i < height; i++)
  {
    for ( j = 0; j < width; j++)
    {
      cell = ACCESS(__data, x + j , y + i);
      cell->character = FG_CHAR;
      cell->charColor = BLACK;
      cell->background = WHITE;
    }
  }
  return area;
}

void screen_area_destroy(Area* area){
  if(area){
    free(area);
  }
}

void screen_area_clear(Area* area){
  int i = 0, j = 0;
  Cell *cell = NULL;
  if (area){
    area->cursor = ACCESS(__data,area->x, area->y);
    area->cX = 0;
    area->cY = 0;

    for (i = 0; i < area->height; i++)
    {
      for ( j = 0; j < area->width; j++)
      {
        /*Reset characters to default color and character*/
        cell = ACCESS(__data, area->x + j , area->y + i);
        cell->character = FG_CHAR;
        cell->charColor = BLACK;
        cell->background = WHITE;
      }
    }
  }
}

void screen_area_reset_cursor(Area* area){
  if (area){
    area->cursor = ACCESS(__data, area->x, area->y);
  }
}

void screen_area_puts(Area* area, char *str){
  int len = 0;
  char *ptr = NULL;

  char *tagE = str;
  int tagLen;
  char tag[20] = "";

  Frame_color cColor = BLACK;
  Frame_color bColor = WHITE;

  Cell *cell = NULL;
  short skipT = 0;


  if(!area || !str) return;

  if(area->cY >= area->height){
    area->cY = 0;
  }
//  if (screen_area_cursor_is_out_of_bounds(area)){
//    screen_area_scroll_up(area);
//  }

  screen_utils_replaces_special_chars(str);

  ptr = str;
  len = strlen(str);
  
  while(*ptr != '\0' && ptr <= str + len && area->cY < area->height){
    for (area->cX = 0; area->cX < area->width && ptr <= str + len; (area->cX)++ , ptr++)
    {
      if(*ptr == '[' && !skipT){
        strcpy(tag, "");

        tagE = ptr + 1;
        for (tagLen = 0; tagLen + 1 < 20 && *tagE != ']' && tagE <= str + len; tagLen++, tagE++){
          tag[tagLen] = *tagE;
        }
        if(*tagE == ']'){
          cColor = color_tag_to_color(tag);
          if(cColor == NO_TAG){
            cColor = BLACK;
            ptr--;
            skipT = 1;
          }else{
            ptr = tagE;
            area->cX--;
          }
        }
        continue;
      }
      if(ptr > str + len) break;

      if(skipT) skipT = 0;

      cell = ACCESS(__data, area->x + area->cX , area->y + area->cY);
      cell->character = *ptr;
      cell->charColor = cColor;
      cell->background = bColor;
    }
    area->cY++;
  }

//  for (ptr = str; ptr < (str + strlen(str)); ptr+=area->width){
//    memset(area->cursor, FG_CHAR, area->width);
//    len = (strlen(ptr) < area->width)? strlen(ptr) : area->width;
//    memcpy(area->cursor, ptr, len);
//    area->cursor += COLUMNS;
//  }
}

int screen_area_cursor_is_out_of_bounds(Area* area){
  return area->cursor > ACCESS(__data,
			       area->x + area->width,
			       area->y + area->height - 1);
}

void screen_area_scroll_up(Area* area){
  for(area->cursor = ACCESS(__data, area->x, area->y);
      area->cursor < ACCESS(__data, area->x + area->width, area->y + area->height - 2);
      area->cursor += COLUMNS){
    memcpy(area->cursor, area->cursor+COLUMNS, area->width);
  }
}

void screen_utils_replaces_special_chars(char* str){
  char *pch = NULL;

  /* Replaces acutes and tilde with '??' */
  while ((pch = strpbrk (str, "ÁÉÍÓÚÑáéíóúñ")))
    memcpy(pch, "??", 2);
}

