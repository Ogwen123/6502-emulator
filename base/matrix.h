#ifndef UTILITIES_MATRIX_H
#define UTILITIES_MATRIX_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <bits/stdc++.h>
#include <string>

using namespace std;

namespace Matrix {

    int RAND_LIMIT = 99;

    int random_number() {
        return rand() % RAND_LIMIT;
    }

    int digits_in_number(int number) { return to_string(number).size(); } //this also work apparently > return floor(log10(number) + 1);

    int** make_matrix(int cols, int rows, bool zero_matrix = false) {
        int** matrix = new int*[cols];
        for (int i = 0; i < cols; i++) {
            int* row = new int[rows];
            for (int j = 0; j < rows; j++) {
                if (zero_matrix) row[j] = 0;
                else row[j] = random_number();
            }
            matrix[i] = row;
        }
        return matrix;
    }

    class Matrix {
    public:
        int** matrix = {};
        int rows, cols;
        string name;

        Matrix(int cols, int rows, string name) : rows(rows), cols(cols), name(name), matrix(make_matrix(rows, cols)) {};

        void display() {
            string empty;
            string top = "---" + name + "---";
            int number_of_spaces = floor((cols*3-name.size())/2);

            int max_digits = digits_in_number(pow(RAND_LIMIT, 2) * cols) + 1;

            cout << top.insert(3, number_of_spaces, (char) " "[0]).insert(3 + name.size() + number_of_spaces, number_of_spaces - 1 - name.size() % 2, (char) " "[0]) << endl;

            for (int i = 0; i < cols; i++){
                cout << "| ";
                for (int j = 0; j < rows; j++){
                    string empty;
                    cout << matrix[i][j] << empty.insert(0, max_digits-digits_in_number(matrix[i][j]), (char) " "[0]);
                }
                cout << " |" << endl;
            }
            cout << top << endl;
        }

        void multiply_by(Matrix multiplier) {
            int** initial_mat = matrix;

            int** multiplier_mat = multiplier.matrix;

            // for matrices of nxm and mxp (n = row, m = col, etc)
            int n = rows;// initial matrix columns
            int m = cols;// initial matrix rows and multiplier matrix columns
            int p = multiplier.cols;// multiplier matrix rows

            if (m != multiplier.rows ) {
                cout << "Cannot multiply a " << n << "x" << m << " with a " << multiplier.rows  << "x" << p << endl;
                return;
            }

            int** new_matrix = make_matrix(n, p, true);

            for (int i = 0; i < n; i++) { //O(npm) or O(n^3) for two nxn matrices
                for (int j = 0; j < p; j++) {
                    int sum = 0;
                    for (int k = 0; k < m; k++) {
                        sum += initial_mat[i][k]*multiplier_mat[k][j];
                    }
                    new_matrix[i][j] = sum;
                }
            }
            this->matrix = new_matrix;

        }
    };
}

#endif //UTILITIES_MATRIX_H
