/**
 * @brief It implements the effect_manager module
 * 
 * 
 *
 * @file effect_manager.c
 * @author Aaron Charameli Mair
 * @version 0
 * @date 30-04-2025
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "effect.h"
#include "collection.h"
#include "entity.h"
#include "debug_printing.h"

#define EFFECT_MANAGER_INIT_SIZE 10 /*!< Effects manager collection initial size*/
#define INIT_AFFECTED 10 /*!< Initial size of affecteds collection in an effect (irrelevant for the game functionality)*/

typedef struct{
    Entity *ent;
    int turns; /*a counter of turns that the effect is applied to the entity*/

}Affected; /*<! Struct that stores an entity and the number of turns that it suffers an effect*/

struct _Effect{
    Id id;
    Collection *affecteds; /*!< stores the affecteds that have a certain effect*/
    char *name; /*!< a string containing the effect's name*/
    char *data; /*!< a string containing data related to the effect. A function will use the data to apply the effect*/
    bool inf_turns; /*!< a boolean describing if the effect is applied for an infinite amount of turns or not*/
    int default_turns; /*!< an int with the default value of turns an effect is applied to an enemy for. e.g.: if an entity is applied an effect twice, he'll have this amount x2 of turns left with the effect*/
    EffectType ET; /*!< a type that defines the effect so it can be identified and applied*/
    EffectAffects EA;
};

struct _EffectsManager{
    Collection *effects; /*!< an effects collection*/
};

/*--------------------------------------------------------------------------------------------------------------------------*/
/*PRIVATE FUNCTIONS HEADERS*/

/**
 * @brief this function applies an effect of type poison to an entity
 * @author Aaron Charameli Mair
 * 
 * @param effect a pointer to the effect
 * @param affected a pointer to the affected (that contains the entity)
 * @param enemy_stats an array of enemy stats needed if in combat. Otherwise, leave as NULL
 * @param enemy_count an int describing the number of elements in the enemy Stats array
 * @return Status 
 * @note data string will be "DamageTaken" (a number with the number of damage dealt per turn)
 */
Status _effect_apply_poison(Effect*effect, Affected*affected, Stats*enemy_stats, int enemy_count);

/**
 * @brief this function applies an effect of type fire to an entity
 * @author Aaron Charameli Mair
 * 
 * @param effect a pointer to the effect
 * @param affected a pointer to the affected (that contains the entity)
 * @param enemy_stats an array of enemy stats needed if in combat. Otherwise, leave as NULL
 * @param enemy_count an int describing the number of elements in the enemy Stats array
 * @return Status 
 * @note data string will be "DamageTaken" (a number with the number of damage dealt per turn)
 */
Status _effect_apply_fire(Effect*effect, Affected*affected, Stats*enemy_stats, int enemy_count);

/**
 * @brief This function applies an effect of type regeneration to an entity
 * @author Aaron Charameli Mair
 * 
 * @param effect a pointer to the effect
 * @param affected a pointer to the affected (that contains the entity)
 * @param ally_stats an array of ally Stats, needed if in combat. Otherwise, leave as NULL
 * @param ally_count an int describing the number of elements in the ally Stats array
 * @return Status 
 * @note data string will be a number with the number of health regenerated per turn
 */
Status _effect_apply_regeneration(Effect*effect, Affected*affected, Stats*ally_stats, int ally_count);

/**
 * @brief This function gets the affected struct of an effect given its entity
 * @author Aaron Charameli Mair
 * 
 * @param effect a pointer to the effet
 * @param ent a pointer to the entity
 * @return Affected* or NULL if not fount or ERROR
 */
Affected *_effect_get_affected(Effect *effect, Entity *ent);

/**
 * @brief this function creates an affected
 * @author Aaron Charameli Mair
 * 
 * @param ent a pointer to the entity that will be affected
 * @param turns the number of turns the affected will suffer an effect
 * @return Affected* 
 */
Affected *_affected_create(Entity *ent, int turns);

/**
 * @brief this function frees the allocated memory of an affected struct
 * @author Aaron Charameli Mair
 * 
 * @param a a pointer to the affected
 * @note this function doesn't free the memory of the entity in the affected struct
 */
void _affected_destroy(void *a);

/*END OF PRIVATE FUNCTIONS DECLARATIONS*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------*/
/*PUBLIC FUNCTIONS IMPLEMENTATION*/

Effect *effect_create(Id id, char *name, char *data, EffectType ET, EffectAffects EA, bool inf_turns, int default_turns){
    Effect *e=NULL;
    
    if((id<=UNDEFINED_ID) || (ET == UNKNOWN_EFFECT)) return NULL;
    if((name == NULL) || (data == NULL)) return NULL;
    if(default_turns <= 0) return NULL;
    
    if((e = (Effect *)malloc(sizeof(Effect))) == NULL) return NULL;


    e->id = id;
    if((e->affecteds = collection_create(INIT_AFFECTED,false,true, effect_cmp, effect_print)) == NULL) return NULL;
    e->name = strdup(name);
    e->data = strdup(data);
    e->ET = ET;
    e->EA = EA;
    e->inf_turns = inf_turns;
    e->default_turns = default_turns;

    return e;
}

EffectManager *effect_manager_create(){
    EffectManager *em=NULL;

    if((em = (EffectManager*)malloc(sizeof(EffectManager))) == ERROR) return NULL;

    em->effects = collection_create(EFFECT_MANAGER_INIT_SIZE,false,true,effect_cmp,effect_print);
    if(!em->effects) return NULL;

    return em;
}

void effect_destroy(void *e){
    Effect *aux=e;
    if(aux){
        if(aux->name)
            free(aux->name);
        if(aux->data)
            free(aux->data);
        if(aux->affecteds){
            collection_free_elements(aux->affecteds, _affected_destroy);
            collection_destroy(aux->affecteds);
        }
        free(e);
    }
    return;
}

Collection *effect_manager_get_effects(EffectManager *em){
    if(!em) return NULL;
    return em->effects;
}

Effect *effect_get_by_id(EffectManager *em, Id id){
    Effect *effect=NULL;
    int i,len;
    if(!em || (id<UNDEFINED_ID)) return NULL;

    len = collection_length(em->effects);

    for(i=0;i<len;i++){
        effect = collection_get_element_at(em->effects, i);
        if(effect && (effect->id == id))
            return effect;
    }
    return NULL;
}

void effect_manager_destroy(EffectManager *em){
    if(em){
        if(em->effects){
            collection_free_elements(em->effects, effect_destroy);
            collection_destroy(em->effects);
        }
        free(em);
    }
    return;
}

Status effect_manager_add_effect(EffectManager *em, Effect *effect){
    if(!em || !effect) return ERROR;
    return collection_add(em->effects, (void *)effect);
}

Status effect_add_affected(Effect *e, Entity *ent){
    Status st = OK;
    Affected *a=NULL;
    if(!e || !ent) return ERROR;
    if(effect_has_affected(e, ent) == false){
        if((a = _affected_create(ent,e->default_turns)) == NULL) return ERROR;
            st = collection_add(e->affecteds,a);
        }
    else{

    }
    
    return st;
}

bool effect_has_affected(Effect *e, Entity *ent){
    return (_effect_get_affected(e,ent)!=NULL) ? true : false;
}

Status effect_update(Effect *effect, Stats *ent_stats, int ent_count){
    int i,n;
    Affected *aux=NULL;

    if((ent_stats != NULL) && (ent_count<=0)) return ERROR;
    if(!effect || ((n = collection_length(effect->affecteds)) == -1)) return ERROR;

    for(i=0; i<n; i++){
        aux = collection_get_element_at(effect->affecteds, i);
        if(aux){
            switch (effect->ET){
                case POISON:
                    _effect_apply_poison(effect, aux, ent_stats, ent_count);
                    break;
                case REGENERATION:
                    _effect_apply_regeneration(effect, aux, ent_stats, ent_count);
                    break;
                case FIRE:
                    _effect_apply_fire(effect, aux, ent_stats, ent_count);
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
    #efdat:n
    Entity1Id|Entity1Type|turns
    Entity2Id|Entity2Type|turns
    */

    if((file = fopen(filename,"a")) == NULL) return ERROR;
    fprintf(file,"\n\n");

    fprintf(file,"#efdat:%d\n", n);
    for(i=0; i<n; i++){
        aux = collection_get_element_at(effect->affecteds,i);
        
        /*id print*/
        sprintf(aux_str,"%d",(int)entity_get_id(aux->ent));
        strcat(str, aux_str);
        strcat(str, "|");
        /*entity type print*/
        sprintf(aux_str,"%d",(int)entity_get_entityType(aux->ent));
        strcat(str, aux_str);
        strcat(str, "|");
        /*turns print*/
        sprintf(aux_str,"%d",aux->turns);
        strcat(str, aux_str);

        /*add to file*/
        fprintf(file,"%s\n",str);
        strcpy(str,"\0");
    }

    if (ferror(file)) {
        debug_log(LOG_ERROR, "Error in file at: effect_write_affected_save_data(Effect*, char*) in effect.c");
        fclose(file);
        return ERROR;
      }

    fclose(file);
    return OK;
}

Status effect_get_as_str(Effect *effect, long destiny_size ,char *destiny){
    /*#ef:Id|name|EffectType|inf_turns|default_turns|data*/
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

    /*add EffectType*/
    sprintf(aux,"%d", (int)effect->ET);
    strcat(str,aux);
    strcat(str,"|");

    /*add inf_turns*/
    (effect->inf_turns==false)? strcat(str,"0|") : strcat(str,"1|");

    /*add default_turns*/
    sprintf(aux,"%d", (int)effect->default_turns);
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

EffectAffects effect_get_effectAffects(Effect *effect){
    if(!effect) return NO_EFFECT;
    return effect->EA;
}

int effect_cmp(void*e1, void*e2){
    if(!e1 || !e2) return -2;
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

Status _effect_apply_poison(Effect*effect, Affected*affected, Stats*enemy_stats, int enemy_count){
    double DamageTaken;
    char aux[WORD_SIZE];
    int i;
    Status st=OK;
    /*data string will be: "(int)DamageTaken"*/
    if(!effect || !affected) return ERROR;
    if(effect->ET != POISON) return ERROR;
    
    strcpy(aux, effect->data);
    DamageTaken = atoi(aux);
    
    /*this is gameState != COMBAT*/
    if(enemy_stats == NULL)
        st = entity_set_health(affected->ent,entity_get_health(affected->ent)-DamageTaken);
    else{
        for(i=0; i<=enemy_count-1; i++){
            if(affected->ent == (enemy_stats+i)->entity)
                (enemy_stats+i)->stats.health -= DamageTaken;
        }
    }

    if((affected->turns>0) && (st == OK))
        affected->turns--;

    return st;
}

Status _effect_apply_fire(Effect*effect, Affected*affected, Stats*enemy_stats, int enemy_count){
    double DamageTaken;
    char aux[WORD_SIZE];
    int i;
    Status st=OK;
    /*data string will be: "(int)DamageTaken"*/
    if(!effect || !affected) return ERROR;
    if(effect->ET != FIRE) return ERROR;
    
    strcpy(aux, effect->data);
    DamageTaken = atoi(aux);
    
    /*this is gameState != COMBAT*/
    if(enemy_stats == NULL)
        st = entity_set_health(affected->ent,entity_get_health(affected->ent)-DamageTaken);
    /*this is gameState == COMBAT*/
    else{
        for(i=0; i<=enemy_count-1; i++){
            if(affected->ent == (enemy_stats+i)->entity)
                (enemy_stats+i)->stats.health -= DamageTaken;
        }
    }

    if((affected->turns>0) && (st == OK))
        affected->turns--;

    return st;
}

Status _effect_apply_regeneration(Effect *effect, Affected *affected, Stats*ally_stats, int ally_count){
    double regeneration;
    char aux[WORD_SIZE];
    int i;
    Status st=OK;
    /*data string will be: "(int)regeneration_value"*/
    if(!effect || !affected) return ERROR;
    if(effect->ET != REGENERATION) return ERROR;
    
    strcpy(aux, effect->data);
    regeneration = atoi(aux);
    
    if(ally_stats == NULL){
        if(entity_get_health(affected->ent) >= (entity_get_max_health(affected->ent) - regeneration))
            st = entity_set_health(affected->ent, entity_get_max_health(affected->ent));
        else
            st = entity_set_health(affected->ent,entity_get_health(affected->ent)+regeneration);
    }
    else{
        for(i=0; i<=ally_count-1; i++){
            if(affected->ent == (ally_stats+i)->entity){
                if((ally_stats+i)->stats.health >= ((ally_stats+i)->stats.maxhealth - regeneration))
                    (ally_stats+i)->stats.health = (ally_stats+i)->stats.maxhealth;
                else
                    (ally_stats+i)->stats.health += regeneration;
            }
        }
    }

    if((affected->turns>0) && (st == OK))
        affected->turns--;

    return st;
}

Affected *_effect_get_affected(Effect *effect, Entity *ent){
    Affected *aux=NULL;
    int i,n;
    if(!effect || !ent) return NULL;

    if((n=collection_length(effect->affecteds)) == -1) return NULL;

    for(i = 0; i<n; i++){
        aux = collection_get_element_at(effect->affecteds,i);
        if(aux->ent==ent)
            return aux;
    }
    return NULL;
}

Affected *_affected_create(Entity *ent, int turns){
    Affected *a=NULL;
    if(!ent || (turns<=0)) return NULL;
    if((a = (Affected*)malloc(sizeof(Affected))) == NULL) return NULL;

    a->ent = ent;
    a->turns = turns;
    return a;
}

void _affected_destroy(void *a){
    if(a)
        free((Affected *)a);

    return;
}

/*END OF PRIVATE FUNCTIONS IMLPEMENTATION*/
/*--------------------------------------------------------------------------------------------------------------------------*/