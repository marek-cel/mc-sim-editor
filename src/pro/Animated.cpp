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

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

////////////////////////////////////////////////////////////////////////////////

Animated::Animated(osg::Group* group)
    : Group(group)
{
    animation_ = std::make_shared<Animation>(node_);
}

////////////////////////////////////////////////////////////////////////////////

Result Animated::Read(const QDomElement* node)
{
    Result result = Component::Read(node);

    QDomElement child_node = node->firstChildElement();
    while ( !child_node.isNull() && result == Result::Success )
    {
        if ( child_node.tagName() == Animation::kTagName )
        {
            result = ReadAnimation(&child_node);
        }
        else
        {
            result = ReadChild(&child_node);
        }

        child_node = child_node.nextSiblingElement();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Result Animated::Save(QDomDocument* doc, QDomElement* parent)
{
    Result result = Component::Save(doc, parent);

    QDomElement node = parent->lastChildElement();
    if ( result == Result::Success ) result = SaveAnimation(doc, &node);
    if ( result == Result::Success ) result = SaveChildren(doc, &node);

    return result;
}

////////////////////////////////////////////////////////////////////////////////

void Animated::Update()
{
    ////////////////
    Group::Update();
    ////////////////

    animation_->SetTime(time_);
}

////////////////////////////////////////////////////////////////////////////////

void Animated::SetAnimationTime(double time)
{
    //////////////////////////////
    Group::SetAnimationTime(time);
    //////////////////////////////

    time_ = time;
    animation_->SetTime(time_);
}

////////////////////////////////////////////////////////////////////////////////

Result Animated::ReadAnimation(const QDomElement* node)
{
    Result result = Result::Success;

    animation_ = std::make_shared<Animation>(node_);
    result = animation_->Read(node);

    if ( result == Result::Success )
    {
        // TODO
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Result Animated::SaveAnimation(QDomDocument* doc, QDomElement* parent)
{
    return animation_->Save(doc, parent);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace pro
} // namespace mc
