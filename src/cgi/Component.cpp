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
    : data_(data)
{
    root_ = new osg::Group();

    children_.clear();
}

Component::~Component()
{
    RemoveAllChildren();
}

void Component::AddChild(std::shared_ptr<Component> child)
{
    children_.push_back(child);
    root_->addChild(child->root().get());
}

void Component::Update()
{
    for ( auto child : children_ )
    {
        child->Update();
    }
}

void Component::SetProject(std::shared_ptr<pro::Project> proj)
{
    for ( auto child : children_ )
    {
        child->SetProject(proj);
    }
}

void Component::RemoveAllChildren()
{
    if ( root_->getNumChildren() > 0 )
    {
        root_->removeChildren(0, root_->getNumChildren());
    }

    List::iterator it = children_.begin();
    while ( it != children_.end() )
    {
        it = children_.erase(it);
    }
}

} // namespace cgi
} // namespace mc
