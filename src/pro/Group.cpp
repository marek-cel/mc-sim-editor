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

#include <pro/Group.h>

#include <pro/Animated.h>
#include <pro/Components.h>

namespace mc {
namespace pro {

Group::Group(osg::Group* group)
    : Component(group ? group : new osg::Group())
{
    group_ = dynamic_cast<osg::Group*>(node_.get());
    SetName("Group");
}

std::unique_ptr<Component> Group::Clone() const
{
    std::unique_ptr<Group> group = std::make_unique<Group>();
    group->SetName(GetName());
    group->CloneChildren(&children_);
    return group;
}

void Group::SetProjFile(QString proj_file)
{
    Component::SetProjFile(proj_file);

    for ( auto child : children_ )
    {
        child->SetProjFile(proj_file);
    }
}

void Group::SetAnimationTime(double time)
{
    for ( auto child : children_ )
    {
        child->SetAnimationTime(time);
    }
}

void Group::SetChildrenAnimationState(bool enabled)
{
    for ( auto child : children_ )
    {
        child->SetChildrenAnimationState(enabled);

        std::shared_ptr<pro::Animated> anim
                = std::dynamic_pointer_cast<pro::Animated>(child);
        if ( anim )
        {
            anim->SetAnimationEnabled(enabled);
        }
    }
}

Result Group::Read(const QDomElement* node)
{
    Result result = Component::Read(node);

    QDomElement child_node = node->firstChildElement();
    while ( !child_node.isNull() && result == Result::Success )
    {
        result = ReadChild(&child_node);
        child_node = child_node.nextSiblingElement();
    }

    return result;
}

Result Group::Save(QDomDocument* doc, QDomElement* parent)
{
    QDomElement node = doc->createElement(GetTagName());
    parent->appendChild(node);

    Result result = SaveParameters(doc, &node);

    if ( result == Result::Success ) result = SaveChildren(doc, &node);

    return result;
}

void Group::Update()
{
    for ( auto child : children_ )
    {
        child->Update();
    }
}

Result Group::AddChild(std::shared_ptr<Component> child)
{
    child->SetParent(shared_from_this());
    children_.push_back(child);
    group_->addChild(child->GetNode());
    return Result::Success;
}

Result Group::RemoveChild(std::shared_ptr<Component> child)
{
    Component* child_raw_ptr = child.get();
    Children::iterator iter = children_.begin();

    for ( unsigned int i = 0; i < children_.size(); ++i )
    {
        if ( child_raw_ptr == (*iter).get() )
        {
            children_.erase(iter);
            group_->removeChild(i);

            return Result::Success;
        }

        ++iter;
    }

    return Result::Failure;
}

void Group::CloneChildren(const Children* children)
{
    for ( auto child : *children )
    {
        AddChild(std::shared_ptr<Component>(child->Clone()));
    }
}

Result Group::ReadChild(QDomElement* node)
{
    Result result = Result::Success;

    Components::Type type = Components::Instance()->GetComponentByTagName(node->tagName());
    if ( !type.component )
    {
        return Result::Failure;
    }

    std::shared_ptr<Component> comp = std::move(type.component->Clone());
    comp->SetProjFile(proj_file_);
    if ( result == Result::Success ) result = AddChild(comp);
    if ( result == Result::Success ) result = comp->Read(node);

    return result;
}

Result Group::SaveChildren(QDomDocument* doc, QDomElement* parent)
{
    Result result = Result::Success;
    for ( auto child : children_ )
    {
        if ( result == Result::Success ) result = child->Save(doc, parent);
    }
    return result;
}

Result Group::SaveParameters(QDomDocument* doc, QDomElement* node)
{
    ////////////////////////////////////////////
    return Component::SaveParameters(doc, node);
    ////////////////////////////////////////////
}

} // namespace pro
} // namespace mc
