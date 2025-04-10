/**
 * @brief It implements the effect_manager module
 * 
 * 
 *
 * @file effect_manager.c
 * @author Aaron Charameli Mair
 * @version 0
 * @date 10-04-2025
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>

#include "effect.h"
#include "collection.h"
#include "entity.h"

#define INIT_AFFECTED 10 /*!< Initial size of affecteds collection in an effect (irrelevant for the game functionality)*/

typedef struct{
    Entity *ent;
    int turns; /*a counter of turns that the effect is applied to the entity*/

}Affected; /*<! Struct that stores an entity and the number of turns that it suffers an effect*/

struct _Effect{
  Id id;
  Collection *affecteds; /*!< stores the affecteds that have a certain effect*/
  char *name;
  char *data; /*a string containing data related to the effect. A function will use the data to apply the effect*/
  EffectIn Eloc;
  EffectType ET;
};

/*--------------------------------------------------------------------------------------------------------------------------*/
/*PRIVATE FUNCTIONS HEADERS*/

Status _effect_apply_poison(Effect *effect, Entity *entity);


/*END OF PRIVATE FUNCTIONS DECLARATIONS*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------*/
/*PUBLIC FUNCTIONS IMPLEMENTATION*/

Effect *effect_create(Id id, char *name, char *data, EffectIn Eloc, EffectType ET){
    Effect *e=NULL;
    
    if((id<=UNDEFINED_ID) || (ET == UNKNOWN_EFFECT) || ((Eloc != ENTITY_EFFECT) && (Eloc != SPACE_EFFECT))) return NULL;
    if((name == NULL) || (data == NULL)) return NULL;

    if((e = (Effect *)malloc(sizeof(Effect))) == NULL) return NULL;


    e->id = id;
    if((e->affecteds = collection_create(INIT_AFFECTED,false,true, effect_cmp, effect_print)) == NULL) return NULL;
    e->name = strdup(name);
    e->data = strdup(data);
    e->Eloc = Eloc;
    e->ET = ET;
}

void effect_destroy(Effect *e){
    if(e){
        if(e->name)
            free(e->name);
        if(e->data)
            free(e->data);
        if(e->affecteds)
            collection_destroy(e->affecteds);
        free(e);
    }
    return;
}

Status effect_update(Effect *effect){
    int i,n;
    Affected *aux=NULL;
    void **del=NULL;

    if(!effect) return ERROR;

    n = collection_length(effect->affecteds);

    if((del = (void **)calloc(n,sizeof(void *))) == NULL) return ERROR;

    for(i=0; i<n; i++){
        aux = collection_get_element_at(effect->affecteds, i);
        if(aux){
            switch (effect->ET){
                case POISON:
                    _effect_apply_poison(effect, aux->ent);
                    break;
                case REGENERATION:
                    _effect_apply_regeneration(effect, aux->ent);
                    break;
                default:
                    break;
            }
            if(aux->turns == 0)
                del[i]=aux;
            
        }
    }

    for(i=0; i<n; i++){
        if(del[i])
            collection_remove(effect->affecteds,del[i]);
    }

    return OK;
}

int effect_cmp(const Effect*e1, const Effect*e2){
    if(!e1 || !e2) return -1;
    return e1->id - e2->id;
}

void effect_print(FILE *pf ,const Effect*effect){
    if(!effect) return;
    /*for now this will be the print format. Can be modified in the future.*/
    fprintf(pf, "%s: %s", effect->name, effect->data);
}

/*END OF PUBLIC FUNCTIONS IMPLEMENTATION*/
/*--------------------------------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------------------------------*/
/*PRIVATE FUNCTIONS IMPLEMENTATION*/

Status _effect_apply_poison(Effect *effect, Entity *entity){
    if(!effect) return ERROR;
}

Status _effect_apply_regeneration(Effect *effect, Entity *entity){
    if(!effect) return ERROR;
}

/*END OF PRIVATE FUNCTIONS IMLPEMENTATION*/
/*--------------------------------------------------------------------------------------------------------------------------*/