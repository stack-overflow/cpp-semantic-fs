// STD lib
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cmath>

#include "linear_regression.h"

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
    virtual ~data_source() {}
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
        throw std::runtime_error("Not implemented\n");
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
