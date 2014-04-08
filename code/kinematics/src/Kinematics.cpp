#include "Kinematics.hpp"
#include <rw/kinematics/StateStructure.hpp>
#include <rw/kinematics/Frame.hpp>
#include <rw/kinematics/MovableFrame.hpp>
#include <rw/kinematics/FixedFrame.hpp>
#include <rw/kinematics/Kinematics.hpp>
#include "boost/shared_ptr.hpp"
#include "KinematicsException.hpp"
#include "test_macros.hpp"

rw::math::Rotation3D<double>& assign(rw::math::Rotation3D<double>& lhs, const RotationMatrix& rhs);
rw::math::Vector3D<double>& assign(rw::math::Vector3D<double>& lhs, const Point& P);

rw::math::Rotation3D<double>& assign(rw::math::Rotation3D<double>& lhs, const RotationMatrix& rhs)
{
    for (size_t i = 0 ; i < 3 ; ++i)
    {
        for (size_t j = 0 ; j < 3 ; ++j)
        {
            lhs(i,j) = rhs(i,j);
        }
    }
    return lhs;
}

rw::math::Vector3D<double>& assign(rw::math::Vector3D<double>& lhs, const Point& P)
{
    lhs(0) = P.x;
    lhs(1) = P.y;
    lhs(2) = P.z;
    return lhs;
}

class Kinematics::Impl
{
    public:
        Impl() : tree(new rw::kinematics::StateStructure()) {}

        ~Impl(){}
        Impl& operator=(const Impl& rhs)
        {
            if (this!=&rhs)
            {
                tree = rhs.tree;
            }
            return *this;
        }

        Impl(const Impl& rhs) : tree(rhs.tree)
        {
        }

        void add_frame(const std::string& name, const Point& P, const RotationMatrix& R)
        {
            rw::kinematics::State state;
            state = tree->upgradeState(state);
            rw::kinematics::State state2 = tree->getDefaultState();
            (void)state2;
            rw::kinematics::MovableFrame* child = new rw::kinematics::MovableFrame(name);
            rw::math::Rotation3D<double> Rrw;
            assign(Rrw, R);
            rw::math::Vector3D<double> u;
            assign(u, P);
            rw::math::Transform3D<double> T(u, Rrw);
            child->setTransform(T, state);
            rw::kinematics::Frame* parent = get_frame(P);
            if (parent)
            {
                tree->addFrame(child, parent);
            }
            else
            {
                tree->addFrame(child);
            }
        }

        rw::kinematics::Frame* get_frame(const Point& P) const
        {
            if (P.frame == "fixed") return NULL;
            rw::kinematics::Frame* f = tree->findFrame(P.frame);
            if (not(f))
            {
                THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Unable to find frame '") + P.frame + "'");
            }
            return f;
        }

        boost::shared_ptr<rw::kinematics::StateStructure> tree;
};

Kinematics::Kinematics() : pimpl(std::tr1::shared_ptr<Impl>(new Impl()))
{

}

void Kinematics::add_frame(const std::string& name, const Point& P, const RotationMatrix& R)
{
    pimpl->add_frame(name, P, R);
}

Point Kinematics::project(const Point& , const std::string& frame)
{
    return Point(frame, 0, 0, 0);
}
