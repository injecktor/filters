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

vector_t ft::dft_m(vector_t a_input)
{
    size_t N = a_input.size();
    auto dftc = dft(a_input);
    vector_t result;
    result.resize(N);
    for (size_t var = 0; var < N; ++var) {
        result[var] = abs(dftc[var]);
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
