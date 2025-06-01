#include "queue.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

struct node {
  TNode *data;
  int depth;
  struct node *next;
};

struct queue {
  struct node *front;
  struct node *back;
};

Queue* Q_create() {
  Queue *queue = (Queue*) malloc(sizeof(Queue));

  if (queue == NULL)
    ERR_print("queue allocation failed.");

  queue->front = queue->back = NULL;

  return queue;
}

void Q_free(Queue* queue) {
  struct node *current = queue->front;
  struct node *next;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }

  free(queue);
}

void Q_enqueue(Queue* queue, TNode* data, int depth) {
  QNode *node = (QNode*) malloc(sizeof(QNode));

  if (node == NULL)
    ERR_print("queue node allocation failed.");

  node->data = data;
  node->depth = depth;
  node->next = NULL;

  if (queue->back == NULL) {
    queue->front = queue->back = node;
    return;
  }

  queue->back->next = node;
  queue->back = node;
}

TNode* Q_dequeue(Queue* queue, int* depth) {
  if (queue->front == NULL)
    return NULL;

  QNode *node = queue->front;
  TNode *data = node->data;

  if (depth != NULL)
    *depth = node->depth;

  queue->front = node->next;

  if (queue->front == NULL)
    queue->back = NULL;

  free(node);

  return data;
}

int Q_is_empty(Queue* queue) {
  return queue->front == NULL;
}
