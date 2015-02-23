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

struct BodyStates;
struct EnvironmentAndFrames;
class ForceModel;

class Observer;

typedef TR1(shared_ptr)<ForceModel> ForcePtr;
typedef std::function<boost::optional<ForcePtr>(const std::string&, const std::string&, const std::string&, const EnvironmentAndFrames&)> ForceParser;

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
        ForceModel(const std::string& force_name, const std::string& body_name);
        virtual ~ForceModel(){}
        void update(const BodyStates& body, const double t);
        virtual ssc::kinematics::Wrench operator()(const BodyStates& body, const double t) const = 0;
        virtual double potential_energy(const BodyStates& body, const std::vector<double>& x) const {(void)body;(void)x;return 0;}
        virtual std::string get_name() const;
        virtual bool is_a_surface_force_model() const;
        ssc::kinematics::Wrench get_force_in_body_frame() const;
        ssc::kinematics::Wrench get_force_in_ned_frame() const;
        void feed(Observer& observer) const;

        template <typename ForceType>
        static typename boost::enable_if<HasParse<ForceType>, ForceParser>::type build_parser()
        {
            auto parser = [](const std::string& model, const std::string& yaml, const std::string& body, const EnvironmentAndFrames& env) -> boost::optional<ForcePtr>
                          {
                              boost::optional<ForcePtr> ret;
                              if (model == ForceType::model_name)
                              {
                                  ret.reset(ForcePtr(new ForceType(ForceType::parse(yaml), body, env)));
                              }
                              return ret;
                          };
            return parser;
        }

        template <typename ForceType>
        static typename boost::disable_if<HasParse<ForceType>, ForceParser>::type build_parser()
        {
            auto parser = [](const std::string& model, const std::string& , const std::string& body, const EnvironmentAndFrames& env) -> boost::optional<ForcePtr>
                          {
                              boost::optional<ForcePtr> ret;
                              if (model == ForceType::model_name)
                              {
                                  ret.reset(ForcePtr(new ForceType(body, env)));
                              }
                              return ret;
                          };
            return parser;
        }

        static ssc::kinematics::Wrench project_into_NED_frame(const ssc::kinematics::Wrench& F, const ssc::kinematics::RotationMatrix& R);

    protected:
        virtual void extra_observations(Observer& observer) const;

    private:
        ForceModel(); // Disabled

        std::string force_name;
        std::string body_name;
        ssc::kinematics::Wrench force_in_body_frame;
        ssc::kinematics::Wrench force_in_ned_frame;
};

typedef std::vector<ForcePtr> ListOfForces;

#endif /* FORCEMODEL_HPP_ */
