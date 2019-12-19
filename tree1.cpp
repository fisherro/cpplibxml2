//A port of libxml2's tree1.cpp example to xpp
#include <cstdlib>
#include <iostream>
#include <string>

#include "xpp.hpp"

namespace {
    void print_element_names(xpp::node* node)
    {
        xpp::for_each_node(node, [](xpp::node* n){
                    if (XML_ELEMENT_NODE == n->type) {
                        std::cout << "node type: Element, name: "
                                  << n->name << '\n';
                    }
                });
    }
}

int main(int argc, const char** argv)
{
    LIBXML_TEST_VERSION;
    if (argc < 2) return EXIT_FAILURE;
    auto doc { xpp::read_file(argv[1]) };
    auto root { xpp::get_root_element(doc) };
    print_element_names(root);
    xpp::cleanup_parser();
    return EXIT_SUCCESS;
}
