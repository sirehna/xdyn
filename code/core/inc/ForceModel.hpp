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

#include <ssc/macros.hpp>
#include TR1INC(memory)

class Body;
struct EnvironmentAndFrames;
class ForceModel;

typedef TR1(shared_ptr)<ForceModel> ForcePtr;
typedef std::function<boost::optional<ForcePtr>(const std::string&, const std::string, const EnvironmentAndFrames&)> ForceParser;

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
        ForceParser build_parser()
        {
            auto parser = [](const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) -> boost::optional<ForcePtr>
                          {
                              boost::optional<ForcePtr> ret;
                              if (model == ForceType::name)
                              {
                                  ret.reset(ForcePtr(new ForceType(ForceType::parse(yaml), env)));
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
