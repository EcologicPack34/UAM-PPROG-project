#include "inventory.h"

#include <stdio.h>
#include <stdlib.h>

int main(void){
    Inventory *inv1 = NULL, *inv2 = NULL;
    Object *object1 = NULL, *object2 = NULL;

    inv1 = inventory_create(INVENTORY_PLAYER_MAX_SIZE, 10);
    if(!inv1){
        printf("ERR1\n");
        return 1;
    }

    inv2 = inventory_create(INVENTORY_SPACE_MAX_SIZE, 11);
    if(!inv2){
        printf("ERR2\n");
        return 1;
    }

    object1 = object_create(1,"OBJETO1", PLAYER_INVENTORY);
    if(!object1){
        printf("ERR3\n");
        return 1;
    }
    object2 = object_create(2,"OBJETO2", SPACE_INVENTORY);
    if(!object2){
        printf("ERR4\n");
        return 1;
    }
    
    if(inventory_add_object(inv1, object1) == ERROR){
        printf("ERR5\n");
        return 1;
    }

    if(inventory_add_object(inv2, object2) == ERROR){
        printf("ERR6\n");
        return 1;
    }

    if(inventory_move_object(inv2, inv1, 2) == ERROR){
        printf("ERR7\n");
        return 1;
    }

    object_print(inventory_get_object_by_id(inv1, 1));
    object_print(inventory_get_object_by_name(inv1,"OBJETO2"));

    inventory_destroy(inv1);
    inventory_destroy(inv2);

    object_destroy(object1);
    object_destroy(object2);

    return 0;
}