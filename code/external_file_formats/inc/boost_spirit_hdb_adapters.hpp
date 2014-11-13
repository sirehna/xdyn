/*
 * boost_spirit_adapters.hpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cady
 */

#ifndef BOOST_SPIRIT_ADAPTERS_HPP_
#define BOOST_SPIRIT_ADAPTERS_HPP_

#include "boost_spirit_includes.hpp"
#include "hdb_parser_internal_data_structures.hpp"

// We need to tell fusion about our structures
// to make them a first-class fusion citizen.
BOOST_FUSION_ADAPT_STRUCT(
    hdb::VectorSection,
    (hdb::Header, header)
    (hdb::Values, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::MatrixSection,
    (hdb::Header,       header)
    (hdb::ListOfValues, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::SectionWithId,
    (hdb::Header,       header)
    (hdb::Value,        id)
    (hdb::ListOfValues, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::ListOfMatrixSections,
    (hdb::Header,                     header)
    (std::vector<hdb::MatrixSection>, sections)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::ListOfMatrixSectionsWithId,
    (hdb::Header,                     header)
    (std::vector<hdb::SectionWithId>, sections_with_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::Key<std::string>,
    (hdb::Header, header)
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::Key<double>,
    (hdb::Header, header)
    (double, value)
)

namespace hdb
{
    typedef boost::variant<
                         hdb::Key<std::string>
                        ,hdb::Key<double>
                        ,hdb::VectorSection
                        ,hdb::MatrixSection
                        ,hdb::ListOfMatrixSections
                        ,hdb::ListOfMatrixSectionsWithId
                 > Ast;
}

namespace boost { namespace spirit { namespace traits {

    template<>
        struct is_container<hdb::AST, void> : mpl::true_ { };
    template<>
        struct container_value<hdb::AST, void> {
             typedef hdb::Ast type;
        };

    template <>
        struct push_back_container<hdb::AST, hdb::Key<std::string>, void> {
            static bool call(hdb::AST& f, const hdb::Key<std::string>& val) {
                f.string_keys.push_back(val);
                return true;
            }
        };
    template <>
            struct push_back_container<hdb::AST, hdb::Key<double>, void> {
                static bool call(hdb::AST& f, const hdb::Key<double>& val) {
                    f.value_keys.push_back(val);
                    return true;
                }
            };
    template <>
                struct push_back_container<hdb::AST, hdb::VectorSection, void> {
                    static bool call(hdb::AST& f, const hdb::VectorSection& val) {
                        f.vector_sections.push_back(val);
                        return true;
                    }
                };
    template <>
                struct push_back_container<hdb::AST, hdb::MatrixSection, void> {
                    static bool call(hdb::AST& f, const hdb::MatrixSection& val) {
                        f.matrix_sections.push_back(val);
                        return true;
                    }
                };
    template <>
                    struct push_back_container<hdb::AST, hdb::ListOfMatrixSectionsWithId, void> {
                        static bool call(hdb::AST& f, const hdb::ListOfMatrixSectionsWithId& val) {
                            f.lists_of_matrix_sections_with_id.push_back(val);
                            return true;
                        }
                    };
    template <>
                    struct push_back_container<hdb::AST, hdb::ListOfMatrixSections, void> {
                        static bool call(hdb::AST& f, const hdb::ListOfMatrixSections& val) {
                            f.lists_of_matrix_sections.push_back(val);
                            return true;
                        }
                    };
}}}

#endif /* BOOST_SPIRIT_ADAPTERS_HPP_ */
