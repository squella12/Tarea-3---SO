#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define MAX 100

int buffer[MAX];
int fill = 0;
int use = 0;
int count = 0;
int noditos = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

typedef struct node{
    int data;
    struct node* next;
} node;

void print_list(node* head)
{
    node* cursor = head;
    while(cursor != NULL)
    {
        printf("%d \n", cursor->data);
        cursor = cursor->next;
    }
}

node* create(int data,node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;
 
    return new_node;
}

node* append(node* head, int data)
{
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
 
    /* create a new node */
    node* new_node =  create(data,NULL);
    cursor->next = new_node;
 
    return head;
}

node* remove_front(node* head)
{
    if(head == NULL)
        return NULL;
    node *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last node in the list */
    if(front == head)
        head = NULL;
    free(front);
    return head;
}

node* remove_back(node* head)
{
    if(head == NULL)
        return NULL;
 
    node *cursor = head;
    node *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }
    if(back != NULL)
        back->next = NULL;
 
    /* if this is the last node in the list*/
    if(cursor == head)
        head = NULL;
 
    free(cursor);
 
    return head;
}

void put(int value) {
  buffer[fill] = value;
  fill = (fill + 1) % MAX;
  count++;
}

void *append2(void *arg) {
  int i;
  int loops = (int) arg;
  for (i = 0; i < loops; i++) {
    pthread_mutex_lock(&lock);
    while (count >= MAX)
      pthread_cond_wait(&empty, &lock);
    put(i);
    printf("%d\n", i);
    pthread_cond_signal(&data);
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}
node* append3(node* head, int data)
{
    /* go to the last node */
    node *cursor = head;
    pthread_mutex_lock(&lock);

    while(cursor->next != NULL)
        cursor = cursor->next; 
    /* create a new node */
    node* new_node =  create(noditos,NULL);
    noditos++;
    cursor->next = new_node;
    pthread_mutex_unlock(&lock);
  
    return head;
}

int main(void) {

  pthread_t p1[100];
  node* head = NULL;
  head = (node *) malloc(sizeof(node));
  node* head2 = NULL;
  head2 = (node *) malloc(sizeof(node));

  // Agregando 100 nodos de manera secuencial

  for(int i=1; i<100; i++)
    append(head, i);
  print_list(head);
  printf("\nListo!\n");
  // En paralelo
  for(int i=1; i<100; i++)
    pthread_create(&p1[i], NULL, (void*)append3, (node*) head2);
  for(int j=1; j<99; j++)
    pthread_join(p1[j], NULL);

  // INSTRUCCIONES
  // inserte código para agregar 100 nodos
  // primero secuencialmente (usando un ciclo)
  // luego en paralelo

  print_list(head2);
  printf("\nListo!\n");
  return 0;
}