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

#include <cgi/Component.h>

namespace mc {
namespace cgi {

Component::Component(std::shared_ptr<Data> data)
    : _data(data)
{
    _root = new osg::Group();
    _children.clear();
}

Component::~Component()
{
    removeAllChildren();
}

void Component::addChild(std::shared_ptr<Component> child)
{
    _children.push_back(child);
    _root->addChild(child->root().get());
}

void Component::update()
{
    for ( auto child : _children )
    {
        child->update();
    }
}

void Component::setProject(std::shared_ptr<pro::Project> proj)
{
    for ( auto child : _children )
    {
        child->setProject(proj);
    }
}

void Component::removeAllChildren()
{
    if ( _root->getNumChildren() > 0 )
    {
        _root->removeChildren(0, _root->getNumChildren());
    }

    List::iterator it = _children.begin();
    while ( it != _children.end() )
    {
        it = _children.erase(it);
    }
}

} // namespace cgi
} // namespace mc
