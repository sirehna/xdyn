#include "FixedFrame.hpp"
#include <rw/kinematics/StateStructure.hpp>
#include <rw/kinematics/Frame.hpp>
#include <rw/kinematics/MovableFrame.hpp>
#include <rw/kinematics/Kinematics.hpp>
#include "boost/shared_ptr.hpp"
#include "FrameException.hpp"
#include <map>

#include "test_macros.hpp"

using namespace kinematics;

class FixedFrame::Impl
{
    public:
        ~Impl(){}
        Impl& operator=(const Impl& rhs)
        {
            if (this!=&rhs)
            {
                tree = rhs.tree;
                world = rhs.world;
                name2frame = rhs.name2frame;
            }
            return *this;
        }

        Impl(const Impl& rhs) : tree(rhs.tree), world(rhs.world), name2frame(rhs.name2frame)
        {
        }

        Impl() : tree(new rw::kinematics::StateStructure()), world(tree->getRoot()), name2frame(std::map<std::string,rw::kinematics::Frame*>())
        {
        }

        void add_frame_to_tree(AbstractFrame* parent_frame, const FramePtr& child_frame)
        {
            if (parent_frame->is_base_frame())
            {
                tree->addFrame(create_rw_frame(child_frame),world);
            }
            else
            {
                tree->addFrame(create_rw_frame(child_frame),convert_to_rw_frame(parent_frame));
            }
        }

        rw::kinematics::Frame* convert_to_rw_frame(const AbstractFrame* frame) const
        {
            COUT(frame->get_name());
            if (not(frame)) return NULL;
            COUT("");
            if (frame->is_base_frame())
            {
                return NULL;
            }
            COUT("");
            std::map<std::string,rw::kinematics::Frame*>::const_iterator it = name2frame.find(frame->get_name());
            if (it == name2frame.end())
            {
                THROW(__PRETTY_FUNCTION__, FrameException, std::string("Unable to find frame '") + frame->get_name() + "' in tree.");
            }
            return it->second;
        }

        rw::kinematics::MovableFrame* create_rw_frame(const FramePtr& frame) const
        {
            rw::kinematics::State state = tree->getDefaultState();
            COUT(frame->get_name());
            rw::kinematics::MovableFrame* f = new rw::kinematics::MovableFrame(frame->get_name());
            f->setTransform(frame->get_transformation_from_parent_to_this(), state);
            COUT("");
            return f;
        }

        rw::math::Transform3D<double> get_transform(const rw::kinematics::Frame* from, const rw::kinematics::Frame* to) const
        {
            rw::kinematics::State state = tree->getDefaultState();
            return rw::kinematics::Kinematics::frameTframe(from, to, state);
        }

        rw::math::Transform3D<double> from_fixed_to_frame(const FramePtr& frame) const
        {
            const rw::kinematics::Frame* to = convert_to_rw_frame(frame.get());
            return get_transform(world, to);
        }

        rw::math::Transform3D<double> get_transformation_from_frame_to_frame(const std::string& from, const FramePtr& to) const
        {
            std::map<std::string,rw::kinematics::Frame*>::const_iterator it = name2frame.find(from);
            if (it == name2frame.end())
            {
                THROW(__PRETTY_FUNCTION__, FrameException, std::string("Unable to find frame '") + from + "' in tree.");
            }
            const rw::kinematics::Frame* from_frame = it->second;
            const rw::kinematics::Frame* to_frame = convert_to_rw_frame(to.get());
            return get_transform(from_frame, to_frame);
        }

        boost::shared_ptr<rw::kinematics::StateStructure> tree;
        rw::kinematics::Frame* world;
        std::map<std::string,rw::kinematics::Frame*> name2frame;
};


FixedFrame::FixedFrame() : AbstractFrame(NULL, rw::math::Rotation3D<double>(), "base"), pimpl(new FixedFrame::Impl())
{
}

std::tr1::shared_ptr<FixedFrame> FixedFrame::get_instance()
{
    static std::tr1::shared_ptr<FixedFrame> instance;
    if (not(instance.get()))
    {
        instance = std::tr1::shared_ptr<FixedFrame>(new FixedFrame());
    }
    return instance;
}

rw::math::Transform3D<double> FixedFrame::get_transformation_from_parent_to_this() const
{
    return rw::math::Transform3D<double>();
}

rw::math::Transform3D<double> FixedFrame::get_transformation_from_this_to_frame(const FramePtr& from) const
{
    return pimpl->from_fixed_to_frame(from);
}

void FixedFrame::add_frame_to_tree(AbstractFrame* parent_frame_, const FramePtr& child_frame)
{
    pimpl->add_frame_to_tree(parent_frame_, child_frame);
}

rw::math::Transform3D<double> FixedFrame::get_transformation_from_frame_to_frame(const std::string& from, const FramePtr& to) const
{
    return pimpl->get_transformation_from_frame_to_frame(from, to);
}

bool FixedFrame::is_base_frame() const
{
    return true;
}
