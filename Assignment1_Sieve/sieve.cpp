/*
 * Name			: sieve.cpp
 * Author		: Eric Altenburg
 * Date			: Feb 11, 2019
 * Description	: Sieve of Eratosthenes
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
	public:
	PrimesSieve(int limit): is_prime_{new bool[limit + 1]}, limit_{limit} {
		sieve(); // sieve is called
	}

	~PrimesSieve() {
		delete [] is_prime_;
	}

	inline int num_primes() const {
		return num_primes_;
	}

	void display_primes() const {
		// TODO: write code to display the primes in the format specified in the
		// requirements document.

		const int max_prime_width = num_digits(max_prime_), primes_per_row = 80 / (max_prime_width + 1);
		int count_primes = 0;

		if (num_primes_ < primes_per_row) { // In the case that all the primes can fit on one row
			for (int i = 2; i <= limit_; ++i) {
				if (is_prime_[i] == true) { // A prime is found in the array
					if (count_primes != 0) { // The prime number is not the first prime number in the row
						cout << " ";
					}

					cout << i;
					++count_primes; //increment num of primes in row to show that it is no longer the first one
				}
			}
		} else { // In the case that all the primes cannot fit on one row
			for (int i = 2; i <= limit_; ++i) {
				if (count_primes == primes_per_row) { // The max amount of primes on one row has been reached
					cout << "\n"; // New line
					count_primes = 0; //reset the number of primes on row
				}

				if (is_prime_[i] == true) { // A prime num is found
					if (count_primes != 0) { // Same as above for case of first prime in row
						cout << " ";
					}

					cout << setw(max_prime_width) << i; //sets width of field to the max_prime_width
					++count_primes; //increment num of primes in row to show that it is no longer the first one and keep track of how many there are
				}
			}
		}

	}

private:
	bool * const is_prime_;
	const int limit_;
	int num_primes_, max_prime_;

	int count_num_primes() const {
		// TODO: write code to count the number of primes found
		int count = 0;

		for (int i = 2; i <= limit_;  ++i) { // Start at two as we don't want 0 or 1 to count
			if (is_prime_[i] == true) {
				++count;
			}
		}

		return count;
	}

	int num_digits(int num) const {
		// TODO: write code to determine how many digits are in an integer
		// Hint: No strings are needed. Keep dividing by 10.

		// No need to worry about whether or not 0 has 1 digit or not as we never start with 0, only 2

		int digits = 0;

		while (num != 0) {
			num /= 10;
			++digits;
		}

		return digits;
	}

	void sieve() { // in this call, the member variables are modified as all other methods cannot modify them

		for (int i = 0; i <= limit_; ++i) { // sets all values of array to true
			is_prime_[i] = true;
		}

		for (int i = 2; i <= sqrt(limit_); ++i) { // loops that change values in the array to false if not prime
			if (is_prime_[i] == true) {
				for (int j = i*i; j <= limit_; j+=i) {
					is_prime_[j] = false;
				}
			}
		}

		// Giving values to num_primes_ and max_prime_
		num_primes_ = count_num_primes();

		for (int i = limit_+1; i >= 2; --i) { // traverses the array backwards until a true value is found
			if (is_prime_[i] == true) {
				max_prime_ = i;
				break; // break because the loop will continue to go to the beginning of the array, stops at first true value
			}
		}
	}
};

int main() {
	cout << "**************************** " <<  "Sieve of Eratosthenes" <<
	            " ****************************" << endl;
	cout << "Search for primes up to: ";
	string limit_str;
	cin >> limit_str;
	int limit;

	// Use stringstream for conversion. Don't forget to #include <sstream>
	istringstream iss(limit_str);

	// Check for error.
	if ( !(iss >> limit) ) {
		cerr << "Error: Input is not an integer." << endl;
		return 1;
	}
	if (limit < 2) {
		cerr << "Error: Input must be an integer >= 2." << endl;
		return 1;
	}

	// TODO: write code that uses your class to produce the desired output.
	PrimesSieve s(limit);
	cout << endl;
	cout << "Number of primes found: " << s.num_primes() << endl;
	cout << "Primes up to " << limit << ":" << endl;
	s.display_primes();
	cout << endl;
	return 0;
}
