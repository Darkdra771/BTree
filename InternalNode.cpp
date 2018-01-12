#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
	BTreeNode* ptr = NULL;
	//bool check = false;
	int halfSize;

	if ( internalSize % 2 == 0){ halfSize = internalSize/2;}
	else { halfSize = internalSize/2 + 1;}

	//make sure pointer of leaf node is to the correct location
	if (this->children[0] != NULL){
	if (leftSibling != NULL){
		for ( int i = internalSize; i > 0; i--){
			if (((InternalNode*)leftSibling)->children[i - 1] != NULL){
				children[0]->setLeftSibling(((InternalNode*)leftSibling)->children[i - 1]);
				break;
			}
		}
	}
	}

	if(this->children[count - 1] != NULL){
		if (rightSibling != NULL){
			if (((InternalNode*)rightSibling)->children[0] != NULL){
					children[count - 1]->setRightSibling(((InternalNode*)rightSibling)->children[0]);
			}
		}
	}


	for ( int i = count; i > 0; i--){//check which children to insert
		if( value > keys[i - 1]){
			ptr = children[i - 1]->insert(value); break;}
		else if ( i == 1){
			ptr = children[i - 1]->insert(value); break;}
	}


	if ( ptr != NULL){ // if insert returned a new leafNode
		if ( this->count < internalSize ){ //if internal node is not full
			this->insert(ptr);}
		else if ( this->count == internalSize) { //if internal node is full
			if ( leftSibling == NULL && this->parent == NULL){ //check left sibling first
				InternalNode* newRoot = new InternalNode(internalSize, leafSize, NULL, NULL, NULL); //FIX ME
				InternalNode* newNode = new InternalNode(internalSize, leafSize, newRoot, this, NULL); //this->parent --- newRoot
				Queue<BTreeNode*> sort(1000); //makes queue
				for (int i = 0; i < count; i++){
					sort.enqueue(this->children[i]);
				}
				for ( int i = 0; i < count; i++){ //makes all the children null
					this->children[i] = NULL;}
				this->count = 0;
				while( !sort.isEmpty()){
					if( (sort.getFront())->getMinimum() < ptr->getMinimum()){
						//cout << "a" << endl;
						this->insert(sort.dequeue());
					}
					else {
						//cout << "b" << endl;
						newNode->insert(sort.dequeue());}
				}

				if (this->count < halfSize){ //check which node to add ptr
					//cout << "c" << endl;
					this->insert(ptr);}
				else {
					//cout << "d" << endl;
					newNode->insert(ptr);}

				while( this->count > halfSize ){ //add nodes to newNode if too little
					//cout << "this count: " << this->count << endl;
					newNode->insert(children[count - 1]);
					children[count - 1] = NULL;
					this->count--;}

				this->setParent(newRoot);
				this->setRightSibling(newNode);
				newRoot->insert(this);
				newRoot->insert(newNode);	 //BIG CHANGES FROM newRoot->insert(this, newNode);///////////////////
				return newRoot;
			}
			else if (leftSibling == NULL && this->parent != NULL){
				if ( rightSibling != NULL && ((InternalNode*)rightSibling)->children[internalSize - 1] == NULL){
	                        	if( (ptr->getMinimum() < rightSibling->getMinimum()) && (ptr->getMinimum() < this->children[count - 1]->getMinimum())){
						((InternalNode*)rightSibling)->insert(this->children[count - 1]);
						this->children[count - 1] = NULL;
						this->count--;
						this->insert(ptr);
					}
					else {
						//cout << "hello" << endl;
						((InternalNode*)rightSibling)->insert(ptr);
					}
				}// end of if statement
				else {
				InternalNode* newNode = new InternalNode(internalSize, leafSize, this->parent, this, NULL); //this->parent --- newRoot
				if (rightSibling != NULL){
					//check = true;
					this->rightSibling->setLeftSibling(newNode);
					newNode->setRightSibling(this->rightSibling);
				}
				this->setRightSibling(newNode);
				Queue<BTreeNode*> sort(1000); //makes queue
				for (int i = 0; i < count; i++){
					sort.enqueue(this->children[i]);
				}
				for ( int i = 0; i < count; i++){ //makes all the children null
					this->children[i] = NULL;}
				this->count = 0;
				while( !sort.isEmpty()){
					if( (sort.getFront())->getMinimum() < ptr->getMinimum()){
						//cout << "a" << endl;
						this->insert(sort.dequeue());
					}
					else {
						//cout << "b" << endl;
						newNode->insert(sort.dequeue());}
				}
				if (this->count < halfSize){
					//cout << "c" << endl;
					this->insert(ptr);}
				else {
					//cout << "d" << endl;
					newNode->insert(ptr);}

				while( this->count > halfSize ){
					newNode->insert(children[count - 1]);
					children[count - 1] = NULL;
					this->count--;}

				this->parent->insert(newNode);
				//this->parent->insert(newNode->rightSibling);
				}

			}
			else if ( leftSibling != NULL){ //check leftSibling, if not full, transfer node
				if ( ((InternalNode*)leftSibling)->children[internalSize - 1] == NULL){
					((InternalNode*)leftSibling)->insert(children[0]);
					for (int i = 0; i < count; i++){
						if ( children[i + 1] != NULL){
						children[i] = children[i + 1];}
					}
					children[count - 1] = NULL;
					count--;
					this->insert(ptr);
				}
				else {
					InternalNode* newNode = new InternalNode(internalSize, leafSize, this->parent, this, NULL);
					if (rightSibling != NULL){
						//check = true;
						this->rightSibling->setLeftSibling(newNode);
						newNode->setRightSibling(this->rightSibling);
					}
					this->setRightSibling(newNode);
					Queue<BTreeNode*> sort(1000); //makes queue
					for (int i = 0; i < count; i++){
						sort.enqueue(this->children[i]);
					}
					for ( int i = 0; i < count; i++){ //makes all the children null
						this->children[i] = NULL;}
					this->count = 0;
					while( !sort.isEmpty()){
						if( (sort.getFront())->getMinimum() < ptr->getMinimum()){
						//cout << "a" << endl;
						this->insert(sort.dequeue());
					}
					else {
						//cout << "b" << endl;
						newNode->insert(sort.dequeue());}
					}
					if (this->count < halfSize){
						//cout << "c" << endl;
						this->insert(ptr);}
					else {
						//cout << "d" << endl;
						newNode->insert(ptr);}

					while( this->count > halfSize ){
						newNode->insert(children[count - 1]);
						children[count - 1] = NULL;
						this->count--;}

					this->parent->insert(this, newNode);
					//this->parent->insert(newNode->rightSibling);
				}
			}
		}
	}

	count = 0;
	for ( int i = 0; i < internalSize; i++){// this is to store keys
		if ( children[i] != NULL){
			count++;
			keys[i] = children[i]->getMinimum();
		}
	}

  // students must write this
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{
	BTreeNode* keep = this->children[0];
	InternalNode* store = new InternalNode(internalSize, leafSize, this, NULL, NULL);
	store->insert(keep);
	store->keys[0] = keep->getMinimum();
	this->children[0] = NULL;
	this->children[1] = NULL;
	this->count-=2;

	InternalNode* newNode = new InternalNode(internalSize, leafSize, this, NULL, NULL);
	newNode->insert(oldRoot);
	newNode->insert(node2);

	this->insert(store);
	this->insert(newNode);

  // Node must be the root, and node1
  // students must write this
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
	//bool check = false;
	BTreeNode* swap = NULL;
	BTreeNode* temp = NULL;

	if ( children[0] == NULL){// if no children, add node
		children[0] = newNode;
		count++;
		return;
	}


	else { //if there are children, swap the nodes from minimum to maximum
		temp = newNode;
		for ( int i = 0; i < count; i++){
			if( children[i]->getMinimum() > temp->getMinimum()){
				swap = children[i];
				children[i] = temp;
				temp = swap;
			}
		}

		children[count] = temp;
		count++;
	}

//NO Problem here
	count = 0;
	for ( int i = 0; i < internalSize; i++){// this is to store keys and also change parents
		if ( children[i] != NULL){
			count++;
			keys[i] = children[i]->getMinimum();
		}
	}

	for ( int i = count - 1; i > 0; i--){ //swap children from smallest to highest
		for (int j = i - 1; j > -1; j--){
			if (keys[i] < keys[j]){
				swap = children[j];
				children[j] = children[i];
				children[i] = swap;
			}
		}
	}


	for ( int i = 1; i < count; i++){
		// FIX ME: set the leftest children->leftSibling to the rightest children of left Sibling of this internal node;
		if (children[i] != NULL){
			children[i]->setLeftSibling(children[i - 1]);
			children[i - 1]->setRightSibling(children[i]);
		}
	}

  // students may write this
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()
