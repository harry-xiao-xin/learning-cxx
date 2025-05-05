#include "../exercise.h"
#include <cstring>
#include <iostream>
#include <vector>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>
template <class T> struct Tensor4D {
  unsigned int shape[4];
  T *data;

  // Add a public static helper for index calculation
  static unsigned int getIndex(unsigned int i, unsigned int j, unsigned int k,
                               unsigned int l, const unsigned int shape_[4]) {
    return i * shape_[1] * shape_[2] * shape_[3] + j * shape_[2] * shape_[3] +
           k * shape_[3] + l;
  }

  Tensor4D(unsigned int const shape_[4], T const *data_) {
    // Copy shape
    std::memcpy(shape, shape_, sizeof(shape));
    unsigned int size = shape[0] * shape[1] * shape[2] * shape[3];
    data = new T[size];
    std::memcpy(data, data_, size * sizeof(T));
  }

  ~Tensor4D() { delete[] data; }

  Tensor4D(Tensor4D const &) = delete;
  Tensor4D(Tensor4D &&) noexcept = delete;

  Tensor4D &operator+=(Tensor4D const &others) {
    // 检查形状兼容性
    for (int d = 0; d < 4; ++d) {
      if (this->shape[d] != others.shape[d] && this->shape[d] != 1 &&
          others.shape[d] != 1) {
        throw std::invalid_argument("Incompatible shape for broadcasting");
      }
    }

    const auto &this_shape = this->shape;
    const auto &others_shape = others.shape;

    for (unsigned int i = 0; i < this_shape[0]; ++i) {
      for (unsigned int j = 0; j < this_shape[1]; ++j) {
        for (unsigned int k = 0; k < this_shape[2]; ++k) {
          for (unsigned int l = 0; l < this_shape[3]; ++l) {
            unsigned int oi = (others_shape[0] == 1) ? 0 : i;
            unsigned int oj = (others_shape[1] == 1) ? 0 : j;
            unsigned int ok = (others_shape[2] == 1) ? 0 : k;
            unsigned int ol = (others_shape[3] == 1) ? 0 : l;

            unsigned int this_idx = getIndex(i, j, k, l, this_shape);
            unsigned int others_idx = getIndex(oi, oj, ok, ol, others_shape);

            this->data[this_idx] += others.data[others_idx];
          }
        }
      }
    }
    return *this;
  }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
  {
    unsigned int shape[]{1, 2, 3, 4};
    // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
    // clang-format on
    auto t0 = Tensor4D(shape, data);
    auto t1 = Tensor4D(shape, data);
    t0 += t1;
    for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
      ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
    }
  }
  {
    unsigned int s0[]{1, 2, 3, 4};
    // clang-format off
          float d0[]{
              1, 1, 1, 1,
              2, 2, 2, 2,
              3, 3, 3, 3,

              4, 4, 4, 4,
              5, 5, 5, 5,
              6, 6, 6, 6};
    // clang-format on
    unsigned int s1[]{1, 2, 3, 1};
    // clang-format off
          float d1[]{
              6,
              5,
              4,

              3,
              2,
              1};
    // clang-format on

    auto t0 = Tensor4D(s0, d0);
    auto t1 = Tensor4D(s1, d1);
    t0 += t1;
    for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
      ASSERT(t0.data[i] == 7.f,
             "Every element of t0 should be 7 after adding t1 to it.");
    }
  }
  {
    unsigned int s0[]{1, 2, 3, 4};
    // clang-format off
          double d0[]{
               1,  2,  3,  4,
               5,  6,  7,  8,
               9, 10, 11, 12,

              13, 14, 15, 16,
              17, 18, 19, 20,
              21, 22, 23, 24};
    // clang-format on
    unsigned int s1[]{1, 1, 1, 1};
    double d1[]{1};

    auto t0 = Tensor4D(s0, d0);
    auto t1 = Tensor4D(s1, d1);
    t0 += t1;
    for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
        ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 " "to it.");
    }
  }
}
