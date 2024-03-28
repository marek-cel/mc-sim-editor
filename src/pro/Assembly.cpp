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

#include <pro/Assembly.h>

#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

#include <pro/Components.h>

namespace mc {
namespace pro {

Result Assembly::exportModel(QString file)
{
    osgUtil::Optimizer optimizer;

    int options = osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS |
            osgUtil::Optimizer::REMOVE_REDUNDANT_NODES |
            osgUtil::Optimizer::REMOVE_LOADED_PROXY_NODES |
            osgUtil::Optimizer::COMBINE_ADJACENT_LODS |
            osgUtil::Optimizer::SHARE_DUPLICATE_STATE |
            osgUtil::Optimizer::MERGE_GEOMETRY |
            osgUtil::Optimizer::MAKE_FAST_GEOMETRY |
            osgUtil::Optimizer::CHECK_GEOMETRY |
            //osgUtil::Optimizer::OPTIMIZE_TEXTURE_SETTINGS |
            osgUtil::Optimizer::STATIC_OBJECT_DETECTION;

    optimizer.optimize(_root->getNode(), options);

    osgDB::writeNodeFile(*_root->getNode(), file.toStdString());

    return Result::Success;
}

Result Assembly::read(const QDomElement* node)
{
    if ( node->isNull() )
    {
        return Result::Failure;
    }

    QDomElement root_node = node->firstChildElement();
    if ( node->isNull() || root_node != node->lastChildElement() )
    {
        return Result::Failure;
    }

    Components::Type type = Components::instance()->getComponentByTagName(root_node.tagName());
    if ( !type.component || !type.can_be_parent )
    {
        return Result::Failure;
    }

    std::shared_ptr<Component> comp = std::move(type.component->clone());
    comp->setProjFile(_proj_file);
    std::shared_ptr<Group> group = std::dynamic_pointer_cast<Group>(comp);
    if ( !group )
    {
        return Result::Failure;
    }

    _root = group;
    return _root->read(&root_node);
}

Result Assembly::save(QDomDocument* doc, QDomElement* parent)
{
    QDomElement node = doc->createElement("assembly");
    parent->appendChild(node);

    return _root->save(doc, &node);
}

void Assembly::setProjFile(QString proj_file)
{
    _proj_file = proj_file;
    _root->setProjFile(proj_file);
}

void Assembly::setAnimationTime(double time)
{
    _root->setAnimationTime(time);
}

} // namespace pro
} // namespace mc
