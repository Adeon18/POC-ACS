// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cmath>
#include <array>

// Langermann Function constants
constexpr int ARG_M = 5;
constexpr std::array ARG_A1{1, 2, 1, 1, 5};
constexpr std::array ARG_A2{4, 5, 1, 2, 4};
constexpr std::array ARG_C{2, 1, 4, 7, 2};

double langermann_function(double x, double y) {
    double result = 0;
    double summand1, summand2;

    for (size_t i = 0; i < ARG_M; i++) {
        summand1 = (x - ARG_A1[i]) * (x - ARG_A1[i]);
        summand2 = (y - ARG_A2[i]) * (y - ARG_A2[i]);

        result += ARG_C[i] * std::exp(- M_1_PI * (summand1 + summand2)) * std::cos(M_PI * (summand1 + summand2));
    }

    return -result;
}