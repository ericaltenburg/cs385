/*
 * Name			: gcd.cpp
 * Author		: Eric Altenburg
 * Date			: Jan 30, 2019
 * Description	: gcd program between 2 numbers
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

int gcd_iterative(int m, int n) {
	//if there is a negative number
	if(m < 0) {
		m=abs(m);
	}
	if (n < 0) {
		n = abs(n);
	}

	//in the case where one of the variable is a 0
	if (m == 0) {
		return n;
	} else if (n == 0) {
		return m;
	}

	while (m != n) { //subtract from each other until m is equal to n
		if (m > n) {
			m-=n;
		} else {
			n-=m;
		}
	}

	return m;
}

int gcd_recursive(int m, int n) {
	//if there is a negative number
	if(m < 0) {
		m=abs(m);
	}
	if (n < 0) {
		n = abs(n);
	}

	if(n == 0) {
		return m;
	}

	return gcd_recursive(n, m % n);
}

int main(int argc, char *argv[]) {
	int num1, num2;
	istringstream iss;

	if (argc != 3) { //What does this do?
		cerr << "Usage: " << argv[0] << " <integer m> <integer n>"  << endl;
		return 1;
	}
	iss.str(argv[1]);
	if (!(iss >> num1 )) {
		cerr << "Error: The first argument is not a valid integer." << endl;
		return 1;
	}
	iss.clear();
	iss.str(argv[2]);
	if (!(iss >> num2 )) {
		cerr << "Error: The second argument is not a valid integer." << endl;
		return 1;
	}

	cout << "Iterative: gcd(" << num1 << ", " << num2 << ") = " << gcd_iterative(num1, num2) <<endl;
	cout << "Recursive: gcd(" << num1 << ", " << num2 << ") = " << gcd_recursive(num1, num2) <<endl;

	return 0;
}
