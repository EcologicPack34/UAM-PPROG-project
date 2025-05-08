/**
 * @file store.h
 * @author Maksym Polyak
 * @brief 
 * @version 0.1
 * @date 2025-05-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef STORE_H
#define STORE_H

#include "collection.h"
#include "types.h"

/**
 * @brief Store ADT definition
 * 
 */
typedef struct _Store Store;

/**
 * @brief Creates a store struct
 * @author Maksym Polyak
 * 
 * @param type type of the store
 * @param seller seller where objects are located
 * @param buyer where the objects are dropped after buy
 * @param money where the money is taken
 * @return Store* 
 */
Store *store_create(StoreType type, void *seller, void *buyer, int *money);

/**
 * @brief Frees a store struct
 * @author Maksym Polyak
 * 
 * @param store store struct
 */
void store_destroy(Store *store);

/**
 * @brief Adds an item to the store
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @param element element to sell
 * @param cost cost of the element
 * @param id of the object
 * @return Status 
 */
Status store_add_item(Store *store, void *element, int cost, Id id);

/**
 * @brief Removes an item from the store
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @param element element to add
 * @return void * of the element removed 
 */
void *store_remove_item(Store *store, Id id);

/**
 * @brief Gets the item that is being selled at the index
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @param i index
 * @return void * with the element 
 */
void *store_get_item_element_at(Store *store, int i);

/**
 * @brief Gets the cost of the item at the index
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @param i index
 * @return int with the cost 
 */
int store_get_item_cost_at(Store *store, int i);

/**
 * @brief Removes an item on the store at the index
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @param i index
 * @return Status 
 */
Status store_remove_item_at(Store *store, int i);

/**
 * @brief Gets the num of items on the store
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @return int or -1 if error
 */
int store_get_size(Store *store);

/**
 * @brief Gets the seller
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @return void* 
 */
void *store_get_seller(Store *store);

/**
 * @brief Gets the client
 * @author Maksym Polyak
 * 
 * @param store store struct
 * @return void* 
 */
void *store_get_client(Store *store);

#endif