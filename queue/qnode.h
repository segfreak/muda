#ifndef __QNODE_IMP_H
#define __QNODE_IMP_H

typedef struct muda_qnode
{
  void              *data;
  struct muda_qnode *next;
} muda_qnode_t;

#endif