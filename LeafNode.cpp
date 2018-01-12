#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  // students must write this
	bool check = false;
	int temp = 0;
	LeafNode* ptr = NULL;


	//FIX ME
	if (count == leafSize) {// if no more space
		if (leftSibling == NULL && rightSibling == NULL){ //if no leaft and right sibling, create a new node depending on values
			if ( value > this->getMinimum()){
			    ptr = new LeafNode(leafSize, this->parent, this, NULL);
				this->rightSibling = ptr;
			}
			else if ( value < this->getMinimum()){
				ptr = new LeafNode(leafSize, this->parent, NULL, this);
				this->leftSibling = ptr;
			}
			ptr->insert(value);
		}
		else if (leftSibling != NULL && ((LeafNode*)leftSibling)->count < leafSize){ //if there is a left sibling 
			leftSibling->insert(value);
		}
		else if (rightSibling != NULL && ((LeafNode*)rightSibling)->count < leafSize){ //if left is full, look at right sibling
			rightSibling->insert(value);
		}
		else if (leftSibling != NULL && rightSibling == NULL){ //if there is space to the right
			ptr = new LeafNode(leafSize, this->parent, this, NULL);
			this->rightSibling = ptr;
			ptr->insert(value);
		}
		else if (leftSibling != NULL && rightSibling != NULL){ //if no more space thorough out
			ptr = new LeafNode(leafSize, NULL, this, this->rightSibling);
			this->rightSibling = ptr;
			ptr->insert(value);
		}
		else if (leftSibling == NULL && rightSibling != NULL){
			ptr = new LeafNode(leafSize, NULL, this, this->rightSibling);
			this->rightSibling = ptr;
			ptr->insert(value);
		}
	}

	else if (count == 0) {// if it is a new leaf
		values = new int[leafSize];
		values[count] = value;
		count++;
	}

	else if (count > 0) {
		count++;
		for ( int i = 0; i < count; i++){
			if (check){ //swapping int 
				int swap;
				swap = values[i];
				values[i] = temp;
				temp = swap;
			}
			else if (values[i] > value){ //reorder the leaf if value is smaller
				temp = values[i];
				values[i] = value;
				check = true;
			}	

		}
		if (!check) {
			values[count - 1] = value;
		}
	}

	if (rightSibling != NULL) { //this is to swap int between siblings, biggest to the right
		for (int i = 0; i < count; i++) {
			if (values[i] > ((LeafNode*)rightSibling)->getMinimum()) {
				temp = ((LeafNode*)rightSibling)->values[0];
				((LeafNode*)rightSibling)->values[0] = values[i];
				values[i] = temp;
			}
		}
		//MODIFIED
		if (rightSibling == ptr){
			int j = 0;
			for ( int i = count; i > 0; i--){
				if (((LeafNode*)rightSibling)->count > leafSize/2){ break;}
				((LeafNode*)rightSibling)->insert(values[i - 1]);
				this->count--;
				j++;
			}
		return ptr;
		}
		if (((LeafNode*)rightSibling)->count < this->count){// make sure the right has more values
			int j = 0;
			for ( int i = count; i > 0; i--){
				if (((LeafNode*)rightSibling)->count > leafSize/2){ break;}
				((LeafNode*)rightSibling)->values[((LeafNode*)rightSibling)->count + j] = values[i - 1];
				((LeafNode*)rightSibling)->count++;
				count--;
				j++;
			}
		}
		for ( int i = 0; i < ((LeafNode*)rightSibling)->count - 1; i++){// sort the right sibling
			if (((LeafNode*)rightSibling)->values[i] > ((LeafNode*)rightSibling)->values[i + 1]){
				temp = ((LeafNode*)rightSibling)->values[i];
				((LeafNode*)rightSibling)->values[i] = ((LeafNode*)rightSibling)->values[i + 1];
				((LeafNode*)rightSibling)->values[i + 1] = temp;
			}
		}
	}


  return ptr; // to avoid warnings for now.
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


