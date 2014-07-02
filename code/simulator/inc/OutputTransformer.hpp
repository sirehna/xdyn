/*
 * OutputGenerator.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#ifndef OUTPUTTRANSFORMER_HPP_
#define OUTPUTTRANSFORMER_HPP_

#include <map>
#include <string>

#include "Res.hpp"

/** \author cec
 *  \date Jul 1, 2014, 9:52:49 AM
 *  \brief
 *  \details
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/OutputTransformerTest.cpp OutputTransformerTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/OutputTransformerTest.cpp OutputTransformerTest expected output
 */
class OutputTransformer
{
    public:
        OutputTransformer();
        std::map<std::string,double> operator()(const Res& r) const;
};

#endif /* OUTPUTTRANSFORMER_HPP_ */
