/**
 * @file skills_manager.c
 * @author Maksym Polyak
 * @brief The gestion of cooldowns is not controled by the skills module
 * @version 0.1
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
 * 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skills_manager.h"
#include "types.h"
#include "collection.h"
#include "queue.h"

#include "debug_printing.h"

#define INITIAL_SKILLS_SIZE 5

char *skillsTags[N_SKILLS] = { "" , "heal_self", "heal_ally"};

struct _Skill {
  Id id;
  char *data;
  AbilityType type;

  Id entityid;
  bool is_player_skill;
  
  int cooldown_count;
  int cooldown_length;
};

struct _SkillManager{
  Collection *skills;
  Queue *queue_skills;
};

/*
  * Private functions
*/



/*
  * Public functions
*/

Skill *skill_create(Id id, char *data, AbilityType type, Id entityid, bool is_player_skill, int cd_count, int cd_length){
  Skill *skill = NULL;
  
  if(!data) return NULL;

  skill = (Skill *)malloc(sizeof(Skill));
  if(!skill){
    debug_log(LOG_ERROR,"Couldn't allocate memory when creating skill");
    return NULL;
  }

  skill->data = (char *)malloc(strlen(data) * sizeof(char) + 1);
  if(!skill->data){
    debug_log(LOG_ERROR,"Couldn't allocate memory when creating skill");
    free(skill);
    return NULL;
  }

  strcpy(skill->data, data);

  skill->entityid = entityid;
  skill->is_player_skill = is_player_skill;
  skill->type = type;
  skill->id = id;
  skill->cooldown_count = cd_count;
  skill->cooldown_length = cd_length;

  return skill;
}

void skill_destroy(void *skill){
  Skill *sk = NULL;
  
  if(skill){
    sk = (Skill *)skill;
    free(sk->data);
    free(sk);
  }
}



/*GETTERS*/

Id skill_get_id(Skill *skill){
  if(!skill) return NO_ID;

  return skill->id;
}

char *skill_get_data(Skill *skill){
  if(!skill) return NULL;

  return skill->data;
}

AbilityType skill_get_type(Skill *skill){
  if(!skill) return NO_SKILL;

  return skill->type;
}

int skill_cooldown_count(Skill *skill){
  if(!skill) return -1;

  return skill->cooldown_count;
}

int skill_get_cooldown_length(Skill *skill){
  if(!skill) return -1;

  return skill->cooldown_length;
}

bool skill_get_is_player_skill(Skill *skill){
  if(!skill) return FALSE;

  return skill->is_player_skill;
}

Id skill_get_entityid(Skill *skill){
  if(!skill) return NO_ID;

  return skill->entityid;
}

Status skill_set_cooldown_to_0(Skill *skill){
  if(!skill) return ERROR;

  skill->cooldown_count = 0;

  return OK;
}

Status skill_set_cooldown_to_length(Skill *skill){
  if(!skill) return ERROR;

  skill->cooldown_count = skill->cooldown_length;

  return OK;
}

Status skill_reduce_cooldown(Skill *skill){
  if(!skill) return ERROR;

  skill->cooldown_count -= 1;

  return OK;
}

Status skill_augment_cooldown(Skill *skill){
  if(!skill) return ERROR;

  skill->cooldown_count += 1;

  return OK;
}

int skills_compare(void * e1, void *e2){
  if(!e1 || !e2) return -2;

  return ((Skill*)e1)->id - ((Skill*)e2)->id;
}

AbilityType skill_type_from_str(char *string){
  int i;
  
  if(!string) return NO_SKILL;

  for (i = 0; i < N_SKILLS; i++)
  {
    if(strcmp(string, skillsTags[i]) == 0)
      return i + NO_SKILL;
  }
  return NO_SKILL;
}

/* SKILLS MANAGER */

SkillManager *skill_manager_create(){
  SkillManager *sm = NULL;

  sm = (SkillManager *)malloc(sizeof(SkillManager));
  if(!sm)
    return NULL;

  sm->skills = collection_create(INITIAL_SKILLS_SIZE, false, true, skills_compare, NULL);
  if(!sm->skills){
    free(sm);
    return NULL;
  }

  sm->queue_skills = queue_create();
  if(!sm->queue_skills){
    collection_destroy(sm->skills);
    free(sm);
    return NULL;
  }

  return sm;
}

void skill_manager_destroy(SkillManager *sm){
  if(sm){
    collection_free_elements(sm->skills, skill_destroy);
    collection_destroy(sm->skills);
    queue_destroy(sm->queue_skills);
  }
}

Status skill_manager_add_skill(SkillManager *sm, Skill *skill){
  if(!sm || !skill) return ERROR;

  return collection_add(sm->skills, (void *)skill);
}

Status skill_manager_remove_skill(SkillManager *sm, Skill *skill){
  if(!sm || !skill) return ERROR;

  return collection_remove(sm->skills, (void *)skill);
}

long skill_manager_get_skill_count(SkillManager *sm){
  if(!sm) return ERROR;

  return collection_length(sm->skills);
}

Skill *skill_manager_get_skill_at(SkillManager *sm, long index){
  if(!sm) return ERROR;

  return (Skill *)collection_get_element_at(sm->skills, index);
}

Status skill_manager_update_cooldowns(SkillManager *sm){
  int size, i;

  if(!sm) return ERROR;

  size = skill_manager_get_skill_count(sm);

  for(i = 0; i < size; i++){
    if((skill_reduce_cooldown((Skill *)skill_manager_get_skill_at(sm, i))) == ERROR)
      return ERROR;
  }

  return OK;
}

Status skill_manager_reset_cooldowns(SkillManager *sm){
  int size, i;

  if(!sm) return ERROR;

  size = skill_manager_get_skill_count(sm);

  for(i = 0; i < size; i++){
    if((skill_set_cooldown_to_0((Skill *)skill_manager_get_skill_at(sm, i))) == ERROR)
      return ERROR;
  }

  return OK;
}

Status skill_manager_use_skill(SkillManager *sm, Skill *skill){
  if(!sm || !skill) return ERROR;

  if(queue_push(sm->queue_skills, (void *)skill) == ERROR)
    return ERROR;

  return OK;
}

Queue *skill_manager_get_queue(SkillManager *sm){
  if(!sm) return ERROR;

  return sm->queue_skills;
}