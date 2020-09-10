/*******************************************************************************
 * Name          : student.cpp
 * Author        : Brian S. Borowski
 * Version       : 1.0
 * Date          : January 9, 2016
 * Last modified : September 7, 2017
 * Description   : Class that represents a student with some basic attributes.
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

/**
 * Simple class that represents a student with first and last names, a GPA,
 * and an ID number.
 */
class Student {
public:
    /**
     * Constructor
     */
    Student(
        const string &first_name, const string &last_name, float gpa, int id):
            first_{first_name}, last_{last_name}, gpa_{gpa}, id_{id} { }

    /**
     * Returns the string combining the student's first and last names.
     */
    string full_name() const {
        return first_ + " " + last_;
    }

    /**
     * Returns the student's ID number.
     */
    int id() const {
        return id_;
    }

    /**
     * Returns the student's GPA.
     */
    float gpa() const {
        return gpa_;
    }

    /**
     * Displays the student's information on the screen.
     */
    void print_info() const {
        cout << first_ << " " << last_ << ", GPA: " << fixed << setprecision(2)
             << gpa_ << ", ID: "  << id_ << endl;
    }

// Google style guide suggests trailing underscores for private members.
private:
    string first_;
    string last_;
    float gpa_;
    int id_;
};

/**
 * Takes a vector of Student objects, and returns a new vector
 * with all Students whose GPA is < 1.0.
 */
vector<Student> find_failing_students(const vector<Student> &students) {
    vector<Student> failing_students;

    // Use a for-each loop and bind the value with a const reference.
    for (const auto &student : students) {
        if (student.gpa() < 1.0) {
            failing_students.push_back(student);
        }
    }

    return failing_students;
}

/**
 * Takes a vector of Student objects and prints them to the screen.
 */
void print_students(const vector<Student> &students) {
    for (const auto &student : students) {
        student.print_info();
    }
}

/**
 * Allows the user to enter information for multiple students, then
 * find those students whose GPA is below 1.0 and prints them to the
 * screen.
 */
int main() {
    string first_name, last_name;
    float gpa;
    int id;
    char repeat;
    vector<Student> students;

    do {
        cout << "Enter student's first name: ";
        cin >> first_name;
        cout << "Enter student's last name: ";
        cin >> last_name;
        gpa = -1;
        while (gpa < 0 || gpa > 4) {
            cout << "Enter student's GPA (0.0-4.0): ";
            cin >> gpa;
        }
        cout << "Enter student's ID: ";
        cin >> id;
        students.push_back(Student(first_name, last_name, gpa, id));
        cout << "Add another student to database (Y/N)? ";
        cin >> repeat;
    } while (repeat == 'Y' || repeat == 'y');

    cout << endl << "All students:" << endl;
    print_students(students);

    cout << endl << "Failing students:";
    // Print a space and the word 'None' on the same line if no students are
    // failing.
    // Otherwise, print each failing student on a separate line.
    vector<Student> failing = find_failing_students(students);
    if (failing.size() == 0) {
        cout << " None" << endl;
    } else {
        cout << endl;
        print_students(failing);
    }
    return 0;
}
