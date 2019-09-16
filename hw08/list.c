/**
 * CS 2110 - Fall 2018 - Homework #8
 *
 * @author Bojun Yang
 *
 * struct list.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return something (or return 0 if
 * the function returns a int).
 */

// Do not add ANY additional includes!!!
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED_PARAMETER(x) (void)(x)

/* The struct list_node struct. Has a next pointer, and data. */
/* DO NOT MODIFY THIS STRUCTURE */
/* Only this file should deal with the struct list_node struct */
struct list_node
{
    struct list_node *next;
    struct person *data;
};

/* You should NOT have any global variables. */

/* The create_node function is static because this is the only file that should
   have knowledge about the nodes backing the linked struct list. */
static inline struct list_node *create_node(struct person *data);

/** create_node
  *
  * Helper function that creates a struct list_node by allocating memory for it on the heap.
  * Be sure to set its next pointer to NULL.
  *
  * If malloc returns NULL, you should return NULL to indicate failure.
  *
  * @param data a void pointer to data the user wants to store in the struct list
  * @return a struct list_node
  */
static struct list_node* create_node(struct person *data)
{
    struct list_node *memPtr = (struct list_node*) malloc(sizeof(struct list_node));
    if (memPtr == NULL) {
        return NULL;
    }
    memPtr -> next = NULL;
    memPtr -> data = data;
    return memPtr;
    // UNUSED_PARAMETER(data);
    // return NULL;
}

/** create_list
  *
  * Creates a struct list by allocating memory for it on the heap.
  * Be sure to initialize size to zero and head and tail to NULL.
  *
  * If malloc returns NULL, you should return NULL to indicate failure.
  *
  * @return a pointer to a new struct list or NULL on failure
  */
struct list *create_list(void)
{
    struct list *listPtr = (struct list*) malloc(sizeof(struct list));
    if (listPtr == NULL) {
        return NULL;
    }
    listPtr -> head = NULL;
    listPtr -> tail = NULL;
    listPtr -> size = 0;
    return listPtr;
}

/** copy_list
  *
  * Create a new struct list structure, new nodes, and new copies of the data
  * by using the person_copy function (check out the description for this in
  * person.c).
  *
  * If listToCopy is NULL return NULL. For memory allocations
  * failures, including if person_copy returns 0 (indicating a memory
  * allocation failure), your code must not leak memory. This means that if any
  * memory allocation failures occur, before you return NULL, you must go back
  * and free all data in the new struct list, nodes in the new struct list, and
  * the new struct list itself (you may find the empty_list function useful for
  * this).
  *
  * If you need to free a newly allocated person struct created by person_copy,
  * you should use person_free.
  *
  * @param listToCopy A pointer to the struct list structure to make a copy of
  * @return The struct list structure created by copying the old one, or NULL on
  *         failure (includes any of the parameters being null)
  */
struct list *copy_list(struct list *listToCopy)
{
    if (listToCopy == NULL || listToCopy -> head == NULL) {
        return NULL;
    }

    struct list *newList = (struct list*) malloc(sizeof(struct list));
    if(newList == NULL) {
    	free(newList);
        return NULL;
    }
    newList -> head = NULL;
    newList -> tail = NULL;
    newList -> size = 0;

    struct person *clone;
    if (!person_copy(listToCopy -> head -> data, &clone)) {
        person_free(clone);
        empty_list(newList);
        free(newList);
        return NULL;
    }



    struct list_node *headNode = (struct list_node*) malloc(sizeof(struct list_node));
    if (headNode == NULL) {
        person_free(clone);
        empty_list(newList);
        free(headNode);
        free(newList);
        return NULL;
    }
    headNode -> data = clone;
    headNode -> next = NULL;
    newList -> size++;
    newList -> head = headNode;
    newList -> tail = headNode;
    // struct list_node *newListCurr = headNode;
    struct list_node *oldListCurr = listToCopy -> head;
    while (newList -> size < listToCopy -> size) {
        oldListCurr = oldListCurr -> next;
        if (!person_copy(oldListCurr -> data, &clone)) {
            person_free(clone);
            empty_list(newList);
            free(newList);
            return NULL;
        }
        struct list_node *nextNewNode = (struct list_node*) malloc(sizeof(struct list_node));
        if (nextNewNode == NULL) {
            person_free(clone);
            empty_list(newList);
            free(newList);
            return NULL;
        }
        nextNewNode -> data = clone;
        nextNewNode -> next = NULL;
        newList -> tail -> next = nextNewNode;
        newList -> tail = nextNewNode;
        newList -> size++;
    }
    return newList;






    //
    // struct list *newList = (struct list*) malloc(sizeof(struct list));
    // if(newList == NULL) {
    //     return NULL;
    // }
    // newList -> head = NULL;
    // newList -> tail = NULL;
    // newList -> size = 0;
    //
    // struct list_node *copyNode = listToCopy -> head;
    // struct person *newPerson = (struct person*) malloc(sizeof(struct person));
    // person_copy(copyNode -> data, **newPerson);
    // struct list_node *newNode = (struct list_node*) malloc(sizeof(struct list_node));
    // newNode -> data = newPerson;
    // newList -> head = newNode;
    // newList -> tail = newNode;
    // newList -> size++;
    //
    // struct list_node *newTail = newList -> tail;
    //
    // for (int i = 0; i < listToCopy -> size - 1; i = i + 1) {
    //     struct person *newPerson = (struct person*) malloc(sizeof(struct person));
    //     person_copy(copyNode -> data, **newPerson);
    //     struct list_node *newNode = (struct list_node*) malloc(sizeof(struct list_node));
    //     newNode -> data = newPerson;
    //     newTail -> next = newNode;
    //     newList -> tail = newNode;
    //     newTail = newList -> tail;
    //     newList -> size++;
    // }
    //
    // int success = 1;
    // struct list_node *copyNode = listToCopy -> head;
    // for (int i = 0; i < listToCopy -> size - 1; i = i + 1) {
    //     success = push_back(newList, copyNode -> data);
    //     if (success == 0) {
    //         empty_list(newList);
    //         free(newList);
    //         return NULL;
    //     }
    //     copyNode = copyNode -> next;
    // }
    // return newList;
    // UNUSED_PARAMETER(listToCopy);
    // return 0;
}

/** push_front
  *
  * Adds the element at the front of the struct list.
  *
  * @param listToPrependTo a pointer to the struct list structure.
  * @param data the data to add to the struct list.
  * @return 0 if the struct list is NULL, 1 if successful.
  */
int push_front(struct list *listToPrependTo, struct person *data)
{
    if (listToPrependTo == NULL) {
        return 0;
    }
    struct list_node *newNodePtr = (struct list_node*) malloc(sizeof(struct list_node));
    newNodePtr -> data = data;
    newNodePtr -> next = NULL;
    if (listToPrependTo -> size == 0) {
        listToPrependTo -> head = newNodePtr;
        listToPrependTo -> tail = newNodePtr;
    } else {
        newNodePtr -> next = listToPrependTo -> head;
        listToPrependTo -> head = newNodePtr;
    }
    listToPrependTo -> size++;
    return 1;
    // UNUSED_PARAMETER(listToPrependTo);
    // UNUSED_PARAMETER(data);
    // return 0;
}

/** push_back
  *
  * Adds the element to the back of the struct list.
  *
  * @param listToAppendTo a pointer to the struct list structure.
  * @param data the data to add to the struct list.
  * @return 0 if the struct list is NULL, 1 if successful.
  */
int push_back(struct list *listToAppendTo, struct person *data)
{
    if (listToAppendTo == NULL) {
        return 0;
    }
    struct list_node *newEndNodePtr = (struct list_node*) malloc(sizeof(struct list_node));
    newEndNodePtr -> data = data;
    newEndNodePtr -> next = NULL;
    if (listToAppendTo -> size == 0) {
        listToAppendTo -> head = newEndNodePtr;
        listToAppendTo -> tail = newEndNodePtr;
    } else {
        listToAppendTo -> tail -> next = newEndNodePtr;
        listToAppendTo -> tail = newEndNodePtr;
    }
    listToAppendTo -> size++;
    return 1;
    // UNUSED_PARAMETER(listToAppendTo);
    // UNUSED_PARAMETER(data);
    // return 0;
}

/** add
  *
  * Add the element at the specified index in the struct list. This index must lie in
  * the inclusive range [0,size].
  * For example, if you have no elements in the struct list,
  * you should be able to add to index 0 but no further.
  * If you have two elements in the struct list,
  * you should be able to add to index 2 but no further.
  *
  * @param listToAddTo a pointer to the struct list structure
  * @param index 0-based, starting from the head in the inclusive range
  *              [0,size]
  * @param data the data to add to the struct list
  * @return 0 if the index is out of bounds or the struct list is NULL
  *         (do not add the data in this case)
  *         otherwise return 1
  */
int add(struct list *listToAddTo, int index, struct person *data)
{
    if (listToAddTo == NULL || index > listToAddTo -> size || index < 0) {
        return 0;
    }
    if (index == 0) {
        return push_front(listToAddTo, data);
    }
    if (index == listToAddTo -> size) {
        return push_back(listToAddTo, data);
    }
    // struct list_node *currNode = (struct list_node*) malloc(sizeof(struct list_node));
    struct list_node *currNode = listToAddTo -> head;
    if (index > 1){
        for (int i = 0; i < index; i = i + 1) {
            currNode = currNode -> next; //get to the index to insert
        }
    }
    struct list_node *newNode = (struct list_node*) malloc(sizeof(struct list_node));
    newNode -> data = data;
    newNode -> next = currNode -> next;
    currNode -> next = newNode;
    listToAddTo -> size++;
    return 1;
    // UNUSED_PARAMETER(listToAddTo);
    // UNUSED_PARAMETER(index);
    // UNUSED_PARAMETER(data);
    // return 0;
}

/** front
  *
  * Gets the data at the front of the struct list
  * If the struct list is empty or the struct list is NULL, return 0.
  *
  * @param listToGetFrontOf a pointer to the struct list
  * @param dataOut A pointer to a pointer used to return the data at the first
  *                struct list_node in the struct list or NULL on failure.
  * @return 0 if dataOut is NULL or the struct list is NULL or empty, 1 if
  *         successful.
  */
int front(struct list *listToGetFrontOf, struct person **dataOut)
{
    if (listToGetFrontOf == NULL || dataOut == NULL) {
        return 0;
    }
    if (listToGetFrontOf -> size == 0) {
        *dataOut = NULL;
        return 0;
    }
    *dataOut = listToGetFrontOf -> head -> data;
    if (*dataOut == NULL || dataOut == NULL) {
        return 0;
    }
    return 1;
    // UNUSED_PARAMETER(listToGetFrontOf);
    // UNUSED_PARAMETER(dataOut);
    // return 0;
}

/** back
  *
  * Gets the data at the back of the struct list
  * If the struct list is empty or the struct list is NULL, return 0.
  *
  * @param listToGetBackOf a pointer to the struct list
  * @param dataOut A pointer to a pointer used to return the data at the last
  *                struct list_node in the struct list or NULL on failure.
  * @return 0 if dataOut is NULL or the struct list is NULL or empty, 1
  *         otherwise
  */
int back(struct list *listToGetBackOf, struct person **dataOut)
{
    if (listToGetBackOf == NULL || dataOut == NULL) {
        return 0;
    }
    if (listToGetBackOf -> size == 0) {
        *dataOut = NULL;
        return 0;
    }
    *dataOut = listToGetBackOf -> tail -> data;
    if (*dataOut == NULL || dataOut == NULL) {
        return 0;
    }
    return 1;
    // UNUSED_PARAMETER(listToGetBackOf);
    // UNUSED_PARAMETER(dataOut);
    // return 0;
}

/** get
  *
  * Gets the data at the specified index in the struct list
  *
  * @param listToGetDataFrom a pointer to the struct list structure
  * @param index 0-based, starting from the head.
  * @param dataOut A pointer to a pointer used to return the data from the
  *        specified index in the struct list or NULL on failure.
  * @return 0 if dataOut is NULL or index is out of range of the struct list or
  *         the struct list is NULL, 1 otherwise
  */
int get(struct list *listToGetDataFrom, int index, struct person **dataOut)
{
    if (listToGetDataFrom == NULL) {
        *dataOut = NULL;
        return 0;
    }
    if (dataOut == NULL) {
        return 0;
    }
    if (listToGetDataFrom -> size == 0) {
        *dataOut = NULL;
        return 0;
    }
    if (index < 0) {
        *dataOut = NULL;
        return 0;
    }
    if (index > listToGetDataFrom -> size - 1) {
        *dataOut = NULL;
        return 0;
    }
    if (index == 0) {
        return front(listToGetDataFrom, dataOut);
    }
    if (index == listToGetDataFrom -> size - 1) {
        return back(listToGetDataFrom, dataOut);
    }
    struct list_node *currNode = listToGetDataFrom -> head;
    for (int i = 0; i < index; i = i + 1) {
        currNode = currNode -> next;
    }
    *dataOut = currNode -> data;
    if (*dataOut == NULL || dataOut == NULL) {
        return 0;
    }
    return 1;
    // UNUSED_PARAMETER(listToGetDataFrom);
    // UNUSED_PARAMETER(index);
    // UNUSED_PARAMETER(dataOut);
    // return 0;
}

/** size
  *
  * Gets the size of the struct list
  *
  * if the struct list is NULL, return 0.
  *
  * @param listToGetSizeOf a pointer to the struct list structure
  * @param A pointer used to return the size of the struct list, set to -1 on NULL struct list
  * @return 0 if sizeOut is NULL or struct list is NULL, 1 otherwise
  */
int size(struct list *listToGetSizeOf, int *sizeOut)
{
    if (listToGetSizeOf == NULL) {
        *sizeOut = -1;
        return 0;
    }
    if (sizeOut == NULL) {
        return 0;
    }
    *sizeOut = listToGetSizeOf -> size;
    return 1;
    // UNUSED_PARAMETER(listToGetSizeOf);
    // UNUSED_PARAMETER(sizeOut);
    // return 0;
}

/** contains
  *
  * Traverses the struct list, trying to see if the struct list contains some
  * data.
  *
  * The "data" parameter may not necessarily point to the same address as the
  * equal data you are returning from this function, it's just data which the
  * comparator (person_eq_name in this case) says is equal.
  *
  * If there are multiple pieces of data in the struct list which are equal to
  * the "data" parameter, return the one at the lowest index.
  *
  * Use person_eq_name to compare person structs. Be sure to take a look at that
  * function before you use it! Also be sure not to use person_eq.
  *
  * @param listToSearch a pointer to the struct list structure
  * @param data The data, to see if it exists in the struct list
  * @param dataOut A pointer to a pointer used to return the data contained in
  *                the struct list or NULL on failure
  * @return int    0 if dataOut is NULL, the list is NULL, or the list
  *                does not contain data, else 1
  */
int contains(struct list *listToSearch, struct person *data, struct person **dataOut)
{
    if (dataOut == NULL) {
        return 0;
    }
    if (listToSearch == NULL || listToSearch -> size == 0) {
        *dataOut = NULL;
        return 0;
    }
    int found = 0;
    int i = 0;
    struct list_node *currNode = listToSearch -> head;
    if (person_eq_name(currNode -> data, data) == 0) {
        found = 1;
    }
    while (found == 0 && i < listToSearch -> size - 1) {
        i = i + 1;
        currNode = currNode -> next;
        if (person_eq_name(currNode -> data, data) == 0) {
            found = 1;
        }
    }
    if (found == 1) {
        *dataOut = currNode -> data;
        return 1;
    }
    *dataOut = NULL;
    return 0;
    UNUSED_PARAMETER(listToSearch);
    UNUSED_PARAMETER(data);
    UNUSED_PARAMETER(dataOut);
    return 0;
}


/** pop_front
  *
  * Removes the struct list_node at the front of the struct list, and returns its data to the user
  *
  * @param listToPopFrom a pointer to the struct list.
  * @param dataOut A pointer to a pointer used to return the data in the first
  *                struct list_node or NULL on failure
  * @return 0 if dataOut is NULL or the struct list is NULL or empty, else 1
  */
int pop_front(struct list *listToPopFrom, struct person **dataOut)
{
    if (dataOut == NULL) {
        return 0;
    }
    if (listToPopFrom == NULL || listToPopFrom -> size == 0) {
        *dataOut = NULL;
        return 0;
    }
    struct list_node *popped = listToPopFrom -> head;
    *dataOut = popped -> data;
    listToPopFrom -> head = popped -> next;
    popped -> data = NULL;
    popped -> next = NULL;
    free(popped);
    listToPopFrom -> size--;
    if (listToPopFrom -> size == 0) {
        listToPopFrom -> head = NULL;
        listToPopFrom -> tail = NULL;
    }
    return 1;
    // UNUSED_PARAMETER(listToPopFrom);
    // UNUSED_PARAMETER(dataOut);
    // return 0;
}

/** pop_back
  *
  * Removes the struct list_node at the back of the struct list, and returns its data to the user
  *
  * @param listToPopFrom a pointer to the struct list.
  * @param dataOut A pointer to a pointer used to return the data in the last
  *                struct list_node or NULL on failure
  * @return 0 if dataOut is NULL or the struct list is NULL or empty, else 1
  */
int pop_back(struct list *listToPopFrom, struct person **dataOut)
{
    if (dataOut == NULL) {
        return 0;
    }
    if (listToPopFrom == NULL || listToPopFrom -> size == 0 || listToPopFrom -> head == NULL) {
        *dataOut = NULL;
        return 0;
    }
    struct list_node *notLast = listToPopFrom -> head;
    if (listToPopFrom -> size == 1) {
        *dataOut = notLast -> data;
        free(notLast);
        listToPopFrom -> size--;
        listToPopFrom -> head = NULL;
        listToPopFrom -> tail = NULL;
        return 1;
    }
    for (int i = 0; i < listToPopFrom -> size - 2; i = i + 1) {
        notLast = notLast -> next;
    }
    listToPopFrom -> tail = notLast;
    // struct list_node *popped = notLast -> next;
    *dataOut = notLast -> next -> data;
    // popped -> data = NULL;
    // popped -> next = NULL;
    free(notLast -> next);
    notLast -> next = NULL;
    listToPopFrom -> size--;
    return 1;
    // UNUSED_PARAMETER(listToPopFrom);
    // UNUSED_PARAMETER(dataOut);
    // return 0;
}

/** list_remove
  *
  * Removes a specified data element from the struct list.
  *
  * The element to be removed is the one that is equal to the data_to_remove
  * parameter by way of the comparator function passed in. This function
  * returns 0 when its arguments are equal.
  *
  * If the struct list contains multiple elements that are equal to data_to_remove,
  * then the one with the lowest index only is to be removed.
  *
  * Use person_eq_name to compare person structs. Be sure to take a look at that
  * function before you use it! Also be sure not to use person_eq.
  *
  * @param listToRemoveFrom             a pointer to the struct list structure
  * @param data_to_remove               The element to remove from the struct list.
  * @param dataOut                      A pointer to a pointer used to
  *                                     return the data from the specified
  *                                     index in the struct list or NULL on failure
  *
  * @return int                         0 if dataOut is NULL, or the list is
  *                                     NULL, else 1.
  */
int list_remove(struct list *listToRemoveFrom,
                 struct person *data_to_remove,
                 struct person **dataOut)
{
    if (dataOut == NULL) {
        return 0;
    }
    if (listToRemoveFrom == NULL || listToRemoveFrom -> size == 0) {
        *dataOut = NULL;
        return 0;
    }
    int found = 0;
    int i = 0;
    struct list_node *currNode = listToRemoveFrom -> head;
    if (person_eq_name(currNode -> data, data_to_remove) == 0) {
        //found at head
        found = 1;
        *dataOut = currNode -> data;
        pop_front(listToRemoveFrom, dataOut);
        return 1;
    }
    while (found == 0 && i < listToRemoveFrom -> size - 1) {
        i = i + 1;
        currNode = currNode -> next;
        if (person_eq_name(currNode -> data, data_to_remove) == 0) {
            found = 1;
        }
    }
    if (found == 1) {
        if (i == listToRemoveFrom -> size - 1) {
            *dataOut = currNode -> data;
            pop_back(listToRemoveFrom, dataOut);
            return 1;
        }
        struct list_node *beforeNode = listToRemoveFrom -> head;
        for (int j = 0; j < i - 1; j = j + 1) {
            beforeNode = beforeNode -> next;
        }
        *dataOut = currNode -> data;
        beforeNode -> next = currNode -> next;
        free(currNode);
        listToRemoveFrom -> size--;
        return 1;
    }
    *dataOut = NULL;
    return 0;
    UNUSED_PARAMETER(listToRemoveFrom);
    UNUSED_PARAMETER(data_to_remove);
    UNUSED_PARAMETER(dataOut);
    return 0;
}

/** empty_list
  *
  * Empties the struct list. After this is called, the struct list should be
  * empty. This does not free the struct list struct itself, just all nodes and
  * data within. Make sure to check that listToEmpty and is not NULL before
  * using it.
  *
  * Call person_free to free a person struct.
  *
  * @param listToEmpty a pointer to the struct list structure
  */
void empty_list(struct list *listToEmpty)
{
    if (listToEmpty == NULL) {
        return;
    }
    struct list_node *currNode = listToEmpty -> head;
    struct list_node *nextNode = listToEmpty -> head;
    while (listToEmpty -> size > 0) {
        currNode = nextNode;
        person_free(currNode -> data);
        nextNode = currNode -> next;
        free(currNode);
        listToEmpty -> size--;
    }
    listToEmpty -> head = NULL;
    listToEmpty -> tail = NULL;
    // UNUSED_PARAMETER(listToEmpty);
}

/** reverse
  *
  * Reverses the singly linked struct list in place.
  *
  * @param listToReverse to append to.
  * @return 1 if the operation was a success, 0 otherwise (listToReverse
  *         was NULL)
  */
int reverse(struct list *listToReverse)
{
    if (listToReverse == NULL) {
        return 0;
    }
    int halfLen = (listToReverse -> size) / 2;
    struct list_node *start = listToReverse -> head;
    struct list_node *end = listToReverse -> tail;
    for (int i = 0; i < halfLen; i = i + 1) {
        struct person *temp = start -> data;
        start -> data = end -> data;
        end -> data = temp;
        start = start -> next;
        end = listToReverse -> head;
        for (int j = 0; j < listToReverse -> size - 1 - i; j = j + 1) {
            end = end -> next;
        }
    }
    return 1;
    // UNUSED_PARAMETER(listToReverse);
    // return 0;
}

/** concat
  *
  * Concatinates firstList and secondList. This function should append
  * secondList to the end of firstList.
  *
  * NOTE: this function will effectively empty lastList onto firstList which
  *       means that lastList will be empty (head == tail == NULL and size == 0)
  *       at the end of the function and firstList will contain all the elements
  *       from both lists. Be sure to update the size, head, and tail pointers
  *       accordingly.
  *
  * example: if firstList is a->b->c and second struct list is d->e->f
  * after the function call, firstList will be a->b->c->d->e->f and
  * secondList will be empty.
  *
  *
  * @param firstList to append to.
  * @param secondList the struct list to append to firstList
  * @return 1 if the operation was a success, 0 otherwise (firstList or
  *         secondList is null)
  */
int concat(struct list *firstList, struct list *lastList)
{
    if (firstList == NULL || lastList == NULL) {
        return 0;
    }
    // if (firstList -> size == 0) {
    //
    // }
    // struct list_node *tailNode = firstList -> tail;
    struct list_node *currNode = lastList -> head;
    struct list_node *nextNode = lastList -> head;
    while (lastList -> size > 0) {
        currNode = nextNode;

        push_back(firstList, currNode -> data);
        // struct list_node *appendNode = (struct list_node*) malloc(sizeof(struct list_node));
        // appendNode -> data = currNode -> data;
        // appendNode -> next = NULL;
        // tailNode -> next = appendNode;
        // firstList -> tail = appendNode;
        // tailNode = firstList -> tail;

        nextNode = currNode -> next;
        // person_free(currNode -> data);
        free(currNode);
        lastList -> size--;
        // firstList -> size++;
    }
    lastList -> head = NULL;
    lastList -> tail = NULL;
    return 1;
    // UNUSED_PARAMETER(firstList);
    // UNUSED_PARAMETER(lastList);
    // return 0;
}
