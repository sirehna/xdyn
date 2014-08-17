#ifndef DEFAULTSURFACEELEVATIONBUILDER_HPP_
#define DEFAULTSURFACEELEVATIONBUILDER_HPP_

#include "DefaultSurfaceElevation.hpp"
#include "SurfaceElevationBuilder.hpp"

template <>
class SurfaceElevationBuilder<DefaultSurfaceElevation> : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                    const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;
};



#endif /* DEFAULTSURFACEELEVATIONBUILDER_HPP_ */
