#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "concurrent_list.h"

struct node
{
  int value;
  pthread_mutex_t lock;
  node *next;
};

struct list
{
  node *head;
  pthread_mutex_t lock;
};

void print_node(node *node)
{
  // DO NOT DELETE
  if (node)
  {
    printf("%d ", node->value);
  }
}

list *create_list()
{
  list *List = (list *)malloc(sizeof(list)); // allocating the memory
  if (List == NULL)                          // in case that the allocation failed
  {
    printf("Error: failed to allocate memory for new list.\n");
    exit(1);
  }
  List->head = NULL;
  if (pthread_mutex_init(&(List->lock), NULL) != 0)
  {
    printf("Error: failed to initialize lock.\n");
    free(List);
    exit(1);
  }
  return List;
}

void delete_list(list *list)
{
  if (!list)
    return;

  pthread_mutex_lock(&(list->lock)); // Lock the list

  node *curr = list->head;
  while (curr != NULL)
  {
    pthread_mutex_lock(&(curr->lock)); // Lock the current node

    node *temp = curr;
    curr = curr->next;
    // Unlock the current node and then we need to destroy it
    pthread_mutex_unlock(&(temp->lock));
    pthread_mutex_destroy(&(temp->lock));
    // Deallocate the current node
    free(temp);
  }

  pthread_mutex_destroy(&(list->lock)); // Destroy the list
  free(list);                           // Deallocate the list
}

void insert_value(list *list, int value)
{
  if (!list)
    return;

  node *new = (node *)malloc(sizeof(node));
  if (!new)
  {
    printf("Error: failed to allocate memory for new list.\n");
    exit(1);
  }
  new->value = value;
  new->next = NULL;
  pthread_mutex_init(&(new->lock), NULL);
  pthread_mutex_lock(&(new->lock));

  pthread_mutex_lock(&(list->lock));

  // find the correct position to insert the new node
  node *curr = list->head;
  pthread_mutex_unlock(&(list->lock));

  node *prev = NULL;

  while (curr && curr->value < value) {
    if (curr->next)
      pthread_mutex_lock(&(curr->next->lock));
    if (prev)
      pthread_mutex_unlock(&(prev->lock));
    prev = curr;
    curr = curr->next;
  }

  // updating the positions and adding the new node
  if (prev)
    prev->next = new;
  else
    list->head = new;
  new->next = curr;


  // unlock the nodes that we dealt with at last
  pthread_mutex_unlock(&(new->lock));
  // in case the node is at the middle or the last of the list
  if (prev)
    pthread_mutex_unlock(&(prev->lock));
  if(curr)
    pthread_mutex_unlock(&(curr->lock));
  // unlock the list
}

void remove_value(list *list, int value) {
  if (!list)
    return;

  pthread_mutex_lock(&(list->lock));

  node *curr = list->head;
  node *prev = NULL;

  while (curr && curr->value != value) {
    if (curr->next)
      pthread_mutex_lock(&(curr->next->lock));
    if (prev)
      pthread_mutex_unlock(&(prev->lock));
    prev = curr;
    curr = curr->next;
  }

  if (!curr) {
    pthread_mutex_unlock(&(list->lock));
    return; // the value is not in the list
  }

  // unlink the node from the list
  if (prev)
    prev->next = curr->next;
  else
    list->head = curr->next;

  // unlock the nodes that we dealt with at last
  if (prev)
    pthread_mutex_unlock(&(prev->lock));
  if (curr->next)
    pthread_mutex_unlock(&(curr->next->lock));
  pthread_mutex_unlock(&(list->lock));

  // destroy and free the node
  pthread_mutex_destroy(&(curr->lock));
  free(curr);
}

void print_list(list *list)
{
  if (!list) // the list is empty
  {
    printf("\n");
    return;
  }

  pthread_mutex_lock(&(list->lock));

  if (!list->head)// the head is NULL (there is no nodes)
  {
    printf("\n");
    pthread_mutex_unlock(&(list->lock));
    return;
  }

  pthread_mutex_lock(&(list->head->lock));
  pthread_mutex_unlock(&(list->lock));

  node *curr = list->head;
  print_node(curr);//print the first node

  // print the rest of the nodes
  while (curr->next)
  {
    // lock the next node and unlock the previous
    pthread_mutex_lock(&(curr->next->lock));
    pthread_mutex_unlock(&(curr->lock));
    curr = curr->next;
    // Print the current node
    print_node(curr);
  }
  // unlock for the last node
  pthread_mutex_unlock(&(curr->lock));
  printf("\n");
}

void count_list(list* list, int (*predicate)(int))
{
    int count = 0;
    if (list == NULL) {
        printf("%d items were counted\n", count);
        return;
    }

    pthread_mutex_lock(&(list->lock));
    node* curr = list->head;
    pthread_mutex_unlock(&(list->lock));

    while (curr) {
        pthread_mutex_lock(&(curr->lock));
        if (predicate(curr->value)) {
            count++;
        }
        node* next = curr->next;
        pthread_mutex_unlock(&(curr->lock));
        curr = next;
    }

    printf("%d items were counted\n", count);
}

