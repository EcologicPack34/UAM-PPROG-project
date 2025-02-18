/**
 * @brief Defines the internal functionality of the links.
 * 
 * links are defined by an id, two spaces, a bool that regulates if they are locked
 * or not and the id of an object that can unlock them. This makes the game able to
 * create labyrints, teleports and more because two spaces don't need to be near in
 * order for the player to move between them.
 * 
 * @file link.c
 * @author Daniel Gómez Rodríguez
 * @version 0.1
 * @date 2025-02-06
 * 
 * @copyright GNU Public License
 */

#include "link.h"
#include "debug_printing.h"

#include <stdio.h>
#include <stdlib.h>

struct _Link{
    Id id;
    Id space1;              /*<! One of the spaces connected by a link*/
    Id space2;              /*<! One of the spaces connected by a link*/
    bool adjacent;          /*<! Stores if the connected spaces are adjacent or not*/
    bool locked;            /*<! Stores if the link can be used*/
    Id unlockingObject;     /*<! Stores id of object used to unlock link*/
};

/**
 * @brief Checks if an entity is on one of the spaces connected by the link
 * 
 * @param link 
 * @param entity entity to be checked
 * @return bool 
 */
bool link_is_entity_on_valid_spaces(Link *link, Entity *entity){
    Id entityID = entity_get_location(entity);
    
    if(link == NULL || entity == NULL){
        debug_log(LOG_ERROR, "Null argument reference: at link_is_entity_on_valid_spaces(Link*, Entity*) in link.c");
        return false;
    }

    if(entityID == link_get_space1(link) || entityID == link_get_space2(link))
        return true;
    
    return false;
}

Link *link_create(Id id,Id space1, Id space2, bool adjacent,bool locked, Id unlockingObject){
    Link *link = NULL;
    
    /*Comprobacion de errores y reserva de memoria */
    if(space1 == NO_ID && space2 == NO_ID){
        debug_log(LOG_ERROR, "Error Initializing link: link must have at least 1 space: at link_create(Id,Id, Id, bool, Id) in link.c");
        return NULL;
    }
    if(id <= NO_ID){
        debug_log(LOG_WARNING, "No ID assigned to link : at link_create(Id, Id, Id, bool, Id) in link.c");
        unlockingObject = UNDEFINED_ID;
    }
    /*Usar un id no valido para el objecto se considerará como no asignado y no fallará la función*/
    if(unlockingObject == NO_ID){
        debug_log(LOG_WARNING, "unlocking object is set to NO_ID, did you mean UNDEFINED_ID? : at link_create(Id, Id, Id, bool, Id) in link.c");
        unlockingObject = UNDEFINED_ID;
    }
    
    link = (Link*)malloc(sizeof(Link));
    if(link == NULL){
        debug_log(LOG_ERROR, "Error Initializing link: could not allocate memory for link : at link_create(Id, Id, Id, bool, Id) in link.c");
        return NULL;
    }

    /*asignacion de valores*/
    /*no se comprueba el status pues sabemos que si hemos llegado aqui link no es null*/
    link_set_id(link,id);
    link_set_spaces(link, space1, space2);
    link_set_is_adjacent(link, adjacent);
    link_set_locked(link, locked);
    link_set_unlocking_object(link, unlockingObject);

    return link;
}

void link_destroy(Link *link){
    if(link)
        free(link);
}

Status link_set_id(Link *link, Id id){
    if(link == NULL || id == NO_ID) return ERROR;

    link->id = id;
    return OK;
}

Status link_set_spaces(Link* link, Id id1, Id id2){
    if(link == NULL){
        debug_log(LOG_ERROR, "Null Link* argument: at link_set_spaces(Link*, Id, Id) in link.c");
        return ERROR;
    }
    
    link->space1 = id1;
    link->space2 = id2;

    return OK;
}

Status link_set_is_adjacent(Link *link, bool status){
    if(link == NULL){
        debug_log(LOG_ERROR, "Null Link* argument: at link_set_is_adjacent(Link*, bool) in link.c");
        return ERROR;
    }

    link->adjacent = status;

    return OK;
}

Status link_set_locked(Link* link, bool status){
    if(link == NULL){
        debug_log(LOG_ERROR, "Null Link* argument: at link_set_locked(Link*, bool) in link.c");
        return ERROR;
    }

    link->locked = status;

    return OK;
}

Status link_set_unlocking_object(Link *link, Id object){
    if(link == NULL){
        debug_log(LOG_ERROR, "Null Link* argument: at link_set_unlocking_object(Link*, Id) in link.c");
        return ERROR;
    }

    link->unlockingObject = object;

    return OK;
}

Id link_get_id(Link *link){
    if(link == NULL){
        return NO_ID;
    }
    return link->id;
}

Id link_get_space1(Link *link){
    if(link == NULL){
        return NO_ID;
    }
    return link->space1;
}

Id link_get_space2(Link *link){
    if(link == NULL){
        return NO_ID;
    }
    return link->space2;
}

bool link_is_adjacent(Link *link){
    if(link == NULL){
        return false;
    }
    return link->adjacent;
}

bool link_is_locked(Link *link){
    if(link == NULL){
        return false;
    }
    return link->locked;
}

Id link_get_unlocking_object(Link *link){
    if(link == NULL){
        return NO_ID;
    }
    return link->unlockingObject;
}

Status link_move_entity(Link *link, Entity *entity){
    Id space = entity_get_location(entity);
    
    if(link == NULL || entity == NULL){
        debug_log(LOG_ERROR, "Null argument reference: at link_move_entity(Link*, Entity*) in link.c");
        return ERROR;
    }

    /*Comprobacion de si la entidad está en uno de los espacios*/
    
    if(!link_is_entity_on_valid_spaces(link, entity)){
        debug_log(LOG_WARNING, "Entity is not in a spaces connected by the link: at link_move_entity(Link*, Entity*) in link.c");
        return ERROR;
    }

    /*mover entidad*/

    if(space == link_get_space1(link)){
        /*caso en el que la entidad se encuentra en el espacio 1*/
        if(link_get_space2(link) <= UNDEFINED_ID){
            /*Gets random space from game*/
        }else{
            if(entity_set_location(entity, link_get_space2(link)) == ERROR){
                debug_log(LOG_WARNING, "Couldn't move entity: at link_move_entity(Link*, Entity*) in link.c");
                return ERROR;
            }
            debug_log(PRINT,"Moved entity %ld from space %ld to %ld", entity_get_id(entity), link_get_space1(link), link_get_space2(link));
        }
    }else{
        /*caso en el que la entidad se encuentra en el espacio 2*/
        if(link_get_space1(link) <= UNDEFINED_ID){
            /*Gets random space from game*/
        }
        else{
            if(entity_set_location(entity, link_get_space1(link)) == ERROR){
                debug_log(LOG_WARNING, "Couldn't move entity: at link_move_entity(Link*, Entity*) in link.c");
                return ERROR;
            }
            debug_log(PRINT,"Moved entity %ld from space %ld to %ld", entity_get_id(entity), link_get_space2(link), link_get_space1(link));
        }
    }

    return OK;
}

