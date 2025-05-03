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
#include "message.h"
#include "combat.h"
#include "ability_manager.h"
#include "libscreen.h"
#include "dialogue.h"
#include "effect.h"
#include "graphic_description.h"

#include <stdbool.h>

#define MAX_SPACES 100  /*!< Maximum number of spaces on the map */
#define MAX_LINKS (MAX_SPACES * 6)   /*!< Maximum number of links on the map */
#define MAX_PLAYERS 4   /*!< Maximum number of players*/

#define MAX_PROCEDURAL_SIZE 50

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
 * @brief Gets a spaces from game by its id
 * @author Profesores PPROG
 *
 * @param game struct that saves all information related to the game
 * @param id long that saves the location of a space
 * @return *space if everything goes well or NULL if there was some mistake
 */
Space *game_get_space(Game *game, Id id);

/**
 * @brief Gets a space from the game by it's spatial position
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @param pos vector with the position where the space is located
 * @param block number of the block to search
 * @return Space* 
 */
Space *game_get_space_by_position(Game *game, Vector2 pos, int block);

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
 * @author Maksym Polyak
 *
 * @param game struct that saves all information related to the game
 * @return player pointer from game or NULL if there was a mistake
 */
Player* game_get_player(Game *game);

/**
 * @brief Gets the number of players
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @return number of players, -1 if error
 */
int game_get_n_players(Game *game);

/**
 * @brief Gets pointer to player at certain index
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @param index index where the player is located
 * @return Player* 
 */
Player* game_get_player_at(Game *game, int index);

/**
 * @brief Gets a frame color based on the current player
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @return Frame_color or BLUE if error
 */
Frame_color game_get_player_color(Game *game);

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
 * @param game game struct
 * @return long or -1 if error
 */
long game_get_n_links(Game *game);

/**
 * @brief Trys to get the reference of a link by its Id
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @param id id of the link
 * @return the pointer if found, NULL if not found
 */
Link *game_get_link_by_id(Game *game, Id id);

/**
 * @brief Gets the current state of the game
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @return GameState or ERROR_STATE if error
 */
GameState game_get_state(Game *game);

/**
 * @brief Gets the event manager from game
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @return EventManager* or NULL if error
 */
EventManager *game_get_event_manager(Game *game);

/**
 * @brief Gets the npcs collection from game
 * 
 * @param game game struct
 * @return Collection* or NULL if error
 */
Collection *game_get_npcs(Game *game);

/**
 * @brief Gets player by id
 * @author Aaron Charameli Mair
 * 
 * @param game 
 * @param id 
 * @return Player* 
 */
Player *game_get_player_by_id(Game *game, Id id);

/**
 * @brief it gets the status of god mode
 * @author Aaron Charameli Mair
 * 
 * @param game game struct
 * @return true if god mode is active
 * @return false if god mode is not active or error
 */
bool game_get_god_mode(Game *game);

/**
 * @brief Gets if the turn was valid or not
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @return VALID or NOT_VALID
 */
TurnValidation game_get_is_turn_valid(Game *game);

/**
 * @brief Gets if the game was generated proceduraly
 * @author Daniel Gómez
 * 
 * @param game 
 * @return true 
 * @return false 
 */
bool game_get_is_procedural(Game *game);

/**
 * @brief Gets a graphic description by id
 * @author Daniel Gómez
 * 
 * @param game 
 * @param id 
 * @return GDesc* 
 */
GDesc *game_get_gdesc_by_id(Game *game, Id id);

/**
 * @brief This function gets an effect by its id
 * @author Aaron Charameli Mair
 * 
 * @param game game struct
 * @param id id of the effect
 * @return Effect* or NULL if not found or ERROR
 */
Effect *game_get_effect_by_id(Game *game, Id id);

/**
 * @brief This function gets the effect manager of a game
 * @author Aaron Charameli Mair
 * 
 * @param game a pointer to game
 * @return EffectManager* or NULL if ERROR
 */
EffectManager *game_get_effect_manager(Game *game);

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
 * @param game game struct
 * @param state game state to set
 * @return Status 
 */
Status game_set_state(Game *game, GameState state);

/**
 * @brief It sets the value of god mode
 * @author Aaron Charameli Mair
 * 
 * @param game game struct
 * @param value value of god mode to set
 * @return Status 
 */
Status game_set_godmode(Game *game, bool value);

/**
 * @brief Sets if the turn was valid or not
 * 
 * @param game game struct
 * @param value VALID or NOT_VALID
 * @return Status
 */
Status game_set_is_turn_valid(Game *game, TurnValidation value);

/*----------OTHERS----------*/

/**
 * @brief Assigns a position to all the spaces based on the first space added to the game
 *          Requires that the first spaces is adjascent to all the other ones, if not it wont work as intended
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @return Ok if everything went ok, ERROR if couldn't execute all
 */
Status game_spatial_map(Game *game);

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
 * @param game game struct
 * @param link link to add
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
 * @param game game struct
 * @param event event to add
 * @return Status 
 */
Status game_add_event(Game *game, Event *event);

/**
 * @brief Adds an NPC struct to the game struct
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @param npc npc to add
 * @return Status 
 */
Status game_add_npc(Game *game, NPC *npc);

/**
 * @brief Adds a message to the queue. Internaly memory will be allocated to store a copy of message
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @param type type of the message to add
 * @param message message to add to the queue
 * @return Status 
 */
Status game_add_log_message(Game *game, MessageType type,char *message);

/**
 * @brief This function adds an effect to the game
 * @author Aaron Charameli Mair
 * 
 * @param game game struct
 * @param effect effect to add
 * @return Status 
 */
Status game_add_effect(Game *game, Effect *effect);

/**
 * @brief Gets the first message in the queue and frees its memory
 * @author Daniel Gómez
 * 
 * @param game game struct
 * @param str string where the log message is saved
 * @return char* or NULL if error
 */
Status game_get_log_message(Game *game, char *str);

/**
 * @brief Returns if there are messages to display or not
 * @author Daniel Gómez
 * 
 * @param game 
 * @return true if log has a message
 * @return false if it does not have message or error
 */
bool game_log_hasMessage(Game *game);

/**
 * @brief Initializes and starts combat(changes game state and creates combat module)
 * @author Daniel Gómez y Maksym Polyak
 * 
 * @param game game struct
 * @return Status 
 */
Status game_combat_start(Game *game);

/**
 * @brief Ends combat(changes game state and creates combat module)
 * @author Daniel Gómez y Maksym Polyak
 * 
 * @param game game struct
 * @return Status 
 */
Status game_combat_end(Game *game);

/**
 * @brief Returns the combat reference stored in game
 * 
 * @param game game struct
 * @return Combat* or NULL if error
 */
Combat *game_get_combat(Game *game);

/**
 * @brief Switchs to the indicated player on the array
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @param player player index to switch
 * @return int -2 if all dead, -1 if not found or 0 if ok
 */
int game_switch_player(Game *game, int player);

/**
 * @brief Gets the ability manager struct from the game struct
 * 
 * @param game game struct
 * @return AbilityManager* or NULL if error
 */
AbilityManager *game_get_ability_manager(Game *game);

/**
 * @brief Gets a player by its id on game struct
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @param id id of the player
 * @return Player* or NULL if error or not found
 */
Player *game_get_player_by_id(Game *game, Id id);

/**
 * @brief Gets a NPC by its id on game struct
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @param id id of the NPC
 * @return NPC* or NULL if error or not found
 */
NPC *game_get_NPC_by_id(Game *game, Id id);

/**
 * @brief Gets an object from the object array by its id
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @param objectid id of the object to search
 * @return Object* or NULL if error or not found
 */
Object *game_get_object_by_id(Game *game, Id objectid);

/**
 * @brief Adds the ability to the ability manager and to the related entity
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @param ability ability to add
 * @return Status 
 */
Status game_add_ability(Game *game, Ability *ability);

/**
 * @brief Initializes a dialogue with an NPC
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @param npc npc struct
 * @return Status 
 */
Status game_dialogue_init(Game *game, NPC *npc);

/**
 * @brief Gets the dialogue struct from game
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @return Dialogue * or NULL if error
 */
Dialogue *game_get_dialogue(Game *game);

/**
 * @brief Frees dialogue struct and sets game state to default
 * @author Maksym Polyak
 * 
 * @param game game struct
 * @return Status 
 */
Status game_end_dialogue(Game *game);
/**
 * @brief Adds a graphic description to the game
 * @author Daniel Gómez
 * 
 * @param game 
 * @param gdesc 
 * @return Status 
 */
Status game_add_gdesc(Game *game, GDesc *gdesc);

/**
 * @brief Gets the collection of attacks
 * @author Sofía Calvo
 * 
 * @param game game struct
 * @return Collection
 */
Collection *game_get_attacks(Game *game);


/**
 * @brief Generates a level proceduraly
 * @author Daniel Gómez
 * 
 * @param game 
 * @return Status 
 */
Status game_generate_procedural(Game *game);

#endif
