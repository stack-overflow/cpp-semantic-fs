// STD lib
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cmath>

struct node {
public:
    typedef size_t node_id;
public:
    node(const std::string& in_name) : id(0), name(in_name) {}
    node_id id;
    std::string name;
    std::vector< std::reference_wrapper<node> > similiar;
};

class data_source {
public:
    virtual std::reference_wrapper<node> get(node::node_id id) = 0;
    virtual std::reference_wrapper<node> get(const std::string& name) = 0;
    virtual void debug_print_nodes() {}
};

class dummy_data_source :
    public data_source
{
public:
    virtual std::reference_wrapper<node> get(const std::string& name) {
        auto ret = nodes.insert(std::make_pair(name, node(name)));
        return std::ref((*ret.first).second);
    }
    virtual std::reference_wrapper<node> get(node::node_id id) { 

    }
    virtual void  debug_print_nodes() {
        for (auto& node : nodes) {
            std::cout << node.second.name << "\n";
            for (auto& doc : node.second.similiar) {
                std::cout << doc.get().name << " ";
            }
            std::cout << std::endl;
        }
    }
private:
    std::map<std::string, node> nodes;
};

std::vector< std::pair<std::string, std::string> > desc {
    {"file_a.txt", "text"},
    {"file_b.txt", "text"}
};

class linear_regression {
public:
    linear_regression(size_t num_features) :
        params(num_features + 1, 0.0f), // +1 due to the x0 feature
        alpha(0.05f)
    {}

    inline float h_fun(const std::vector<float>& in_params,
               const std::vector<float>& in_x)
    {
        float h_val = in_params[0];
        for (size_t i = 0; i < in_x.size(); ++i) {
            h_val += in_params[i + 1] * in_x[i];
        }
        return h_val;
    }

    void train(
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
private:
    std::vector<float> params;
    float alpha;
};

int main(int argc, char **argv)
{
    data_source *ts = new dummy_data_source;

    for (auto& p : desc)
    {
        std::cout << p.first << " " << p.second << std::endl;
        auto file_node = ts->get(p.first);
        ts->get(p.second).get().similiar.push_back(file_node);
    }

    ts->debug_print_nodes();
    delete ts;

    linear_regression lr(1);
    std::vector< std::vector<float> > x {
        {1}, {2}, {3}, {4}, {5}
    };
    std::vector<float> y { 2.0f, 4.0f, 5.0f, 4.0f, 5.0f };
    lr.train(x, y);



    return 0;
};
