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
    lod_ = dynamic_cast<osg::LOD*>(node_.get());
    SetName("LOD");
    AddInterval(1000.0);
}

Result LOD::AddChild(std::shared_ptr<Component> child)
{
    child->SetParent(shared_from_this());
    children_.push_back(child);

    InflateLOD();

    return Result::Success;
}

std::unique_ptr<Component> LOD::Clone() const
{
    std::unique_ptr<LOD> lod = std::make_unique<LOD>();
    lod->SetName(GetName());
    lod->CloneChildren(&children_);
    return lod;
}

void LOD::AddInterval(double value)
{
    intervals_.push_back(value);
    InflateLOD();
}

void LOD::EditInterval(int index, double value)
{
    intervals_.at(index) = value;
    InflateLOD();
}

double LOD::GetInterval(int index) const
{
    return intervals_.at(index);
}

int LOD::GetIntervalsCount() const
{
    return intervals_.size();
}

void LOD::RemoveInterval(int index)
{
    intervals_.erase(intervals_.begin() + index);
    InflateLOD();
}

void LOD::InflateLOD()
{
    if ( lod_->getNumChildren() > 0 )
    {
        lod_->removeChildren(0, lod_->getNumChildren());
    }

    double r0 = 0.0;
    double r1 = 0.0;

    for ( size_t i = 0; i < intervals_.size() && i < children_.size(); ++i )
    {
        r0 = r1;
        r1 = r0 + intervals_.at(i);

        lod_->addChild(children_.at(i)->GetNode(), r0, r1);
    }
}

Result LOD::ReadParameters(const QDomElement* node)
{
    ////////////////////////////////////////////
    Result result = Group::ReadParameters(node);
    ////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    intervals_.clear();

    int index = 0;
    QString tag = "interval_" + QString::number(index);
    while ( node->hasAttribute(tag) )
    {
        double value = node->attribute(tag).toDouble();
        AddInterval(value);
        ++index;
        tag = "interval_" + QString::number(index);
    }

    return index > 0 ? Result::Success : Result::Failure;
}

Result LOD::SaveParameters(QDomDocument* doc, QDomElement* node)
{
    /////////////////////////////////////////////////
    Result result = Group::SaveParameters(doc, node);
    /////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    int index = 0;
    QString tag = "interval_" + QString::number(index);
    for ( auto interval : intervals_ )
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
