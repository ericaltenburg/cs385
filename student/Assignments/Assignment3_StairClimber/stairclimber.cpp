/*
 * Name			: stairclimber.cpp
 * Author		: Eric Altenburg
 * Date			: Feb 21, 2019
 * Description	: Lists the number of ways to climb n stairs.
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
	// TODO: Return a vector of vectors of ints representing
	// the different combinations of ways to climb num_stairs
	// stairs, moving up either 1, 2, or 3 stairs at a time.

	vector <vector<int> > ways = {}; // Initializing empty vector of vector of ints

	if (num_stairs <= 0) { // Base case
		ways.push_back({});
	} else { // Base case was not reached yet
		for (int i = 1; i < 4; ++i) {
			if (num_stairs >= i) {
				vector< vector<int> > result = get_ways(num_stairs-i);

				for(unsigned int j = 0; j < result.size(); ++j) {
					result[j].insert(result[j].begin(), i);
				}

				ways.insert(ways.end(), result.begin(), result.end()); // Insert result at the end of ways iterating from beginning to end of result
			}
		}
	}
	return ways;
}

void display_ways(const vector< vector<int> > &ways) {
	// TODO: Display the ways to climb stairs by iterating over
	// the vector of vectors and printing each combination.

	int maxw = ways.size(), digits = 0;

	while (maxw != 0) { // Just like in sieve
		maxw /= 10;
		++digits;
	}

	for (unsigned int i = 0; i < ways.size(); ++i) {
		cout << setw(digits) << i+1 << ". ["; // Number corresponding to the order

		int x = 0;
		for (unsigned int j = 0; j < ways[i].size()-1; ++j) { // The one order of stairs climbed
			cout << ways[i][j] << ", ";
			++x;
		}

		cout << ways[i][x] << "]" << endl; // Closing off that specific row of orders
	}
}

int main(int argc, char * const argv[]) {
	istringstream iss;
	int value;

	if (argc == 1) { // There are no input arguments or there are too many
		cerr << "Usage: " << argv[0] << " <number of stairs>" << endl;
		return 1;
	}
	if (argc > 2) { // There are more than two arguments
		cerr << "Usage: " << argv[0] << " <number of stairs>" << endl;
		return 1;
	}

	iss.str(argv[1]); // Convert the string to an integer stored in value

	if (!(iss >> value)) { // The input value is a bad input (not a number)
		cerr << "Error: Number of stairs must be a positive integer." << endl;
		return 1;
	}

	if (value <= 0) { // The input is either 0 stairs or negative
		cerr << "Error: Number of stairs must be a positive integer." << endl;
		return 1;
	}

	vector< vector<int> > v = get_ways(value); // New vector of vector of ints housing the possible ways

	// Grammar
	if (v[0].size() == 1) { // There is only one way to climb the stairs so singular is used in output
			cout << v.size() << " way to climb " << v[0].size() << " stair." << endl;
		} else { // There is more than one stair that can be climbed
			cout << v.size() << " ways to climb " << v[0].size() << " stairs." << endl;
	}
	display_ways(v);

	return 0;
}
