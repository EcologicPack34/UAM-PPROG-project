/**
 * @file store.c
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-05-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "store.h"
#include <stdio.h>

/**
 * @brief Internal item definition
 * 
 */
typedef struct {
    void *item;     /*!< Element that is being sold*/
    int cost;       /*!< Cost of the item*/
    Id id;          /*!< Representative id of the item*/
}_Item;

/**
 * @brief Internal store ADT implementation
 * 
 */
struct _Store{
    Collection *items;      /*!< Collection with the items to sell*/
    StoreType type;         /*!< Defines what the store is selling*/

    void *seller;           /*!< Where the items are located*/
    void *client;           /*!< Where items are deposited after buy*/
    int *money;             /*!< Where the money is taken*/
};

/*
    * PRIVATE Functions
*/

/**
 * @brief Creates an internal item struct
 * @author Maksym Polyak
 * 
 * @param element element that the item represents
 * @param cost cost of the item
 * @param id id to identify object
 * @return _Item* 
 */
_Item *_item_create(void *element, int cost, Id id);

/**
 * @brief Destroys an item struct
 * @author Maksym Polyak
 * 
 * @param item item struct
 */
void _item_destroy(_Item *item);

/**
 * @brief Compares two items ids
 * @author Maksym Polyak
 * 
 * @param ele1 item 1
 * @param ele2 item 2
 * @return int > 0 if id1 > id2 or int < 0 if id1 < id2 or int = 0 if id1 ==id2
 */
int _item_cmp(void *ele1, void *ele2);

/**
 * @brief Returns the item struct at the index
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @param i index
 * @return _Item* 
 */
_Item *_store_search_item_by_index(Store *store, int i);

_Item *_item_create(void *element, int cost, Id id){
    _Item *item = NULL;

    if(!element || cost < 0) return NULL;

    item = (_Item *)malloc(sizeof(_Item));
    if(!item) return NULL;

    item->item = element;
    item->id = id;
    item->cost = cost;

    return item;
}

void _item_destroy(_Item *item){
    if(item) free(item);
}

int _item_cmp(void *ele1, void *ele2){
    _Item *item1 = NULL, *item2 = NULL;

    if(!ele1 || !ele2) return 0;

    item1 = (_Item *)ele1;
    item2 = (_Item *)ele2;

    return item1->id - item2->id;
}

_Item *_store_search_item_by_index(Store *store, int i){
    
    if(!store) return NULL;

    return collection_get_element_at(store->items, i);
}

/*
    * PUBLIC Functions
*/

Store *store_create(StoreType type, void *seller, void *buyer, int *money){
    Store *store = NULL;

    if(type < 0 || type >= STORETYPE_ENUM_NUM) return NULL;

    store = (Store *)malloc(sizeof(Store));
    if(!store) return NULL;

    store->items = collection_create(10, false, true, _item_cmp, NULL);
    if(!store->items){
        free(store);
        return NULL;
    }

    store->seller = seller;
    store->client = buyer;
    store->money = money;
    store->type = type;

    return store;
}

void store_destroy(Store *store){
    if(store){
        collection_destroy(store->items);
        free(store);
    }
}

Status store_add_item(Store *store, void *element, int cost, Id id){
    if(!element || cost < 0) return ERROR;

    return collection_add(store->items, _item_create(element, cost, id));
}

void *store_remove_item(Store *store, Id id);

void *store_get_item_element_at(Store *store, int i){
    _Item *item = NULL;

    if(!store) return ERROR;

    item = _store_search_item_by_index(store, i);

    return item->item;
}

int store_get_item_cost_at(Store *store, int i){
    _Item *item = NULL;

    if(!store) return ERROR;

    item = _store_search_item_by_index(store, i);

    return item->cost;
}

Status store_remove_item_at(Store *store, int i){
    if(!store) return ERROR;

    return collection_remove_at(store->items, i);
}

int store_get_size(Store *store){
    if(!store) return -1;

    return collection_length(store->items);
}

void *store_get_seller(Store *store){
    if(!store) return NULL;

    return store->seller;
}

void *store_get_client(Store *store){
    if(!store) return NULL;

    return store->client;
}

int *store_get_money(Store *store){
    if(!store) return NULL;

    return store->money;
}