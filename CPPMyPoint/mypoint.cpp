/*
 * Name			: mypoint.cpp
 * Author		: Eric Altenburg
 * Date			: Feb 1, 2019
 * Description	:
 * Pledge		: I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <iostream>
#include <iomanip> //input output manipulation
#include <vector> //resizable growable array

using namespace std;

class MyPoint { //normally would put description in header and implement in source file
//everything is private by default except for struct
public:
	//Constructor that uses an initializer list
	//Order in which member variables are declared are the order in which it must be initialized in the initializer list
	//Initialize member variables before the constructor
	MyPoint(int x, int y, float z) : x_{x}, y_{y}, z_{z} { }

	//Methods that do not modify member variables should be declared const.
	void print_coords() const {
		cout <<"(x, y, z) = (" << x_<< ", " << y_ << ", " << fixed
				<< setprecision(2) << z_ << ")" << endl;
	}

	//Mutator = setter
	void set_x (int x) {
		x_ = x;
	}

	//Accessor = getter
	int get_x() const {
		return x_;
	}

private:
	//Member variables(fields). Use the trailing _ for these variables
	int x_,y_;
	float z_;


};

void display_points(const vector<MyPoint> &points) { //pass by value creates a copy, with & it does not copy, it just passes address in memory
	for (auto it = points.cbegin(); it != points.cend(); ++it) { //++it not it++ as it has extra overhead
		it->print_coords();
	}
}

void display_points_v2(const vector<MyPoint> &points) { //pass by value creates a copy, with & it does not copy, it just passes address in memory
	for(int i = 0; i <points.size(); i++) { //can use i++ since it is an integer not an iterator
		points[i].print_coords();
	}
}

int main() {
	int x, y;

	MyPoint point1(5, 7, 1.9823412); //This way not java way, because it will put it on heap not stack
	//string s("123");
	MyPoint point2(1, 2, 3);
	MyPoint point3(4, 5, 6);

	vector<MyPoint> points;

	points.push_back(point1);
	points.push_back(point2);
	points.push_back(point3);

	vector<MyPoint> points_above_two;

	for(const auto &point : points) { //for each loop
		if (point.get_x() > 2) { //if an object exists on a stack, use a dot notation to call a method or whatever to access a public memeber variables
								//if its on a heap, you must use an arrow ->
			points_above_two.push_back(point);
		}
	}

	display_points(points_above_two);

	//Practice using standard in (STDIN)
	cout <<"Input x: ";
	cin >> x;

	cout <<"Input y: ";
	cin >> y;

	MyPoint *point4 = new MyPoint(x, y, 7);
	point4->print_coords();
	delete point4;

	return 0;
}
