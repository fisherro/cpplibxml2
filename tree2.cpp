//A port of libxml2's tree2 example to fisherro::libxml2
#include <sstream>
#include "libxml2.hpp"

namespace xml = fisherro::libxml2;

int main(int argc, const char** argv)
{
    LIBXML_TEST_VERSION;
    auto doc { xml::new_doc("1.0") };
    auto unique_root { xml::new_node("root") };
    //TODO: Needs to take ownership of root
    //      Or have a single call that does new_node and set_root_element
    auto root { unique_root.get() }; //Save for future use
    xml::set_root_element(doc, std::move(unique_root));
    xml::create_int_subset(doc, "root", std::nullopt, "tree2.dtd");
    xml::new_child(root, "node1", "content of node 1");
    xml::new_child(root, "node2");
    auto node3 { xml::new_child(root, "node3", "this node has attributes") };
    xml::new_prop(node3, "attribute", "yes");
    xml::new_prop(node3, "foo", "bar");
    auto node4 { xml::new_node("node4") };
    auto text { xml::new_text(
            "other way to create content (which is also a node)") };
    //Ownership of text needs to be given away
    xml::add_child(node4, std::move(text));
    xml::add_child(root, std::move(node4));
    for (int i{5}; i < 7; ++i) {
        std::ostringstream node_name;
        node_name << "node" << i;
        auto node { xml::new_child(root, node_name.str()) };
        for (int j{1}; j < 4; ++j) {
            std::ostringstream child_name;
            child_name << "node" << i << j;
            auto child { xml::new_child(node, child_name.str()) };
            xml::new_prop(child, "odd", (j % 2)? "no": "yes");
        }
    }
    xml::save_format_file_enc(argc > 1? argv[1]: "-", doc, "UTF-8", 1);
    xml::cleanup_parser();
    xml::memory_dump();
    return EXIT_SUCCESS;
}
