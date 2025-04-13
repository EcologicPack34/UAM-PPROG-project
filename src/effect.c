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

    em = collection_create(EFFECT_MANAGER_INIT_SIZE,false,true,effect_cmp,effect_print);
    if(!em) return NULL;

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

Status effect_get_affecteds_as_str(Effect *effect, long destiny_len,char *destiny){
    int i,n;
    Affected *aux=NULL;
    char str[WORD_SIZE]="";
    char sentid[5]=""; /*string entity id*/
    char sentt[N_ENTITY_TYPE]=""; /*string entity type*/
    char sturns[5]; /*string turns*/

    Id entid = NO_ID;
    EntityType entt;
    
    if(!effect) return NULL;
    if((collection_length(n = effect->affecteds)) == -1) return NULL;

    /*
    format is as follows:
    Entity1Id|Entity1Type|turns:Entity2Id|Entity2Type|turns: ...#
    "|" delimits values from an entity
    ":" delimits entities
    */
    for(i=0; i<n; i++){
       /*convert data to string*/
       aux = collection_get_element_at(effect->affecteds,i);
       entid = entity_get_id(aux->ent);
       entt = entity_get_entityType(aux->ent);
       sprintf(sentid,"%d", entid);
       sprintf(sentt,"%d", entt);
       sprintf(sturns,"%d",aux->turns);

       strcat(str, sentid);
       strcat(str, "|");
       strcat(str, sentt);
       strcat(str, "|");
       strcat(str,sturns);
       strcat(str,":");
    }
    strcat(str,"#");

}

Status effect_get_as_str(Effect *effect, char *destiny){
    char str[WORD_SIZE]="";
    char eloc[2]=""; /*EffectIn (effect location)*/
    /*im assuming there wont be more than 99 effects */
    char et[3]=""; /*EffectType*/
    if(!effect || !destiny) return ERROR;

    /*add id*/
    sprintf(str,"%d|",effect->id);
    /*add name*/
    strcat(str,effect->name);
    strcat(str,"|");

    /*add EffectIn*/
    eloc[0]=effect->Eloc+'0';
    strcat(str,eloc);
    strcat(str,"|");

    /*add EffectType*/
    sprintf(et,"%d",effect->ET);
    strcat(str,et);
    strcat(str,"|");

    strcat(str,effect->data);

    

    et[0]=effect->ET+'0';
    strcat(str,et);
    strcat(str,"|");
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