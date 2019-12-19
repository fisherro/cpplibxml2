//A port of libxml2's tree1.cpp example to fisherro::libxml2
#include <cstdlib>
#include <iostream>
#include <string>

#include "fisherro_libxml2.hpp"

namespace xml = fisherro::libxml2;

namespace {
    void print_element_names(xml::node* node)
    {
        xml::for_each_node(node, [](xml::node* n){
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
    auto doc { xml::read_file(argv[1]) };
    auto root { xml::get_root_element(doc) };
    print_element_names(root);
    xml::cleanup_parser();
    return EXIT_SUCCESS;
}
