/*
 * Name			: waterjugpuzzle.cppp
 * Authors		: Eric Altenburg and Anthony Quattrocchi
 * Date			: Feb 26, 2019
 * Description	: Solves the famous water jug puzzle using breadth-first search
 * E Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 * A pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <queue>
#include <string>

using namespace std;

// Struct to represent state of water in the jugs
struct State {
	int a, b, c;
	vector<string> directions;

	State(int _a, int _b, int _c) : a(_a), b(_b), c(_c) { }

	// String representation of state in tuple form.
	string to_string() {
		ostringstream oss;
		oss << "(" << a << ", " << b << ", " << c << ")";
		return oss.str();
	}
};

string WJP(int capA, int capB, int capC, int goalA, int goalB, int goalC) {
	vector<string> dir; // Vector that holds the directions for the entire thing, eventually passes it on to the current during push to queue
	queue<State> BFSvisited; // creation of the queue for BFS
	int currA = 0, currB = 0, currC = capC; // Values currently being looked at through each iteration of the while loop
	int pour = 0; // Used to store the amount poured between each transition of jugs

	bool **visited = new bool*[capA+1]; // Creation of 2D array for visited variables
	for(int i = 0; i < capA + 1; i++) {
		visited[i] = new bool [capB + 1]; // creation of row B
		for(int j = 0; j < capB + 1; j++) {
			visited[i][j] = false; // Sets all values to false
		}
	}
	visited[currA][currB] = true; // To show that the initial case has been visited

	string x = "Initial state. (0, 0, " + to_string(currC) + ")\n"; //<< "Initial state. (" << currA << ", " << currB << ", " << currC << ")" << "\n"; // Appends the initial state to the oss

	State current(0, 0, capC); // Creation of current state "object" that contains the initial state as well

	current.directions.push_back(x);

	BFSvisited.push(current); // Pushes the initial state of jugs onto queue

	State end(-1, -1, -1); // Used to determine if no solution

	while(!BFSvisited.empty()) { // So long as the queue is not empty, continue BFS algorithm
		current = BFSvisited.front(); // Takes on the values of the next state in line in queue // This is where it gains the directions

		visited[current.a][current.b] = true; // Visited the current location in the BFS array

		if(current.a == goalA && current.b == goalB && current.c == goalC) { // If the current state of the jugs is the end goal, then change end state for no solution case and break
			end.a = 1;
			end.b = 1;
			end.c = 1;

			break;
		}

		/* C to A */
		//Resets the curr values to what the current state is
		currA = current.a;
		currB = current.b;
		currC = current.c;
		if (current.a + current.c > capA) { // A already has something in it
			currA = capA;
			pour = capA - current.a;
			currC = current.c - pour;
		} else { // A is empty and can be filled with its capacity
			currA = current.a + current.c;
			currC = 0;
			pour = current.c;
		}

		if(!visited[currA][currB]) { // If this variation of jugs has not been visited
			string s; // Eventually will hold the directions temporarily

			if(pour == 1) { // Singular
				s = ("Pour 1 gallon from C to A. (" + to_string(currA) + ", " + to_string(current.b) + ", " + to_string(currC) + ")");
			}
			else { // Plural
				s = ("Pour " + to_string(pour) + " gallons from C to A. (" + to_string(currA) + ", " + to_string(current.b) + ", " + to_string(currC) + ")");
			}

			dir = current.directions;
			dir.push_back(s);
			State temp(currA, current.b, currC);
			temp.directions = dir;

			BFSvisited.push(temp);
		}

		/* B to A */
		currA = current.a;
		currB = current.b;
		currC = current.c;
		if( current.a + current.b > capA) {
			currA = capA;
			pour = capA - current.a;
			currB = current.b - pour;
		} else {
			currA = current.a + current.b;
			currB = 0;
			pour = current.b;
		}

		if (!visited[currA][currB]) {
			string s;

			if (pour == 1) {
				s = ("Pour 1 gallon from B to A. (" + to_string(currA) + ", " + to_string(currB) + ", " + to_string(current.c) + ")");
			} else {
				s = ("Pour " + to_string(pour) + " gallons from B to A. (" + to_string(currA) + ", " + to_string(currB) + ", " + to_string(current.c) + ")");
			}

			dir = current.directions;
			dir.push_back(s);
			State temp(currA, currB, current.c);
			temp.directions = dir;

			BFSvisited.push(temp);
		}

		/* C to B */
		currA = current.a;
		currB = current.b;
		currC = current.c;
		if (current.b + current.c > capB) {
			currB = capB;
			pour = capB - current.b;
			currC = current.c - pour;
		} else {
			currB = current.b + current.c;
			currC = 0;
			pour = current.c;
		}

		if (!visited[currA][currB]) {
			string s;

			if(pour == 1) {
				s = ("Pour 1 gallon from C to B. (" + to_string(current.a) + ", " + to_string(currB) + ", " + to_string(currC) + ")");
			} else {
				s = ("Pour " + to_string(pour) + " gallons from C to B. (" + to_string(current.a) + ", " + to_string(currB) + ", " + to_string(currC) + ")");
			}

			dir = current.directions;
			dir.push_back(s);
			State temp(current.a, currB, currC);
			temp.directions = dir;

			BFSvisited.push(temp);
		}

		/* A to B */
		currA = current.a;
		currB = current.b;
		currC = current.c;
		if (current.b + current.a > capB) {
			currB = capB;
			pour = capB - current.b;
			currA = current.a - pour;
		} else {
			currB = current.b + current.a;
			currA = 0;
			pour = current.a;
		}

		if (!visited[currA][currB]) {
			string s;

			if(pour == 1){
				s = ("Pour 1 gallon from A to B. (" + to_string(currA) + ", " + to_string(currB) + ", " + to_string(current.c) + ")");
			} else {
				s = ("Pour " + to_string(pour) + " gallons from A to B. (" + to_string(currA) + ", " + to_string(currB) + ", " + to_string(current.c) + ")");
			}

			dir = current.directions;
			dir.push_back(s);
			State temp(currA, currB, current.c);
			temp.directions = dir;

			BFSvisited.push(temp);
		}

		/* B to C */
		currA = current.a;
		currB = current.b;
		currC = current.c;
		if (current.b + current.c > capC) {
			currC = capC;
			pour = capC - current.c;
			currB = current.b - pour;
		} else {
			currC = current.b + current.c;
			currB = 0;
			pour = current.b;
		}

		if(!visited[currA][currB]) {
			string s;

			if (pour == 1) {
				s = ("Pour 1 gallon from B to C. (" + to_string(current.a) + ", " + to_string(currB) + ", " + to_string(currC) + ")");
			} else {
				s = ("Pour " + to_string(pour) + " gallons from B to C. (" + to_string(current.a) + ", " + to_string(currB) + ", " + to_string(currC) + ")");
			}

			dir = current.directions;
			dir.push_back(s);
			State temp(current.a, currB, currC);
			temp.directions = dir;

			BFSvisited.push(temp);
		}

		/* A to C */
		currA = current.a;
		currB = current.b;
		currC = current.c;
		if (current.a + current.c > capC) {
			currC = capC;
			pour = capC - current.c;
			currA = current.a - pour;
		} else {
			currC = current.a + current.c;
			currA = 0;
			pour = current.a;
		}

		if (!visited[currA][currB]) {
			string s;

			if(pour == 1){
				s = ("Pour 1 gallon from A to C. (" + to_string(currA) + ", " + to_string(current.b) + ", " + to_string(currC) + ")");
			} else {
				s = ("Pour " + to_string(pour) + " gallons from A to C. (" + to_string(currA) + ", " + to_string(current.b) + ", " + to_string(currC) + ")");
			}

			dir = current.directions;
			dir.push_back(s);
			State temp(currA, current.b, currC);
			temp.directions = dir;

			BFSvisited.push(temp);
		}

		BFSvisited.pop(); // Pops what ever is on top of queue
	}

	string solution; // What's going to be returned

	if (end.a == -1 && end.b == -1 && end.c == -1) { // If the state of the jugs was never changed then there was no solution
		solution = "No solution.";
	} else { // end state was changed so a solution was found
		solution = current.directions[0];

		for (unsigned int k = 1; k < current.directions.size(); ++k) { // Traverses through the current state holding all of the directions and appends them to oss
			solution += current.directions[k] + "\n";
		}
	}

	// Cleans up the dynamic 2D array
	for (int i = 0; i < capA+1; ++i) {
		delete[] visited[i];
	}
	delete[] visited;

	return solution;
}


int main(int argc, char* argv[]) {
	istringstream iss;
	int capA, capB, capC, goalA, goalB, goalC;

	if (argc != 7) { // Incorrect amount of input
		cout << "Usage: " << argv[0] << " <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
		return 1;
	}

	iss.str(argv[1]);
	if (!(iss >> capA) || capA <= 0) { // Value input for A's capacity is not an integer or is negative/0
		cout << "Error: Invalid capacity '" << argv[1] << "' for jug A." << endl;
		return 1;
	}
	iss.clear();

	iss.str(argv[2]);
	if (!(iss >> capB) || capB <= 0) { // Value input for B's capacity is not an integer or is negative/0
		cout << "Error: Invalid capacity '" << argv[2] << "' for jug B." << endl;
		return 1;
	}
	iss.clear();

	iss.str(argv[3]);
	if (!(iss >> capC) || capC <= 0) { // Value input for C's capacity is not an integer or is negative/0
		cout << "Error: Invalid capacity '" << argv[3] << "' for jug C." << endl;
		return 1;
	}
	iss.clear();

	iss.str(argv[4]);
	if (!(iss >> goalA) || goalA < 0) { // Value input for A's goal is not an integer or is negative
		cout << "Error: Invalid goal '" << argv[4] << "' for jug A." << endl;
		return 1;
	}
	iss.clear();

	iss.str(argv[5]);
	if (!(iss >> goalB) || goalB < 0) { // Value input for B's goal is not an integer or is negative
		cout << "Error: Invalid goal '" << argv[5] << "' for jug B." << endl;
		return 1;
	}
	iss.clear();

	iss.str(argv[6]);
	if (!(iss >> goalC) || goalC < 0) { // Value input for C's goal is not an integer or is negative
		cout << "Error: Invalid goal '" << argv[6] << "' for jug C." << endl;
		return 1;
	}
	iss.clear();

	if (goalA > capA) { // The goal of A exceeded the capacity of A
		cout << "Error: Goal cannot exceed capacity of jug A." << endl;
		return 1;
	}

	if (goalB > capB) { // The goal of B exceeded the capacity of B
		cout << "Error: Goal cannot exceed capacity of jug B." << endl;
		return 1;
	}

	if (goalC > capC) { // The goal of C exceeded the capacity of C
		cout << "Error: Goal cannot exceed capacity of jug C." << endl;
		return 1;
	}

	if ((goalA + goalB + goalC) != capC) { // Goal total must be equal to C
		cout << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
		return 1;
	}

	cout << WJP(capA, capB, capC, goalA, goalB, goalC) << endl;

	return 0;
}
