#include "external/BinTree.h"

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