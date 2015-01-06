/*
 * ForceModel.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef FORCEMODEL_HPP_
#define FORCEMODEL_HPP_

#include <functional>
#include <vector>
#include <ssc/kinematics.hpp>

#include <boost/optional/optional.hpp>
#include <boost/utility/enable_if.hpp>

#include <ssc/macros.hpp>
#include TR1INC(memory)

class Body;
struct EnvironmentAndFrames;
class ForceModel;

typedef TR1(shared_ptr)<ForceModel> ForcePtr;
typedef std::function<boost::optional<ForcePtr>(const std::string&, const std::string, const EnvironmentAndFrames&)> ForceParser;


// SFINAE test for 'parse' method
template<typename T>
struct HasParse
{
    typedef char yes[1];
    typedef char no [2];
    template<typename U> static yes &check(typeof(&U::parse)*);
    template<typename U> static no &check(...);
    static const bool value = sizeof(check<T>(0)) == sizeof(yes);
};

class ForceModel
{
    public:
        ForceModel(const std::string& name);
        virtual ~ForceModel(){}
        virtual ssc::kinematics::Wrench operator()(const Body& body, const double t) const = 0;
        virtual double potential_energy(const Body& body, const std::vector<double>& x) const {(void)body;(void)x;return 0;}
        std::string get_name() const;
        virtual bool is_a_surface_force_model() const;

        template <typename ForceType>
        static typename boost::enable_if<HasParse<ForceType>, ForceParser>::type build_parser()
        {
            auto parser = [](const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) -> boost::optional<ForcePtr>
                          {
                              boost::optional<ForcePtr> ret;
                              if (model == ForceType::model_name)
                              {
                                  ret.reset(ForcePtr(new ForceType(ForceType::parse(yaml), env)));
                              }
                              return ret;
                          };
            return parser;
        }

        template <typename ForceType>
        static typename boost::disable_if<HasParse<ForceType>, ForceParser>::type build_parser()
        {
            auto parser = [](const std::string& model, const std::string& , const EnvironmentAndFrames& env) -> boost::optional<ForcePtr>
                          {
                              boost::optional<ForcePtr> ret;
                              if (model == ForceType::model_name)
                              {
                                  ret.reset(ForcePtr(new ForceType(env)));
                              }
                              return ret;
                          };
            return parser;
        }

    private:
        ForceModel(); // Disabled
        std::string name;
};

typedef std::vector<ForcePtr> ListOfForces;

#endif /* FORCEMODEL_HPP_ */
