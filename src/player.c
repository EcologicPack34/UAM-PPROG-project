/**
 * @brief It implements the player module
 * 
 * A player ADT for now is just a struct with an entity ADT inside, in the next iterations this module
 * will be improved to add further functionality in game.
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
#include "equipment.h"

/**
 * @brief Player
 *
 * This struct stores all the information of a player
 */
struct _Player {
    Entity *entity;                     /*!< Entity type of the player */

    NPC *followers[NPC_MAX_ALLIES - 1]; /*!< Pointer with all the actual followers allies*/

    Equipment *equipment;               /*!< Equipment of the player*/

    int money;                          /*!< Money quantity of the player*/

    CommandInfo *cmdData;               /*!< Stores the local data of the command in the player*/
};


/**
   Private functions
*/
/**
 * @brief Sets the entity pointer of the player
 * @author Maksym Polyak
 *
 * @param player struct with the information of a player
 * @param entity entity of the player
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
    int i;

    if(!(player = (Player *)malloc(sizeof(Player))))
        return NULL;
    
    /*by default, all stats are set to lvl 1*/
    if(player_set_entity(player,entity_create(name, identity, location, PLAYER_INVENTORY)) == ERROR){
        free(player);
        return NULL;
    }

    if((player->equipment = equipment_create()) == ERROR){
        entity_destroy(player->entity);
        free(player);
    }

    player->money = 0;

    for(i = 0; i < NPC_MAX_FOLLOWERS; i++){
        player->followers[i] = NULL;
    }

    player->cmdData = command_info_create();
    if(!(player->cmdData)){
        entity_destroy(player->entity);
        equipment_destroy(player->equipment);
        free(player);
    }
    
    return player;
}

void player_destroy(Player *player){
    if(!player)
        return;

    entity_destroy(player_get_entity(player));
    equipment_destroy(player->equipment);
    command_info_destroy(player->cmdData);
    free(player);
}

/*Player GETTERS*/

Entity *player_get_entity(Player *player){
    if (!player)
        return NULL;

    return player->entity;
}

Equipment *player_get_equipment(Player *player){
    if (!player)
        return NULL;

    return player->equipment;
}

Status player_get_str_desc(Player *player, char *str){
    Entity *ent = NULL;
    
    if(!player || !str)
        return ERROR;

    ent = player_get_entity(player);
    sprintf(str, "%s (%s): H:%.1lf/%.1lf,L:%ld (%ld)", entity_get_graphic_description(ent), entity_get_name(ent), entity_get_health(ent), entity_get_max_health(ent),entity_get_location(ent), entity_get_id(ent));

    return OK;
}

int player_get_money(Player *player){
    if(!player) return -1;

    return player->money;
}

CommandInfo *player_get_cmdData(Player *player){
    if(!player) return NULL;
    return player->cmdData;
}

/*Player SETTERS*/

Status player_add_money(Player *player, int value_added){
    if(!player) return ERROR;

    if(player->money + value_added > 0)
        player->money += value_added;
    else
        player->money = 0;

    return OK;
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

Status player_set_stats(Player *p, double maxhealth, double health, double baseDamage, int strength, int defense, int magicLevel){
    if(!p) return ERROR;

    return entity_set_stats(p->entity, maxhealth, health, baseDamage, strength, defense, magicLevel);
}

Status player_equip_piece(Player *player, Object *object){
    Entity *entity = NULL;
    Inventory *inventory = NULL;

    if(!player || !object) return ERROR;

    entity = player_get_entity(player);
    inventory = entity_get_inventory(entity);

    if(inventory_contains_object(inventory, object_get_id(object)) == false)
        return ERROR;

    if(equipment_add_piece(entity, player->equipment, object) == ERROR)
        return ERROR;

    inventory_remove_object(inventory, object);

    return OK;
}

Status player_unequip_piece(Player *player, char *data){
    Entity *entity = NULL;
    Inventory *inventory = NULL;
    Object *object = NULL;

    if(!player) return ERROR;

    entity = player_get_entity(player);
    inventory = entity_get_inventory(entity);

    if(inventory_get_size(inventory) == INVENTORY_PLAYER_MAX_SIZE)
        return ERROR;

    object = equipment_remove_piece(entity, player->equipment, data);
    if(object == NULL)
        return ERROR;

    inventory_add_object(inventory, object);

    return OK;
}

Status player_add_follower(Player *player, NPC *npc){
    int i, index = -1;

    if(!player || !npc || (npc_get_can_follow(npc) == false)) return ERROR;

    if(npc_get_player_following_id(npc) != NO_ID)
        return ERROR;

    /*Checks if its already a follower and saves the first NULL index*/
    for(i = 0; i < NPC_MAX_FOLLOWERS; i++){
        if(npc_cmp(npc, player->followers[i]) == 0)
            return OK;
        if(player->followers[i] == NULL)
            index = i;
    }

    /*If an empty pointer wasn't found then followers are full*/
    if(index == -1) return ERROR;

    npc_set_player_following_id(npc, entity_get_id(player_get_entity(player)));

    player->followers[index] = npc;

    return OK;
}

Status player_remove_follower_by_name(Player *player, char *npc_name){
    int i;

    if(!player || !npc_name) return ERROR;

    /*Searches by name and removes it*/
    for(i = 0; i < NPC_MAX_FOLLOWERS; i++){
        if(player->followers[i] != NULL && strcmp(entity_get_name(npc_get_entity(player->followers[i])), npc_name) == 0){
            npc_set_player_following_id(player->followers[i], NO_ID);
            player->followers[i] = NULL;
            return OK;
        }
    }

    /*If not found then its removed*/
    return ERROR;
}

NPC **player_get_followers(Player *player){
    
    if(!player) return NULL;

    return player->followers;
}