//Experiments in yet another C++ wrapper of libxml2
#pragma once

#include <memory>
#include <optional>
#include <string>

#include "zstring_view.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace xpp {

    using doc  = xmlDoc;
    using node = xmlNode;
    using dtd  = xmlDtd;
    using ns   = xmlNs;
    using attr = xmlAttr;

    struct doc_deleter  { void operator()(doc* p)  { xmlFreeDoc(p);  }};
    struct node_deleter { void operator()(node* p) { xmlFreeNode(p); }};

    using u_doc_ptr = std::unique_ptr<doc, doc_deleter>;
    using u_node_ptr = std::unique_ptr<node, node_deleter>;

    u_doc_ptr read_file(zstring_view path)
    {
        return u_doc_ptr{xmlReadFile(path.c_str(), nullptr, 0)};
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
        node* result { xmlAddChild(parent, child.release()) };
        return result;
    }

    node* add_child(u_node_ptr& parent, u_node_ptr child)
    {
        return add_child(parent.get(), std::move(child));
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
        return u_doc_ptr{xmlNewDoc(BAD_CAST version.c_str())};
    }

    u_node_ptr new_node(zstring_view name)
    {
        return u_node_ptr{xmlNewNode(nullptr, BAD_CAST name.c_str())};
    }

    attr* new_prop(node* node, zstring_view name, zstring_view value)
    {
        return xmlNewProp(node, BAD_CAST name.c_str(), BAD_CAST value.c_str());
    }

    u_node_ptr new_text(zstring_view content)
    {
        return u_node_ptr{xmlNewText(BAD_CAST content.c_str())};
    }

    //Returns the OLD root node!
    u_node_ptr set_root_element(u_doc_ptr& doc, u_node_ptr root)
    {
        //TODO: Is root freed on error?
        u_node_ptr old{xmlDocSetRootElement(doc.get(), root.release())};
        return old;
    }

    int save_format_file_enc(
            zstring_view filename,
            u_doc_ptr& doc,
            std::optional<zstring_view> encoding,
            bool format = true)
    {
        return xmlSaveFormatFileEnc(
                filename.c_str(),
                doc.get(),
                encoding? encoding->c_str(): nullptr,
                format);
    }
};
