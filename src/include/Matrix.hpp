#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>

template<class T>
class Matrix {

 private:
  int rows_;
  int cols_;

  std::unique_ptr<T []> pointer_;

 public:
  Matrix(int rows = 0, int cols = 0);
  ~Matrix();

  Matrix<T> &operator=(const Matrix<T> &matrix);

  void Resize(int, int);
  void Initialize();
  void Export(std::fstream &) const;

  T &operator()(int row, int col) const { return pointer_[row + rows_ * col]; }
};

template<class T>
Matrix<T>::Matrix(int rows, int cols) : rows_(rows), cols_(cols) { pointer_ = std::make_unique<T []>(rows_ * cols_); }

template<class T>
void Matrix<T>::Resize(int rows, int cols) {
  rows_ = rows;
  cols_ = cols;

  pointer_ = std::make_unique<T []>(rows_ * cols_);
}

template<class T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &matrix){
  // Do the copy
  for (int ii=0; ii<rows_; ++ii) {
    for (int jj=0; jj<cols_; ++jj){
      this->pointer_[ii + this->rows_ * jj] = matrix.pointer_[ii + matrix.rows_ * jj];
    }
  }

  // Return the existing object so that we can chain this operator
  return (*this);
}

template<class T>
void Matrix<T>::Initialize() {
  for (int ii = 0; ii < rows_; ++ii) {
    for (int jj = 0; jj < cols_; ++jj) {
      pointer_[ii + this->rows_ * jj] = 0;
    }
  }
}

template<class T>
void Matrix<T>::Export(std::fstream &file) const {
  for (int ii = 0; ii < cols_; ++ii) {
    for (int jj = 0; jj < rows_; ++jj) {
      file << std::setprecision(6) << std::scientific << pointer_[ii + this->rows_ * jj] << "   ";
    }
    file << '\n';
  }
  file << '\n';
}

template<class T>
Matrix<T>::~Matrix() {}

#endif // MATRIX_HPP
