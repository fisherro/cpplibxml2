//Experiments in yet another C++ wrapper of libxml2
#pragma once

#include <memory>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

namespace xpp {

    using doc = xmlDoc;
    using node = xmlNode;
    using dtd = xmlDtd;

    using u_doc_ptr = std::unique_ptr<doc, decltype(&xmlFreeDoc)>;
    using u_node_ptr = std::unique_ptr<node, decltype(&xmlFreeNode)>;

    u_doc_ptr read_file(const std::string& path)
    {
        return u_doc_ptr(xmlReadFile(path.c_str(), nullptr, 0), &xmlFreeDoc);
    }

    //Would there ever be a reason to have a raw doc pointer?
    xmlNode* get_root_element(u_doc_ptr& doc)
    {
        return xmlDocGetRootElement(doc.get());
    }

    template <typename F>
    void for_each_node(xmlNode* node, F f)
    {
        for (; node; node = node->next) {
            f(node);
            for_each_node(node->children, f);
        }
    }

    void cleanup_parser() { xmlCleanupParser(); }
    void memory_dump() { xmlMemoryDump(); }

    node* add_child(node* parent, u_node_ptr child)
    {
        //TODO: If an error occurs, is child freed?
        node* result { xmlAddChild(parent, child.get()) };
        //Ownership has been handed over
        child.release();
        return result;
    }

    dtd* create_int_subset(u_doc_ptr& doc, const std::string& name,
            const std::string& external_id, const std::string& system_id)
    {
        return xmlCreateIntSubset(
                doc.get(),
                name.c_str(),
                external_id.empty()? nullptr: external_id.c_str(),
                system_id.c_str());
    }

    //std::optional references aren't OK, right?
    //Something like std::string_view that takes NUL-terminated strings would
    //be handy
    node* new_child_internal(
            node* parent,
            ns* ns,
            const std::string& name,
            const std::string* content)
    {
        return xmlNewChild(parent, ns, name.c_str(),
                content? content->c_str(): nullptr);
    }

    node* new_child(node* parent, const std::string& name)
    {
        return new_child_internal(parent, nullptr, name, nullptr);
    }

    node* new_child(
            node* parent,
            const std::string& name,
            const std::string& content)
    {
        return new_child_internal(parent, nullptr, name, &content);
    }

    u_doc_ptr new_doc(const std::string& version)
    {
        return u_doc_ptr{xmlNewDoc(version.c_str()), &xmlFreeDoc};
    }
};
#if 0
xpp::new_node
xpp::new_prop
xpp::new_text
xpp::save_format_file_enc
xpp::set_root_element
#endif
