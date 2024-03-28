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

namespace mc {
namespace pro {

Component::Component(osg::Node* node)
{
    _node = node;
}

Result Component::read(const QDomElement* node)
{
    if ( node->isNull() )
    {
        return Result::Failure;
    }

    return readParameters(node);
}

Result Component::save(QDomDocument* doc, QDomElement* parent)
{
    QDomElement node = doc->createElement(getTagName());
    parent->appendChild(node);

    return saveParameters(doc, &node);
}

void Component::update() {}

bool Component::isRoot() const
{
    if ( _parent.expired() )
    {
        return true;
    }

    return false;
}

void Component::setName(QString name)
{
    _name = name;
    _node->setName(name.toStdString());
}

void Component::setParent(std::shared_ptr<Component> parent)
{
    _parent = parent;
}

void Component::setAnimationTime(double time) {}

void Component::setDepthSortedBinValue(int value)
{
    _depth_sorted_bin_value = value;

    if ( _depth_sorted_bin_state && _node.valid() )
    {
        _node->getOrCreateStateSet()->setRenderBinDetails(_depth_sorted_bin_value, "DepthSortedBin");
    }

    setTransparencyMode(_node.get());
}

void Component::setDepthSortedBinState(bool state)
{
    _depth_sorted_bin_state = state;

    if ( !state )
    {
        _depth_sorted_bin_value = 1;
    }

    if ( _depth_sorted_bin_state && _node.valid() )
    {
        _node->getOrCreateStateSet()->setRenderBinDetails(_depth_sorted_bin_value, "DepthSortedBin");
    }

    setTransparencyMode(_node.get());
}

Result Component::readParameters(const QDomElement* node)
{
    int  depth_sorted_bin_value = node->attribute("depth_sorted_bin_value").toInt();
    bool depth_sorted_bin_state = node->attribute("depth_sorted_bin_state").toInt();

    setName(node->attribute("name"));
    setDepthSortedBinState(depth_sorted_bin_state); // State First
    setDepthSortedBinValue(depth_sorted_bin_value);

    return Result::Success;
}

Result Component::saveParameters(QDomDocument* doc, QDomElement* node)
{
    QDomAttr node_name = doc->createAttribute("name");
    node_name.setValue(_name);
    node->setAttributeNode(node_name);

    QDomAttr node_depth_sorted_bin_value = doc->createAttribute("depth_sorted_bin_value");
    QDomAttr node_depth_sorted_bin_state = doc->createAttribute("depth_sorted_bin_state");

    node_depth_sorted_bin_value.setValue(QString::number(_depth_sorted_bin_value));
    node_depth_sorted_bin_state.setValue(QString::number(_depth_sorted_bin_state ? 1 : 0));

    node->setAttributeNode(node_depth_sorted_bin_value);
    node->setAttributeNode(node_depth_sorted_bin_state);

    return Result::Success;
}

void Component::setTransparencyMode(osg::Node* node)
{
    osg::ref_ptr<osg::StateSet> ss = node->getOrCreateStateSet();

    ss->setMode(GL_DEPTH_TEST , osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
    ss->setMode(GL_ALPHA_TEST , osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
    ss->setMode(GL_BLEND      , osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);

    if ( _depth_sorted_bin_state && _depth_sorted_bin_value > 1 )
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

} // namespace pro
} // namespace mc
