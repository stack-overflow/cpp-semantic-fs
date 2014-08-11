#include <vector>
#include <stddef.h>

class linear_regression {
public:
    linear_regression(size_t num_features);

    void train(
        const std::vector< std::vector<float> >& vx,
        const std::vector<float>& vy);

private:
    float h_fun(
        const std::vector<float>& in_params,
        const std::vector<float>& in_x);

private:
    std::vector<float> params;
    float alpha;
};

