/*
 * Name			: test.cpp
 * Author		: Eric Altenburg
 * Date			: Jan 28, 2019
 * Description	:
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <iostream>
#include <sstream>

using namespace std;

int max (int m, int n) {
	return m > n? m : n;
}

int main(int argc, char *argv[]) {
	int m, n;
	istringstream iss;

	if (argc != 3) {
		cerr << "Usage: " << argv[0] << " <integer m> <integer n>"  << endl;
		return 1;
	}
	iss.str(argv[1]);
	if (!(iss >> m )) {
		cerr << "Error: The first argument is not a valid integer." << endl;
		return 1;
	}
	iss.clear();
	iss.str(argv[2]);
	if (!(iss >> n )) {
		cerr << "Error: The second argument is not a valid integer." << endl;
		return 1;
	}
	cout << "max(" << m << ", " << n << ") = " << max(m,n) << endl;
	return 0;
}
