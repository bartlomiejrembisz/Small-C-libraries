#include <string>
#include <functional>
#include <iostream>
#include <algorithm>

#ifndef DICTIONARY_H
#define DICTIONARY_H

namespace Containers
{
	template<typename K, typename I>
	class Dictionary
	{
	public:
		using Key = K;
		using Item = I;

		void insert(Key, Item);
		Item* lookup(Key) const;
		void remove(Key);
		void printEntries() const;
		void clear();
		bool empty() const;
		size_t size() const;
		void swap(Dictionary&);
		void sort(std::function<bool(Key, Key)> comparison = std::greater<Key>);

		Dictionary() = default;
		Dictionary(Dictionary &&);
		Dictionary(const Dictionary &);
		~Dictionary();

		Dictionary &operator=(const Dictionary &);
		Dictionary &operator=(Dictionary &&);

		template<typename K, typename I>
		friend std::ostream& operator<<(std::ostream &, const Dictionary<K, I> &);

		//Static comparison functions for the user
		static bool ascending(const Item, const Item);
		static bool descending(const Item, const Item);

	private:
		struct Node;
		Node* root = nullptr;
		size_t listSize = 0;

		static Item* lookupRec(Key, Node*);
		static bool insertRec(Key, Item, Node*&);
		static bool removeRec(Key, Node*&);
		static void deepDelete(Node*);
		static Node* deepCopy(Node*);
		static void printEntriesRec(std::ostream&, Node*);
	};

	//Node struct definiton
	template<typename K, typename I>
	struct Dictionary<K, I>::Node
	{
		Key key;
		Item item;

		Node* nextNode;

		Node(Key _key, Item _item)
		{
			key = _key;
			item = _item;
			nextNode = nullptr;
		}

		Node(const Node &original)
		{
			key = original.key;
			item = original.item;
			nextNode = nullptr;
		}
	};

	//Move constructor
	template<typename K, typename I>
	Dictionary<K, I>::Dictionary(Dictionary<K, I> &&original)
	{
		this->root = original.root;
		original.root = nullptr;
		this->listSize = original.listSize;
		original.listSize = 0;
	}

	//Copy constructor
	template<typename K, typename I>
	Dictionary<K, I>::Dictionary(const Dictionary<K, I> &original)
	{
		this->root = deepCopy(original.root);
		this->listSize = original.listSize;
	}

	//Destructor
	template<typename K, typename I>
	Dictionary<K, I>::~Dictionary()
	{
		deepDelete(root);
	}

	//Copy assignment operator
	template<typename K, typename I>
	Dictionary<K, I> & Dictionary<K, I>::operator=(const Dictionary<K, I> &original)
	{
		if (this == &original)
			return *this;
		else
		{
			if (this->root != nullptr)
				deepDelete(root);
			this->root = deepCopy(original.root);
			this->listSize = original.listSize;
		}
		return *this;
	}

	//Move assignment operator
	template<typename K, typename I>
	Dictionary<K, I> & Containers::Dictionary<K, I>::operator=(Dictionary<K, I> &&original)
	{
		if (this == &original)
			return *this;
		else
		{
			if (this->root != nullptr)
				deepDelete(this->root);
			this->root = original.root;
			original.root = nullptr;
			this->listSize = original.listSize;
			original.listSize = 0;
		}
	}

	//Comparison functions
	template<typename K, typename I>
	bool Containers::Dictionary<K, I>::ascending(const Item a, const Item b)
	{
		return a > b;
	}

	template<typename K, typename I>
	bool Containers::Dictionary<K, I>::descending(const Item a, const Item b)
	{
		return a < b;
	}

	//Insert wrapper function
	template<typename K, typename I>
	void Dictionary<K, I>::insert(Key _key, Item _item)
	{
		if (insertRec(_key, _item, root)) listSize++; //Increment the listSize variable as the list size has increased after the insertion
	}

	//Lookup wrapper function
	template<typename K, typename I>
	typename Dictionary<K, I>::Item* Dictionary<K, I>::lookup(Key _key) const
	{
		return lookupRec(_key, root);
	}

	//Remove wrapper function
	template<typename K, typename I>
	void Dictionary<K, I>::remove(Key _key)
	{
		if (removeRec(_key, root)) listSize--; //Decrement the listSize variable as the list size has decreased after the removal
	}

	template<typename K, typename I>
	void Containers::Dictionary<K, I>::printEntries() const
	{
		printEntriesRec(std::cout, root);
	}

	//Clear the list
	template<typename K, typename I>
	void Containers::Dictionary<K, I>::clear()
	{
		deepDelete(root);
		root = nullptr;
		listSize = 0;
	}

	//Check if list is empty
	template<typename K, typename I>
	bool Containers::Dictionary<K, I>::empty() const
	{
		return (root == nullptr);
	}

	//Return the size of the list
	template<typename K, typename I>
	size_t Containers::Dictionary<K, I>::size() const
	{
		return listSize;
	}

	//Swapping two dictionaries
	template<typename K, typename I>
	void Containers::Dictionary<K, I>::swap(Dictionary<K, I> &original)
	{
		Node* tempNode = this->root;
		this->root = original.root;
		original.root = tempNode;

		unsigned int tempSize = this->listSize;
		this->listSize = original.listSize;
		original.listSize = tempSize;
	}

	//Bubble sort algorithm utilising higher order functions for comparison
	//Algorithm made available by Lakshay Chawla on https://stackoverflow.com/questions/19556427/sorting-a-singly-linked-list-with-pointers
	//and modified by me in order to utilise higher order functions
	template<typename K, typename I>
	void Containers::Dictionary<K, I>::sort(std::function<bool(Key, Key)> comparison)
	{
		Node *i = root, *j = root;
		while (i != nullptr) {
			while (j->nextNode != nullptr) {

				if (comparison(j->item, j->nextNode->item)) {
					Item tempData = j->item;
					Key tempKey = j->key;
					j->item = j->nextNode->item;
					j->key = j->nextNode->key;

					j->nextNode->item = tempData;
					j->nextNode->key = tempKey;
				}
				j = j->nextNode;
			}
			j = root;
			i = i->nextNode;
		}
	}

	//Lookup worker function
	template<typename K, typename I>
	typename Dictionary<K, I>::Item* Dictionary<K, I>::lookupRec(Key _key, Node* _current)
	{
		if (_current == nullptr)
			return nullptr;
		else if (_current->key == _key)
			return &_current->item;
		else
			return lookupRec(_key, _current->nextNode);
	}

	//Insert worker function
	template<typename K, typename I>
	bool Dictionary<K, I>::insertRec(Key _key, Item _item, Node* &_current)
	{
		if (_current == nullptr)
		{
			_current = new Node(_key, _item);
			return true; //Return true if new node is added in order to increment the size variable;
		}
		else if (_current->key == _key)
			_current->item = _item;
		else
			return insertRec(_key, _item, _current->nextNode);

		return false;
	}

	//Remoce worker funciton
	template<typename K, typename I>
	bool Dictionary<K, I>::removeRec(Key _key, Node* &_current)
	{
		if (_current == nullptr)
			return false;

		else if (_key == _current->key)
		{
			if (_current->nextNode == nullptr)
			{
				delete _current;
				_current = nullptr;
			}

			else
			{
				Node* temp = _current;
				_current = temp->nextNode;
				delete temp;
			}
			return true;
		}
		else
			return removeRec(_key, _current->nextNode);

		return false;
	}

	//Deep delete recursive function
	template<typename K, typename I>
	void Containers::Dictionary<K, I>::deepDelete(Node* _current)
	{
		if (_current == nullptr)
			return;

		deepDelete(_current->nextNode);
		delete _current;
	}

	//Deep copy recursive function
	template<typename K, typename I>
	typename Dictionary<K, I>::Node* Containers::Dictionary<K, I>::deepCopy(Node* original)
	{
		if (original == nullptr)
			return nullptr;

		Node* tempNode = new Node(*original);
		tempNode->nextNode = deepCopy(original->nextNode);
		return tempNode;
	}

	//Printing entries recursive worker function
	template<typename K, typename I>
	void Containers::Dictionary<K, I>::printEntriesRec(std::ostream &os, Node* _current)
	{
		if (_current == nullptr)
			return;
		os << _current->key << " " << _current->item << std::endl;
		printEntriesRec(os, _current->nextNode);
	}

	//operator<< overloading to transfer the entries into the stream
	template<typename K, typename I>
	std::ostream& operator<< (std::ostream &os, const Dictionary<K, I> &dictionary)
	{
		Dictionary<K, I>::printEntriesRec(os, dictionary.root);
		return os;
	}
};
#endif // !DICTIONARY_H