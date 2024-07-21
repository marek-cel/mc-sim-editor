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

namespace mc {
namespace pro {

File::File()
    : Component(new osg::Group())
{
    _group = dynamic_cast<osg::Group*>(Component::_node.get());
    setName("File");
}

std::shared_ptr<Component> File::clone() const
{
    std::shared_ptr<File> file = std::make_shared<File>();
    file->setName(getName());
    file->setFile(getFile());
    file->_depth_sorted_bin_value = _depth_sorted_bin_value;
    file->_depth_sorted_bin_state = _depth_sorted_bin_state;
    return file;
}

void File::setFile(QString file)
{
    _group->removeChild(0, _group->getNumChildren());
    _file = file;
    reloadFile();
}

void File::setProjFile(QString proj_file)
{
    Component::setProjFile(proj_file);
    reloadFile();
}

void File::reloadFile()
{
    if ( _file.length() == 0 ) return;

    QDir proj_dir = QFileInfo(_proj_file).absoluteDir();
    QString file = proj_dir.absoluteFilePath(_file);

    _node = cgi::Models::get(file.toStdString());
    if ( _node.valid() )
    {
        setTransparencyMode(_node.get());
        _group->removeChildren(0, _group->getNumChildren());
        _group->addChild(_node.get());
    }
}

Result File::readParameters(const QDomElement* node)
{
    ////////////////////////////////////////////////
    Result result = Component::readParameters(node);
    ////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QString file = node->attribute("file");
    setFile(file);

    return result;
}

Result File::saveParameters(QDomDocument* doc, QDomElement* node)
{
    /////////////////////////////////////////////////////
    Result result = Component::saveParameters(doc, node);
    /////////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QDomAttr node_file = doc->createAttribute("file");
    node_file.setValue(_file);
    node->setAttributeNode(node_file);

    return result;
}

} // namespace pro
} // namespace mc
