#include "message.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Array of names for each message type
 */
char msg_type_to_name[MESSAGE_TYPE_COUNT][WORD_SIZE] = {"", "Log", "Error", "NPC", "Players", "Help", "Inspection"};

/**
 * @brief Internal struct of Message
 * 
 */
struct _Message{
    MessageType type;          /*!<Enum containing the message type*/
    char message[WORD_SIZE];   /*!<String containing the message*/
};


Message *message_new(MessageType type, char *message){
    Message *m = NULL;
    if(!message) return NULL;

    m = calloc(1, sizeof(Message));
    if(!m) return NULL;

    m->type = type;
    strcpy(m->message, message);

    return m;
}

void message_destroy(void *message){
    free(message);
}

/*----------GETTERS----------*/

void message_get_str(Message *message, char *dest){
    if(!message || !dest) return;

    strcpy(dest, msg_type_to_name[message->type]);
    strcat(dest, ": ");
    strcat(dest, message->message);
}
