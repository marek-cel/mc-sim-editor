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

#include <pro/LOD.h>

namespace mc {
namespace pro {

LOD::LOD(osg::LOD* lod)
    : Group(lod ? lod : new osg::LOD())
{
    _lod = dynamic_cast<osg::LOD*>(_node.get());
    setName("LOD");
    addInterval(1000.0);
}

Result LOD::addChild(std::shared_ptr<Component> child)
{
    child->setParent(shared_from_this());
    _children.push_back(child);

    inflateLOD();

    return Result::Success;
}

std::unique_ptr<Component> LOD::clone() const
{
    std::unique_ptr<LOD> lod = std::make_unique<LOD>();
    lod->setName(getName());
    lod->cloneChildren(&_children);
    return lod;
}

void LOD::addInterval(double value)
{
    _intervals.push_back(value);
    inflateLOD();
}

void LOD::editInterval(int index, double value)
{
    _intervals.at(index) = value;
    inflateLOD();
}

double LOD::getInterval(int index) const
{
    return _intervals.at(index);
}

int LOD::getIntervalsCount() const
{
    return _intervals.size();
}

void LOD::removeInterval(int index)
{
    _intervals.erase(_intervals.begin() + index);
    inflateLOD();
}

void LOD::inflateLOD()
{
    if ( _lod->getNumChildren() > 0 )
    {
        _lod->removeChildren(0, _lod->getNumChildren());
    }

    double r0 = 0.0;
    double r1 = 0.0;

    for ( size_t i = 0; i < _intervals.size() && i < _children.size(); ++i )
    {
        r0 = r1;
        r1 = r0 + _intervals.at(i);

        _lod->addChild(_children.at(i)->getNode(), r0, r1);
    }
}

Result LOD::readParameters(const QDomElement* node)
{
    ////////////////////////////////////////////
    Result result = Group::readParameters(node);
    ////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    _intervals.clear();

    int index = 0;
    QString tag = "interval_" + QString::number(index);
    while ( node->hasAttribute(tag) )
    {
        double value = node->attribute(tag).toDouble();
        addInterval(value);
        ++index;
        tag = "interval_" + QString::number(index);
    }

    return index > 0 ? Result::Success : Result::Failure;
}

Result LOD::saveParameters(QDomDocument* doc, QDomElement* node)
{
    /////////////////////////////////////////////////
    Result result = Group::saveParameters(doc, node);
    /////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    int index = 0;
    QString tag = "interval_" + QString::number(index);
    for ( auto interval : _intervals )
    {
        QDomAttr node_interval = doc->createAttribute(tag);
        node_interval.setValue(QString::number(interval));
        node->setAttributeNode(node_interval);

        ++index;
        tag = "interval_" + QString::number(index);
    }

    return result;
}

} // namespace pro
} // namespace mc
