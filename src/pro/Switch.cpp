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

#include <pro/Switch.h>

namespace mc {
namespace pro {

Switch::Switch(osg::Switch* pat)
    : Group(pat ? pat : new osg::Switch())
{
    switch_ = dynamic_cast<osg::Switch*>(node_.get());
    SetName("Switch");
}

std::unique_ptr<Component> Switch::Clone() const
{
    std::unique_ptr<Switch> sw = std::make_unique<Switch>();
    sw->SetName(GetName());
    sw->CloneChildren(&children_);
    return sw;
}

void Switch::SetVisible(bool visible)
{
    visible_ = visible;

    if ( visible_ )
    {
        switch_->setAllChildrenOn();
    }
    else
    {
        switch_->setAllChildrenOff();
    }
}

Result Switch::ReadParameters(const QDomElement* node)
{
    ////////////////////////////////////////////
    Result result = Group::ReadParameters(node);
    ////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    bool visible = node->attribute("visible").toInt();
    SetVisible(visible);

    return result;
}

Result Switch::SaveParameters(QDomDocument* doc, QDomElement* node)
{
    /////////////////////////////////////////////////
    Result result = Group::SaveParameters(doc, node);
    /////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QDomAttr node_visible = doc->createAttribute( "visible" );
    node_visible.setValue(QString::number(GetVisible() ? 1 : 0));
    node->setAttributeNode(node_visible);

    return result;
}

} // namespace pro
} // namespace mc
