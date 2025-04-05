/**
 * @file message.h
 * @author Daniel Gómez
 * @brief ADT Containing the information about a game message/log to screen
 * @version 0.1
 * @date 2025-03-11
 * 
 * @copyright GNU Public License
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#define MESSAGE_TYPE_COUNT 7 /*!< Number of message types implemented*/

/**
 * @brief Typedef for enum containing the different types of messages
 */
typedef enum {MESSAGE_EMPTY ,MESSAGE_LOG, MESSAGE_ERROR, MESSAGE_NPC, MESSAGE_PLAYER_LIST, MESSAGE_HELP, MESSAGE_INSPECT}MessageType;

/**
 * @brief ADT to carry messages to the graphic engine
 */
typedef struct _Message Message;

/**
 * @brief Creates a new message by allocating new memory for the string(i.e. it makes a copy of the string)
 * @author Daniel Gómez
 * 
 * @param type type of the message
 * @param message string with the information
 * @return Message* 
 */
Message *message_new(MessageType type, char *message);

/**
 * @brief Frees the memory in use by a message
 * @author Daniel Gómez
 * 
 * @param message message struct
 */
void message_destroy(void *message);

/*----------GETTERS----------*/

/**
 * @brief Copies the message to a given string
 * 
 * @param message message struct
 * @param dest destination string, must have enough size
 */
void message_get_str(Message *message, char *dest);

#endif
