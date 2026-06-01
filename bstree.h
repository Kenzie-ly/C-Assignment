#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//Item struct, this holds all detail of singular warehouse items
struct Item {
	std::string id;
	std::string name;
	std::string zone;
	std::string aisle;
	std::string shelf;
};

//BSTNode struct, this is a singular node in the Binary Search Tree
//Each node stores one item and pointers to left/right child

struct BSTNode {
	Item data;
	BSTNode* left;
	BSTNode* right;

	BSTNode(Item item) : data(item), left(nullptr), right(nullptr) {}
};

//BST class, this is to manage all item storage nd retrieval operations
class BST {
private:
	BSTNode* root; //entry
	const std::string FILE_NAME = "items.txt";

	//private recursive helper functions
	BSTNode* insertHelper(BSTNode* node, Item item);
	BSTNode* searchByIDHelper(BSTNode* node, const std::string& id);
	void	searchByNameHelper(BSTNode* node, const std::string& name, bool& found);
	void	displaySortedHelper(BSTNode* node);
	BSTNode* removeHelper(BSTNode* node, const std::string& id, bool& removed);
	BSTNode* findMin(BSTNode* node);
	void saveToFileHelper(BSTNode* node, std::ofstream& outFile);
	void destroyTree(BSTNode* node);

public:
	BST(); ~BST();

	//file operations
	void loadFromFile();
	void saveToFile();

	//core Module OPerations
	void insert(Item item);
	void searchByID(const std::string& id);
	void searchByName(const std::string& name);
	void update(const std::string& id);
	void remove(const std::string& id);
	void displaySorted();

	//initial interface created for providing location string for task3, commented out for possible later use.
	std::string getLocation(const std::string& id);
};



