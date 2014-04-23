#include "Kinematics.hpp"
#include "KinematicsException.hpp"
#include "DataSource.hpp"
#include "Transform.hpp"

class Kinematics::Impl
{
    public:
        Impl() : ds(DataSource()) {}

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

        void add(const kinematics::Transform& t)
        {
            ds.set(t.get_from_frame() + " -> " + t.get_to_frame(), t);
        }

        kinematics::Transform get(const std::string& from_frame, const std::string& to_frame)
        {
            try
            {
                return ds.get<kinematics::Transform>(from_frame + " -> " + to_frame);
            }
            catch (const DataSourceException& )
            {
                THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Unable to find transform from ") + from_frame + " to " + to_frame);
            }
        }

    private:
        DataSource ds;

};

void Kinematics::add(const kinematics::Transform& t)
{
    pimpl->add(t);
}

kinematics::Transform Kinematics::get(const std::string& from_frame, const std::string& to_frame)
{
    return pimpl->get(from_frame, to_frame);
}

Kinematics::Kinematics() : pimpl(new Impl())
{

}
