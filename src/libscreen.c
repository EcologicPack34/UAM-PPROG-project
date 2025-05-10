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
  Frame_color backgroudColor;
  Frame_color charColor;
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
  Cell *cell;

  int y,x;
  int pos = 0;

  Frame_color cChar = NO_TAG;
  Frame_color cBack = NO_TAG;

  char line[COLUMNS + 1000];
  
  if(!__data) return;

  printf("\r\033[2J");

  for (y = 0; y < ROWS; y++)
  {
    pos = 0;
    cChar = NO_TAG;
    cBack = NO_TAG;
    for (x = 0; x < COLUMNS; x++)
    {
      cell = ACCESS(__data, x, y);
      if(cell->character == BG_CHAR){
        cell->charColor = color;
        cell->background = color;
      }
      if(cell->background != cBack || cChar != cell->charColor){
        cChar = cell->charColor;
        cBack = cell->background;
        pos += sprintf(line + pos,"%s",color_to_ansi(cChar, cBack));
      }
      line[pos++] = cell->character;
    }
    line[pos] = 0;
    printf("%s\033[0m\n", line);
  }
  

  /*if (__data){
    for (i = 0; i < TOTAL_DATA - 1; i++)
    {
      cell = __data + i;
      if(cell->character == BG_CHAR){
        cell->charColor = color;
        cell->background = color;
      }
      printf("%s%c\033[0m", color_to_ansi(cell->charColor, cell->background), cell->character);
      if( (i + 1) % (COLUMNS) == 0){
        printf("\n");
      }
    }
  }*/
}

void area_set_color(Area *area, Frame_color backGround, Frame_color charColor){
  int i,j;
  Cell *cell;
  if(backGround == NO_TAG || backGround == RESET) backGround = WHITE;
  if(charColor == NO_TAG || charColor == RESET) charColor = WHITE;
  if(area){
    area->backgroudColor = backGround;
    area->charColor = charColor;
    for (i = 0; i < area->height; i++)
    {
      for (j = 0; j < area->width; j++)
      {
        cell = ACCESS(__data, area->x + j, area->y + i);
        cell->charColor = charColor;
        cell->background = backGround;
      }
    }
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
  if(strcmp(tag, "RESET") == 0) return RESET;
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
  area->backgroudColor = WHITE;
  area->charColor = BLACK;
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
        cell->charColor = area->charColor;
        cell->background = area->backgroudColor;
      }
    }
  }
}

void screen_area_reset_cursor(Area* area){
  if (area){
    area->cursor = ACCESS(__data, area->x, area->y);
  }
}

int word_length(const char *word, int maxLen){
  int visible = 0;
  char tag[50];
  int len;

  Frame_color color;

  len = strlen(word);

  for (int i = 0, j = 0; i < maxLen && word[i] != 0 && word[i] != ' ' && word[i] != '\n'; i++)
  {
    if(word[i] == '['){
      for (j = i + 1; j < len; j++)
      {
        if(word[j] == '[' || word[j] == ']'){
          break;
        }
        tag[j - i - 1] = word[j];
      }
      tag[j - i - 1] = 0;
      if(word[j] == '['){
        visible++;
        continue;
      }
      if(word[j] == ']'){
        color = color_tag_to_color(tag);
        if(color == NO_TAG){
          visible++;
          continue;
        }
        i = j;
      }
    }
    visible++;
  }
  
  return visible;
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
  Cell *cAux = NULL;
  short skipT = 0;
  short skipS = 0;

  if(!area || !str) return;

  bColor = area->backgroudColor;
  cColor = area->charColor;

  if(area->cY >= area->height){

    for (area->cY = 0 ; area->cY < area->height - 1; area->cY++)
    {
      for (area->cX = 0; area->cX < area->width; (area->cX)++)
      {
        cell = ACCESS(__data, area->x + area->cX , area->y + area->cY);
        cAux = ACCESS(__data, area->x + area->cX , area->y + area->cY + 1);
        cell->character = cAux->character;
        cell->background = bColor;
        cell->charColor = cAux->charColor;
      }
    }
    area->cY = area->height - 1;
  }
  screen_utils_replaces_special_chars(str);

  ptr = str;
  len = strlen(str);
  
  while(*ptr != '\0' && ptr <= str + len && area->cY < area->height){
    /*Clear line*/
    for (area->cX = 0; area->cX < area->width; (area->cX)++)
    {
      cell = ACCESS(__data, area->x + area->cX , area->y + area->cY);
      cell->character = FG_CHAR;
      cell->background = bColor;
      cell->charColor = cColor;
    }
    /*Print the actual line to data*/
    for (area->cX = 0; area->cX < area->width && ptr <= str + len && *ptr != '\0'; (area->cX)++ , ptr++)
    { 
      if(word_length(ptr, area->width) > area->width - area->cX){
        skipS = 1;
        break;
      }
      if(*ptr == ' ' && skipS) {
        skipS = 0;
      }

      if(*ptr == '\n'){
        ptr++;
        break;
      }
      /*Looks for tag*/
      if(*ptr == '[' && !skipT){
        strcpy(tag, "");

        /*copy content between [ ] */
        tagE = ptr + 1;
        for (tagLen = 0; tagLen + 1 < 20 && *tagE != ']' && *tagE != '[' && tagE <= str + len; tagLen++, tagE++){
          tag[tagLen] = *tagE;
        }
        /*Checks if closing bracket exits*/
        if(*tagE == '['){
          ptr--;
          skipT = 1;
          area->cX--;
        }
        if(*tagE == ']'){
          tag[tagLen] = '\0';
          cColor = color_tag_to_color(tag);
          /*Checks if tag is valid, if not go back to [ */
          if(cColor == NO_TAG){
            cColor = BLACK;
            ptr--;
            skipT = 1;
          }
          /*If tag is RESET color sets to default*/
          else{
            if(cColor == RESET){
              cColor = area->charColor;
            }
            ptr = tagE;
          }
          area->cX--;
        }
        continue;
      }
      if(ptr > str + len) break;

      if(skipT) skipT = 0;
      /*writes actual data*/
      cell = ACCESS(__data, area->x + area->cX , area->y + area->cY);
      cell->character = *ptr;
      cell->charColor = cColor;
      cell->background = bColor;
    }
    /*move one line*/
    area->cY++;
  }
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
  while ((pch = strpbrk (str, "ÁÉÍÓÚÑáéíóúñ\r")))
    memcpy(pch, "??", 2);
}

