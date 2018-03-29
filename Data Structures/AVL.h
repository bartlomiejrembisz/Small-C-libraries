#ifndef AVL_H
#define AVL_H

#include <string>
#include <iostream>
#include <algorithm>
#include <assert.h>


template<typename K, typename I>
class AVL
{
public:
	using Key = K;
	using Item = I;


	void insert(Key, Item);
	void remove(Key);
	Item* lookup(Key);

	void printEntries();
	void printTree();


	friend std::ostream & operator<<(std::ostream &, const AVL &);



	AVL() = default;
	~AVL();

private:
	struct Node;
	static Item* lookupRec(Key, Node*);
	static void insertRec(Key, Item, Node*&);
	static void printEntriesRec(std::ostream &, Node*);
	static void printTreeRec(Node*, int);
	static void removeRec(Key, Node*&);
	static Node* detachMinimumNode(Node*&);
	static void rebalance(Node*&);

	static void rotateRight(Node* &);
	static void rotateLeft(Node* &);

	Node* root = nullptr;
};

template<typename K, typename I>
struct AVL<K, I>::Node
{
	Key key;
	Item item;
	short int balance = 0;

	Node* leftChild;
	Node* rightChild;

	Node(Key _key, Item _item)
	{
		key = _key;
		item = _item;
		leftChild = nullptr;
		rightChild = nullptr;
	}
};

template<typename K, typename I>
AVL<K, I>::~AVL()
{
}

template<typename K, typename I>
typename AVL<K, I>::Item* AVL<K, I>::lookupRec(Key _key, Node* current)
{
	if (current == nullptr)
		return nullptr;

	if (current->key == _key)
		return &current->item;
	else if (current->key < _key)
		return lookupRec(_key, current->rightChild);
	else
		return lookupRec(_key, current->leftChild);
}

//Return bool indicating whether 
template<typename K, typename I>
void AVL<K, I>::insertRec(Key _key, Item _item, Node* &current)
{
	if (current == nullptr)
		current = new Node(_key, _item);
	else if (current->key == _key)
		current->item = _item;
	else if (current->key < _key)
		insertRec(_key, _item, current->rightChild);
	else
		insertRec(_key, _item, current->leftChild);
}

template<typename K, typename I>
void AVL<K, I>::removeRec(Key _key, Node* &current)
{
	if (current == nullptr)
		return;
	else if (current->key == _key)
	{
		if (current->leftChild == nullptr && current->rightChild == nullptr)
		{
			delete current;
			current = nullptr;
		}
		else if (current->leftChild == nullptr || current->rightChild == nullptr)
		{
			Node* nonLeaf = (current->leftChild == nullptr) ?
				current->rightChild : current->leftChild;

			delete current;
			current = nonLeaf;
		}
		else
		{
			Node* bestFit = detachMinimumNode(current->rightChild);

			bestFit->leftChild = current->leftChild;
			bestFit->rightChild = current->rightChild;

			current = bestFit;
		}
	}
	else if (current->key < _key)
		removeRec(_key, current->rightChild);
	else
		removeRec(_key, current->leftChild);
}

template<typename K, typename I>
typename AVL<K, I>::Node* AVL<K, I>::detachMinimumNode(Node* &current)
{
	if (current->leftChild == nullptr)
	{
		Node* temp = current;
		current = current->rightChild;
		return temp;
	}

	return detachMinimumNode(current->leftChild);

}

template<typename K, typename I>
void AVL<K, I>::rebalance(Node* &localRoot)
{
	short int localBalance = localRoot->balance;
	short int rightChildBalance = localRoot->rightChild->balance;
	short int leftChildBalance = localRoot->leftChild->balance;

	if (localBalance == 2 && rightChildBalance == 1)
		rotateLeft(localRoot);

	if (localBalance == 2 && rightChildBalance == 0)
		rotateLeft(localRoot);

	if (localBalance == 2 && rightChildBalance == -1)
	{
		rotateRight(localRoot->rightChild);
		rotateLeft(localRoot);
	}

	if (localBalance == -2 && leftChildBalance == -1)
		rotateRight(localRoot);

	if (localBalance == -2 && leftChildBalance == 0)
		rotateRight(localRoot);

	if (localBalance == -2 && leftChildBalance == 1)
	{
		rotateLeft(localRoot->leftChild);
		rotateRight(localRoot);
	}
}

template<typename K, typename I>
void AVL<K, I>::rotateRight(Node* &localRoot)
{
	//Assertions before dereferencing a pointer
	Node* b = localRoot;
	assert(b != nullptr);
	

	Node* a = b->leftChild;
	assert(a != nullptr);

	b->leftChild = a->rightChild;
	a->rightChild = b;
	localRoot = a;

	//Update Node balance
	b->balance = b->balance + 1 + std::max(-a->balance, 0);
	a->balance = a->balance + 1 + std::max(b->balance, 0);
}

template<typename K, typename I>
void AVL<K, I>::rotateLeft(Node* &localRoot)
{
	//Assertions before dereferencing a pointer;
	Node* a = localRoot;
	assert(a != nullptr);

	Node* b = a->rightChild;
	assert(b != nullptr);

	a->rightChild = b->leftChild;
	b->leftChild = a;
	localRoot = b;

	//Update Node balance
	a->balance = a->balance - 1 - max(b->balance, 0);
	b->balance = b->balance - 1 - max(-a->balance, 0);
}

template<typename K, typename I>
void AVL<K, I>::insert(Key _key, Item _item)
{
	insertRec(_key, _item, root);
}

template<typename K, typename I>
void AVL<K, I>::remove(Key _key)
{
	removeRec(_key, root);
}

template<typename K, typename I>
typename AVL<K, I>::Item* AVL<K, I>::lookup(Key _key)
{
	return lookupRec(_key, root);
}

template<typename K, typename I>
void AVL<K, I>::printEntries()
{
	printEntriesRec(std::cout, root);
}

template<typename K, typename I>
void AVL<K, I>::printTree()
{
	printTreeRec(root, 0);
}

template<typename K, typename I>
void AVL<K, I>::printEntriesRec(std::ostream& os, Node* current)
{
	if (current == nullptr)
		return;
	printEntriesRec(os, current->leftChild);
	os << current->key << " " << current->item << " (" << current->balance << ")" << std::endl;
	printEntriesRec(os, current->rightChild);
}

template<typename K, typename I>
void AVL<K, I>::printTreeRec(Node* current, int indent)
{
	if (current == nullptr)
		return;
	std::cout << std::string(indent, '\t') << current->key << " " <<
		((current->leftChild == nullptr && current->rightChild == nullptr) ? "*" : " ") <<
		current->item << " (" << current->balance << ")" << std::endl;
	printTreeRec(current->leftChild, indent + 1);
	printTreeRec(current->rightChild, indent + 1);
}

template<typename K, typename I>
std::ostream & operator<<(std::ostream &os, const AVL<K, I> &AVL)
{
	AVL<K, I>::printEntriesRec(os, AVL.root);
	return os;
}



#endif