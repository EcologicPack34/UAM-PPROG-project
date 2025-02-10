/**
 * @brief It implements the player module
 *
 * @file player.c
 * @author Maksym Polyak
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_NAME ""

/**
 * @brief Player
 *
 * This struct stores all the information of a player
 */
struct _Player {
    Entity *entity;           /*!< Entity type of the player */
};


/**
   Private functions
*/

/**
 * @brief Sets the entity pointer of the player
 * @author Profesores PPROG
 *
 * @param player struct with the information of a player
 * @return OK if everything went fine or NULL if there was a mistake
 */
Status player_set_entity(Player *player, Entity *entity){
    if (!player || !entity)
        return ERROR;


    player->entity = entity;

    return OK;
}

/**
   Game interface implementation
*/

Player *player_create(char *name, Id identity, Id location){
    Player *player = NULL;

    if(!(player = (Player *)malloc(sizeof(player))))
        return NULL;
    
    if(player_set_entity(player,entity_create(name, identity, location, PLAYER_INVENTORY)) == ERROR)
        return NULL;
    
    return player;
}

void player_destroy(Player *player){
    if(!player)
        return;

    entity_destroy(player_get_entity(player));
}

void player_print(Player *player){
    Entity *entityPlayer;
    if(!player)
        return;
    
    printf("\n\n-------------\n\n");

    printf("=> Player:  \n");
    
    entityPlayer = player_get_entity(player);
    printf("=> Player id: %d\n", (int)entity_get_id(entityPlayer));
    printf("=> Player name: %s\n", entity_get_name(entityPlayer));
    printf("=> Player location: %d\n", (int)entity_get_location(entityPlayer));
}

Entity *player_get_entity(Player *player){
    if (!player)
        return NULL;

    return player->entity;
}

