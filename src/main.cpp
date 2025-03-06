#include <iostream>
#include <sstream>
#include "raylib.h"
#include <chrono>
#include "BinTree.h"


void DrawNodePrefix(ptrNODE& node, int x, int y, int level);
void DrawNodePostfix(ptrNODE& node, int x, int y, int level);

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
		while (!WindowShouldClose())
		{
			BeginDrawing();
			ClearBackground(BLACK);

			// drawing tree
			if (tree.root != nullptr)
			{
				DrawNodePrefix(tree.root, screenWidth / 2, 50, 0);
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

			// drawing tree
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
	DrawCircle(x, y, 20, BLUE);
	DrawText(TextFormat("%c", node->info), x - 5, y - 10, 20, WHITE);
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

