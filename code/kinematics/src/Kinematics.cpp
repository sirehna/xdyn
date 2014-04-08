#include "Kinematics.hpp"
#include "KinematicsException.hpp"
#include "DataSource.hpp"
#include "test_macros.hpp"

class Kinematics::Impl
{
    public:
        Impl() :ds(DataSource()) {}

        ~Impl(){}
        Impl& operator=(const Impl& rhs)
        {
            if (this!=&rhs)
            {
                ds = rhs.ds;
            }
            return *this;
        }

        Impl(const Impl& rhs) : ds(rhs.ds)
        {
        }

        void add_frame(const std::string& , const Point& , const RotationMatrix& )
        {

        }


    private:
        DataSource ds;
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
