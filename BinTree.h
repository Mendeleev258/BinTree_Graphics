#include <iostream>
#include <sstream>

using TInfo = char;

struct NODE
{
	TInfo info;
	bool repeat;
	NODE* left, * right;
	NODE(TInfo info = 0, bool repeat = false, NODE* ptr_left = nullptr, NODE* ptr_right = nullptr)
	{
		this->info = info;
		this->repeat = false;
		left = ptr_left;
		right = ptr_right;
	}
	~NODE()
	{
		clear();
	}
	void add(TInfo key);
	void clear();
	void print(int level);
	void removeNode(TInfo key, ptrNODE& node);
};

using ptrNODE = NODE*;

struct BinTree
{
	ptrNODE root;
	BinTree();
	BinTree(std::stringstream& ss);			 // search
	~BinTree();
	void clear();
	void print();
	void removeNode(TInfo key);
	void removeRepeats();
};