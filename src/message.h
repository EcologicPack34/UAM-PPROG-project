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

typedef enum {MESSAGE_EMPTY ,MESSAGE_LOG, MESSAGE_ERROR, MESSAGE_NPC}MessageType;

typedef struct _Message Message;

Message *message_new(MessageType type, char *message);

void message_destroy(void *message);

/*----------GETTERS----------*/

void message_get_str(Message *message, char *dest);

#endif
