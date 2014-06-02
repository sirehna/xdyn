/*
 * pairwise_sum.hpp
 *
 *  Created on: May 19, 2014
 *      Author: cady
 */

#ifndef PAIRWISE_SUM_HPP_
#define PAIRWISE_SUM_HPP_

#include <cmath>
#include <iterator>  // std::distance
#include <boost/iterator/counting_iterator.hpp>

namespace sum
{
    template <typename T> T pairwise(typename std::vector<T> x)
    {
        if (x.empty()) return T()-T();
        if (x.size() == 1) return x.front();
        size_t n = x.size();
        const size_t p = (size_t)floor(n/2.+0.5);
        size_t last = 0;
        for (size_t i = 0 ; i < p ; ++i)
        {
            last = n-1;
            if (n % 2)
            {
                n = (n+1)/2;
                for (size_t i = 0 ; i < n-1 ; ++i)
                {
                    x[i] = x[i] + x[last-i];
                }
            }
            else
            {
                n = n/2;
                for (size_t i = 0 ; i < n ; ++i)
                {
                    x[i] = x[i] + x[last-i];
                }
            }
        }
        return x.front();
    }

    template <typename IdxIterator, typename ListIterator, typename T>  T pairwise(const IdxIterator& begin, const IdxIterator& end, const ListIterator& first)
    {
        IdxIterator it = begin;
        std::vector<T> x;
        for (;it!=end ; ++it)
        {
            x.push_back(*(first+*it));
        }
        return pairwise(x);
    }

    template <typename Iterator, typename T>  T pairwise(const Iterator& begin, const Iterator& end)
    {
        return pairwise<typename boost::counting_iterator<size_t>,
                         Iterator,
                         T>(boost::counting_iterator<size_t>(0),boost::counting_iterator<size_t>(std::distance(begin,end)), begin);
    }

    template <typename T>  T pairwise(const std::vector<size_t>& idx, const std::vector<T>& points)
    {
        return pairwise<typename std::vector<size_t>::const_iterator,
                            typename std::vector<T>::const_iterator,
                            T>(idx.begin(), idx.end(), points.begin());
    }
}
#endif /* PAIRWISE_SUM_HPP_ */
