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

#include <pro/Animated.h>

namespace mc {
namespace pro {

Animated::Animated(osg::Group* group)
    : Group(group)
{
    _animation = std::make_shared<Animation>(_node);
}

Result Animated::read(const QDomElement* node)
{
    Result result = Component::read(node);

    QDomElement child_node = node->firstChildElement();
    while ( !child_node.isNull() && result == Result::Success )
    {
        if ( child_node.tagName() == Animation::kTagName )
        {
            result = readAnimation(&child_node);
        }
        else
        {
            result = readChild(&child_node);
        }

        child_node = child_node.nextSiblingElement();
    }

    return result;
}

Result Animated::save(QDomDocument* doc, QDomElement* parent)
{
    Result result = Component::save(doc, parent);

    QDomElement node = parent->lastChildElement();
    if ( result == Result::Success ) result = saveAnimation(doc, &node);
    if ( result == Result::Success ) result = saveChildren(doc, &node);

    return result;
}

void Animated::update()
{
    ////////////////
    Group::update();
    ////////////////

    _animation->setTime(_time);
}

void Animated::setAnimationTime(double time)
{
    //////////////////////////////
    Group::setAnimationTime(time);
    //////////////////////////////

    if ( _enabled )
    {
        _time = time;
    }

    _animation->setTime(_time);
}

Result Animated::readAnimation(const QDomElement* node)
{
    Result result = Result::Success;

    _animation = std::make_shared<Animation>(_node);
    result = _animation->read(node);

    if ( result == Result::Success )
    {
        // TODO
    }

    return result;
}

Result Animated::saveAnimation(QDomDocument* doc, QDomElement* parent)
{
    return _animation->save(doc, parent);
}

Result Animated::readParameters(const QDomElement* node)
{
    ////////////////////////////////////////////
    Result result = Group::readParameters(node);
    ////////////////////////////////////////////

    _enabled = node->attribute("anim_enabled").toInt();

    return result;
}

Result Animated::saveParameters(QDomDocument* doc, QDomElement* node)
{
    /////////////////////////////////////////////////
    Result result = Group::saveParameters(doc, node);
    /////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QDomAttr node_enabled = doc->createAttribute("anim_enabled");
    node_enabled.setValue(QString::number(_enabled ? 1 : 0));
    node->setAttributeNode(node_enabled);

    return result;
}

} // namespace pro
} // namespace mc
