/****************************************************************************//*
 *  Copyright (C) 2024 Marek M. Cel
 *
 *  This file is part of MC-Sim Editor.
 *
 *  MC-Sim Editor is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MC-Mass is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 ******************************************************************************/

#include <pro/Animation.h>

#include <iostream>

namespace mc {
namespace pro {

Animation::Animation(osg::ref_ptr<osg::Node> node)
{
    node_ = node;
}

Result Animation::Read(const QDomElement* node)
{
    Result result = Result::Success;

    t_min_ = node->attribute("t_min").toDouble();
    t_max_ = node->attribute("t_max").toDouble();

    QDomElement child_node = node->firstChildElement();
    while ( !child_node.isNull() && result == Result::Success )
    {
        result = ReadKeyframe(&child_node);
        child_node = child_node.nextSiblingElement();
    }

    SortKeyframes();
    UpdateAnimationPath();

    return result;
}

Result Animation::Save(QDomDocument* doc, QDomElement* parent)
{
    Result result = Result::Success;

    if ( keyframes_.size() > 0 )
    {
        QDomElement node = doc->createElement(kTagName);
        parent->appendChild(node);

        QDomAttr node_t_min = doc->createAttribute("t_min");
        QDomAttr node_t_max = doc->createAttribute("t_max");

        node_t_min.setValue(QString::number(GetTimeMin(), 'f', 4));
        node_t_max.setValue(QString::number(GetTimeMax(), 'f', 4));

        node.setAttributeNode(node_t_min);
        node.setAttributeNode(node_t_max);

        for ( auto keyframe : keyframes_ )
        {
            if ( result == Result::Success ) result = keyframe->Save(doc, &node);
        }
    }

    return result;
}

void Animation::AddKeyframe(std::shared_ptr<Keyframe> keyframe)
{
    keyframes_.push_back(keyframe);
    SortKeyframes();
    UpdateAnimationPath();
}

void Animation::RemoveKeyframe(int index)
{
    keyframes_.erase(keyframes_.begin() + index);
    UpdateAnimationPath();
}

void Animation::SetKeyframe(int index, std::shared_ptr<Keyframe> keyframe)
{
    keyframes_.at(index) = keyframe;
    SortKeyframes();
    UpdateAnimationPath();
}

void Animation::SetTimeMin(double t_min)
{
    t_min_ = std::min(t_min, t_max_);
}

void Animation::SetTimeMax(double t_max)
{
    t_max_ = std::max(t_max, t_min_);
}

void Animation::SetTime(double time)
{
    if ( apcb_.valid() )
    {
        apcb_->reset();
        apcb_->setTimeOffset(-time);
    }
}

Result Animation::ReadKeyframe(const QDomElement* node)
{
    Result result = Result::Success;

    std::shared_ptr<Keyframe> keyframe = std::make_shared<Keyframe>();

    if ( result == Result::Success ) result = keyframe->Read(node);

    if ( result == Result::Success )
    {
        keyframes_.push_back(keyframe);
    }

    return result;
}

void Animation::SortKeyframes()
{
    std::sort(keyframes_.begin(), keyframes_.end(),
        [](std::shared_ptr<Keyframe> a, std::shared_ptr<Keyframe> b)
        {
            return a->t() < b->t();
    });
}

void Animation::UpdateAnimationPath()
{
    node_->setUpdateCallback(nullptr);

    if ( keyframes_.size() > 0 )
    {
        osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
        path->setLoopMode(osg::AnimationPath::SWING);

        for ( auto keyframe : keyframes_ )
        {
            osg::Vec3d v(keyframe->px(), keyframe->py(), keyframe->pz());
            osg::Quat q(osg::DegreesToRadians(keyframe->ax()), osg::X_AXIS,
                        osg::DegreesToRadians(keyframe->ay()), osg::Y_AXIS,
                        osg::DegreesToRadians(keyframe->az()), osg::Z_AXIS);
            path->insert(keyframe->t(), osg::AnimationPath::ControlPoint(v, q));

            if ( false )
            {
                std::cout << keyframe->t();
                std::cout << "\t" << keyframe->px();
                std::cout << "\t" << keyframe->py();
                std::cout << "\t" << keyframe->pz();
                std::cout << "\t" << keyframe->ax();
                std::cout << "\t" << keyframe->ay();
                std::cout << "\t" << keyframe->az();
                std::cout << std::endl;
            }
        }

        apcb_ = new osg::AnimationPathCallback();
        //apcb_->setPause(true);
        //apcb_->setTimeMultiplier(0.0);

        apcb_->setAnimationPath(path.get());
        node_->setUpdateCallback(apcb_.get());
    }
}

} // namespace pro
} // namespace mc
