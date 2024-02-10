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

Result Assembly::Export(QString file)
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

    optimizer.optimize(root_->GetNode(), options);

    osgDB::writeNodeFile(*root_->GetNode(), file.toStdString());

    return Result::Success;
}

Result Assembly::Read(const QDomElement* node)
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

    Components::Type type = Components::Instance()->GetComponentByTagName(root_node.tagName());
    if ( !type.component || !type.can_be_parent )
    {
        return Result::Failure;
    }

    std::shared_ptr<Component> comp = std::move(type.component->Clone());
    comp->SetProjFile(proj_file_);
    std::shared_ptr<Group> group = std::dynamic_pointer_cast<Group>(comp);
    if ( !group )
    {
        return Result::Failure;
    }

    root_ = group;
    return root_->Read(&root_node);
}

Result Assembly::Save(QDomDocument* doc, QDomElement* parent)
{
    QDomElement node = doc->createElement("assembly");
    parent->appendChild(node);

    return root_->Save(doc, &node);
}

void Assembly::SetProjFile(QString proj_file)
{
    proj_file_ = proj_file;
    root_->SetProjFile(proj_file);
}

void Assembly::SetAnimationTime(double time)
{
    root_->SetAnimationTime(time);
}

} // namespace pro
} // namespace mc
