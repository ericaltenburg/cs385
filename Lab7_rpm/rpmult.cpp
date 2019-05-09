/*
 * Name			: rpmult.cpp
 * Author		: Eric Altenburg
 * Date			: Mar 29, 2019
 * Description	: Russian peasant multiplier
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;



unsigned long russian_peasant_multiplication(unsigned int m, unsigned int n) {
    unsigned long x = 0;

    // To fix the problems with very large numbers
    unsigned long newM = m;
    unsigned long newN = n;

    if (newM < newN) { // In the case where the smaller number isn't n, change the var vals
    	unsigned long temp = newM;
    	newM = newN;
    	newN = temp;
    }

    while (newN > 0) { // Loop so long as the n is still greater than 0
    	if(newN % 2 == 1) { // n is odd
    		x = x + newM;
    	}

    	newM = newM << 1; // Doubles m
    	newN = newN >> 1; // Divides n in half
    }

    return x;
}

int main(int argc, char* const argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <integer m> <integer n>" << endl;
        return 1;
    }

    int m, n;
    istringstream iss;

    iss.str(argv[1]);
    if ( !(iss >> m) || m < 0 ) {
        cerr << "Error: The first argument is not a valid nonnegative integer."
             << endl;
        return 1;
    }

    iss.clear();
    iss.str(argv[2]);
    if ( !(iss >> n) || n < 0 ) {
        cerr << "Error: The second argument is not a valid nonnegative integer."
             << endl;
        return 1;
    }

    // TODO - produce output
    cout << m << " x " << n << " = ";
    cout << russian_peasant_multiplication(m,n) << endl;

    return 0;
}
