/*******************************************************************************
 * Name          : sqrt.cpp
 * Author        : Brian S. Borowski
 * Version       : 1.0
 * Date          : January 23, 2019
 * Last modified : February 10, 2019
 * Description   : Computes the square root of a number via Newton's method.
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <sstream>
using namespace std;

const double EPSILON = 1e-7;

double sqrt(double num, double epsilon) {
    if (num < 0) {
        return numeric_limits<double>::quiet_NaN();
    }
    if (num == 0 || num == 1) {
        return num;
    }

    double next_guess = num, last_guess;
    do {
        last_guess = next_guess;
        next_guess = 0.5 * (last_guess + num / last_guess);
    } while (abs(last_guess - next_guess) > epsilon);
    return next_guess;
}



int main(int argc, char* const argv[]) {
    if (argc == 1 || argc > 3) {
        cerr << "Usage: " << argv[0] << " <value> [epsilon]" << endl;
        return 1;
    }

    double num = 0.0, epsilon = EPSILON;
    istringstream iss;
    iss.str(argv[1]);
    if ( !(iss >> num) ) {
        cerr << "Error: Value argument must be a double." << endl;
        return 1;
    }

    if (argc == 3) {
        iss.clear();
        iss.str(argv[2]);
        if ( !(iss >> epsilon) || epsilon <= 0 ) {
            cerr << "Error: Epsilon argument must be a positive double."
                 << endl;
            return 1;
        }
    }

    cout << fixed << setprecision(8) << sqrt(num, epsilon) << endl;
    return 0;
}
