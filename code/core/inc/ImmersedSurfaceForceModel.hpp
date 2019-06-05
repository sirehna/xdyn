/*
 * ImmersedSurfaceForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#ifndef IMMERSEDSURFACEFORCEMODEL_HPP_
#define IMMERSEDSURFACEFORCEMODEL_HPP_

#include "SurfaceForceModel.hpp"

/** \brief This class implemented SurfaceForceModel's begin & end methods
 *  \addtogroup mesh_wrappers
 *  \ingroup mesh_wrappers
 *  \section ex1 Example
 *  \snippet mesh_wrappers/unit_tests/src/ImmersedSurfaceForceModelTest.cpp ImmersedSurfaceForceModelTest example
 *  \section ex2 Expected output
 *  \snippet mesh_wrappers/unit_tests/src/ImmersedSurfaceForceModelTest.cpp ImmersedSurfaceForceModelTest expected output
 */
class ImmersedSurfaceForceModel : public SurfaceForceModel
{
    public:
        ImmersedSurfaceForceModel(const std::string& name, const std::string& body_name, const EnvironmentAndFrames& env);
        virtual ~ImmersedSurfaceForceModel();

    private:
        ImmersedSurfaceForceModel();
        FacetIterator begin(const MeshIntersectorPtr& intersector) const;
        FacetIterator end(const MeshIntersectorPtr& intersector) const;
};

#endif /* IMMERSEDSURFACEFORCEMODEL_HPP_ */
