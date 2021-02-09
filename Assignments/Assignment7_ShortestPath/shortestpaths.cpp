/*******************************************************************************
 * Name          : shortestpaths.cpp
 * Author        : Brian S. Borowski
 * Version       : 1.0
 * Date          : April 26, 2018
 * Last modified : December 4, 2018
 * Description   : Implements Floyd's theta(n^3) algorithm for finding all
 *                 pairs shortest paths.
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

long **distance_matrix, **path_lengths_matrix, **intermediate_matrix;
int num_vertices;

const long INF = numeric_limits<long>::max();

/**
 * Computes the number of digits in an integer.
 */
int len(long value) {
    int num_digits = 1;
    while (value > 9) {
        value /= 10;
        num_digits++;
    }
    return num_digits;
}

/**
 * Left trims string in place.
 */
inline void left_trim(string &s) {
    s.erase(s.begin(),
            find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
}

/**
 * Right trims string in place.
 */
static inline void right_trim(string &s) {
    s.erase(find_if(
                s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
            s.end());
}

/**
 * Trims string on both ends in place.
 */
inline void trim(string &s) {
    left_trim(s);
    right_trim(s);
}

vector<string> split(const string& s, const string& delim,
                     const bool keep_empty = true) {
    vector<string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        string temp(substart, subend);
        if (keep_empty || !temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

long** initialize_matrix(const bool set_diagonal = false) {
    long **matrix = new long*[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
        matrix[i] = new long[num_vertices];
        for (int j = 0; j < num_vertices; j++) {
            matrix[i][j] = INF;
        }
        if (set_diagonal) {
            matrix[i][i] = 0;
        }
    }
    return matrix;
}

long** copy_matrix(long** const matrix, int num_vertices) {
    long **copy = new long*[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
        copy[i] = new long[num_vertices];
        for (int j = 0; j < num_vertices; j++) {
            copy[i][j] = matrix[i][j];
        }
    }
    return copy;
}

void delete_matrix(long** matrix) {
    for (int i = 0; i < num_vertices; ++i) {
        delete [] matrix[i];
    }
    delete [] matrix;
}

bool load_values_from_file(const string &filename) {
    ifstream input_file(filename.c_str());
    if (!input_file) {
        cerr << "Error: Cannot open file '" << filename << "'." << endl;
        return false;
    }
    input_file.exceptions(ifstream::badbit);

    string line;
    getline(input_file, line);
    trim(line);
    istringstream converter;
    converter.str(line);
    if (!(converter >> num_vertices) || num_vertices < 1 || num_vertices > 26) {
        cerr << "Error: Invalid number of vertices '" << line << "' on line 1."
             << endl;
        return false;
    }
    distance_matrix = initialize_matrix(true);
    char max_vertex = 'A' + num_vertices - 1;

    int line_number = 2;
    try {
        while (getline(input_file, line)) {
            trim(line);
            vector<string> parts = split(line, " ");
            if (parts.size() != 3) {
                cerr << "Error: Invalid edge data '"<< line << "' on line "
                     << line_number << "." << endl;
                goto cleanup_failure;
            }
            if (parts[0].length() > 1 ||
                    parts[0][0] < 'A' || parts[0][0] > max_vertex) {
                cerr << "Error: Starting vertex '"<< parts[0] << "' on line "
                        << line_number << " is not among valid values A-"
                        << max_vertex << "." << endl;
                goto cleanup_failure;
            }
            if (parts[1].length() > 1 ||
                    parts[1][0] < 'A' || parts[1][0] > max_vertex) {
                cerr << "Error: Ending vertex '"<< parts[1] << "' on line "
                        << line_number << " is not among valid values A-"
                        << max_vertex << "." << endl;
                goto cleanup_failure;
            }
            int edge_weight;
            converter.clear();
            converter.str(parts[2]);
            if (!(converter >> edge_weight) || edge_weight <= 0) {
                cerr << "Error: Invalid edge weight '" << parts[2]
                     << "' on line " << line_number << "." << endl;
                goto cleanup_failure;
            }
            distance_matrix[parts[0][0] - 'A'][parts[1][0] - 'A'] = edge_weight;
            line_number++;
        }
        input_file.close();
    } catch (const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << filename << "'.";
        goto cleanup_failure;
    }
    return true;
cleanup_failure:
    delete_matrix(distance_matrix);
    return false;
}

/**
 * Displays the matrix on the screen formatted as a table.
 */
void display_table(long** const matrix, const string &label,
                   const bool use_letters = false) {
    cout << label << endl;
    long max_val = 0;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            long cell = matrix[i][j];
            if (cell < INF && cell > max_val) {
                max_val = matrix[i][j];
            }
        }
    }

    int max_cell_width = use_letters ? len(max_val) :
            len(max(static_cast<long>(num_vertices), max_val));
            cout << len(max_val) << endl;
            cout << len(max(static_cast<long>(num_vertices), max_val)) << endl;
            cout << max_cell_width << endl;
    cout << ' ';
    for (int j = 0; j < num_vertices; j++) {
        cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
    }
    cout << endl;
    for (int i = 0; i < num_vertices; i++) {
        cout << static_cast<char>(i + 'A');
        for (int j = 0; j < num_vertices; j++) {
            cout << " " << setw(max_cell_width);
            if (matrix[i][j] == INF) {
                cout << "-";
            } else if (use_letters) {
                cout << static_cast<char>(matrix[i][j] + 'A');
            } else {
                cout << matrix[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * Uses Floyd's algorithm to determine the lengths of the shortest path
 * between every pair of vertices.
 *
 * Precondition:
 *   distance_matrix - the initial distance matrix is populated
 *
 * Postconditions:
 *   - path_lengths_matrix contains the lengths of the shortest path between
 *     every pair of vertices
 *   - intermediate_matrix contains the vertex number used as the intermediate
 *     vertex in the path.
 *     0 = A, 1 = B, and so on.
 *     In other words, if there is a path from A to D that uses C as an
 *     intermediate step, P[0][3] = 2.
 */
void floyd() {
    path_lengths_matrix = copy_matrix(distance_matrix, num_vertices);
    intermediate_matrix = initialize_matrix(false);

    for (int k = 0; k < num_vertices; k++) {
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                long a = path_lengths_matrix[i][k];
                if (a == INF) {
                    continue;
                }
                long b = path_lengths_matrix[k][j];
                if (b == INF) {
                    continue;
                }
                long val = a + b;
                if (val < path_lengths_matrix[i][j]) {
                    path_lengths_matrix[i][j] = val;
                    // k is the vertex used in transitivity
                    intermediate_matrix[i][j] = k;
                }
            }
        }
    }
}

/**
 * Backtracks over the table of intermediate vertices to determine the
 * vertices visited on the shortest path between vertices i and j.
 *
 * Returns:
 *     A vector of characters describing the shortest path between vertices i
 *     and j, in reverse order.
 *
 */
vector<char> get_path(int i, int j) {
    if (intermediate_matrix[i][j] == INF) {
        vector<char> solution;
        char first = static_cast<char>(i + 'A'),
             second = static_cast<char>(j + 'A');
        solution.push_back(first);
        if (first != second) {
            solution.push_back(second);
        }
        return solution;
    }
    int k = intermediate_matrix[i][j];
    vector<char> first = get_path(i, k),
                 second = get_path(k, j);
    first.insert(first.end(), second.begin() + 1, second.end());
    return first;
}

string path_to_string(const vector<char> &path) {
    ostringstream oss;
    size_t len = path.size();
    if (len > 0) {
        oss << path[0];
    }
    for (size_t i = 1; i < len; i++) {
        oss << " -> " << path[i];
    }
    return oss.str();
}

void display_paths() {
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            cout << static_cast<char>(i + 'A') << " -> "
                 << static_cast<char>(j + 'A') << ", distance: ";
            long distance = path_lengths_matrix[i][j];
            if (distance < INF) {
                cout << distance << ", path: " << path_to_string(get_path(i, j))
                     << endl;
            } else {
                cout << "infinity" << ", path: none" << endl;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    if (!load_values_from_file(argv[1])) {
        return 1;
    }
    floyd();
    display_table(distance_matrix, "Distance matrix:");
    display_table(path_lengths_matrix, "Path lengths:");
    display_table(intermediate_matrix, "Intermediate vertices:", true);
    display_paths();
    delete_matrix(distance_matrix);
    delete_matrix(path_lengths_matrix);
    delete_matrix(intermediate_matrix);
    return 0;
}
