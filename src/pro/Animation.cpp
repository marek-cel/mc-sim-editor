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
    _node = node;
}

Result Animation::read(const QDomElement* node)
{
    Result result = Result::Success;

    _t_min = node->attribute("t_min").toDouble();
    _t_max = node->attribute("t_max").toDouble();

    QDomElement child_node = node->firstChildElement();
    while ( !child_node.isNull() && result == Result::Success )
    {
        result = readKeyframe(&child_node);
        child_node = child_node.nextSiblingElement();
    }

    sortKeyframes();
    updateAnimationPath();

    return result;
}

Result Animation::save(QDomDocument* doc, QDomElement* parent)
{
    Result result = Result::Success;

    if ( _keyframes.size() > 0 )
    {
        QDomElement node = doc->createElement(kTagName);
        parent->appendChild(node);

        QDomAttr node_t_min = doc->createAttribute("t_min");
        QDomAttr node_t_max = doc->createAttribute("t_max");

        node_t_min.setValue(QString::number(getTimeMin(), 'f', 4));
        node_t_max.setValue(QString::number(getTimeMax(), 'f', 4));

        node.setAttributeNode(node_t_min);
        node.setAttributeNode(node_t_max);

        for ( auto keyframe : _keyframes )
        {
            if ( result == Result::Success ) result = keyframe->save(doc, &node);
        }
    }

    return result;
}

void Animation::addKeyframe(std::shared_ptr<Keyframe> keyframe)
{
    _keyframes.push_back(keyframe);
    sortKeyframes();
    updateAnimationPath();
}

void Animation::removeKeyframe(int index)
{
    _keyframes.erase(_keyframes.begin() + index);
    updateAnimationPath();
}

void Animation::setKeyframe(int index, std::shared_ptr<Keyframe> keyframe)
{
    _keyframes.at(index) = keyframe;
    sortKeyframes();
    updateAnimationPath();
}

void Animation::setTimeMin(double t_min)
{
    _t_min = std::min(t_min, _t_max);
}

void Animation::setTimeMax(double t_max)
{
    _t_max = std::max(t_max, _t_min);
}

void Animation::setTime(double time)
{
    if ( _apcb.valid() )
    {
        _apcb->reset();
        _apcb->setTimeOffset(-time);
    }
}

Result Animation::readKeyframe(const QDomElement* node)
{
    Result result = Result::Success;

    std::shared_ptr<Keyframe> keyframe = std::make_shared<Keyframe>();

    if ( result == Result::Success ) result = keyframe->read(node);

    if ( result == Result::Success )
    {
        _keyframes.push_back(keyframe);
    }

    return result;
}

void Animation::sortKeyframes()
{
    std::sort(_keyframes.begin(), _keyframes.end(),
        [](std::shared_ptr<Keyframe> a, std::shared_ptr<Keyframe> b)
        {
            return a->t() < b->t();
    });
}

void Animation::updateAnimationPath()
{
    _node->setUpdateCallback(nullptr);

    if ( _keyframes.size() > 0 )
    {
        osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
        path->setLoopMode(osg::AnimationPath::SWING);

        for ( auto keyframe : _keyframes )
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

        _apcb = new osg::AnimationPathCallback();
        //_apcb->setPause(true);
        //_apcb->setTimeMultiplier(0.0);

        _apcb->setAnimationPath(path.get());
        _node->setUpdateCallback(_apcb.get());
    }
}

} // namespace pro
} // namespace mc
