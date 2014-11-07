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
    hdb::Section,
    (hdb::Header,    header)
    (hdb::Values,    values)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::SectionWithId,
    (hdb::Header,    header)
    (hdb::Value,     id)
    (hdb::Values,    values)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::ListOfSections,
    (hdb::Header,               header)
    (std::vector<hdb::Section>, sections)
)

BOOST_FUSION_ADAPT_STRUCT(
    hdb::ListOfSectionsWithId,
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

namespace boost { namespace spirit { namespace traits {

    template<>
        struct is_container<hdb::AST, void> : mpl::true_ { };
    template<>
        struct container_value<hdb::AST, void> {
             typedef boost::variant<
                     hdb::Key<std::string>
                    ,hdb::Key<double>
                    ,hdb::Section
                    ,hdb::SectionWithId
                    ,hdb::ListOfSections
                    ,hdb::ListOfSectionsWithId
             > type;
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
                struct push_back_container<hdb::AST, hdb::Section, void> {
                    static bool call(hdb::AST& f, const hdb::Section& val) {
                        f.sections.push_back(val);
                        return true;
                    }
                };
    template <>
                    struct push_back_container<hdb::AST, hdb::SectionWithId, void> {
                        static bool call(hdb::AST& f, const hdb::SectionWithId& val) {
                            f.sections_with_id.push_back(val);
                            return true;
                        }
                    };
    template <>
                    struct push_back_container<hdb::AST, hdb::ListOfSections, void> {
                        static bool call(hdb::AST& f, const hdb::ListOfSections& val) {
                            f.list_of_sections.push_back(val);
                            return true;
                        }
                    };
    template <>
                    struct push_back_container<hdb::AST, hdb::ListOfSectionsWithId, void> {
                        static bool call(hdb::AST& f, const hdb::ListOfSectionsWithId& val) {
                            f.list_of_sections_with_id.push_back(val);
                            return true;
                        }
                    };
}}}

#endif /* BOOST_SPIRIT_ADAPTERS_HPP_ */
