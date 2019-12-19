#include <sstream>
#include "xpp.hpp"

int main(int argc, const char** argv)
{
    LIBXML_TEST_VERSION;
    auto doc { xpp::new_doc("1.0") };
    auto root { xpp::new_node("root") };
    //TODO: Needs to take ownership of root
    //      Or have a single call that does new_node and set_root_element
    xpp::set_root_element(doc, root);
    xpp::create_int_subset(doc, "root", nullptr, "tree2.dtd");
    xpp::new_child(root, nullptr, "node1", "content of node 1");
    xpp::new_child(root, nullptr, "node2");
    auto node3 { xpp::new_child(root, nullptr, "node3",
            "this node has attributes") };
    xpp::new_prop(node3, "attribute", "yes");
    xpp::new_prop(node3, "foo", "bar");
    auto node4 { xpp::new_node("node4"); }
    auto text { xpp::new_text(
            "other way to create content (which is also a node)") };
    //Ownership of text needs to be given away
    xpp::add_child(node4, text);
    xpp::add_child(root, node4);
    for (int i{5}; i < 7; ++i) {
        std::ostringstream node_name;
        oss << "node" << i;
        auto node { xpp::new_child(root, node_name.str()) };
        for (int j{1}; j < 4; ++j) {
            std::ostringstream child_name;
            child_name << "node" << i << j;
            auto child { xpp::new_child(node, child_name.str()) };
            xpp::new_prop(child, "odd", (j % 2)? "no": "yes");
        }
    }
    xpp::save_format_file_enc(argc > 1? argv[1]: "-", doc, "UTF-8", 1);
    xpp::cleanup_parser();
    xpp::memory_dump();
    return EXIT_SUCCESS;
}
