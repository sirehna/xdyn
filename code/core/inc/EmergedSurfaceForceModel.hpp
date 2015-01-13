/*
 * EmergedSurfaceForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#ifndef EMERGEDSURFACEFORCEMODEL_HPP_
#define EMERGEDSURFACEFORCEMODEL_HPP_

#include "SurfaceForceModel.hpp"

/** \brief This class implemented SurfaceForceModel's begin & end methods
 *  \addtogroup mesh_wrappers
 *  \ingroup mesh_wrappers
 *  \section ex1 Example
 *  \snippet mesh_wrappers/unit_tests/src/ImmersedSurfaceForceModelTest.cpp ImmersedSurfaceForceModelTest example
 *  \section ex2 Expected output
 *  \snippet mesh_wrappers/unit_tests/src/ImmersedSurfaceForceModelTest.cpp ImmersedSurfaceForceModelTest expected output
 */
class EmergedSurfaceForceModel : public SurfaceForceModel
{
    public:
        EmergedSurfaceForceModel(const std::string& name, const std::string& body_name, const EnvironmentAndFrames& env);
        virtual ~EmergedSurfaceForceModel();

    private:
        EmergedSurfaceForceModel();
        FacetIterator begin(const MeshIntersectorPtr& intersector) const;
        FacetIterator end(const MeshIntersectorPtr& intersector) const;
};

#endif /* EMERGEDSURFACEFORCEMODEL_HPP_ */
