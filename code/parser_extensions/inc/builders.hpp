/*
 * builders.hpp
 *
 *  Created on: Aug 12, 2014
 *      Author: cady
 */

#ifndef BUILDERS_HPP_
#define BUILDERS_HPP_

#include "DefaultSurfaceElevation.hpp"
#include "SurfaceElevationBuilder.hpp"
#include "ForceBuilder.hpp"
#include "HydrostaticForceModel.hpp"
#include "GravityForceModel.hpp"


template <>
class SurfaceElevationBuilder<DefaultSurfaceElevation> : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                                const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;
};


/*
template <>
class ForceBuilder<GravityForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};



template <>
class ForceBuilder<HydrostaticForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};
*/
#endif /* BUILDERS_HPP_ */
