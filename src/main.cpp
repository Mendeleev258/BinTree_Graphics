#include <iostream>
#include <fstream>
#include <sstream>
#include "raylib.h"
#include <chrono>

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
	void removeNode(TInfo key, NODE*& node);
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

void DrawNodePrefix(ptrNODE& node, int x, int y, int level)
{
	if (node == nullptr) return;

	// draw node
	if (node->repeat)
		DrawCircle(x, y, 20, GREEN);
	else
		DrawCircle(x, y, 20, BLUE);
	DrawText(TextFormat("%c", node->info), x - 5, y - 10, 20, WHITE);

	// left subtree
	if (node->left != nullptr)
	{
		DrawLine(x, y + 20, x - 100, y + 120, WHITE);
		DrawNodePrefix(node->left, x - 100, y + 120, level + 1);
	}

	// right subtree
	if (node->right != nullptr)
	{
		DrawLine(x, y + 20, x + 100, y + 120, WHITE);
		DrawNodePrefix(node->right, x + 100, y + 120, level + 1);
	}
}

void DrawNodePostfix(ptrNODE& node, int x, int y, int level)
{
	if (node == nullptr) return;

	// left subtree
	if (node->left != nullptr)
	{
		DrawLine(x, y + 20, x - 100, y + 120, WHITE);
		DrawNodePostfix(node->left, x - 100, y + 120, level + 1);
	}

	// right subtree
	if (node->right != nullptr)
	{
		DrawLine(x, y + 20, x + 100, y + 120, WHITE);
		DrawNodePostfix(node->right, x + 100, y + 120, level + 1);
	}

	// draw node
	if (node->repeat)
		DrawCircle(x, y, 20, GREEN);
	else
		DrawCircle(x, y, 20, BLUE);
	DrawText(TextFormat("%c", node->info), x - 5, y - 10, 20, WHITE);
}

void collectKeys(ptrNODE& root, std::string& keys)
{
	if (root)
	{
		if (root->repeat)
			keys += root->info;
		if (root->left)
			collectKeys(root->left, keys);
		if (root->right)
			collectKeys(root->right, keys);
	}
}

template<typename T, typename Predicat>
void validation(T& x, Predicat condition, const char* message);
std::string removeRepeats(const std::string& str);


int main()
{
	bool exitFlag{};
	const int screenWidth = 800;
	const int screenHeight = 800;
	do
	{
		std::string inputStr{};
		validation(inputStr, [](std::string x) {return true; }, "Enter your string");
		std::stringstream ss(inputStr);
		BinTree tree(ss);
		InitWindow(screenWidth, screenHeight, "Binary Tree");
		// tree.removeRepeats();
		while (!WindowShouldClose())
		{
			BeginDrawing();
			ClearBackground(BLACK);

			// Рисуем дерево
			if (tree.root != nullptr)
			{
				DrawNodePostfix(tree.root, screenWidth / 2, 50, 0);
			}
			EndDrawing();
		}
		CloseWindow();
		
		bool next{};
		validation(next, [](bool x) { return x == true; }, "Enter '1' to delete repeats");
		InitWindow(screenWidth, screenHeight, "Binary Tree Task");

		auto startTimeTree = std::chrono::high_resolution_clock::now();
		tree.removeRepeats();
		auto endTimeTree = std::chrono::high_resolution_clock::now();
		auto elapsedTimeTree = std::chrono::duration_cast<std::chrono::microseconds>(endTimeTree - startTimeTree).count();

		auto startTimeStr = std::chrono::high_resolution_clock::now();
		std::string outputStr = removeRepeats(inputStr);
		auto endTimeStr = std::chrono::high_resolution_clock::now();
		auto elapsedTimeStr = std::chrono::duration_cast<std::chrono::microseconds>(endTimeStr - startTimeStr).count();

		while (!WindowShouldClose())
		{
			BeginDrawing();
			ClearBackground(BLACK);

			// Рисуем дерево
			if (tree.root != nullptr)
			{
				DrawNodePostfix(tree.root, screenWidth / 2, 50, 0);
			}
			EndDrawing();
		}
		CloseWindow();

		std::cout << "Your string without repeated chars: " << outputStr << '\n';
		std::cout << "Tree algorithm time: " << elapsedTimeTree << " ms\n";
		std::cout << "String algorithm time: " << elapsedTimeStr << " ms\n";

		validation(exitFlag, [](bool x) { return true; }, "Exit? 1 - yes, 0 - no");

	} while (!exitFlag);
	
}



void NODE::add(TInfo elem)
{
	if (elem == info)
	{
		repeat = true;
		return;
	}
	if (elem < info)
	{
		if (left == nullptr)
			left = new NODE(elem);
		else
			left->add(elem);
	}
	else
	{
		if (right == nullptr)
			right = new NODE(elem);
		else
			right->add(elem);
	}
}

void NODE::clear()
{
	if (left)
	{
		delete left;
		left = nullptr;
	}
	if (right)
	{
		delete right;
		right = nullptr;
	}
}

void NODE::print(int level)
{
	if (left)
		left->print(level + 1);
	for (int i{}; i < level; ++i)
		std::cout << "   ";
	std::cout << info << '\n';
	if (right)
		right->print(level + 1);
}


void NODE::removeNode(TInfo key, ptrNODE& node)
{
	if (node == nullptr) return;

	if (key < node->info)
		removeNode(key, node->left);
	else if (key > node->info)
		removeNode(key, node->right);
	else // Найден узел для удаления
	{
		// Узел без детей
		if (node->left == nullptr && node->right == nullptr)
		{
			delete node;
			node = nullptr;
		}
		// Узел с одним ребенком (левым)
		else if (node->left != nullptr && node->right == nullptr)
		{
			ptrNODE temp = node;
			node = node->left;
			temp->left = nullptr; // Предотвращаем удаление ребенка в деструкторе
			temp->right = nullptr;
			delete temp;
		}
		// Узел с одним ребенком (правым)
		else if (node->left == nullptr && node->right != nullptr)
		{
			ptrNODE temp = node;
			node = node->right;
			temp->left = nullptr; // Предотвращаем удаление ребенка в деструкторе
			temp->right = nullptr;
			delete temp;
		}
		// Узел с двумя детьми
		else
		{
			// Найти минимальный элемент в правом поддереве
			ptrNODE minNode = node->right;
			ptrNODE parentMinNode = nullptr; // Родитель минимального узла
			while (minNode->left != nullptr)
			{
				parentMinNode = minNode;
				minNode = minNode->left;
			}

			node->info = minNode->info;

			// Удаление минимального узла
			if (parentMinNode != nullptr)
			{
				parentMinNode->left = minNode->right;
			}
			else
			{
				node->right = minNode->right;
			}
			minNode->left = nullptr; // Предотвращаем удаление детей в деструкторе
			minNode->right = nullptr;
			delete minNode;
		}
	}
}

void BinTree::removeNode(TInfo key)
{
	if (root != nullptr)
		root->removeNode(key, root);
}

void BinTree::removeRepeats()
{
	if (root == nullptr) return;

	std::string keys{};
	collectKeys(root, keys);

	for (size_t i{}; i < keys.length(); ++i)
		removeNode(keys[i]);
}


BinTree::BinTree()
{
	root = nullptr;
}

BinTree::BinTree(std::stringstream& ss) // search
{
	TInfo elem{};
	root = nullptr;
	while (ss.get(elem))
		if (!root) 
			root = new NODE(elem);
		else 
			root->add(elem);
}

BinTree::~BinTree()
{
	clear();
}

void BinTree::clear()
{
	if (root)
	{
		root->clear();
		root = nullptr;
	}
}

void BinTree::print()
{
	if (!root) std::cout << "Tree is empty!\n";
	else root->print(0);
}

template<typename T, typename Predicat>
void validation(T& x, Predicat condition, const char* message)
{
	std::cout << message << "\n>>> ";
	while (!(std::cin >> x && condition(x)))
	{
		std::cout << "Input error!" << '\n';
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cout << message << "\n>>> ";
	}
}

std::string removeRepeats(const std::string& str) {
	std::string result;
	for (size_t i = 0; i < str.length(); ++i) {
		bool isRepeat = false;
		for (size_t j = 0; j < str.length(); ++j) {
			if (i != j && str[i] == str[j]) {
				isRepeat = true;
				break;
			}
		}
		if (!isRepeat) {
			result += str[i];
		}
	}
	return result;
}

