/**
 * @file equipment.c
 * @author Maksym Polyak
 * @brief Controls equipment of a Player module
 * @version 0.1
 * @date 2025-03-29
 * 
 * @copyright Copyright (c) 2025
 * 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipment.h"


#define N_PIECES 9 /*!< Maximum number of types of pieces an object can be + empty type*/

/**
 * @brief Array of strings to save the text references to each piece type
 */
char *equip_to_str[N_PIECES] = {"","helmet","chest","arms","leg_armor","shoes","two_handed","handl","handr"};

/**
 * @brief Array of strings to save the text references to each stat
 */
char *stat_to_str[N_STATS] = {"","max_health","health","base_damage","strength","defense","magic_Level"};


/**
 * @brief Equipment internal struct for the player
 */
struct _Equipment{
    Object *helmet;             /*!< Helmet of the entity*/
    Object *arms;               /*!< Arms armor of the entity*/
    Object *chest;              /*!< Chest armor of the entity*/
    Object *leg_armor;          /*!< Leg armor of the entity*/
    Object *shoes;              /*!< Shoes of the entity*/

    bool is_two_handed;         /*!< Determines if the entity is using a two handed weapon or not*/
    Object *weapon_two_hands;   /*!< Two hands weapon of the entity*/
    Object *weapon1_one_hand;   /*!< weapon on first hand of the entity*/
    Object *weapon2_one_hand;   /*!< weapon on second hand of the entity*/
};

/*
    * PRIVATE FUNCTIONS
*/

/**
 * @brief Equips a piece by the code received
 * @author Maksym Polyak
 * 
 * @param player equipment ADT where the piece is placed
 * @param code EquipmentCode to determine which piece is the object
 * @param object object to be equipped
 * @return Status 
 */
Status equipment_equip_from_code(Equipment *player, EquipmentCode code, Object *object);

/**
 * @brief Unequips a piece by the code received and returns the object removed
 * @author Maksym Polyak
 * 
 * @param equipment equipment struct
 * @param code EquipmentCode to determine piece type
 * @return Object * or NULL if error 
 */
Object *equipment_unequip_from_code(Equipment *equipment, EquipmentCode code);

/**
 * @brief Gets the statcode from a string, has to match one of stat_to_str
 * @author Maksym Polyak
 * 
 * @param data string with the type
 * @return StatCode or NO_STAT if error
 */
StatCode equipment_statcode_from_str(char *data);

/**
 * @brief Changes the stats of an entity by its parameters
 * @author Maksym Polyak
 * 
 * @param entity entity that has the stats adjusted
 * @param code type of stat affected --> Stat_code
 * @param value double with the value of the stat affected
 * @return Status 
 */
Status equipment_change_stat_value_by_code(Entity *entity, StatCode code, double value);

/**
 * @brief Adds the stats of an object to the entity
 * @author Maksym Polyak
 * 
 * @param entity entity that has the stats of an object added
 * @param equipment equipment struct
 * @param object object from where the stats are taken
 * @return Status 
 */
Status equipment_add_stats(Entity *entity, Equipment *equipment, Object *object);

/**
 * @brief Removes the stats of an object from the entity
 * @author Maksym Polyak
 * 
 * @param entity entity that has the stats of an object removed
 * @param equipment equipment struct
 * @param object object from where the stats are taken
 * @return Status 
 */
Status equipment_remove_stats(Entity *entity, Equipment *equipment, Object *object);

Status equipment_equip_from_code(Equipment *equipment, EquipmentCode code, Object *object){
    if(!equipment || !object) return ERROR;

    switch (code){
    case EQUIPMENT_ERROR:
        break;
    case HELMET:
        if(equipment->helmet == NULL){
            equipment->helmet = object;
            return OK;
        }
        break;
    case CHEST:
        if(equipment->chest == NULL){
            equipment->chest = object;
            return OK;
        }
        break;
    case ARMS:
        if(equipment->arms == NULL){
            equipment->arms = object;
            return OK;
        }
        break;
    case LEG_ARMOR:
        if(equipment->leg_armor == NULL){
            equipment->leg_armor = object;
            return OK;
        }
        break;
    case SHOES:
        if(equipment->shoes == NULL){
            equipment->shoes = object;
            return OK;
        }
        break;
    case TWO_HANDED:
        if(equipment->weapon_two_hands == NULL){
            equipment->weapon_two_hands = object;
            equipment->is_two_handed = true;
            return OK;
        }
        break;
    case HANDL:
        if(equipment->weapon1_one_hand == NULL && equipment->weapon_two_hands == NULL){
            equipment->weapon1_one_hand = object;
            equipment->is_two_handed = false;
            return OK;
        } 
        break;
    case HANDR:
        if(equipment->weapon2_one_hand == NULL && equipment->weapon_two_hands == NULL){
            equipment->weapon2_one_hand = object;
            equipment->is_two_handed = false;
            return OK;
        }
        break;

    default:
        break;
    }

    return ERROR;
}

Object *equipment_unequip_from_code(Equipment *equipment, EquipmentCode code){
    Object *object = NULL;
    
    if(!equipment) return NULL;

    switch (code){
    case EQUIPMENT_ERROR:
        break;
    case HELMET:
        object = equipment->helmet;
        equipment->helmet = NULL;
        return object;
    case CHEST:
        object = equipment->chest;
        equipment->chest = NULL;
        return object;
    case ARMS:
        object = equipment->arms;
        equipment->arms = NULL;
        return object;
    case LEG_ARMOR:
        object = equipment->leg_armor;
        equipment->leg_armor = NULL;
        return object;
    case SHOES:
        object = equipment->shoes;
        equipment->shoes = NULL;
        return object;
    case TWO_HANDED:
        object = equipment->weapon_two_hands;
        equipment->weapon_two_hands = NULL;
        return object;
    case HANDL:
        object = equipment->weapon1_one_hand;
        equipment->weapon1_one_hand = NULL;
        return object;
    case HANDR:
        object = equipment->weapon2_one_hand;
        equipment->weapon2_one_hand = NULL;
        return object;

    default:
        break;
    }

    return NULL;
}

StatCode equipment_statcode_from_str(char *data){
    int i;
    
    if(!data) return NO_SKILL;

    for(i = 0; i < N_STATS; i++){
        if(strcmp(stat_to_str[i], data) == 0)
            return i + NO_STAT;
    }

    return NO_STAT;
}

Status equipment_change_stat_value_by_code(Entity *entity, StatCode code, double value){
    if(!entity) return ERROR;

    switch(code){
        case MAX_HEALTH:
            entity_set_max_health(entity, entity_get_max_health(entity) + value);
            break;
        case HEALTH:
            break;
        case BASE_DAMAGE:
            entity_set_baseDamage(entity, entity_get_baseDamage(entity) + value);
            break;
        case STRENGTH:
            entity_set_strength(entity, entity_get_strength(entity) + value);
            break;
        case DEFENSE:
            entity_set_defense(entity, entity_get_defense(entity) + value);
            break;
        case MAGIC_LEVEL:
            entity_set_magicLevel(entity, entity_get_magicLevel(entity) + value);
            break;
        
        default:
            break;
    }

    return OK;
}

Status equipment_add_stats(Entity *entity, Equipment *equipment, Object *object){
    char *data = NULL;
    char line[WORD_SIZE];
    char *toks = NULL;
    StatCode code;
    double value;

    if(!entity || !equipment || !object) return ERROR;

    data = object_get_data(object);
    if(!data) return ERROR;

    strcpy(line, data);

    toks = strtok(line," ");
    toks = strtok(NULL," ");

    do{
        toks = strtok(NULL, ":");
        if(toks == NULL) break;

        code = equipment_statcode_from_str(toks);
        if(code == NO_STAT) return ERROR;

        if(toks != NULL){
            toks = strtok(NULL, " ");
            sscanf(toks, "%lf", &value);
            if(equipment_change_stat_value_by_code(entity, code, value) == ERROR) return ERROR;
        }
    }while(toks != NULL);

    return OK;
}

Status equipment_remove_stats(Entity *entity, Equipment *equipment, Object *object){
    char *data = NULL;
    char line[WORD_SIZE];
    char *toks = NULL;
    EquipmentCode code;
    double value;

    if(!entity || !equipment || !object) return ERROR;

    data = object_get_data(object);
    if(!data) return ERROR;

    strcpy(line, data);

    toks = strtok(line," ");
    toks = strtok(NULL," ");

    do{
        toks = strtok(NULL, ":");
        if(toks == NULL) break;

        code = equipment_statcode_from_str(toks);
        if(code == EQUIPMENT_ERROR) return ERROR;

        if(toks != NULL){
            toks = strtok(NULL, " ");
            sscanf(toks, "%lf", &value);
            if(equipment_change_stat_value_by_code(entity, code, -value) == ERROR) return ERROR;
        }
    }while(toks != NULL);

    return OK;
}

/*
    * Equipment public functions
*/

Equipment *equipment_create(){
    Equipment *equipment = NULL;

    equipment = (Equipment *)calloc(1, sizeof(Equipment));
    if(!equipment)
        return NULL;

    equipment->is_two_handed = 1;

    return equipment;
}

void equipment_destroy(Equipment *equipment){
    if(equipment)
        free(equipment);
}

/*
    * Equipment getters
*/

EquipmentCode equipment_code_from_str(char *data){
    int i;
    
    if(!data) return EQUIPMENT_ERROR;

    for(i = 0; i < N_PIECES; i++){
        if(strcmp(equip_to_str[i], data) == 0)
            return i + EQUIPMENT_ERROR;
    }

    return EQUIPMENT_ERROR;
}

Object *equipment_get_piece(Equipment *equipment, EquipmentCode code){
    Object *retobject = NULL;
    
    if(!equipment) return NULL;

    retobject = equipment_unequip_from_code(equipment, code);
    equipment_equip_from_code(equipment, code, retobject);

    return retobject;
}

/*
    * Equipment setters
*/

Status equipment_add_piece(Entity *entity, Equipment *equipment, Object *object){
    char *data = NULL;
    char line[WORD_SIZE];
    char *toks = NULL;
    EquipmentCode code = EQUIPMENT_ERROR;
    
    if(!equipment || !object) return ERROR;

    data = object_get_data(object);
    if(data == NULL)
        return ERROR;

    strcpy(line, data);

    toks = strtok(line, " ");
    if(strcmp(toks, "wearable") != 0 || object_get_is_equipped(object) == true)
        return ERROR;

    toks = strtok(NULL, " ");
    code = equipment_code_from_str(toks);
    if(code == EQUIPMENT_ERROR)
        return ERROR;

    if(equipment_equip_from_code(equipment, code, object) == ERROR)
        return ERROR;

    if(equipment_add_stats(entity, equipment, object) == ERROR) return ERROR;

    object_set_is_equipped(object, true); 

    return OK;
}

Object *equipment_remove_piece(Entity *entity, Equipment *equipment, char *data){
    Object *retobject = NULL;
    EquipmentCode code;
    
    if(!equipment) return NULL;

    code = equipment_code_from_str(data);
    if(code == EQUIPMENT_ERROR) return NULL;

    retobject = equipment_unequip_from_code(equipment, code);

    if(retobject == NULL)
        return NULL;

    if(equipment_remove_stats(entity, equipment, retobject) == ERROR) return NULL;

    object_set_is_equipped(retobject, false);

    return retobject;
}
