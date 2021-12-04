#include "relation.h"

//---------------------------------------------------------------------------------------------------

Relation::Relation(int d) {
    // Construct an empty relation of dimension d
    dimension = d;
    matrix = new int[dimension * dimension];
    memset(matrix, 0, sizeof(int) * dimension * dimension);
}

Relation::Relation(int d, const int* m) {
    // Construct a relation from the dimension of its relational tempMatrix and the array of the relational tempMatrix
    dimension = d;
    matrix = new int[dimension * dimension];
    memcpy(matrix, m, sizeof(int) * dimension * dimension);
}

Relation::Relation(const string filePath) {
    // Read the tempMatrix of a relation from a file
    ifstream readStream(filePath);
    if (!readStream) {
        cout << "Fail to open." << endl;
        exit(1);
    }
    else {
        int row, column;
        readStream >> dimension;
        matrix = new int[dimension * dimension];
        for (row = 0; row < dimension; ++row)
            for (column = 0; column < dimension; ++column)
                readStream >> matrix[row * dimension + column];
    }
    readStream.close();
}

Relation::~Relation() {
    // Destructor
    delete[] matrix;
    matrix = nullptr;
}

Relation::Relation(const Relation & r) {
    dimension = r.dimension;
    matrix = new int[dimension * dimension];
    memcpy(matrix, r.matrix, sizeof(int) * dimension * dimension);
}

Relation& Relation::operator =(const Relation & r) {
    dimension = r.dimension;
    matrix = new int[dimension * dimension];
    memcpy(matrix, r.matrix, sizeof(int) * dimension * dimension);
    return *this;
}

//---------------------------------------------------------------------------------------------------

int Relation::getDimension() const {
    // Get the dimension of relational tempMatrix
    return dimension;
}

int Relation::getAtPosition(int row, int column) const {
    // Get the value at the cross position in row-th row and column-th column of the relational tempMatrix, and this function returns -1 if it is over the boundary.
    if (row >= 0 && row < dimension && column >= 0 && column < dimension)
        return matrix[row * dimension + column];
    else
        return -1;
}

int Relation::operator()(int row, int column) const {
    // You can use R(row, column) to get the value at the cross position in row-th row and column-th column of the relational tempMatrix, and this function returns - 1 if it is over the boundary.
    if (row >= 0 && row < dimension && column >= 0 && column < dimension)
        return matrix[row * dimension + column];
    else
        return -1;
}

int* Relation::getMatrix() const {
    // Get the relational tempMatrix
    return matrix;
}

//---------------------------------------------------------------------------------------------------

void Relation::output() const {
    // Display the tempMatrix of the relation on the screen
    int row, column;
    cout << "The dimension is: " << dimension << endl;
    for (row = 0; row < dimension; ++row)
    {
        for (column = 0; column < dimension; ++column)
        {
            cout << (*this)(row, column) << "   ";
        }
        cout << endl;
    }
}

bool Relation::outputToFile(const string filePath) const {
    // Write the relation tempMatrix to a file in the same format as the read file
    ofstream writeStream(filePath);
    if (!writeStream) {
        cout << "Fail to open." << endl;
        return false;
    }
    else {
        int row, column;
        writeStream << dimension << endl;
        for (row = 0; row < dimension; ++row) {
            for (column = 0; column < dimension; ++column)
                writeStream << matrix[row * dimension + column] << "   ";
            writeStream << endl;
        }
    }
    writeStream.flush();
    writeStream.close();
    return true;
}

//---------------------------------------------------------------------------------------------------

bool Relation::isReflexive() const {
    int row;
    // Traverse all elements on the main diagonal
    for (row = 0; row < getDimension(); ++row) {
        if (getAtPosition(row, row) == 0)
            return false;
    }
    return true;
}

bool Relation::isIrreflexive() const {
    int row;
    // Traverse all elements on the main diagonal
    for (row = 0; row < getDimension(); ++row) {
        if (getAtPosition(row, row) == 1)
            return false;
    }
    return true;
}

bool Relation::isSymmetric() const {
    int row, column;
    // Traverse all elements in the martrix
    for (row = 0; row < getDimension(); ++row) {
        for (column = 0; column < getDimension(); ++column) {
            if (getAtPosition(row, column) != getAtPosition(column, row))
                return false;
        }
    }
    return true;
}

bool Relation::isAsymmetric() const {
    int row, column;
    // Traverse all elements in the martrix
    for (row = 0; row < getDimension(); ++row) {
        for (column = 0; column < getDimension(); ++column) {
            if (getAtPosition(row, column) == 1 && getAtPosition(column, row) == 1)
                return false;
        }
    }
    return true;
}

bool Relation::isAntisymmetric() const {
    int row, column;
    // Traverse all elements in the martrix
    for (row = 0; row < getDimension(); ++row) {
        for (column = 0; column < getDimension(); ++column) {
            if (getAtPosition(row, column) == 1 && getAtPosition(column, row) == 1 && row != column)
                return false;
        }
    }
    return true;
}

bool Relation::isTransitive() const {
    // Traverse all elements in the martrix, and check whether the third element is in relation R
    for (int i = 0; i < getDimension(); ++i) {
        for (int j = 0; j < getDimension(); ++j) {
            if (getAtPosition(i, j) == 1) {
                for (int k = 0; k < getDimension(); ++k) {
                    if (getAtPosition(j, k) == 1 && getAtPosition(i, k) != 1)
                        return false;
                }
            }
        }
    }
    return true;
}

bool Relation::isEquivalence() const {
    return isReflexive() && isSymmetric() && isTransitive();
}

bool Relation::isPartial() const {
    return isReflexive() && isAntisymmetric() && isTransitive();
}

//---------------------------------------------------------------------------------------------------

Relation Relation::hasse() const {
    int* result = getMatrix();

    for (int i = 0; i < dimension; ++i)
        result[i * dimension + i] = 0;

    for (int i = 0; i < dimension; ++i)
        for (int j = 0; j < dimension; ++j)
            for (int k = 0; k < dimension; ++k)
                if (1 == result[i * dimension + j] && 1 == result[j * dimension + k])
                    result[i * dimension + k] = 0;

    Relation r(dimension, result);
    return r;
}
