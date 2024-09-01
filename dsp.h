#ifndef DSP_H
#define DSP_H

#include <QVector>
#include <complex>

#define PI ((double)3.1415926535)

using complex_t=std::complex<double>;
using vector_t=QVector<double>;
using vectorc_t=QVector<complex_t>;

namespace ft {
vectorc_t dft(vector_t a_input);
vectorc_t idft(vectorc_t a_input);
vector_t dft_m(vectorc_t a_input);
vector_t fft(vector_t a_input);
vector_t convolution(vector_t a_input_0, vector_t a_input_1);
}

#endif // DSP_H
