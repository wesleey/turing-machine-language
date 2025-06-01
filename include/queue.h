#ifndef QUEUE_H
#define QUEUE_H

typedef struct node QNode;
typedef struct queue Queue;

#include "tree.h"

Queue* Q_create();
void Q_enqueue(Queue* queue, TNode* node, int depth);
TNode* Q_dequeue(Queue* queue, int* depth);
int Q_is_empty(Queue* queue);
void Q_free(Queue* queue);

#endif // QUEUE_H
