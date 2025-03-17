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

#define MAX_MESSAGE_LENGTH 100
#define MESSAGE_TYPE_COUNT 4

/*Typedef for enum containing the different types of messages*/
typedef enum {MESSAGE_EMPTY ,MESSAGE_LOG, MESSAGE_ERROR, MESSAGE_NPC}MessageType;

/*
    Typedef for Message struct
*/
typedef struct _Message Message;

/**
 * @brief Creates a new message by allocating new memory for the string(i.e. it makes a copy of the string)
 * @author Daniel Gómez
 * 
 * @param type 
 * @param message 
 * @return Message* 
 */
Message *message_new(MessageType type, char *message);

/**
 * @brief Frees the memory in use by a message
 * @author Daniel Gómez
 * 
 * @param message 
 */
void message_destroy(void *message);

/*----------GETTERS----------*/

/**
 * @brief Copies the message to a given string
 * 
 * @param message 
 * @param dest destination string, must have enough size
 */
void message_get_str(Message *message, char *dest);

#endif
