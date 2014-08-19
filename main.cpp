// STD lib
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
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
    std::string system_path;
    std::vector<node*> similiar;
};

class file_graph
{
public:
    ~file_graph() {
        for (auto& node : nodes) {
            delete node.second;
        }
    }
    node *get(const std::string& name) {
        auto result = nodes.equal_range(name);
        if (result.first == result.second) {
            std::cout << "C++ magic!" << std::endl;
            return nodes.insert(result.second, std::make_pair(name, new node(name)))->second;
        }
        std::cout << "C++ standard equal_range worked!" << std::endl;
        return result.first->second;
    }
//    node *get_file(const std::string& name) {
//
//    }
    node *get(node::node_id id) { 
        throw std::runtime_error("Not implemented\n");
    }
    void add_link(node *first, node *second) {
        first->similiar.push_back(second);
        second->similiar.push_back(first);
    }
    void  debug_print_nodes() {
        for (auto& node : nodes) {
            std::cout << node.second->name << "\n";
            for (auto& doc : node.second->similiar) {
                std::cout << doc->name << " ";
            }
            std::cout << std::endl;
        }
    }
private:
    std::map<std::string, node*> nodes;
};

std::vector< std::pair<std::string, std::string> > desc {
    {"file_a.txt", "text"},
    {"file_b.txt", "text"}
};

class data_source {
public:
    virtual std::unique_ptr<file_graph> fetch_dir(const std::string& path) = 0;
};

class config_data_source :
    public data_source
{
public:
    virtual std::unique_ptr<file_graph> fetch_dir(const std::string& path) {
        std::string cfg_file = path + ((path.back() != '/') ? ".tagrc" : "/.tagrc");
        std::ifstream cfg_ifs(cfg_file, std::ifstream::in);
        
        std::unique_ptr<file_graph> graph(new file_graph);

        std::string line, sys_file_path, tag_name;
        while (std::getline(cfg_ifs, line)) {
            std::istringstream iss(line);
            std::getline(iss, sys_file_path, ',');
            std::getline(iss, tag_name, ',');

            std::cout << sys_file_path << " / " << tag_name << std::endl;
            auto file_node = graph->get(sys_file_path);
            auto tag_node = graph->get(tag_name);
            graph->add_link(file_node, tag_node);
        }

        return graph;
    }
};

int main(int argc, char **argv)
{
    config_data_source cds;
    std::unique_ptr<file_graph> ts = cds.fetch_dir("./");

    ts->debug_print_nodes();

//    linear_regression lr(1);
//    std::vector< std::vector<float> > x {
//        {1}, {2}, {3}, {4}, {5}
//    };
//    std::vector<float> y { 2.0f, 4.0f, 5.0f, 4.0f, 5.0f };
//    lr.train(x, y);
    return 0;
}
