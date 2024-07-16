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
    _group = dynamic_cast<osg::Group*>(_node.get());
    setName("Group");
}

std::shared_ptr<Component> Group::clone() const
{
    std::shared_ptr<Group> group = std::make_shared<Group>();
    group->setName(getName());
    group->cloneChildren(&_children);
    return group;
}

void Group::setProjFile(QString proj_file)
{
    Component::setProjFile(proj_file);

    for ( auto child : _children )
    {
        child->setProjFile(proj_file);
    }
}

void Group::setAnimationTime(double time)
{
    for ( auto child : _children )
    {
        child->setAnimationTime(time);
    }
}

void Group::setChildrenAnimationState(bool enabled)
{
    for ( auto child : _children )
    {
        child->setChildrenAnimationState(enabled);

        std::shared_ptr<pro::Animated> anim
                = std::dynamic_pointer_cast<pro::Animated>(child);
        if ( anim )
        {
            anim->setAnimationEnabled(enabled);
        }
    }
}

Result Group::read(const QDomElement* node)
{
    Result result = Component::read(node);

    QDomElement child_node = node->firstChildElement();
    while ( !child_node.isNull() && result == Result::Success )
    {
        result = readChild(&child_node);
        child_node = child_node.nextSiblingElement();
    }

    return result;
}

Result Group::save(QDomDocument* doc, QDomElement* parent)
{
    QDomElement node = doc->createElement(getTagName());
    parent->appendChild(node);

    Result result = saveParameters(doc, &node);

    if ( result == Result::Success ) result = saveChildren(doc, &node);

    return result;
}

void Group::update()
{
    for ( auto child : _children )
    {
        child->update();
    }
}

Result Group::addChild(std::shared_ptr<Component> child)
{
    child->setParent(shared_from_this());
    _children.push_back(child);
    _group->addChild(child->getNode());
    return Result::Success;
}

Result Group::removeChild(std::shared_ptr<Component> child)
{
    Component* child_raw_ptr = child.get();
    Children::iterator iter = _children.begin();

    for ( unsigned int i = 0; i < _children.size(); ++i )
    {
        if ( child_raw_ptr == (*iter).get() )
        {
            _children.erase(iter);
            _group->removeChild(i);

            return Result::Success;
        }

        ++iter;
    }

    return Result::Failure;
}

void Group::cloneChildren(const Children* children)
{
    for ( auto child : *children )
    {
        addChild(child->clone());
    }
}

Result Group::readChild(QDomElement* node)
{
    Result result = Result::Success;

    Components::Type type = Components::instance()->getComponentByTagName(node->tagName());
    if ( !type.component )
    {
        return Result::Failure;
    }

    std::shared_ptr<Component> comp = std::move(type.component->clone());
    comp->setProjFile(_proj_file);
    if ( result == Result::Success ) result = addChild(comp);
    if ( result == Result::Success ) result = comp->read(node);

    return result;
}

Result Group::saveChildren(QDomDocument* doc, QDomElement* parent)
{
    Result result = Result::Success;
    for ( auto child : _children )
    {
        if ( result == Result::Success ) result = child->save(doc, parent);
    }
    return result;
}

Result Group::saveParameters(QDomDocument* doc, QDomElement* node)
{
    ////////////////////////////////////////////
    return Component::saveParameters(doc, node);
    ////////////////////////////////////////////
}

} // namespace pro
} // namespace mc
