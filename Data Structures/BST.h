#ifndef BST_H
#define BST_H

#include <string>
#include <iostream>


template<typename K, typename I>
class BST
{
public:
	using Key = K;
	using Item = I;


	void insert(Key, Item);
	void remove(Key);
	Item* lookup(Key);

	void printEntries();
	void printTree();


	friend std::ostream & operator<<(std::ostream &, const BST<K, I> &);


	BST() = default;
	~BST();

private:
	struct Node;
	static Item* lookupRec(Key, Node*);
	static void insertRec(Key, Item, Node*&);
	static void printEntriesRec(std::ostream &, Node*);
	static void printTreeRec(Node*, int);
	static void removeRec(Key, Node*&);
	static Node* detachMinimumNode(Node*&);
	static void deepDelete(Node*);

	static void rotateRight(Node* &);
	static void rotateLeft(Node* &);

	Node* root = nullptr;
};

template<typename K, typename I>
struct BST<K, I>::Node
{
	Key key;
	Item item;

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
BST<K, I>::~BST()
{
	deepDelete(root);
}

template<typename K, typename I>
typename void BST<K, I>::deepDelete(Node* current)
{
	if (current == nullptr)
		return;

	deepDelete(current->leftChild);
	deepDelete(current->rightChild);
	delete current;
}

template<typename K, typename I>
typename BST<K, I>::Item* BST<K, I>::lookupRec(Key _key, Node* current)
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

template<typename K, typename I>
void BST<K, I>::insertRec(Key _key, Item _item, Node* &current)
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
void BST<K, I>::removeRec(Key _key, Node * &current)
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
typename BST<K, I>::Node* BST<K, I>::detachMinimumNode(Node* &current)
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
void BST<K, I>::rotateRight(Node* &localRoot)
{
	//assertions before dereferencing a pointer
	Node* b = localRoot;
	assert(b != nullptr);

	Node* a = b->leftChild;
	assert(a != nullptr);

	b->leftChild = a->rightChild;
	a->rightChild = b;
	localRoot = a;
}

template<typename K, typename I>
void BST<K, I>::rotateLeft(Node* &localRoot)
{
	//assertions before dereferencing a pointer;
	Node* a = localRoot;
	assert(a != nullptr);

	Node* b = a->rightChild;
	assert(b != nullptr);

	a->rightChild = b->leftChild;
	b->leftChild = a;
	localRoot = b;
}

template<typename K, typename I>
void BST<K, I>::insert(Key _key, Item _item)
{
	insertRec(_key, _item, root);
}

template<typename K, typename I>
void BST<K, I>::remove(Key _key)
{
	removeRec(_key, root);
}

template<typename K, typename I>
typename BST<K, I>::Item* BST<K, I>::lookup(Key _key)
{
	return lookupRec(_key, root);
}

template<typename K, typename I>
void BST<K, I>::printEntries()
{
	printEntriesRec(std::cout, root);
}

template<typename K, typename I>
void BST<K, I>::printTree()
{
	printTreeRec(root, 0);
}

template<typename K, typename I>
void BST<K, I>::printEntriesRec(std::ostream& os, Node* current)
{
	if (current == nullptr)
		return;
	printEntriesRec(os, current->leftChild);
	os << current->key << " " << current->item << std::endl;
	printEntriesRec(os, current->rightChild);
}

template<typename K, typename I>
void BST<K, I>::printTreeRec(Node *current, int indent)
{
	if (current == nullptr)
		return;
	std::cout << std::string(indent, '\t') << current->key << " " <<
		((current->leftChild == nullptr && current->rightChild == nullptr) ? "*" : " ") <<
		current->item << std::endl;
	printTreeRec(current->leftChild, indent + 1);
	printTreeRec(current->rightChild, indent + 1);
}

template<typename K, typename I>
std::ostream & operator<<(std::ostream &os, const BST<K, I> &bst)
{
	BST<K, I>::printEntriesRec(os, bst.root);
	return os;
}


#endif