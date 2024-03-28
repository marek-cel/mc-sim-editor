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
    _switch = dynamic_cast<osg::Switch*>(_node.get());
    setName("Switch");
}

std::unique_ptr<Component> Switch::clone() const
{
    std::unique_ptr<Switch> sw = std::make_unique<Switch>();
    sw->setName(getName());
    sw->cloneChildren(&_children);
    return sw;
}

void Switch::setVisible(bool visible)
{
    _visible = visible;

    if ( _visible )
    {
        _switch->setAllChildrenOn();
    }
    else
    {
        _switch->setAllChildrenOff();
    }
}

Result Switch::readParameters(const QDomElement* node)
{
    ////////////////////////////////////////////
    Result result = Group::readParameters(node);
    ////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    bool visible = node->attribute("visible").toInt();
    setVisible(visible);

    return result;
}

Result Switch::saveParameters(QDomDocument* doc, QDomElement* node)
{
    /////////////////////////////////////////////////
    Result result = Group::saveParameters(doc, node);
    /////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QDomAttr node_visible = doc->createAttribute( "visible" );
    node_visible.setValue(QString::number(getVisible() ? 1 : 0));
    node->setAttributeNode(node_visible);

    return result;
}

} // namespace pro
} // namespace mc
