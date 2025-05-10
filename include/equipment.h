/**
 * @file equipment.h
 * @author Maksym Polyak
 * @brief To read a piece of equipment in the data camp of the object struct it has to have
 * a string named WEARABLE {EquipmentCode in string} {Stats affected}
 * EquipmentCode in string = "helmet" or "chest" or "arms" or "leg_armor" or "shoes" or "two_handed" or "handl" or "handr"
 * Stats affected = max_health:10 baseDamage:-2 (IF POSITIVE SIGN NOT NEEDED)
 * @version 0.1
 * @date 2025-03-29
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "object.h"
#include "entity.h"
#include "types.h"

/**
 * @brief enum to determine which piece of equipment is being treated
 */
typedef enum {EQUIPMENT_ERROR, HELMET, CHEST, ARMS, LEG_ARMOR, SHOES, TWO_HANDED, HANDL, HANDR} EquipmentCode;

/**
 * @brief Equipment ADT that stores all the pieces of equipment of an entity, also modifies their stats
 */
typedef struct _Equipment Equipment;

/*
    * PUBLIC FUNCTIONS
*/

/**
 * @brief Creates an equipment struct with everything empty
 * @author Maksym Polyak
 * 
 * @return Equipment* or NULL if error
 */
Equipment *equipment_create();

/**
 * @brief Frees memory of the struct equipment, not the objects in it
 * @author Maksym Polyak
 * 
 * @param equipment equipment struct
 */
void equipment_destroy(Equipment *equipment);

/**
 * @brief Gets the EquipmentCode from a string
 * @author Maksym Polyak
 * 
 * @param data string from equip_to_str
 * @return EquipmentCode 
 */
EquipmentCode equipment_code_from_str(char *data);

/**
 * @brief Tries to add an object as a piece if it is compatible
 * @author Maksym Polyak
 * 
 * @param entity entity who has the piece equipped
 * @param equipment equipment struct
 * @param object object to equip
 * @return Status 
 */
Status equipment_add_piece(Entity *entity, Equipment *equipment, Object *object);

/**
 * @brief Removes a piece and returns the object
 * @author Maksym Polyak
 * 
 * @param entity removes a piece of equipment
 * @param equipment equipment struct
 * @param data string with the name of the piece type from equip_to_str
 * @return Object* or NULL if error
 */
Object *equipment_remove_piece(Entity *entity, Equipment *equipment, char *data);

/**
 * @brief Gets object in certain equipment slot
 * @author Daniel Gómez
 * 
 * @param equipment equipment struct
 * @param code equipment code of the piece to remove
 * @return Object* 
 */
Object *equipment_get_piece(Equipment *equipment, EquipmentCode code);

#endif