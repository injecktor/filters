#include "dsp.h"

vectorc_t ft::dft(vector_t a_input)
{
    size_t N = a_input.size();
    vectorc_t result;
    result.resize(N);
    for (size_t m = 0; m < N; ++m) {
        for (size_t n = 0; n < N; ++n) {
            result[m] += a_input[n] * exp(complex_t{0, -2 * PI * (double)n * (double)m / (double)N});
        }
    }
    return result;
}

vectorc_t ft::idft(vectorc_t a_input)
{
    size_t N = a_input.size();
    vectorc_t result;
    result.resize(N);
    for (size_t m = 0; m < N; ++m) {
        for (size_t n = 0; n < N; ++n) {
            result[m] += a_input[n] * exp(complex_t{0, 2 * PI * (double)n * (double)m / (double)N});
        }
        result[m] /= N;
    }
    return result;
}

vector_t ft::dft_module(vectorc_t a_input)
{
    size_t N = a_input.size();
    vector_t result;
    result.resize(N);
    for (size_t var = 0; var < N; ++var) {
        result[var] = abs(a_input[var]);
    }
    return result;
}

vector_t ft::dft_phase(vectorc_t a_input)
{
    size_t N = a_input.size();
    vector_t result;
    result.resize(N);
    for (size_t var = 0; var < N; ++var) {
        if (a_input[var].real() == 0) {
            result[var] = 0;
            continue;
        }
        auto ratio = a_input[var].imag() / a_input[var].real();
        result[var] = std::atan(ratio);
    }
    return result;
}

vector_t ft::convolution(vector_t a_input_0, vector_t a_input_1)
{
    vector_t* least = a_input_0.size() < a_input_1.size() ? &a_input_0 : &a_input_1;
    vector_t* most = a_input_0.size() < a_input_1.size() ? &a_input_1 : &a_input_0;
    vector_t result;
    result.resize(most->size() - least->size());
    for (int i = 0; i < most->size() - least->size(); ++i) {
        for (int j = 0; j < least->size(); ++j) {
            result[i] += least->at(least->size() - 1 - j) * most->at(j + i);
        }
    }
    return result;
}

vector_t ft::real(vectorc_t a_input)
{
    auto N = a_input.size();
    vector_t result;
    result.resize(N);
    for (int var = 0; var < N; ++var) {
        result[var] = a_input[var].real();
    }
    return result;
}

vector_t ft::imag(vectorc_t a_input)
{
    auto N = a_input.size();
    vector_t result;
    result.resize(N);
    for (int var = 0; var < N; ++var) {
        result[var] = a_input[var].imag();
    }
    return result;
}

vector_t ft::hilbert_coefs(int a_order, int fs)
{
    vector_t coefs;
    coefs.resize(a_order);
    double extra_add = 0;
    if (a_order % 2 == 0) {
        extra_add = 0.5;
    }
    for (int var = 0; var < a_order; ++var) {
        auto shift = var - a_order / 2 + extra_add;
        if (shift == 0.) {
            coefs[var] = 0.;
        }
        else {
            coefs[var] = fs * (1 - std::cos(PI * shift)) / (PI * shift);
        }
    }
    return coefs;
}

vectorc_t ft::generate_analytic_signal(vector_t a_input, int a_order, int fs)
{
    auto N = a_input.size();
    vectorc_t result;
    result.resize(N - a_order);
    vector_t quadrature_signal = convolution(a_input, hilbert_coefs(a_order, fs));
    if (quadrature_signal.size() < N) {
        N = quadrature_signal.size();
    }
    for (int var = (a_order - 1) / 2; var < N; ++var) {
        result[var - (a_order - 1) / 2].real(a_input[var]);
        result[var - (a_order - 1) / 2].imag(a_input[var - (a_order - 1) / 2]);
    }
    return result;
}






