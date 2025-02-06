/**
 * @file Collector.h
 * @author Daniel Gómez Rodríguez
 * @brief 
 *      This library tries to make memory managment easier, 
 *      keeping track of all pointers in a single place 
 *      so they can be freed at any given moment in an easy way.
 * @version 1.1.2
 * @date 2025-01-30
 * 
 * @copyright GNU Public License
 * 
 **/
#ifndef COLLECTOR_H

#define COLLECTOR_INITIAL_SIZE 10

#define COLLECTOR_H
/* 
    The use of a global Collector variable is recommended to keep track throughout different .c files in an easy way.
*/

/**
 * @brief Collector
 * 
 * Stores necesary information to collect pointers
 */
typedef struct Collector{
    void **pointerList;        /*!< Dynamic void* array of memory location of pointers that are being tracked */ 
    unsigned long amount;      /*!< long: Current number of pointers store in pointerList*/
    unsigned long usedMem;     /*!< long: Current reserved memory for the dynamic array*/
}Collector;

/**
 * @brief Tries to initialize a collector. Returns 1 if done correctly, 0 if not.
 * 
 * @param collector 
 * @return short 
 */
short c_initializeCollector(Collector *collector);

/**
 * @brief Adds a pointer to the list to keep track to the collector. Returns 0 if correct, 1 if there is an error in the parameters, 2 if size reallocation fails.
 * 
 * @param collector 
 * @param pointer 
 * @return short 
 */
short c_addPointerToCollector(Collector *collector, void *pointer);

/**
 * @brief Tries to remove a given pointer from a collector. If not posible it will return 0.
 * 
 * @param collector 
 * @param pointer 
 * @return short 
 */
short c_removeCollectorPointer(Collector *collector, void *pointer);

/**
 * @brief Allocates memory for a pointer and stores that pointer in the given collector.
 * 
 * @param collector 
 * @param size 
 * @return void* 
 */
void *c_malloc(Collector *collector, size_t size);

/**
 * @brief Allocates memory, sets it to 0 and stores the pointer in the given collector.
 * 
 * @param collector 
 * @param numOfElements 
 * @param sizeOfElements 
 * @return void* 
 */
void *c_calloc(Collector* collector, size_t numOfElements ,size_t sizeOfElements);

/**
 * @brief Reallocates memory for a given pointer and changes the pointer value in the collector.
 * 
 * @param collector 
 * @param element 
 * @param newSize 
 * @return void* 
 */
void *c_realloc(Collector *collector, void *element, size_t newSize);

/**
 * @brief Frees a pointer and tries to remove it from a given collector.
 * 
 * @param collector 
 * @param pointer 
 * @return short 
 */
short c_free(Collector *collector, void *pointer);

/**
 * @brief Frees the memory used by the pointers in the collector. 
 * 
 * @param collector 
 * @param stopCollector If true, then the collector will free itself.
 * @return short 
 */
short c_freeCollector(Collector *collector, short stopCollector);
#endif