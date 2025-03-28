#pragma once

#include "hash_tabels/LPHashTable.h"
#include "lists/DynamicArray.h"
#include "Pair.h"
#include "Sort.h"
#include "trees/BinaryTree.h"
#include "queues/AQueue.h"
#include "heaps/MaxHeap.h"
#include "lists/linked_lists/SLinkedList.h"

#include <iostream>
#include <fstream>
#include <sstream>
#define BYTE_SIZE 8

/**
 * @brief Handle and provide huffman prefix encoding to ASCII chars only (char value of 0 to 127).
*/
class Huffman
{
public:
	/**
	 * @brief A struct holding a char and its frequency in some text. 
	 * Comparisons between pairs are comparisons between their frequencies.
	*/
	struct HuffmanPair
	{
		char chr;
		int freq;
		bool init;

		HuffmanPair()
		{
			// both values should not be negative
			// a negative char or freq means that this pair is nt usable
			chr = -1;
			freq = -1;
			init = false;
		}

		HuffmanPair(char c, int f)
		{
			if (c < 0 || f < 0)
			{
				throw std::invalid_argument("char and its frequency must not be negative numbers");
			}

			chr = c;
			freq = f;
		}

		bool operator<(HuffmanPair& other)
		{
			if (!init)
			{
				throw std::logic_error("this pair is not initialized");
			}

			if (!other.init)
			{
				throw std::logic_error("other pair is not initialized");
			}

			return freq < other.freq;
		}

		bool operator>(HuffmanPair& other)
		{
			if (!init)
			{
				throw std::logic_error("this pair is not initialized");
			}

			if (!other.init)
			{
				throw std::logic_error("other pair is not initialized");
			}

			return freq > other.freq;
		}

		bool operator<=(HuffmanPair& other)
		{
			if (!init)
			{
				throw std::logic_error("this pair is not initialized");
			}

			if (!other.init)
			{
				throw std::logic_error("other pair is not initialized");
			}

			return freq <= other.freq;
		}

		bool operator>=(HuffmanPair& other)
		{
			if (!init)
			{
				throw std::logic_error("this pair is not initialized");
			}

			if (!other.init)
			{
				throw std::logic_error("other pair is not initialized");
			}

			return freq >= other.freq;
		}

		bool operator==(HuffmanPair& other)
		{
			if (!init)
			{
				throw std::logic_error("this pair is not initialized");
			}

			if (!other.init)
			{
				throw std::logic_error("other pair is not initialized");
			}

			return freq == other.freq;
		}

		bool operator!=(HuffmanPair& other)
		{
			if (!init)
			{
				throw std::logic_error("this pair is not initialized");
			}

			if (!other.init)
			{
				throw std::logic_error("other pair is not initialized");
			}

			return freq != other.freq;
		}
	};

	/**
	 * @brief A struct that represents a node in a huffman tree.
	 * Comparisons between trees are comparisons between their pairs' frequencies.
	*/
	struct HuffmanTree
	{
		HuffmanTree* left;	// the left child, its frequency is less that of the right child
		HuffmanTree* right;	// the right child, its frequency is more that of the left child
		HuffmanPair pair;	// the char and its frequency in this node
		int size;			// the number of letters/signs (NOT nodes) in the tree
		int height;			// the height of the tree
		std::string code;	// the code for this node (binary string)

		HuffmanTree()
		{
			left = nullptr;
			right = nullptr;

			size = 0;	// although a single node usually has size 1, an uninitialized node has size 0
			height = 0;
		}

		/**
		 * @brief make sure that the size of the tree is right, based on the sizes of its children.
		*/
		void updateSize()
		{
			size = left ? left->size : 0;
			size += right ? right->size : 0;
		}

		/**
		 * @brief make sure that the height of the tree is right, based on the height of its children.
		*/
		void updateHeight()
		{
			int leftHeight = left ? left->height : -1;
			int rightHeight = right ? right->height : -1;

			height = leftHeight < rightHeight ? rightHeight + 1 : leftHeight + 1;
		}

		/**
		 * @brief add a letter to the beginning of the code of this node and all of its predecessors
		 * @param rightStep is this a step to the right (1)?
		*/
		void addStep(bool rightStep)
		{
			code.insert(0, 1, '0' + rightStep);

			if (left)
			{
				left->addStep(rightStep);
			}

			if (right)
			{
				right->addStep(rightStep);
			}
		}

		/**
		 * @brief is this a leaf?
		 * @return true iff this node is a leaf (has no children).
		*/
		bool isLeaf()
		{
			return left == nullptr && right == nullptr;
		}

		bool operator<(HuffmanTree& other)
		{
			return pair.freq < other.pair.freq;
		}

		bool operator>(HuffmanTree& other)
		{
			return pair.freq > other.pair.freq;
		}

		bool operator<=(HuffmanTree& other)
		{
			return pair.freq <= other.pair.freq;
		}

		bool operator>=(HuffmanTree& other)
		{
			return pair.freq >= other.pair.freq;
		}

		bool operator==(HuffmanTree& other)
		{
			return pair.freq == other.pair.freq;
		}

		bool operator!=(HuffmanTree& other)
		{
			return pair.freq != other.pair.freq;
		}
	};

	/**
	 * @brief A class that represents a hufffman coding table.
	*/
	class HuffmanCode
	{
	public:
		/**
		 * @brief get the code of some letter.
		 * @param chr 
		 * @return the code (a string) of the letter chr.
		*/
		std::string getCode(char chr)
		{
			return code_->get(chr);
		}

		/**
		 * @brief check if a letter is coded.
		 * @param chr
		 * @return true if chr is a letter in the code.
		*/
		bool hasCode(char chr)
		{
			return code_->search(chr) != nullptr;
		}

		/**
		 * @return get the alphabet (string) encoded in this table.
		*/
		std::string getAlphabet()
		{
			return alphabet_;
		}

		friend class Huffman;

	private:
		LPHashTable<char, std::string>* code_;	// a hash table for fast search
		std::string alphabet_;					// the alphabet encoded in the table
		HuffmanTree* tree_;						// the tree that corresponds to the coding
	};

	/**
	 * @brief get the frequency of each char in the text.
	 * @param text 
	 * @return a pair of the size of the alphabet and an array of HuffmanPairs.
	*/
	static Pair<int, HuffmanPair*> getFrequencies(std::string text)
	{
		HuffmanPair* frequenciesArray = nullptr;	
		LPHashTable<char, int> frequenciesTable;	// a hash table where the key is a char and the data is its frequency in the text
		Pair<char, int>* pair = nullptr;
		std::string alphabet;						// the alphabet in the text, meaning all the letters in the text, each appearing once

		for (char c : text)
		{
			// try to search the char in the table.
			// if found, increment the frequency in the text,
			// else - add it to the alphabet and to the table.

			pair = frequenciesTable.search(c);

			if (pair)
			{
				pair->setData(pair->getData() + 1);
			}
			else
			{
				alphabet += c;
				frequenciesTable.insert(c, 1);
			}
		}

		// insert all the pairs to the array.

		frequenciesArray = new HuffmanPair[alphabet.size()];

		for (unsigned i = 0; i < alphabet.size(); i++)
		{
			frequenciesArray[i].chr = alphabet[i];
			frequenciesArray[i].freq = frequenciesTable.get(alphabet[i]);
			frequenciesArray[i].init = true;
		}

		return Pair<int, HuffmanPair*>(alphabet.size(), frequenciesArray);
	}

	/**
	 * @brief build a huffman tree from a given list of frequencies.
	 * @param freq 
	 * @return a pointer to a HuffmanTree corresponding to the given set of frequencies.
	*/
	static HuffmanTree* createTree(Pair<int, HuffmanPair*> freq)
	{
		MaxHeap<HuffmanPair>::heapSort(freq.getData(), freq.getKey());

		// TODO: maybe hust use the MinHeap, check what's faster

		return createTreeFromSorted(freq.getData(), freq.getKey());
	}

	/**
	 * @brief build a huffman tree from a sorted list of frequencies.
	 * @param freq a sorted list of frequencies.
	 * @return a pointer to a HuffmanTree corresponding to the given set of frequencies.
	*/
	static HuffmanTree* createTreeFromSorted(HuffmanPair* sortedFreqArr, int alphabetSize)
	{
		// the idea is to use two queues and the fact that the list is sorted.
		// in each step find the two least frequent nodes and combine themand return to the queues.

		AQueue<HuffmanTree*> queue1(alphabetSize), queue2(alphabetSize);
		HuffmanTree* minNodes[2];
		HuffmanTree* newNode, *currNode;
		int remaining = alphabetSize;

		// insert all the pairs to the first queue

		for (int i = 0; i < alphabetSize; i++)
		{
			newNode = new HuffmanTree();
			newNode->pair = sortedFreqArr[i];
			newNode->size = 1;					// a leaf node (contains a sign) is a tree with 1 sign
			queue1.enqueue(newNode);
		}

		while (remaining > 1)
		{
			// find the two nodes with the minimal frequency
			for (int i = 0; i < 2; i++)
			{
				// find the current minimum
				// in case of a tie, choose the first queue

				if (queue2.isEmpty())
				{
					minNodes[i] = queue1.dequeue();
				}
				else if (queue1.isEmpty())
				{
					minNodes[i] = queue2.dequeue();
				}
				else if (queue1.peek()->pair <= queue2.peek()->pair)
				{
					minNodes[i] = queue1.dequeue();
				}
				else
				{
					minNodes[i] = queue2.dequeue();
				}
			}

			// merge the 2 to 1 node, a default sign of '*' and sum the two frequencies

			newNode = new HuffmanTree();

			newNode->left = minNodes[0];	// lower frequency to the left	
			newNode->right = minNodes[1];

			newNode->pair = HuffmanPair('*', minNodes[0]->pair.freq + minNodes[1]->pair.freq);

			newNode->updateSize();
			newNode->updateHeight();

			newNode->right->addStep(true);
			newNode->left->addStep(false);

			queue2.enqueue(newNode);
			remaining--;
		}

		currNode = queue1.isEmpty() ? queue2.dequeue() : queue1.dequeue();

		if (alphabetSize == 1)
		{
			newNode = new HuffmanTree();

			newNode->left = currNode;	

			newNode->pair = HuffmanPair('*', currNode->pair.freq);

			newNode->updateSize();
			newNode->updateHeight();

			newNode->left->addStep(false);

			currNode = newNode;
		}

		return currNode;
	}

	/**
	 * @brief build a huffman tree from a given list of frequencies.
	 * @param freqArr an array of pairs.
	 * @param alphabetSize the size of the array.
	 * @return a pointer to a HuffmanTree corresponding to the given set of frequencies.
	*/
	static HuffmanTree* createTree(HuffmanPair* freqArr, int alphabetSize)
	{
		MaxHeap<HuffmanPair>::heapSort(freqArr, alphabetSize);

		// TODO: maybe hust use the MinHeap, check what's faster

		return createTreeFromSorted(freqArr, alphabetSize);
	}

	/**
	 * @brief create a huffman coding for a given text.
	 * @param text 
	 * @return a pointer to HuffmanCode for the given text.
	*/
	static HuffmanCode* getHuffmanCode(std::string text)
	{
		Pair<int, HuffmanPair*> freq = getFrequencies(text);

		HuffmanTree* tree = createTree(freq);

		return getHuffmanCode(tree);
	}

	/**
	 * @brief create a huffman coding for a given tree.
	 * @param tree 
	 * @return a pointer to HuffmanCode for the given tree.
	*/
	static HuffmanCode* getHuffmanCode(HuffmanTree* tree)
	{
		// a hash table where the key is a char and the data is its coding
		LPHashTable<char, std::string>* codesTable = new LPHashTable<char, std::string>(tree->size);
		LQueue<HuffmanTree*> queue;
		HuffmanTree* current;
		HuffmanCode* code = new HuffmanCode;

		// insert all the leaves in the tree to the queue and then then to the table

		queue.enqueue(tree);

		while (!queue.isEmpty())
		{
			current = queue.dequeue();

			if (current->left)
			{
				queue.enqueue(current->left);
			}

			if (current->right)
			{
				queue.enqueue(current->right);
			}

			if (current->isLeaf())
			{
				codesTable->insert(current->pair.chr, current->code);
				code->alphabet_ += current->pair.chr;
			}
		}

		code->code_ = codesTable;
		code->tree_ = tree;

		return code;
	}

	/**
	 * @brief encode a given text using a given coding.
	 * @param text 
	 * @param code 
	 * @return a string of encoded unsigned chars.
	*/
	static std::string encode(std::string text, HuffmanCode* code)
	{
		std::string bits;
		std::string encodedText;
		char newChar;

		encodedText += std::to_string(text.size());
		encodedText += '#';

		for (char c : text)
		{
			// add the next bits to the array
			for (char t : code->getCode(c))
			{
				bits += t;
			}

			// each character code is a string.
			// each char in the string is either 0 or 1.
			// when reaching (probably) 8 characters, we can create one new character instead.
			// the string "00110110" can turn into a char with value 54.

			// while we have byte size set of chars, write it
			while (bits.size() >= BYTE_SIZE * sizeof(char))
			{
				newChar = 0;

				for (unsigned i = 0; i < BYTE_SIZE * sizeof(char); i++)
				{
					newChar *= 2;
					newChar += bits[i] - '0';
				}

				encodedText += newChar;

				bits.erase(0, BYTE_SIZE * sizeof(char));
			}
		}

		if (!bits.empty())
		{
			newChar = 0;

			for (unsigned i = 0; i < bits.size(); i++)
			{
				newChar *= 2;
				newChar += bits[i] - '0';
			}

			for (unsigned i = bits.size(); i < BYTE_SIZE * sizeof(char); i++)
			{
				newChar *= 2;
			}

			encodedText += newChar;
		}

		return encodedText;
	}

	/**
	 * @brief encode a given text.
	 * @param text
	 * @return a string of encoded unsigned chars.
	*/
	static std::string encode(std::string text)
	{
		auto freq = getFrequencies(text);

		int arrSize = freq.getKey();

		HuffmanPair* arr = freq.getData();

		MaxHeap<HuffmanPair>::heapSort(arr, arrSize);

		auto tree = createTree(arr, arrSize);

		auto code = getHuffmanCode(tree);

		return encode(text, code);
	}

	/**
	 * @brief encode a file into another file.
	 * @param srcFilePath source file, to encode.
	 * @param dstFilePath destination file, write the encoded text here.
	*/
	static HuffmanCode* encode(std::string srcFilePath, std::string dstFilePath)
	{
		std::ifstream srcFile;
		std::ofstream dstFile;
		std::stringstream strStream;
		HuffmanCode* code;
		
		srcFile.open(srcFilePath);
		strStream << srcFile.rdbuf();
		srcFile.close();

		code = getHuffmanCode(strStream.str());

		std::string encoded = encode(strStream.str(), code);

		dstFile.open(dstFilePath, std::ios::trunc);
		dstFile.write(encoded.c_str(), encoded.size());
		dstFile.close();

		return code;
	}

	/**
	 * @brief decode a text, using a given code.
	 * @param text 
	 * @param code 
	 * @return a string of decoded chars.
	*/
	static std::string decode(std::string text, HuffmanCode* code)
	{
		HuffmanTree* current = code->tree_;
		std::string decoded;
		SLinkedList<bool> steps;
		size_t textSize = 0;
		unsigned i = 0;

		// start by learning how many letters are in the text
		while (text[i] != '#')
		{
			textSize *= 10;
			textSize += text[i] - '0';
			i++;
		}

		i++;

		while(i < text.size() && decoded.size() < textSize)
		{
			unsigned char c = text[i];

			// add the steps in reversed order
			for (unsigned i = 0; i < 8 * sizeof(char); i++)
			{
				steps.addFirst(c % 2);
				c /= 2;
			}

			// do them
			while (!steps.isEmpty() && decoded.size() < textSize)
			{
				if (steps.removeFirst())
				{
					current = current->right;
				}
				else
				{
					current = current->left;
				}

				if (current->isLeaf())
				{
					decoded += current->pair.chr;
					current = code->tree_;
				}
			}

			i++;
		}

		return decoded;
	}

	static void decode(std::string srcFilePath, std::string dstFilePath, HuffmanCode* code)
	{
		std::ifstream srcFile;
		std::ofstream dstFile;
		std::stringstream strStream;

		std::string decoded;
		std::string text;

		srcFile.open(srcFilePath);
		strStream << srcFile.rdbuf();
		srcFile.close();

		text = strStream.str();
		decoded = decode(text, code);

		dstFile.open(dstFilePath, std::ios::trunc);
		dstFile.write(decoded.c_str(), decoded.size());
		dstFile.close();
	}
};

