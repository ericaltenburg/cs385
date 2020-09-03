/*
 * Name			: inversioncounter.cpp
 * Author		: Eric Altenburg
 * Date			: Mar 27, 2019
 * Description	: Makes use of mergesort to solve a seemingly unrelated problem to count the number of inversions in an array.
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in theta(n^2) time.
 */
long count_inversions_slow(int array[], int length) {
    // TODO

	long tot_inv = 0; // long that will hold the total number of inversions found

	// Nested loops that iterate through the array comparing the values to each other until an inversion is found
	for (long i = 0; i < length; ++i) {
		for (long j = i+1; j < length; ++j) {
			if (array[i] > array[j]) { // If the value of i is greater than j, then an inversion exists
				++tot_inv;
			}
		}
	}

	return tot_inv;
}

/**
 * Counts the number of inversions in an array in theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    // TODO
    // Hint: Use mergesort!

	long tot_inv = 0;
	int *scratch = new int[length];
	tot_inv = mergesort(array, scratch, 0, length-1);

	delete []scratch; // Deallocates array since not in use anymore

	return tot_inv;
}

static long mergesort(int array[], int scratch[], int low, int high) {
    // TODO

	long tot_inv = 0; // Holds total inversions found

	if (low == high) { // There is only one element in the array (base case)
		return 0;
	} else {
		int mid = (low + high) / 2; // Midpoint of array

		tot_inv = tot_inv + mergesort(array, scratch, low, mid); // Call on left
		tot_inv = tot_inv + mergesort(array, scratch, mid+1, high); // Call on right

		// Begins the modified merging algorithm covered in lecture 3/12/19

		int L = low;
		int H = mid + 1;

		for (int i = low; i <= high; ++i) {
			if (L <= mid && (H > high || array[L] <= array[H])) {
				scratch[i] = array[L];
				++L;
			} else {
				tot_inv = tot_inv + (mid + 1 - L);
				scratch[i] = array[H];
				++H;
			}
		}

		for (int i = low; i <= high; ++i) { // Copy scratch over to array
			array[i] = scratch[i];
		}

		return tot_inv;
	}
}

int main(int argc, char *argv[]) {
    // TODO: parse command-line argument

	if (argc > 2) { // There was more arguments than necessary
		cout << "Usage: " << argv[0] << " [slow]" << endl;
		return 1;
	}

	if (argc == 2 && strcmp(argv[1], "slow") != 0) { // There was an argument entered, but it might not be slow
		cout << "Error: Unrecognized option '" << argv[1] << "'." << endl;
		return 1;
	}

    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    if (values.empty()) { // There were no values entered, only spaces
    	cout << "Error: Sequence of integers not received." << endl;
    	return 1;
    }

    // TODO: produce output

    cout << "Number of inversions: ";
    if (argc == 2) { // There are two arguments being entered, meaning slow
    	cout << count_inversions_slow(&values[0], values.size()) << endl;
    } else { // Fast version
    	cout << count_inversions_fast(&values[0], values.size()) << endl;
    }

    return 0;
}
