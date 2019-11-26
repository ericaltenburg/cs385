/*
 * Name			: commonwordfinder.cpp
 * Author		: Eric Altenburg
 * Date			: May 3, 2019
 * Description	: Finds the most frequent words in text file using red black tree
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include "rbtree.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

/*
 * Compare method to see which node has the highest value instead of key. Will be used for stable_sort
 * return - bool if it's bigger or not
 */
bool val_compare(pair<string, int> a, pair<string, int> b) {
	return a.second > b.second;
}

/**
 * num_digits from seive.cpp which calculates how many digits there are in a number by
 * continually dividing by 10.
 */
int num_digits(int num) {
	// TODO: write code to determine how many digits are in an integer
	// Hint: No strings are needed. Keep dividing by 10.

	int digits = 0;

	while (num != 0) {
		num /= 10;
		++digits;
	}

	return digits;
}

/**
 * Prints out the vector in the specified format
 */
void print_vect(vector< pair<string, int> > vect, unsigned int limit, int longest_length, int longest_freq) {
	// First, have to find the longest word in vector for the setw in output

	for (unsigned int i = 0; i < limit; ++i) { // Iterate through vector
		if (vect[i].first.length() > (unsigned) longest_length) { // For formatting the output
			longest_length = vect[i].first.length();
		}
	}

	// Finding out how many digits in highest number of frequencies
	longest_freq = num_digits(longest_freq);

	// Figure out how many digits in the total number of words being printed
	int limit_digits = num_digits(limit);

	// Printing
	for (unsigned int i = 0; i < limit; ++i) {
		cout << right << setw(limit_digits) << i + 1 << left << setw(1) << ". " << left << setw(longest_length+1) << vect[i].first << left << vect[i].second << endl;
	}
}

/**
 * main method
 */
int main(int argc, char * const argv[]) {

	// Usage Error
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <filename> [limit]" << endl;
		return 1;
	}

	// Checks limit
	int limit = 10; // Stores frequencies to be output
	istringstream iss;
	if (argc == 3) { // Limit was specified
		int templimit;
		iss.str(argv[2]);
		if (!(iss >> templimit) || templimit < 0) { // if nan or negative
			cerr << "Error: Invalid limit '" << argv[2] << "' received." << endl;
			return 1;
		}

		limit = templimit;
	}

	// Seeing if file can be opened
	ifstream input_file(argv[1]);
	if (!input_file) { // File wasn't able to opened
		cerr << "Error: Cannot open file '" << argv[1] << "' for input." << endl;
		return 1;
	}
	input_file.exceptions(ifstream::badbit); // Taken from knapsack and canvas for opening files

	/*
	 * Since it can be opened, we can start going through the file and adding stuff to the rbt.
	 * The only accenbtable characters in a word are letters, hyphens, and single quotes.
	 * Letters are converted to lower case
	 * If string has length 0 after stripping out the unacceptable characters, don't insert.
	 */

	RedBlackTree <string, int> *tree = new RedBlackTree <string, int>();
	char character;
	string line;
	try {
		while(input_file.get(character)) {
			// Test the characters, must be lowercase, a hyphen, or a single quote. If uppercase, convert to lower
			if ((character > 96 && character < 123) || character == 45 || character == 39) {
				// This is a valid character so add it to the string
				line+=character;
			} else if (character > 64 && character < 91) {
				// This character is uppercase, change it to lower case
				line+=tolower(character);
			} else {
				// It passed the tests, and is "sanitized" now insert it into the rbt
				if (line.length() != 0) { // Making sure it isn't an empty word
					RedBlackTree<string, int>::iterator it = tree->find(line);

					if (it == tree->end()) { // Not in tree, insert
						pair<string, int> newNode(line, 1);
						tree->insert(it, newNode);
					} else { // Already exists, increment
						it->set_value(it->value() + 1);
					}
				}

				line = "";
			}
		}
	} catch (const ifstream::failure &f) {
		cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
		return 1;
	}

	// Create the vector that will hold the words after the inorder traversal
	vector < pair<string, int> > vect;

	// Inorder trav
	typename RedBlackTree<string, int>::iterator it = tree->begin();
	while (it != tree->end()) {
		pair<string, const int> temp (it->key(), it->value());
		vect.push_back(temp);
		++it;
	}

	// Sort the words with stable_sort to preserve the alphabetical order and use custom compare that uses the values not keys
	stable_sort(vect.begin(), vect.end(), val_compare);

	// Begin writing desired output based on script
	cout << "Total unique words: " << vect.size() << endl;

	int longest_length = vect[0].first.length();
	int largest_freq = vect[0].second;

	if ((unsigned) limit > vect.size()) { // Print entire vector
		print_vect(vect, vect.size(), longest_length, largest_freq);
	} else { // The limit is less than the vector, so you don't print the entire thing
		print_vect(vect, limit, longest_length, largest_freq);
	}

	// Free up memory
	delete tree;

	return 0;
}
