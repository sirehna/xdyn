/*
 * hdb_to_ast.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cady
 */

#include "hdb_to_ast.hpp"
#include "hdb_grammar.hpp"

class Ast_visitor
    : public boost::static_visitor<>
{
    public:
        Ast_visitor(hdb::AST& ast_) : ast(ast_){}
        void operator()(const hdb::Key<std::string>& k)
        {
            ast.string_keys.push_back(k);
        }

        void operator()(const hdb::Key<double>& k)
        {
            ast.value_keys.push_back(k);
        }

        void operator()(const hdb::VectorSection& val)
        {
            ast.vector_sections.push_back(val);
        }

        void operator()(const hdb::MatrixSection& val)
        {
            ast.matrix_sections.push_back(val);
        }

        void operator()(const hdb::ListOfMatrixSections& val)
        {
            ast.lists_of_matrix_sections.push_back(val);
        }

        void operator()(const hdb::ListOfMatrixSectionsWithId& val)
        {
            ast.lists_of_matrix_sections_with_id.push_back(val);
        }

    private:
        hdb::AST& ast;
};


hdb::AST hdb::parse(const std::string& contents)
{
    hdb::grammar g; // Our grammar
    hdb::AST ast; // Our tree

    using boost::spirit::ascii::blank;

    std::vector<hdb::Ast> tree;

    const bool bb = qi::phrase_parse(contents.begin(), contents.end(), *(g.ast), blank, tree);

    Ast_visitor visitor(ast);
    for (auto it = tree.begin() ; it != tree.end() ; ++it)
    {
        boost::apply_visitor(visitor, *it);
    }

    return ast;
}
