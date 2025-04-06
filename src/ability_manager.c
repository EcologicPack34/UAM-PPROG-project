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

#include "debug_printing.h"

#define INITIAL_SKILLS_SIZE 5 /*!< Initial number of abilities on the collection of the ability manager*/

/**
 * @brief Tags related to the ability references on the .dat. Uses N_SKILLS as maximum size
 * @file ability_manager.c
 */
char *abilityTags[N_SKILLS] = { "" , "heal_self", "heal_ally", "money_bag","link_unlock"}; /*!< Tags related to the type | Same order as AbilityType*/

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
};

/**
 * @brief Struct that saves all the abilities in the game and a queue to trigger abilities
 */
struct _AbilityManager{
  Collection *ability;        /*!< Collection where all abilities are stored*/
  Queue *queue_cooldowns;     /*!< Queue to control cooldowns*/
  Ability *evaluated_ability; /*!< Ability to evaluate*/

};

/*
  * Private functions
*/



/*
  * Public functions
*/

Ability *ability_create(Id id, char *data, char *name, AbilityType type, Id entityid, bool is_player_ability, bool is_object_use, int cd_count, int cd_length){
  Ability *ability = NULL;
  
  if(!data) return NULL;

  ability = (Ability *)malloc(sizeof(Ability));
  if(!ability){
    debug_log(LOG_ERROR,"Couldn't allocate memory when creating ability");
    return NULL;
  }

  ability->data = (char *)malloc(strlen(data) * sizeof(char) + 1);
  if(!ability->data){
    debug_log(LOG_ERROR,"Couldn't allocate memory when creating ability");
    free(ability);
    return NULL;
  }

  ability->name = (char *)malloc(strlen(name) * sizeof(char) + 1);
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

  return ability;
}

void ability_destroy(void *ability){
  Ability *sk = NULL;
  
  if(ability){
    sk = (Ability *)ability;
    free(sk->name);
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

/* SKILLS MANAGER */

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

  return sm;
}

void ability_manager_destroy(AbilityManager *sm){
  if(sm){
    collection_free_elements(sm->ability, ability_destroy);
    collection_destroy(sm->ability);
    queue_destroy(sm->queue_cooldowns);

    free(sm);
  }
}

Status ability_manager_add_evaluated_ability(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  sm->evaluated_ability = ability;

  return OK;
}

Ability *ability_manager_get_evaluated_ability(AbilityManager *sm){
  if(!sm) return ERROR;

  return sm->evaluated_ability;
}

Status ability_manager_add_ability_to_cd(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  return collection_add(sm->ability, (void *)ability);
}

Status ability_manager_remove_ability_from_cd(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  return collection_remove(sm->ability, (void *)ability);
}

long ability_manager_get_ability_count(AbilityManager *sm){
  if(!sm) return ERROR;

  return collection_length(sm->ability);
}

Ability *ability_manager_get_ability_at(AbilityManager *sm, long index){
  if(!sm) return ERROR;

  return (Ability *)collection_get_element_at(sm->ability, index);
}

Status ability_manager_use_ability(AbilityManager *sm, Ability *ability){
  if(!sm || !ability) return ERROR;

  sm->evaluated_ability = ability;

  return OK;
}

Queue *ability_manager_get_queue(AbilityManager *sm){
  if(!sm) return ERROR;

  return sm->queue_cooldowns;
}