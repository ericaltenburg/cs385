/*
 * Name			: quicksort.cpp
 * Author		: Eric Altenburg
 * Date			: Mar 15, 2019
 * Description	: Implementation of quick sort with Lomuto partitioning
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

static void swap(int array[], const int a, const int b) {
    // TODO

	int temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}


/*
 * The code for this came from an already written method found online to save time
 * https://www.geeksforgeeks.org/hoares-vs-lomuto-partition-scheme-quicksort/
 */
static int lomuto_partition(int array[], const int left, const int right) {
    // TODO
    int pivot = array[right];    // pivot
    int i = (left - 1);  // Index of smaller element

    for (int j = left; j <= right- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (array[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(array, i, j);

        }
    }
    swap(array, i+1, right);

    return (i + 1);

}

static void quicksort_helper(int array[], int left, int right) {
    // TODO

	if (left < right) {
		int index = lomuto_partition(array, left, right); //Finds index using the lomuto method

		quicksort_helper(array, left, index-1); // Calls the sort on the left
		quicksort_helper(array, index+1, right); // Calls the sort on right
	}
}

/**
 * Implements quicksort.
 * Calls the helper function that takes in left and right parameters.
 **/
void quicksort(int array[], int length) {
    quicksort_helper(array, 0, length - 1);
}

int main(int argc, char *argv[]) {
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
    quicksort(&values[0], values.size());
    // TODO - produce output

    ostringstream oss;
    if (values.size() == 0) {
    	cout << "Error: Sequence of integers not received." << endl;
    	return 0;
    }
	oss << "[";
	for (unsigned int i = 0; i < values.size(); ++i) {
		oss << values[i];
		if (i == values.size()-1) {
			oss << "]";
		} else {
			oss << ", ";
		}
	}


    cout << oss.str() << endl;

    return 0;
}
