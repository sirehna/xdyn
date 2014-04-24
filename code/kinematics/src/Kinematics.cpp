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

class CompositeTransformComputer : public DataSourceModule
{
    public:
        CompositeTransformComputer(DataSource* const data_source, const std::string& module_name) : DataSourceModule(data_source, module_name), transforms(std::vector<std::pair<std::string,std::string> >())
        {
        }

        CompositeTransformComputer(const CompositeTransformComputer& rhs, DataSource* const data_source) : DataSourceModule(rhs, data_source), transforms(std::vector<std::pair<std::string,std::string> >())
        {
        }

        DataSourceModule* clone() const
        {
            return new CompositeTransformComputer(*this);
        }

        DataSourceModule* clone(DataSource* const data_source) const
        {
            return new CompositeTransformComputer(*this, data_source);
        }

        void update() const
        {
            kinematics::Transform t = ds->get<kinematics::Transform>(make_transform_name(transforms.front()));
            const std::string name_of_transform = make_transform_name(transforms.front().first,transforms.back().second);

            for (size_t i = 1 ; i < transforms.size() ; ++i)
            {
                t = ds->get<kinematics::Transform>(make_transform_name(transforms[i])) * t;
            }
            ds->set(name_of_transform, t);
        }

        PathType transforms;
};


class Kinematics::Impl
{
    public:
        Impl() : ds(DataSource()), tree(KinematicTree()) {}

        ~Impl(){}
        Impl& operator=(const Impl& rhs)
        {
            if (this!=&rhs)
            {
                ds = rhs.ds;
                tree = rhs.tree;
            }
            return *this;
        }

        Impl(const Impl& rhs) : ds(rhs.ds), tree(rhs.tree)
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
            tree.add(t.get_from_frame(), t.get_to_frame());
        }

        kinematics::Transform get(const std::string& from_frame, const std::string& to_frame)
        {
            if (from_frame == to_frame) return kinematics::identity(from_frame);
            try
            {
                return ds.get<kinematics::Transform>(make_transform_name(from_frame, to_frame));
            }
            catch (const DataSourceException& )
            {
                try
                {
                    CompositeTransformComputer computer(&ds, std::string("composite(")+make_transform_name(from_frame, to_frame)+")");
                    computer.transforms = tree.get_path(from_frame, to_frame);
                    ds.add(computer);
                    tree.add(from_frame, to_frame); // Register compose transform in kinematic tree so it can be used when computing shortest paths
                    return ds.get<kinematics::Transform>(make_transform_name(from_frame, to_frame));
                }
                catch (const KinematicsException& )
                {
                    THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Unable to compute transform from ") + make_transform_name(from_frame, to_frame));
                }
            }
        }

    private:
        DataSource ds;
        KinematicTree tree;

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
