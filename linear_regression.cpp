#include "linear_regression.h"

#include <cmath>
#include <iostream>

linear_regression::linear_regression(size_t num_features) :
    params(num_features + 1, 0.0f), // +1 due to the x0 feature
    alpha(0.05f)
{}

float linear_regression::h_fun(
    const std::vector<float>& in_params,
    const std::vector<float>& in_x)
{
    float h_val = in_params[0];
    for (size_t i = 0; i < in_x.size(); ++i) {
        h_val += in_params[i + 1] * in_x[i];
    }
    return h_val;
}

void linear_regression::train(
    const std::vector< std::vector<float> >& vx,
    const std::vector< float >& vy)
{
    float divergence = 1000.0f;
    // Gradient descent
    while (divergence != 0.0f) {
        auto cp_params = params;
        float inner = 0;
        float trail_x = 1.0f;
        divergence = 0;

        for (size_t i = 0; i < vx.size(); ++i) {
            float h_val = h_fun(cp_params, vx[i]);
            inner += (h_val - vy[i]) * trail_x;
        }
        float outer = cp_params[0] - (alpha * (1.0f/vx.size()) * inner);
        divergence += fabs(params[0] - outer);
        params[0] = outer;

        // Params greater than 0;
        for (size_t p = 1; p < cp_params.size(); ++p) {
            inner = 0;
            for (size_t i = 0; i < vx.size(); ++i) {
                float h_val = h_fun(cp_params, vx[i]);
                inner += (h_val - vy[i]) * vx[i][p - 1];
            }
            outer = cp_params[p] - (alpha * (1.0f/vx.size()) * inner);
            divergence += fabs(params[p] - outer);
            params[p] = outer;
        }
    }
    for (auto param : params) { std::cout << param << " "; }
    std::cout << std::endl;
}

