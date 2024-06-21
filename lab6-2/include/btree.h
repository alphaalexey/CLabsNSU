#pragma once

typedef struct BTree BTree;

BTree *InitBTree(int factor);
void FreeBTree(BTree *btree);
int BTreeInsert(BTree *btree, int key);
int BTreeHeight(BTree *btree);
