#include "Kinematics.hpp"
#include "KinematicsException.hpp"
#include "DataSource.hpp"
#include "Transform.hpp"
#include "KinematicTree.hpp"

std::string make_transform_name(const std::string& from_frame, const std::string& to_frame);
std::string make_transform_name(const std::string& from_frame, const std::string& to_frame)
{
    return from_frame + " -> " + to_frame;
}

std::string make_transform_name(const std::pair<std::string,std::string>& from_to);
std::string make_transform_name(const std::pair<std::string,std::string>& from_to)
{
    return make_transform_name(from_to.first, from_to.second);
}


class InverseTransformComputer : public DataSourceModule
{
    public:
        InverseTransformComputer(DataSource* const data_source, const std::string& module_name) : DataSourceModule(data_source, module_name), name_of_direct_transform(""), name_of_inverse_transform("")
        {
        }

        InverseTransformComputer(const InverseTransformComputer& rhs, DataSource* const data_source) : DataSourceModule(rhs, data_source), name_of_direct_transform(rhs.name_of_direct_transform), name_of_inverse_transform(rhs.name_of_inverse_transform)
        {
        }

        DataSourceModule* clone() const
        {
            return new InverseTransformComputer(*this);
        }

        DataSourceModule* clone(DataSource* const data_source) const
        {
            return new InverseTransformComputer(*this, data_source);
        }

        void update() const
        {
            const kinematics::Transform t = ds->get<kinematics::Transform>(name_of_direct_transform);
            ds->set(name_of_inverse_transform, t.inverse());
        }

        std::string name_of_direct_transform;
        std::string name_of_inverse_transform;
};


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
            const std::string direct_transform = make_transform_name(t.get_from_frame(), t.get_to_frame());
            const std::string inverse_transform = make_transform_name(t.get_to_frame(), t.get_from_frame());
            ds.set(direct_transform, t);
            InverseTransformComputer computer(&ds, std::string("reverse(")+direct_transform+")");
            computer.name_of_direct_transform = direct_transform;
            computer.name_of_inverse_transform = inverse_transform;
            ds.add(computer);
        }

        kinematics::Transform get(const std::string& from_frame, const std::string& to_frame)
        {
            try
            {
                return ds.get<kinematics::Transform>(make_transform_name(from_frame, to_frame));
            }
            catch (const DataSourceException& )
            {
                THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Unable to compute transform from ") + from_frame + " to " + to_frame);
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
