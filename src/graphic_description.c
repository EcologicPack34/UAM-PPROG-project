#include "graphic_description.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _GDesc{
    Id id;
    GDescType type;
    int height;             /*!< Height of the description*/
    int width;              /*!< Width of the description*/
    char **description;    /*!< Array containing each line of the description*/
};

GDesc *gdesc_create(Id id, int height, int width, GDescType type){
    GDesc *gdesc = NULL;

    if(height < 0 || width < 0) return NULL;

    gdesc = calloc(1, sizeof(GDesc));
    if(!gdesc) return NULL;

    gdesc->description = (char **)calloc(height, sizeof(char *));
    if(!(gdesc->description)){
        free(gdesc);
        return NULL;
    }

    gdesc->id = id;
    gdesc->height = height;
    gdesc->width = width;
    gdesc->type = type;

    return gdesc;
}


void gdesc_destroy(void *gdesc){
    if(gdesc){
        for (size_t i = 0; i < ((GDesc*)gdesc)->height; i++)
        {
            if(((GDesc*)gdesc)->description[i]){
                free(((GDesc*)gdesc)->description[i]);
            }
        }
        free(((GDesc*)gdesc)->description);
        free(gdesc);
    }
}

Status gdesc_set_line(GDesc *gdesc, int line, char *content){
    if(!gdesc || !content) return ERROR;
    if(line < 0 || line >= gdesc->height) return ERROR;

    gdesc->description[line] = (char *)malloc((strlen(content) + 1) * sizeof(char));
    if(!(gdesc->description[line])) return ERROR;

    strncpy(gdesc->description[line], content, gdesc->width);
    return OK;
}

char *gdesc_get_line(GDesc *gdesc, int line){
    if(!gdesc) return NULL;
    if(line < 0 || line > gdesc->height) return NULL;
    return gdesc->description[line];
}

GDescType gdesc_get_type(GDesc *gdesc){
    if(!gdesc) return NO_DESC;
    return gdesc->type;
}

Id gdesc_get_id(GDesc *gdesc){
    if(!gdesc)return NO_ID;
    return gdesc->id;
}

int gdesc_cmp(void *e1, void *e2){
    int res;
    if(!e1 || !e2) return 0;

    res = ((GDesc*)e1)->id - ((GDesc*)e2)->id;

    return res; 
}