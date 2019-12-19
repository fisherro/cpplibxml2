//Experiments in yet another C++ wrapper of libxml2
#pragma once

#include <memory>
#include <optional>
#include <string>

#include "zstring_view.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace xpp {

    using doc = xmlDoc;
    using node = xmlNode;
    using dtd = xmlDtd;
    using ns = xmlNs;

    using u_doc_ptr = std::unique_ptr<doc, decltype(&xmlFreeDoc)>;
    using u_node_ptr = std::unique_ptr<node, decltype(&xmlFreeNode)>;

    u_doc_ptr read_file(zstring_view path)
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

    dtd* create_int_subset(
            u_doc_ptr& doc,
            zstring_view name, 
            std::optional<zstring_view> external_id,
            zstring_view system_id)
    {
        return xmlCreateIntSubset(
                doc.get(),
                BAD_CAST name.c_str(),
                external_id? BAD_CAST external_id->c_str(): nullptr,
                BAD_CAST system_id.c_str());
    }

    node* new_child_internal(
            node* parent,
            ns* ns,
            zstring_view name,
            std::optional<zstring_view> content)
    {
        return xmlNewChild(parent, ns, BAD_CAST name.c_str(),
                content? BAD_CAST content->c_str(): nullptr);
    }

    node* new_child(node* parent, zstring_view name)
    {
        return new_child_internal(parent, nullptr, name, std::nullopt);
    }

    node* new_child(node* parent, zstring_view name, zstring_view content)
    {
        return new_child_internal(parent, nullptr, name, content);
    }

    u_doc_ptr new_doc(zstring_view version)
    {
        return u_doc_ptr{xmlNewDoc(BAD_CAST version.c_str()), &xmlFreeDoc};
    }
};
#if 0
xpp::new_node
xpp::new_prop
xpp::new_text
xpp::save_format_file_enc
xpp::set_root_element
#endif
