#include "bstree.h"

//constructor, this initializes root and loads data from file
BST::BST() : root(nullptr) {
	loadFromFile();
}
//Destructor, this frees all dynamically allocated nodes
BST::~BST() {
	destroyTree(root);
}
//recursively deletes all nodes (post-order)
void	BST::destroyTree(BSTNode* node) {
	if (node == nullptr) return;
	destroyTree(node->left);
	destroyTree(node->right);
	delete node;
}
//LoadingFrom file, this reads item.txt at startup and inserts each item into BST
//the format per line: ID, NAME, ZONE, AISLE, SHELF
void BST::loadFromFile() {
	std::ifstream inFile(FILE_NAME);

	if (!inFile.is_open()) {
		std::cout << "Warning!, Could not open " << FILE_NAME << ". Initialized with empty DB..\n";
		return;
	}

	std::string line;
	while (std::getline(inFile, line)) {
		if (line.empty()) continue;

		Item item;
		std::stringstream ss(line);
		std::getline(ss, item.id, '|');
		std::getline(ss, item.name, '|');
		std::getline(ss, item.zone, '|');
		std::getline(ss, item.aisle, '|');
		std::getline(ss, item.shelf, '|');

		//insert directly intotree without triggering saveToFile during load
		root = insertHelper(root, item);
	}
	inFile.close();
}
//SAVE TO FILE, this rtrwrites the entier item.txt using in-order traversal
//ensuers file always reflect the present staft of BST. Called after every CRUD OP
void BST::saveToFile() {
	std::ofstream outFile(FILE_NAME);

	if (!outFile.is_open()) {
		std::cout << "Error!, Could not write to " << FILE_NAME << ".\n";
		return;
	}

	saveToFileHelper(root, outFile);
	outFile.close();
}
//the in-irder traversal writes items sorted by ID to File
void BST::saveToFileHelper(BSTNode* node, std::ofstream& outFile) {
	if (node == nullptr) return;
	saveToFileHelper(node->left, outFile);
	outFile << node->data.id << "|"
		<< node->data.name << "|"
		<< node->data.zone << "|"
		<< node->data.aisle << "|"
		<< node->data.shelf << "\n";
	saveToFileHelper(node->right, outFile);
}
//INSERT, this adds a new item into BST based on ITEM ID, rejects doubl ID
//Time complexity: O(Log n) average, O(n) worst case= skewed tree
BSTNode* BST::insertHelper(BSTNode* node, Item item) {
	//base scenario, if empty spot found creates new node here.
	if (node == nullptr) return new BSTNode(item);

	//Comparing IDS to decide left ot right subtrees
	if (item.id < node->data.id)
		node->left = insertHelper(node->left, item);
	else if (item.id > node->data.id)
		node->right = insertHelper(node->right, item);
	return node;
}
void BST::insert(Item item) {
	//rejects if ID already exists
	if (searchByIDHelper(root, item.id) != nullptr) {
		std::cout << "Error!, Item ID " << item.id << " already exists. Use update instead.\n";
		return;
	}
	root = insertHelper(root, item);
	saveToFile();
	std::cout << "Success! Item " << item.id << " inserted successfully.\n";
}
//Searching By item ID. This travereses the BST using ID comparisons
//without needing to visit everynode it has a time complexity of O(log n)
BSTNode* BST::searchByIDHelper(BSTNode* node, const std::string& id) {
	//base case - not found or exact match
	if (node == nullptr) return nullptr;
	if (id == node->data.id) return node;

	//goes left if ID is smallerm right if larger
	if (id < node->data.id) return searchByIDHelper(node->left, id);
	return searchByIDHelper(node->right, id);
}

void BST::searchByID(const std::string& id) {
	BSTNode* result = searchByIDHelper(root, id);
	
	if (result == nullptr) {
		std::cout << "Not Found!, no item with ID: " << id << "\n";
		return;
	}

	std::cout << "\n--- Item Found ---\n";
	std::cout << "ID     : " << result->data.id << "\n";
	std::cout << "Name   : " << result->data.name << "\n";
	std::cout << "Zone   : " << result->data.zone << "\n";
	std::cout << "Aisle  : " << result->data.aisle << "\n";
	std::cout << "Shelf  : " << result->data.shelf << "\n";
}
//SEARCHing by name, must visit every node as BST is not ordered by name
//time complexity= O(n), meaning full tree ttraversal required
void BST::searchByNameHelper(BSTNode* node, const std::string& name, bool& found) {
	if (node == nullptr) return;
	//visits left substree
	searchByNameHelper(node->left, name, found);

	//checking current node
	if (node->data.name == name) {
		std::cout << "\n--- Item Found ---\n";
		std::cout << "ID     : " << node->data.id << "\n";
		std::cout << "Name   : " << node->data.name << "\n";
		std::cout << "Zone   : " << node->data.zone << "\n";
		std::cout << "Aisle  : " << node->data.aisle << "\n";
		std::cout << "Shelf  : " << node->data.shelf << "\n";
		found = true;
	}
	//visiting right subtree
	searchByNameHelper(node->right, name, found);
}
void BST::searchByName(const std::string& name) {
	bool found = false;
	searchByNameHelper(root, name, found);
	if (!found) {
		std::cout << "Not Found!, no item with name: " << name << "\n";
	}
}
//UPDATE, Finds the item by ID, LETS THE USER CHOOSE WHAT FIELD TO UPDATE
// and then saves updated tree into the file immediately after change
void BST::update(const std::string& id) {
	BSTNode* node = searchByIDHelper(root, id);

	if (node == nullptr) {
		std::cout << "Not Found!, No item with ID: " << id << "\n";
		return;
	}


	std::cout << "\nCurrent details for " << id << ":\n";
	std::cout << "1. Name   : " << node->data.name << "\n";
	std::cout << "2. Zone   : " << node->data.zone << "\n";
	std::cout << "3. Aisle  : " << node->data.aisle << "\n";
	std::cout << "4. Shelf  : " << node->data.shelf << "\n";
	std::cout << "\nEnter field number to update (1-4): ";

	int choice;
	std::cin >> choice;
	std::cin.ignore();

	std::string newValue;
	std::cout << "Enter new value: ";
	std::getline(std::cin, newValue);

	switch (choice) {
	case 1: node->data.name = newValue; break;
	case 2: node->data.zone = newValue; break;
	case 3: node->data.aisle = newValue; break;
	case 4: node->data.shelf = newValue; break;
	default:
		std::cout << "Error!, Invalid field choice.\n";
		return;
	}

	saveToFile();
	std::cout << "Success!, Item " << id << " updated successfully.\n";

	}
//REMOVE, Deletes a node from the BST, this handles the 3 all 3 remove cases:
	//case1: lead node, simple delete
	//case2: one child, bypass node and link paren to child
	//case3: two children, replace with in order successor, which is smallest value in right subtree and then delete the successor
	//TIme compleity= O(log n) average

	//findMin - traverses left until leftmost node which is the smallest Id
BSTNode* BST::findMin(BSTNode* node) {
	while (node->left != nullptr)
		node = node->left;
	return node;
}
BSTNode* BST::removeHelper(BSTNode* node, const std::string& id, bool& removed) {
	if (node == nullptr) return nullptr;
	
	if (id < node->data.id) {
		//Targfet os om ;eft subtree
		node->left = removeHelper(node->left, id, removed);
	}
	else if (id > node->data.id) {
		//Target is in right subtree
		node->right = removeHelper(node->right, id, removed);
	}
	else {
		//Node found handles 3 deletion cases
		removed = true;

		//Cas1: leafe node - no children
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			return nullptr;
		}
		//case2a only right child
		else if (node->left == nullptr) {
			BSTNode* temp = node->right;
			delete node;
			return temp;
		}
		//case2b onlyu lefgt child
		else if (node->right == nullptr) {
			BSTNode* temp = node->left;
			delete node;
			return temp;
		}
//case3 where 2 children. find in order successor smallest in right subtree
// 		   // copy successor data into current node and then delete successor node turning into cas1 or 2

		else {
			BSTNode* successor = findMin(node->right);
			node->data = successor->data;
			node->right = removeHelper(node->right, successor->data.id, removed);
		}
	}
	return node;
}

void BST::remove(const std::string& id) {
	bool removed = false;
	root = removeHelper(root, id, removed);

	if (!removed) {
		std::cout << "Not Found!, no item with ID: " << id << "\n";
		return;
	}
	saveToFile();
	std::cout << "Success!, item " << id << " removed successfully.\n";
}
//Displaying Sorted, in order traversal naturally produces output sorted by ID
//left -> root -> right gurantees ascending ID order
// time complexity = O(n) visiting every node
void BST::displaySortedHelper(BSTNode* node) {
	if (node == nullptr) return;
	displaySortedHelper(node->left);
	std::cout << "  " << node->data.id
		<< "\t| " << node->data.name
		<< "\t| " << node->data.zone
		<< "\t| " << node->data.aisle
		<< "\t| " << node->data.shelf << "\n";
	displaySortedHelper(node->right);
}

void BST::displaySorted() {
	if (root == nullptr) {
		std::cout << "[INFO] Item database is empty.\n";
		return;
	}

	std::cout << "\n========================================\n";
	std::cout << "        WAREHOUSE ITEM DATABASE\n";
	std::cout << "========================================\n";
	std::cout << "  ID\t\t| Name\t\t\t| Zone\t\t| Aisle\t\t| Shelf\n";
	std::cout << "  -----------------------------------------------------------------------\n";
	displaySortedHelper(root);
	std::cout << "========================================\n";
}
// 
// GET LOCATION
// Returns formatted location string for Task 3 integration
// Commented out - Task 3 does not require this currently
//
std::string BST::getLocation(const std::string& id) {
	BSTNode* node = searchByIDHelper(root, id);
	if (node == nullptr) return "ITEM NOT FOUND";
	return node->data.zone + ", " + node->data.aisle + ", " + node->data.shelf;
}
