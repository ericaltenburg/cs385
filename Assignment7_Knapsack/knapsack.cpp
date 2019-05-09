/*
 * Name			: knapsack.cpp
 * Author		: Eric Altenburg
 * Date			: Apr 29, 2019
 * Description	: Uses dynamic programming to create best solution for solving knapsack problem.
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

struct Item { // Given through Canvas
	unsigned int item_number, weight, value;
	string description;

	explicit Item(const unsigned int item_number,
			const unsigned int weight,
			const unsigned int value,
			const string &description) :
					item_number(item_number),
					weight(weight),
					value(value),
					description(description) { }

	friend std::ostream& operator<<(std::ostream& os, const Item &item) {
		os << "Item " << item.item_number << ": " << item.description
				<< " (" << item.weight
				<< (item.weight == 1 ? " pound" : " pounds") << ", $"
				<< item.value << " )";
		os.flush();
		return os;
	}
};

/**
 * Helper for knapsack_algo. Takes in the weights and values
 */
int knapsack_algo_helper(int** matrix, int* item_weights, int* item_values, int i, int j) {
	if (matrix[i][j] == 0 && i >= 1 && j >= 1) {
		// Use pseudo code given in class as reference
		if (j >= item_weights[i]) {
			matrix[i][j] = max(knapsack_algo_helper(matrix, item_weights, item_values, i-1, j),
					item_values[i] + knapsack_algo_helper(matrix, item_weights, item_values, i-1, j - item_weights[i]));
		} else { // j < item_weights[i]
			matrix[i][j] = knapsack_algo_helper(matrix, item_weights, item_values, i-1, j);
		}
	}
	return matrix[i][j];
}

/**
 * Main algorithm that creates the end table and computes the final total amount
 * the thief can make
 * return - int of max profit
 * parameters - matrix, items, i, j
 */
int knapsack_algo(int** matrix, vector<Item> items, int i, int j) {
	// Convert the weights and values into arrays to pass onto the helper method
	int* item_weights = new int[items.size() + 1];
	item_weights[0] = 0; // Since starting at 0, they get 0.
	for (unsigned int i = 1; i < items.size() + 1; ++i) { // Add values
		item_weights[i] = items[i-1].weight;
	}

	int* item_values = new int[items.size() + 1];
	item_values[0] = 0; // Like weights, start at 0
	for (unsigned int j = 1; j < items.size() + 1; ++j) { // Add values
		item_values[j] = items[j-1].value;
	}

	int x = knapsack_algo_helper(matrix, item_weights, item_values, i, j);

	// Clear memory
	delete [] item_weights;
	delete [] item_values;

	return x;
}

/**
 * Helper method used to backtrack through the table
 */
vector<int> backtrack_helper(int** matrix, int* item_weights, int i, int j) {
	vector<int> items_stolen; // Where the items stolen are held

	while (i >= 1 && j >= 1) { // While the base case of the edges haven't been reached yet
		if (matrix[i][j] != matrix[i-1][j]) { // The value above it is not equal, so you used the item, add it
			items_stolen.push_back(i);
			j -= item_weights[i]; // move back by weight
			--i; // go up
		} else { // Item was not used as the value above it was equal
			--i; // go up
		}
	}

	return items_stolen;
}

/**
 * Used to backtrack after finding the max you can steal. Calls helper method which takes in different values.
 * This is really just an intermediate step.
 */
vector<int> backtrack(int** matrix, vector<Item> items, int i, int j) {
	int* item_weights = new int[items.size() + 1];
	item_weights[0] = 0; // Since starting at 0, they get 0.
	for (unsigned int i = 1; i < items.size() + 1; ++i) { // Add values
		item_weights[i] = items[i-1].weight;
	}

	vector<int> x = backtrack_helper(matrix, item_weights, i, j);

	// Clear memory
	delete [] item_weights;

	return x;
}

int main (int argc, char * const argv[]) {

	// Error Handling
	if (argc != 3) { // The number of arguments passed through is incorrect
		cerr << "Usage: " << argv[0] << " <capacity> <filename>" << endl;
		return 1;
	}

	istringstream iss;
	int cap;
	iss.str(argv[1]);
	if (!(iss >> cap) || cap < 0) { // The capacity is either not a number or it is negative
		cerr << "Error: Bad value '" << argv[1] << "' for capacity." << endl;
		return 1;
	}
	iss.clear();

	ifstream input_file(argv[2]);
	if (!input_file) { // This can also be used to see if a file was able to be opened
		cerr << "Error: Cannot open file '" << argv[2] << "'." << endl;
		return 1;
	}
	input_file.exceptions(ifstream::badbit); // Adds read errors to list of exceptions the ifstream will handle?

	string line;
	vector<Item> items;
	try {
		unsigned int line_number = 1; // Used to keep track of line numbers for error output
		while (getline(input_file, line)) {

			// TESTS FOR COMMAS
			if (line.find(",") > line.size()) { // If the first instance of a comma is greater than the size of the line, then it doesn't exist
				cerr << "Error: Line number " << line_number << " does not contain 3 fields." << endl;
				return 1;
			}

			// Since there was at least one comma present, that means the item name is there
			string item_name = line.substr(0, line.find(",")); // Stores the item name
			line = line.substr(line.find(",") + 1); // Creates a substring starting from 1 after the comma to the end

			if (line.find(",") > line.size()) {
				cerr << "Error: Line number " << line_number << " does not contain 3 fields." << endl;
				return 1;
			}

			// Since there was at least 2 commas, that means the item name and weight exist, so store it
			string item_weight = line.substr(0, line.find(",")); // Stores the item weight
			line = line.substr(line.find(",") + 1); // Rewrites the line string to be the end of the string after the second comma

			if (line.find(",") < INT_MAX) {
				cerr << "Error: Line number " << line_number << " does not contain 3 fields." << endl;
				return 1;
			}

			// Since third comma exists, then the value is also there. Store it
			string item_value = line; // Stores the item value

			// END TESTS FOR COMMAS

			// CHECK TO SEE IF THE WEIGHT IS A VALID WEIGHT

			int item_weight1;
			iss.str(item_weight);
			if (!(iss >> item_weight1) || item_weight1 < 0) { // Checks to see if integer, checks to see if negative. Can't have negative weight
				cerr << "Error: Invalid weight '" << item_weight << "' on line number " << line_number << "." << endl;
				return 1;
			}
			iss.clear();

			// CHECK TO SEE IF THE VALUE IS A VALID VALUE
			int item_value1;
			iss.str(item_value);
			if (!(iss >> item_value1) || item_value1 < 0) { // Checks to see if integer, checks to see if negative. Can't have negative value
				cerr << "Error: Invalid value '" << item_value << "' on line number " << line_number << "." << endl;
				return 1;
			}
			iss.clear();

			// After everything checks out, make a new item
			Item x(line_number, item_weight1, item_value1, item_name);
			// Add this new item to the list of items, or vector of items
			items.push_back(x);
			// Increment line_number
			++line_number;
		}
	} catch (const ifstream::failure &f) {
		cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
		return 1;
	}

	// Output according to script
	cout << "Candidate items to place in knapsack:" << endl;

	// Prints out items from the vector
	for (unsigned int i = 0; i < items.size(); ++i) {
		cout << "   Item " << items[i].item_number << ": " << items[i].description << " (" << items[i].weight << " ";
		// Check proper grammar
		if (items[i].weight == 1) { // Singular
			cout << "pound, $" << items[i].value << ")" << endl;
		} else {
			cout << "pounds, $" << items[i].value << ")" << endl;
		}
	}

	// Capacity
	if (cap == 1) { // Capacity is 1 pound, singular
		cout << "Capacity: " << cap << " pound" << endl;
	} else { // Plural
		cout << "Capacity: " << cap << " pounds" << endl;
	}

	// Matrix being used for dynamic programming
	int** matrix = new int* [items.size() + 1];
	for (unsigned int i = 0; i < items.size() + 1; ++i) {
		matrix[i] = new int [cap + 1]; // This was the creation of the row B like in WJP

		for (int j = 0; j < cap + 1; ++j) { // Initializes everything to 0;
			matrix[i][j] = 0;
		}
	}

	int amt_stolen = knapsack_algo(matrix, items, items.size(), cap); // Amount stolen

	vector<int> items_stolen = backtrack(matrix, items, items.size(), cap); // Store the items taken

	// Check if empty, if not print out all
	if (items_stolen.empty()) { // No items were stolen
		cout << "Items to place in knapsack: None" << endl;
	} else { // Items are in knapsack
		cout << "Items to place in knapsack:" << endl;

		// Have to reverse the vector as pushback put it in wrong order
		reverse(items_stolen.begin(), items_stolen.end());

		for (unsigned int i = 0; i < items_stolen.size(); ++i) {
			cout << "   Item " << items[items_stolen[i]-1].item_number << ": " << items[items_stolen[i]-1].description << " (" << items[items_stolen[i]-1].weight << " ";

			//Check proper grammar like above
			if (items[items_stolen[i]-1].weight == 1) { // Singular
				cout << "pound, $" << items[items_stolen[i]-1].value << ")" << endl;
			} else {
				cout << "pounds, $" << items[items_stolen[i]-1].value << ")" << endl;
			}
		}
	}

	cout << "Total weight: ";

	//Calculate total weight
	int total_weight = 0;
	for (unsigned int i = 0; i < items_stolen.size(); ++i) {
		total_weight += items[items_stolen[i]-1].weight;
	}

	cout << total_weight << " ";
	if (total_weight == 1) {
		cout << "pound" << endl;
	} else {
		cout << "pounds" << endl;
	}

	cout << "Total value : $" << amt_stolen << endl;

	// Delete to clear up memory to prevent leaks
	for (unsigned int i = 0; i < items.size() + 1; ++i) {
		delete [] matrix[i];
	}

	delete [] matrix;

	return 0;
}
