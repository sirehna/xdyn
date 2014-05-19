/*
 * kahan_sum.hpp
 *
 *  Created on: May 19, 2014
 *      Author: cady
 */

#ifndef KAHAN_SUM_HPP_
#define KAHAN_SUM_HPP_

#include <iterator>  // std::distance

#include <boost/iterator/counting_iterator.hpp>

template <typename T> class Kahan
{
    public:
        Kahan() : zero(T() - T()), compensation(zero), sum(zero)
        {
        }
        void add(const T& t)
        {
            const T myTmp1 = t - compensation;
            const T myTmp2 = sum + myTmp1;
            compensation = (myTmp2 - sum) - myTmp1;
            sum = myTmp2;
        }
        T get_sum() const
        {
            return sum;
        }
    private:
        T zero;
        T compensation;
        T sum;
};

template <typename IdxIterator, typename ListIterator, typename T>  T kahan_sum(const IdxIterator& begin, const IdxIterator& end, const ListIterator& first)
{
    IdxIterator it = begin;
    Kahan<T> k;
    for (;it!=end ; ++it)
    {
        k.add(*(first+*it));
    }
    return k.get_sum();
}

template <typename Iterator, typename T>  T kahan_sum(const Iterator& begin, const Iterator& end)
{
    return kahan_sum<typename boost::counting_iterator<size_t>,
                     Iterator,
                     T>(boost::counting_iterator<size_t>(0),boost::counting_iterator<size_t>(std::distance(begin,end)), begin);
}

template <typename T>  T kahan_sum(const std::vector<T>& points)
{
    return kahan_sum<typename std::vector<T>::const_iterator, T>(points.begin(), points.end());
}

template <typename T>  T kahan_sum(const std::vector<size_t>& idx, const std::vector<T>& points)
{
    return kahan_sum<typename std::vector<size_t>::const_iterator,
                     typename std::vector<T>::const_iterator,
                     T>(idx.begin(), idx.end(), points.begin());
}

#endif /* KAHAN_SUM_HPP_ */
