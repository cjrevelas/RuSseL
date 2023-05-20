#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <memory>

template<class T>
class Matrix {

 private:
  int rows_;
  int cols_;

  using smp1d = std::unique_ptr<T []>;
  using smp2d = std::unique_ptr<std::unique_ptr<T []> []>;

  smp1d pointer_to_row_;
  smp2d pointer_to_row_pointers_;

 public:
  Matrix(int rows = 0, int cols = 0);
  ~Matrix();

  Matrix<T> &operator=(const Matrix<T> &matrix);

  void resize(int, int);
  void initialize();
  void print() const;

  T &operator()(int row, int col) const { return pointer_to_row_pointers_[row][col]; }
};

template<class T>
Matrix<T>::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  pointer_to_row_pointers_ = std::make_unique<smp1d []>(rows_);

  for (int ii = 0; ii < rows_; ++ii) {
    pointer_to_row_ = std::make_unique<T []>(cols_);

    //pointer_to_row_pointers_[ii] = pointer_to_row_; copy assignment operator is deleted
    pointer_to_row_pointers_[ii] = std::move(pointer_to_row_);
  }
}

template<class T>
void Matrix<T>::resize(int rows, int cols) {
  rows_ = rows;
  cols_ = cols;

  pointer_to_row_pointers_ = std::make_unique<smp1d []>(rows_);

  for (int ii = 0; ii < rows_; ++ii) {
    pointer_to_row_ = std::make_unique<T []>(cols_);

    pointer_to_row_pointers_[ii] = std::move(pointer_to_row_);
  }
}

template<class T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &matrix){
  // Do the copy
  for (int ii=0; ii<rows_; ++ii) {
    for (int jj=0; jj<cols_; ++jj){
      this->pointer_to_row_pointers_[ii][jj] = matrix.pointer_to_row_pointers_[ii][jj];
    }
  }

  // return the existing object so that we chain this operator
  return (*this);
}

template<class T>
void Matrix<T>::initialize() {
  for (int ii = 0; ii < rows_; ++ii) {
    for (int jj = 0; jj < cols_; ++jj) {
      pointer_to_row_pointers_[ii][jj] = 0;
    }
  }
}

template<class T>
void Matrix<T>::print() const {
  for (int ii = 0; ii < rows_; ++ii) {
    for (int jj = 0; jj < cols_; ++jj) {
      std::cout << pointer_to_row_pointers_[ii][jj] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

template<class T>
Matrix<T>::~Matrix() {}

#endif // MATRIX_HPP
