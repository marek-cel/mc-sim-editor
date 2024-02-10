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

#include <pro/Project.h>

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

////////////////////////////////////////////////////////////////////////////////

Result Project::Read(const QString& file)
{
    QFile dev_file(file);

    if ( !dev_file.open(QFile::ReadOnly | QFile::Text) )
    {
        return Result::Failure;
    }

    QDomDocument doc;
    doc.setContent(&dev_file, false);

    QDomElement node_root = doc.documentElement();
    if ( node_root.tagName() != "mcedit" )
    {
        return Result::Failure;
    }

    Result result = Result::Success;

#   ifdef MCSIM_EDITOR_MODEL_ASSEMBLER
    QDomElement node_assembly = node_root.firstChildElement("assembly");
    if ( node_assembly.isNull() )
    {
        return Result::Failure;
    }

    assembly_->SetProjFile(file);
    if ( result == Result::Success ) result = assembly_->Read(&node_assembly);

    QDomElement node_playback = node_root.firstChildElement("playback");
    if ( node_playback.isNull() )
    {
        return Result::Failure;
    }

    if ( result == Result::Success ) result = playback_->Read(&node_playback);
#   endif // MCSIM_EDITOR_MODEL_ASSEMBLER

    if ( result == Result::Success )
    {
        file_ = file;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Result Project::Save(const QString& file)
{
    QString file_temp = file;

    if ( QFileInfo(file_temp).suffix() != QString("mcedit") )
    {
        file_temp += ".mcedit";
    }

    QFile dev_file(file_temp);

    if ( dev_file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text) )
    {
        Result result = Result::Success;

        QTextStream out;
        out.setDevice( &dev_file );
        out.setCodec("UTF-8");

        QDomDocument doc("mcedit");
        doc.setContent(&dev_file, false);

        QDomElement root_node = doc.createElement("mcedit");
        doc.appendChild(root_node);

#       ifdef MCSIM_EDITOR_MODEL_ASSEMBLER
        assembly_->SetProjFile(file);
        if ( result == Result::Success ) result = assembly_->Save(&doc, &root_node);
        if ( result == Result::Success ) result = playback_->Save(&doc, &root_node);
#       endif // MCSIM_EDITOR_MODEL_ASSEMBLER

        if ( result == Result::Success )
        {
            file_ = file_temp;
            out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            out << doc.toString();
            dev_file.close();
        }

        return result;
    }

    return Result::Failure;
}

////////////////////////////////////////////////////////////////////////////////

void Project::SetAnimationTime(double time)
{
    assembly_->SetAnimationTime(time);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace pro
} // namespace mc
