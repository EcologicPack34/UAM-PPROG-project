/**
 * @file equipment.h
 * @author Maksym Polyak
 * @brief To read a piece of equipment in the data camp of the object struct it has to have
 * a string named WEARABLE {EquipmentCode in string} {Stats affected}
 * Stats affected = max_health:+10 baseDamage:+2
 * @version 0.1
 * @date 2025-03-29
 * 
 * @copyright Copyright (c) 2025
 * 
*/

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

typedef enum {EQUIPMENT_ERROR, HELMET, CHEST, ARMS, LEG_ARMOR, SHOES, TWO_HANDED, ONE_HANDED} EquipmentCode;

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
 * @param equipment 
 */
void equipment_destroy(Equipment *equipment);

/**
 * @brief Tries to add an object as a piece if it is compatible
 * @author Maksym Polyak
 * 
 * @param equipment 
 * @param object 
 * @return Status 
 */
Status equipment_add_piece(Equipment *equipment, Object *object);

#endif