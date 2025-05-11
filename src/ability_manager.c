/**
 * @file ability_manager.c
 * @author Maksym Polyak
 * @brief The gestion of cooldowns is not controled by the ability module
 * @version 0.1
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
 * 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ability_manager.h"
#include "types.h"
#include "collection.h"
#include "queue.h"
#include "utils.h"

#include "debug_printing.h"

#define INITIAL_SKILLS_SIZE 8 /*!< Initial number of abilities on the collection of the ability manager*/

/**
 * @brief Tags related to the ability references on the .dat. Uses N_SKILLS as maximum size
 * 
 */
char *abilityTags[N_SKILLS] = { "" , "heal_self", "heal_ally", "money_bag","link_unlock","effect_self","effect_enemy"\
  ,"effect_ally"}; /*!< Tags related to the type | Same order as AbilityType*/

/**
 * @brief Struct that contains all the information related to an ability
 */
struct _Ability {
  Id id;                  /*!< Id of the ability*/
  char *name;             /*!< Name of the ability*/
  char *data;             /*!< Data used to determine values in the ability*/
  AbilityType type;       /*!< Type of the ability*/

  Id entityid;            /*!< Id of the entity with the ability*/
  bool is_player_ability; /*!< TRUE if a player has the ability*/
  bool is_object_use;     /*!< TRUE if an object has the ability*/
  
  int cooldown_count;     /*!< Actual cooldown of the ability*/
  int cooldown_length;    /*!< Maximum cooldown of the ability*/

  int cost;               /*!< Cost in SP of the ability*/
};

/**
 * @brief Struct that saves all the abilities in the game and a queue to trigger abilities
 */
struct _AbilityManager{
  Collection *ability;        /*!< Collection where all abilities are stored*/
  Queue *queue_cooldowns;     /*!< Queue to control cooldowns*/
  Ability *evaluated_ability; /*!< Ability to evaluate*/

  Collection *unused_abilities; /*!< Collection with abilities that are not assigned*/
};

/*
  * Private functions
*/



/*
  * Public functions
*/

Ability *ability_create(Id id, char *data, char *name, AbilityType type, Id entityid, bool is_player_ability, bool is_object_use, int cd_count, int cd_length, int cost){
  Ability *ability = NULL;
  
  if(!data) return NULL;

  ability = (Ability *)malloc(sizeof(Ability));
  if(!ability){
    debug_log(LOG_ERROR,"Couldn't allocate memory when creating ability");
    return NULL;
  }

  ability->data = (char *)malloc((strlen(data)+10) * sizeof(char));
  if(!ability->data){
    debug_log(LOG_WARNING,"Couldn't allocate memory for ability's data");
  }

  ability->name = (char *)malloc((strlen(name)+10) * sizeof(char));
  if(!ability->name){
    free(ability->data);
    free(ability);
    return NULL;
  }

  strcpy(ability->name, name);
  strcpy(ability->data, data);

  ability->entityid = entityid;
  ability->is_player_ability = is_player_ability;
  ability->is_object_use = is_object_use;
  ability->type = type;
  ability->id = id;
  ability->cooldown_count = cd_count;
  ability->cooldown_length = cd_length;
  ability->cost = cost;

  return ability;
}

void ability_destroy(void *ability){
  Ability *sk = NULL;
  
  if(ability){
    sk = (Ability *)ability;
    if(sk->name)
      free(sk->name);
    if(sk->data)
      free(sk->data);
    free(sk);
  }
}



/*GETTERS*/

Id ability_get_id(Ability *ability){
  if(!ability) return NO_ID;

  return ability->id;
}

char *ability_get_data(Ability *ability){
  if(!ability) return NULL;

  return ability->data;
}

AbilityType ability_get_type(Ability *ability){
  if(!ability) return NO_SKILL;

  return ability->type;
}

int ability_get_cooldown_count(Ability *ability){
  if(!ability) return -1;

  return ability->cooldown_count;
}

int ability_get_cooldown_length(Ability *ability){
  if(!ability) return -1;

  return ability->cooldown_length;
}

bool ability_get_is_player_ability(Ability *ability){
  if(!ability) return false;

  return ability->is_player_ability;
}

bool ability_get_is_object_use(Ability *ability){
  if(!ability) return false;

  return ability->is_object_use;
}

Id ability_get_entityid(Ability *ability){
  if(!ability) return NO_ID;

  return ability->entityid;
}

char *ability_get_name(Ability *ability){
  if(!ability) return NULL;

  return ability->name;
}

int ability_get_cost(Ability *ability){
  if(!ability) return -1;

  return ability->cost;
}

/*
  * Ability general functions
*/

Status ability_set_cooldown_to_0(Ability *ability){
  if(!ability) return ERROR;

  ability->cooldown_count = 0;

  return OK;
}

Status ability_set_cooldown_to_length(Ability *ability){
  if(!ability) return ERROR;

  ability->cooldown_count = ability->cooldown_length + 1;

  return OK;
}

Status ability_reduce_cooldown(Ability *ability){
  if(!ability) return ERROR;

  ability->cooldown_count -= 1;

  return OK;
}

Status ability_augment_cooldown(Ability *ability){
  if(!ability) return ERROR;

  ability->cooldown_count += 1;

  return OK;
}

int ability_compare(void * e1, void *e2){
  if(!e1 || !e2) return -2;

  return ((Ability*)e1)->id - ((Ability*)e2)->id;
}

AbilityType ability_type_from_str(char *string){
  int i;
  
  if(!string) return NO_SKILL;

  for (i = 0; i < N_SKILLS; i++)
  {
    if(strcmp(string, abilityTags[i]) == 0)
      return i + NO_SKILL;
  }
  return NO_SKILL;
}

/* Ability manager functions */

AbilityManager *ability_manager_create(){
  AbilityManager *sm = NULL;

  sm = (AbilityManager *)malloc(sizeof(AbilityManager));
  if(!sm)
    return NULL;

  sm->ability = collection_create(INITIAL_SKILLS_SIZE, false, true, ability_compare, NULL);
  if(!sm->ability){
    free(sm);
    return NULL;
  }

  sm->evaluated_ability = NULL;

  sm->queue_cooldowns = queue_create();
  if(!sm->queue_cooldowns){
    collection_destroy(sm->ability);
    free(sm);
    return NULL;
  }

  sm->unused_abilities = collection_create(INITIAL_SKILLS_SIZE, false, true, ability_compare, NULL);
  if(!sm->unused_abilities){
    queue_destroy(sm->queue_cooldowns);
    collection_destroy(sm->ability);
    free(sm);
    return NULL;
  }

  return sm;
}

void ability_manager_destroy(AbilityManager *sm){
  if(sm){
    collection_free_elements(sm->unused_abilities, ability_destroy);
    collection_destroy(sm->unused_abilities);
    collection_free_elements(sm->ability, ability_destroy);
    collection_destroy(sm->ability);
    queue_destroy(sm->queue_cooldowns);

    free(sm);
  }
}

/*
  * Ability manager GETTERS
*/

Ability *ability_manager_get_evaluated_ability(AbilityManager *sm){
  if(!sm) return ERROR;

  return sm->evaluated_ability;
}

Collection *ability_manager_get_unused_abilities(AbilityManager *sm){
  if(!sm) return NULL;

  return sm->unused_abilities;
}

Ability *ability_manager_get_unused_ability_at(AbilityManager *sm, int i){
  if(!sm) return NULL;

  return collection_get_element_at(sm->unused_abilities, i);
}

long ability_manager_get_ability_count(AbilityManager *sm){
  if(!sm) return ERROR;

  return collection_length(sm->ability);
}

Ability *ability_manager_get_ability_at(AbilityManager *sm, long index){
  if(!sm) return ERROR;

  return (Ability *)collection_get_element_at(sm->ability, index);
}

Queue *ability_manager_get_queue(AbilityManager *sm){
  if(!sm) return ERROR;

  return sm->queue_cooldowns;
}

/*
  * Ability manager general functions
*/

Status ability_manager_add_evaluated_ability(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  sm->evaluated_ability = ability;

  return OK;
}

Status ability_manager_add_ability_to_cd(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  if(ability_get_entityid(ability) == NO_ID){
    return collection_add(sm->unused_abilities, (void *)ability);
  }

  if(ability_get_cooldown_count(ability) > 0)
    queue_push(sm->queue_cooldowns, ability);

  return collection_add(sm->ability, (void *)ability);
}

Status ability_manager_remove_ability_from_cd(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  return collection_remove(sm->ability, (void *)ability);
}

Status ability_manager_use_ability(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  sm->evaluated_ability = ability;

  return OK;
}

int ability_manager_save_on_file(AbilityManager *sm, FILE *fOUT){
  int i, count = 0, size;

  if(!sm || !fOUT) return -1;

  size = collection_length(sm->ability);
  count += fprintf(fOUT,"%d\n", size);
  for(i = 0; i < size; i++){
    count += ability_save_on_file(collection_get_element_at(sm->ability, i), fOUT);
  }

  return count;
}

Status ability_manager_read_from_file(AbilityManager *sm, FILE *fIN){
  int i, size;

  if(!sm || !fIN) return ERROR;

  fscanf(fIN,"%d\n", &size);
  for(i = 0; i < size; i++){
    if(collection_add(sm->ability, ability_create_from_file(fIN)) == ERROR)
      return ERROR;
  }

  return OK;
}

Status ability_manager_move_ability_to_used(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  collection_remove(sm->unused_abilities, ability);

  return collection_add(sm->ability, ability);
}

int ability_save_on_file(Ability *ability, FILE *fOUT){
  int count = 0;

  if(!ability || !fOUT) return -1;

  count += fprintf(fOUT, "%ld;%d;%ld;%d;%d;%d;%d;%d\n", ability->id,\
    ability->type, ability->entityid, ability->is_player_ability,\
    ability->is_object_use, ability->cooldown_count, ability->cooldown_length, ability->cost);

  count += fprintf(fOUT, "%s\n%s\n", ability->name, ability->data);

  return count;
}

Ability *ability_create_from_file(FILE *fIN){
  Id id, entityid;
  char name[WORD_SIZE], data[WORD_SIZE];
  int type, is_player_ability, is_object_use, cooldown_count, cooldown_length, cost;

  fscanf(fIN, "%ld;%d;%ld;%d;%d;%d;%d;%d\n", &id, &type, &entityid,\
     &is_player_ability, &is_object_use, &cooldown_count, &cooldown_length, &cost);

  fgets(name, WORD_SIZE, fIN);
  string_remove_newline_escape_sequence_on_end(name);
  fgets(data, WORD_SIZE, fIN);
  string_remove_newline_escape_sequence_on_end(data);

  return ability_create(id, data, name, type, entityid, is_player_ability,\
     is_object_use, cooldown_count, cooldown_length, cost);
}