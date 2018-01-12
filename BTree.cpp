#include <iostream>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
  root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()


void BTree::insert(const int value)
{
	bool check = false;
	BTreeNode* ptr = NULL;
	BTreeNode* oldRoot = NULL;

	if (dynamic_cast<InternalNode*>(root)){ //check if root is internal node
		check = true;
	}


	if (!check) {// if empty root just insert to leaf node
		ptr = root->insert(value);
	}

	if (ptr != NULL) {// if change into internal nodes
		oldRoot = root;
		InternalNode* a = new InternalNode(internalSize, leafSize, NULL, NULL, NULL);
		a->insert(oldRoot);
		a->insert(ptr);
		root = a;
	}

	else if (check) {
		ptr = root->insert(value);
		if ( ptr != NULL){ //if internal node returns something
				root = ptr;
		}// root = ptr;
	}



  // students must write this
} // BTree::insert()


void BTree::print(void)
{
  BTreeNode *BTreeNodePtr;
  Queue<BTreeNode*> queue(1000);

  queue.enqueue(root);
  while(!queue.isEmpty())
  {
    BTreeNodePtr = queue.dequeue();
    BTreeNodePtr->print(queue);
  } // while
} // BTree::print()
