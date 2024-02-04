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

#include <pro/Component.h>

#include <osg/CullFace>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

////////////////////////////////////////////////////////////////////////////////

Component::Component(osg::Node* node)
{
    node_ = node;
}

////////////////////////////////////////////////////////////////////////////////

Result Component::Read(const QDomElement* node)
{
    if ( node->isNull() )
    {
        return Result::Failure;
    }

    return ReadParameters(node);
}

////////////////////////////////////////////////////////////////////////////////

Result Component::Save(QDomDocument* doc, QDomElement* parent)
{
    QDomElement node = doc->createElement(GetTagName());
    parent->appendChild(node);

    return SaveParameters(doc, &node);
}

////////////////////////////////////////////////////////////////////////////////

void Component::Update() {}

////////////////////////////////////////////////////////////////////////////////

void Component::SetName(QString name)
{
    name_ = name;
    node_->setName(name.toStdString());
}

////////////////////////////////////////////////////////////////////////////////

void Component::SetParent(std::shared_ptr<Component> parent)
{
    parent_ = parent;
}

////////////////////////////////////////////////////////////////////////////////

void Component::SetAnimationTime(double time) {}

////////////////////////////////////////////////////////////////////////////////

void Component::SetDepthSortedBinValue(int value)
{
    depth_sorted_bin_value_ = value;

    if ( depth_sorted_bin_state_ && node_.valid() )
    {
        node_->getOrCreateStateSet()->setRenderBinDetails(depth_sorted_bin_value_, "DepthSortedBin");
    }

    SetTransparencyMode(node_.get());
}

////////////////////////////////////////////////////////////////////////////////

void Component::SetDepthSortedBinState(bool state)
{
    depth_sorted_bin_state_ = state;

    if ( !state )
    {
        depth_sorted_bin_value_ = 1;
    }

    if ( depth_sorted_bin_state_ && node_.valid() )
    {
        node_->getOrCreateStateSet()->setRenderBinDetails(depth_sorted_bin_value_, "DepthSortedBin");
    }

    SetTransparencyMode(node_.get());
}

////////////////////////////////////////////////////////////////////////////////

Result Component::ReadParameters(const QDomElement* node)
{
    int  depth_sorted_bin_value = node->attribute("depth_sorted_bin_value").toInt();
    bool depth_sorted_bin_state = node->attribute("depth_sorted_bin_state").toInt();

    SetName(node->attribute("name"));
    SetDepthSortedBinState(depth_sorted_bin_state); // State First
    SetDepthSortedBinValue(depth_sorted_bin_value);

    return Result::Success;
}

////////////////////////////////////////////////////////////////////////////////

Result Component::SaveParameters(QDomDocument* doc, QDomElement* node)
{
    QDomAttr node_name = doc->createAttribute("name");
    node_name.setValue(name_);
    node->setAttributeNode(node_name);

    QDomAttr node_depth_sorted_bin_value = doc->createAttribute("depth_sorted_bin_value");
    QDomAttr node_depth_sorted_bin_state = doc->createAttribute("depth_sorted_bin_state");

    node_depth_sorted_bin_value.setValue(QString::number(depth_sorted_bin_value_));
    node_depth_sorted_bin_state.setValue(QString::number(depth_sorted_bin_state_ ? 1 : 0));

    node->setAttributeNode( node_depth_sorted_bin_value );
    node->setAttributeNode( node_depth_sorted_bin_state );

    return Result::Success;
}

////////////////////////////////////////////////////////////////////////////////

void Component::SetTransparencyMode(osg::Node* node)
{
    osg::ref_ptr<osg::StateSet> ss = node->getOrCreateStateSet();

    ss->setMode(GL_DEPTH_TEST , osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
    ss->setMode(GL_ALPHA_TEST , osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
    ss->setMode(GL_BLEND      , osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);

    if ( depth_sorted_bin_state_ && depth_sorted_bin_value_ > 1 )
    {
        //node->setCullingActive(false);
        osg::ref_ptr<osg::CullFace> cull = new osg::CullFace();
        cull->setMode(osg::CullFace::BACK);
        ss->setAttributeAndModes(cull, osg::StateAttribute::ON);

        ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    }
    else
    {
        ss->setRenderingHint(osg::StateSet::DEFAULT_BIN);
    }
}

////////////////////////////////////////////////////////////////////////////////

} // namespace pro
} // namespace mc
