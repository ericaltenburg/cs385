/*
 * Name			: unique.cpp
 * Author		: Eric Altenburg
 * Date			: Feb 17, 2019
 * Description	: Determining uniqueness of chars with int as bit vector.
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
	// TODO: returns true if all characters in string are lowercase
	// letters in the English alphabet; false otherwise

	for (char const& c : s) { // travels through the string letter by letter
		bool isLowercase = islower(c);

		if (!isLowercase) { // checks to see if the letter is lowercase and returns accordingly
			return false;
		}
	}

	return true;
}

bool all_unique_letters(const string &s) {
	// TODO: returns true if all letters in string are unique, that is
	// no duplicates are found; false otherwise.
	// You may use only a single int for storage and work with bitwise
	// and bitshifting operators.
	// No credit will be given for other solutions.

	unsigned int x = 0;

	for (auto const& c : s) { // travels through the string letter by letter like in above method
		int diff = c - 'a';

		if (((x >> diff) & 1) != 0) { // the letter has been seen before
			return false;
		} else { // the letter hasn't been seen before so change x to show the effects of most recent letter
			x = x | (1 << diff);
		}
	}

	return true;
}

int main(int argc, char * const argv[]) {
	// TODO: reads and parses command line arguments.
	// Calls other functions to produce correct output.

	// istringstream iss;


	if (argc != 2) { // either too many or too little arguments have been entered
		cerr << "Usage: " << argv[0] << " <string>" << endl;
		return 1;
	}
	// iss.str(argv[1]); // because no #include <sstream> ?

	string s = argv[1];
	if (!is_all_lowercase(s)) { // is not in lowercase or not all letters of English alphabet
		cerr << "Error: String must contain only lowercase letters." << endl;
		return 1;
	} else { // passes the is_all_lowercase() method
		if (all_unique_letters(s)) { // all of the letters in string s are unique
			cout << "All letters are unique." << endl;
		} else { // there are duplicates found in string s
			cout << "Duplicate letters found." << endl;
		}
	}
	return 0;
}
