/*
 * pairwise_sum.hpp
 *
 *  Created on: May 19, 2014
 *      Author: cady
 */

#ifndef PAIRWISE_SUM_HPP_
#define PAIRWISE_SUM_HPP_

#include <cmath>

template <typename T> T pairwise_sum(typename std::vector<T> x)
{
    if (x.empty()) return 0;
    if (x.size() == 1) return x.front();
    size_t n = x.size();
    const size_t p = floor(n/2.+0.5);
    size_t last = 0;
    for (size_t i = 0 ; i < p ; ++i)
    {
        last = n-1;
        if (n % 2)
        {
            n = (n+1)/2;
            for (size_t i = 0 ; i < n-1 ; ++i)
            {
                x[i] += x[last-i];
            }
        }
        else
        {
            n = n/2;
            for (size_t i = 0 ; i < n ; ++i)
            {
                x[i] += x[last-i];
            }
        }
    }
    return x.front();
}

#endif /* PAIRWISE_SUM_HPP_ */
