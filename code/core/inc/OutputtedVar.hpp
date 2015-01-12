/*
 * OutputtedVar.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */


#ifndef OUTPUTTEDVAR_HPP_
#define OUTPUTTEDVAR_HPP_

#include <string>
#include <vector>

class Observer;


class OutputtedVar
{
    public:
        virtual ~OutputtedVar();
        virtual void write_on(Observer& obs) const = 0;
};

template <typename T> class TypedOutputtedVar : public OutputtedVar
{
    public:
        TypedOutputtedVar(const T& val, const std::vector<std::string>& w, const std::string& k) : what(val), where(w), key(k)
        {
        }

        void write_on(Observer& obs) const;

    private:
        T what;
        std::vector<std::string> where;
        std::string key;
};

#endif  /* OUTPUTTEDVAR_HPP_ */
