#include <stdio.h>
#include <stdlib.h>
#include "collector.h"

short c_initializeCollector(Collector * collector){
    if(!collector)
        return 1;
    collector->amount = 0;
    collector->pointerList = (void **)calloc(COLLECTOR_INITIAL_SIZE , sizeof(void *));
    if(!collector->pointerList)
        return 1;
    collector->usedMem = COLLECTOR_INITIAL_SIZE;

    return 0;
}

short c_addPointerToCollector(Collector* collector, void* pointer){
    void **aux;
    if(!collector)
        return 1;
    if(collector->amount >= collector->usedMem){
        aux = (void**)realloc(collector->pointerList, collector->usedMem * 2);
        if(!aux)
            return 2;
        collector->pointerList = aux;
    }
    collector->pointerList[collector->amount++] = pointer;
    return 0;
}

short c_removeCollectorPointer(Collector* collector, void *pointer){
    short found = 0;
    int i = 0;

    if(!collector || !pointer)
        return 1;

    for (i = 0; i < collector->usedMem; i++)
    {
            if(found){
                collector->pointerList[i - 1] = collector->pointerList[i];
            }

            if(collector->pointerList[i] == pointer)
                found = 1;
    }
    if(found)
    {
        collector->amount--;
        collector->pointerList[collector->usedMem] = NULL;
    }

    return 0;
}

void *c_malloc(Collector* collector, size_t size){
    void *p;

    if(!collector){
        return NULL;
    }
    p = malloc(size);
    if(p){
        c_addPointerToCollector(collector, p);
    }
    return p;
}

void *c_calloc(Collector* collector, size_t numOfElements ,size_t sizeOfElements){
    void *p;
    if(!collector)
        return NULL;
    p = calloc(numOfElements,sizeOfElements);
    if(p)
        c_addPointerToCollector(collector, p);
    return p;
}

void *c_realloc(Collector *collector, void* element ,size_t newSize){
    int i = 0;
    void *p;

    if(!element)
        return NULL;
    if(!collector)
        return element;

    p = realloc(element, newSize);

    if(element != p && p){
        for (i = 0; i < collector->amount; i++)
        {
            if(collector->pointerList[i] == element)
                collector->pointerList[i] = p;
        }
    }
    return p;
}

short c_free(Collector* collector, void* pointer){

    if(!collector || !pointer)
        return 1;

    c_removeCollectorPointer(collector, pointer);
    free(pointer);
    return 0;
}

short c_freeCollector(Collector* collector, short stopCollector){
    int i = 0;
    if(!collector)
        return 1;

    for (i = 0; i < collector->amount; i++)
    {
        if(collector->pointerList[i])
        {
            free((collector->pointerList[i]));
            collector->pointerList[i] = NULL;
        }
    }
    
    if(stopCollector){
        free((collector->pointerList));
        collector->pointerList = NULL;
    }

    return 0;
}