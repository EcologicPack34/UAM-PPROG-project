/**
 * @brief It defines the game interface
 *
 * It is the second highest level module, controls internal functionality of the game,
 * uses the other modules to create the map, place the player and the object, but also keeps
 * track of the spaces, the links and the object, also contians the last command and a bool
 * to determine if the game has finished or not.
 * 
 * @file game.h
 * @author Original: Profesores PPROG. Modified by: Daniel Gómez and Maksym Polyak
 * @version 1
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef GAME_H
#define GAME_H

#include "link.h"
#include "space.h"
#include "command.h"
#include "types.h"
#include "player.h"
#include "collection.h"
#include "event_manager.h"
#include "npc.h"

#include <stdbool.h>

#define MAX_SPACES 100          /*!< Maximum number of spaces on the map */
#define MAX_LINKS 400   /*!< Maximum number of links on the map */

typedef enum {ERROR_STATE ,DEFAULT, COMBAT, INVENTORY, DIALOGUE}GameState;

/**
 * @brief Game struct, defines all the information of the game
 */
typedef struct _Game Game;

/**
 * @brief Initializes game struct to the corresponding 0
 * @author Original:Profesores PPROG, Modified by: Daniel Gómez
 *
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_create(Game **game);

/**
 * @brief Frees corresponding memory from spaces and frees command
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_destroy(Game *game);

/**
 * @brief Prints the ingame screen
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 */
void game_print(Game *game);

/**
 * @brief Converts Game structure into a string that can be later used in, for example, saving the game
 * 
 * @param game Struct to be converted to string
 * @return todavia no se como implementarlo
 */
char* game_to_string(Game * game);

/*----------GETTERS----------*/

/**
 * @brief 
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id long that saves the location of a space
 * @return *space if everything goes well or NULL if there was some mistake
 */
Space *game_get_space(Game *game, Id id);

/**
 * @brief Checks the game->spaces[position] and returns the ID of that position on the array
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param position int that saves position in the array of spaces of the struct game
 * @return Id of the position if everything goes well or NO_ID if there was some mistake
 */
Id game_get_space_id_at(Game *game, int position);

/**
 * @brief Gets the total number of spaces on game
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return n_spaces if everything goes well or -1 if there was a mistake
 */
int game_get_n_spaces(Game *game);

/**
 * @brief Gets the player pointer of the game struct
 * @author maksym Polyak
 *
 * @param game struct that saves all information related to the game
 * @return player pointer from game or NULL if there was a mistake
 */
Player* game_get_player(Game *game);

/**
 * @brief Gets the collection pointer with the objects of the game struct
 * @author Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 * @return object pointer from game or NULL if there was a mistake
 */
Collection* game_get_objects(Game *game);

/**
 * @brief Gets the last command from the game struct
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return last command
 */
Command* game_get_last_command(Game *game);

/**
 * @brief Gets the game state
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return state of game (finished if 1, not finished if 0)
 */
bool game_get_finished(Game *game);

/**
 * @brief Gets the player location
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @return player location id or -1 if there was a mistake
 */
Id game_get_player_location(Game *game);

/**
 * @brief Gets the numer of links stored in a game
 * @author Daniel Gómez
 * 
 * @param game 
 * @return long 
 */
long game_get_n_links(Game *game);

/**
 * @brief Trys to get the reference of a link by its Id
 * @author Daniel Gómez
 * 
 * @param game 
 * @return the pointer if found, NULL if not found
 */
Link *game_get_link_by_id(Game *game, Id id);

/**
 * @brief Gets the current state of the game
 * @author Daniel Gómez
 * 
 * @param game 
 * @return GameState 
 */
GameState game_get_state(Game *game);

/**
 * @brief Gets the event manager from game
 * @author Daniel Gómez
 * 
 * @param game 
 * @return EventManager* 
 */
EventManager *game_get_event_manager(Game *game);

/**
 * @brief Gets the npcs collection from game
 * 
 * @param game 
 * @return Collection* or NULL if error
 */
Collection *game_get_npcs(Game *game);



/*----------SETTERS----------*/


/**
 * @brief Sets the received command on the game struct
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param command struct that saves all information related to the command
 * @return OK if everything goes well
 */
Status game_set_last_command(Game *game, Command *command);

/**
 * @brief Sets the game state to the finished value
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param finished bool that defines the game state
 * @return OK if everything goes well
 */
Status game_set_finished(Game *game, bool finished);

/**
 * @brief Sets the player location
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id id with the location of the player
 * @return OK if everything went well or ERROR if there was a mistake
 */
Status game_set_player_location(Game *game, Id id);

/**
 * @brief Sets the current game state
 * @author Daniel Gómez
 * 
 * @param game 
 * @param state 
 * @return Status 
 */
Status game_set_state(Game *game, GameState state);

/*----------OTHERS----------*/

/**
 * @brief Adds the space received to the array of spaces on game
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param space struct that saves all information related to the space
 * @return OK if everything goes well or ERROR if there was some mistake
 */
Status game_add_space(Game *game, Space *space);

/**
 * @brief Adds the link received to the array of links on game
 * @author Daniel Gómez
 * 
 * @param game 
 * @param link 
 * @return Status 
 */
Status game_add_link(Game *game, Link *link);

/**
 * @brief Adds the object received to the collection of objects on game
 * @author Maksym Polyak
 * 
 * @param game struct that saves all information related to the game
 * @param object struct that saves all information related to the object
 * @return Status 
 */
Status game_add_object(Game *game, Object *object);

/**
 * @brief Adds a player to the game struct
 * @author Maksym Polyak
 * 
 * @param game struct that saves all information related to the game
 * @param player struct that saves all information related to a player
 * @return Status 
 */
Status game_add_player(Game *game, Player *player);

/**
 * @brief Adds an event to game
 * @author Daniel Gómez
 * 
 * @param game 
 * @param event 
 * @return Status 
 */
Status game_add_event(Game *game, Event *event);

#endif
