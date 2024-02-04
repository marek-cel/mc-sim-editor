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

#include <pro/File.h>

#include <QDir>
#include <QFileInfo>

#include <cgi/Models.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

////////////////////////////////////////////////////////////////////////////////

File::File()
    : Component(new osg::Group())
{
    group_ = dynamic_cast<osg::Group*>(Component::node_.get());
    SetName("File");
}

////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Component> File::Clone() const
{
    std::unique_ptr<File> file = std::make_unique<File>();
    file->SetName(GetName());
    return file;
}

////////////////////////////////////////////////////////////////////////////////

void File::SetFile(QString file)
{
    group_->removeChild(0, group_->getNumChildren());
    file_ = file;
    ReloadFile();
}

////////////////////////////////////////////////////////////////////////////////

void File::SetProjFile(QString proj_file)
{
    Component::SetProjFile(proj_file);
    ReloadFile();
}

////////////////////////////////////////////////////////////////////////////////

void File::ReloadFile()
{
    if ( file_.length() == 0 ) return;

    QDir proj_dir = QFileInfo(proj_file_).absoluteDir();
    QString file = proj_dir.absoluteFilePath(file_);

    node_ = cgi::Models::Get(file.toStdString());
    if ( node_.valid() )
    {
        SetTransparencyMode(node_.get());
        group_->removeChildren(0, group_->getNumChildren());
        group_->addChild(node_.get());
    }
}

////////////////////////////////////////////////////////////////////////////////

Result File::ReadParameters(const QDomElement* node)
{
    ////////////////////////////////////////////////
    Result result = Component::ReadParameters(node);
    ////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QString file = node->attribute("file");
    SetFile(file);

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Result File::SaveParameters(QDomDocument* doc, QDomElement* node)
{
    /////////////////////////////////////////////////////
    Result result = Component::SaveParameters(doc, node);
    /////////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QDomAttr node_file = doc->createAttribute("file");
    node_file.setValue(file_);
    node->setAttributeNode(node_file);

    return result;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace pro
} // namespace mc
