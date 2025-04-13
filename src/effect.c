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
#include <string.h>

#include "effect.h"
#include "collection.h"
#include "entity.h"

#define EFFECT_MANAGER_INIT_SIZE 10 /*!< Effects manager collection initial size*/
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

struct _EffectsManager{
    Collection *effects; /*!< an effects collection*/
};

/*--------------------------------------------------------------------------------------------------------------------------*/
/*PRIVATE FUNCTIONS HEADERS*/

Status _effect_apply_poison(Effect *effect, Entity *entity);

Status _effect_apply_regeneration(Effect *effect, Entity *entity);


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

    return e;
}

EffectManager *effect_manager_create(){
    EffectManager *em=NULL;

    if((em = (EffectManager*)malloc(sizeof(EffectManager))) == ERROR) return NULL;

    em->effects = collection_create(EFFECT_MANAGER_INIT_SIZE,false,true,effect_cmp,effect_print);
    if(!em->effects) return NULL;

    return em;
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

void effect_manager_destroy(EffectManager *em){
    if(em){
        if(em->effects)
            collection_destroy(em->effects);
        free(em);
    }
    return;
}

Status effect_manager_add_effect(EffectManager *em, Effect *effect){
    if(!em || !effect) return ERROR;
    return collection_add(em->effects, (void *)effect);
}

Status effect_update(Effect *effect){
    int i,n;
    Affected *aux=NULL;

    if(!effect || ((n = collection_length(effect->affecteds)) == -1)) return ERROR;

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
        }
    }

    for(i=n-1; i>=0 ; i--){
        aux = collection_get_element_at(effect->affecteds, i);
        if(aux->turns == 0)
            collection_remove(effect->affecteds, aux);
    }
    return OK;
}

Status effect_write_affected_save_data(Effect *effect, char *filename){
    int i,n;
    Affected *aux=NULL;
    FILE *file=NULL;
    char str[WORD_SIZE]="";
    char aux_str[WORD_SIZE]="";
    
    if(!effect || !filename) return ERROR;
    if((n = collection_length(effect->affecteds)) == -1) return ERROR;

    /*
    format is as follows:
    #efsave:n
    Entity1Id|Entity1Type|turns
    Entity2Id|Entity2Type|turns
    */

    if((file = fopen(filename,"a")) == NULL) return ERROR;
    fprintf(file,"\n\n");

    fprintf(file,"#efsave:%d\n", n);
    for(i=0; i<n; i++){
        aux = collection_get_element_at(effect->affecteds,i);
        
        /*id print*/
        sprintf(aux_str,"%d",(int)entity_get_id(aux->ent));
        strcat(str, aux_str);
        /*entity type print*/
        sprintf(aux_str,"%d",(int)entity_get_entityType(aux->ent));
        strcat(str, aux_str);
        /*turns print*/
        sprintf(aux_str,"%d",aux->turns);
        strcat(str, aux_str);

        /*add to file*/
        fprintf(file,"%s\n",str);
    }
    return OK;
}

Status effect_get_as_str(Effect *effect, long destiny_size ,char *destiny){
    /*#ef:Id|name|EffectIn|EffectType|data*/
    char str[WORD_SIZE]="";
    char aux[WORD_SIZE]="";
    if(!effect || !destiny) return ERROR;

    /*add "#ef:"*/
    sprintf(str,"#ef:");
    /*add id*/
    sprintf(aux,"%d|",(int)effect->id);
    strcat(str,aux);
    /*add name*/
    strcat(str,effect->name);
    strcat(str,"|");

    /*add EffectIn*/
    sprintf(aux,"%d", (int)effect->Eloc);
    strcat(str,aux);
    strcat(str,"|");

    /*add EffectType*/
    sprintf(aux,"%d", (int)effect->ET);
    strcat(str,aux);
    strcat(str,"|");

    strcat(str,effect->data);
    if(destiny_size+1 < strlen(str)) return ERROR;

    strcpy(destiny, str);
    return OK;
}

EffectType effect_get_effect_type(Effect *effect){
    if(!effect) return -2;
    return effect->ET;
}

EffectIn effect_get_effect_in(Effect *effect){
    if(!effect) return -2;
    return effect->Eloc;
}

char *effect_get_name(Effect *effect){
    if(!effect) return NULL;
    return effect->name;
}

char *effect_get_data(Effect *effect){
    if(!effect) return NULL;
    return effect->data;
}

Id effect_get_id(Effect *effect){
    if(!effect) return NO_ID;
    return effect->id;
}

int effect_cmp(void*e1, void*e2){
    if(!e1 || !e2) return -1;
    return ((Effect*)e1)->id - ((Effect*)e2)->id;
}

void effect_print(void*effect){
    if(!effect) return;
    /*for now this will be the print format. Can be modified in the future.*/
    fprintf(stdout, "%s: %s", ((Effect*)effect)->name, ((Effect*)effect)->data);
}

/*END OF PUBLIC FUNCTIONS IMPLEMENTATION*/
/*--------------------------------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------------------------------*/
/*PRIVATE FUNCTIONS IMPLEMENTATION*/

Status _effect_apply_poison(Effect *effect, Entity *entity){
    if(!effect) return ERROR;
    return ERROR;
}

Status _effect_apply_regeneration(Effect *effect, Entity *entity){
    if(!effect) return ERROR;
    return ERROR;
}

/*END OF PRIVATE FUNCTIONS IMLPEMENTATION*/
/*--------------------------------------------------------------------------------------------------------------------------*/