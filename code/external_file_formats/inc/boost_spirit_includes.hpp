/*
 * boost_spirit_includes.hpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cady
 */

#ifndef BOOST_SPIRIT_INCLUDES_HPP_
#define BOOST_SPIRIT_INCLUDES_HPP_

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>
#include <boost/fusion/include/io.hpp>

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

using qi::double_;
using qi::eol;
using qi::lit;
using qi::lexeme;
using qi::on_error;
using qi::fail;
using ascii::char_;
using ascii::alpha;
using ascii::string;
using namespace qi::labels;


using phoenix::at_c;
using phoenix::push_back;
using phoenix::construct;
using phoenix::val;

#endif /* BOOST_SPIRIT_INCLUDES_HPP_ */
