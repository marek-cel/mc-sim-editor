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

#include <pro/Rotor.h>

#include <QDir>
#include <QFileInfo>

#include <cgi/Models.h>

namespace mc {
namespace pro {

Rotor::Rotor()
    : PAT()
{
    setName("Rotor");
}

std::shared_ptr<Component> Rotor::clone() const
{
    std::shared_ptr<Rotor> rotor = std::make_shared<Rotor>();
    rotor->setName(getName());
    return rotor;
}

void Rotor::setBladesNo(int blades_no)
{
    clear();
    _blades_no = std::max(2, blades_no);
    create();
}

void Rotor::setHingeOffset(double offset)
{
    clear();
    _hinge_offset = fabs(offset);
    create();
}

void Rotor::setDirection(Direction direction)
{
    clear();
    _direction = direction;
    create();
}

void Rotor::setFileBlade(const QString& file)
{
    clear();
    _file_blade = file;
    create();
}

void Rotor::setFileShaft(const QString& file)
{
    clear();
    _file_shaft = file;
    create();
}

Result Rotor::readParameters(const QDomElement* node)
{
    //////////////////////////////////////////
    Result result = PAT::readParameters(node);
    //////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    clear();

    _file_blade = node->attribute("file_blade");
    _file_shaft = node->attribute("file_shaft");

    _blades_no = std::max(2, node->attribute("blades_no").toInt());
    _hinge_offset = fabs(node->attribute("hinge_offset").toDouble());

    _direction = static_cast<int>(Direction::CW) == node->attribute("direction").toInt() ? Direction::CW : Direction::CCW;

    create();

    return result;
}

Result Rotor::saveParameters(QDomDocument* doc, QDomElement* node)
{
    ///////////////////////////////////////////////
    Result result = PAT::saveParameters(doc, node);
    ///////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QDomAttr node_file_blade = doc->createAttribute("file_blade");
    QDomAttr node_file_shaft = doc->createAttribute("file_shaft");

    node_file_blade.setValue(getFileBlade());
    node_file_shaft.setValue(getFileShaft());

    node->setAttributeNode(node_file_blade);
    node->setAttributeNode(node_file_shaft);

    QDomAttr node_blades_no = doc->createAttribute("blades_no");
    node_blades_no.setValue(QString::number(getBladesNo()));
    node->setAttributeNode(node_blades_no);

    QDomAttr node_hinge_offset = doc->createAttribute("hinge_offset");
    node_hinge_offset.setValue(QString::number(getHingeOffset()));
    node->setAttributeNode(node_hinge_offset);

    QDomAttr node_direction = doc->createAttribute("direction");
    node_direction.setValue(QString::number(static_cast<int>(getDirection())));
    node->setAttributeNode(node_direction);

    return result;
}

void Rotor::clear()
{
    if ( _shaft.valid() )
    {
        _shaft->removeChild(0, _shaft->getNumChildren());
    }
    _blades.clear();
    _shaft = nullptr;
}

void Rotor::create()
{
    createShaft();
    createBlades();
}

void Rotor::createShaft()
{
    _shaft = new osg::PositionAttitudeTransform();
    _shaft->setName("Shaft");
    _pat->addChild(_shaft.get());

    if ( _file_shaft.length() == 0 ) return;

    QDir proj_dir = QFileInfo(_proj_file).absoluteDir();
    QString file = proj_dir.absoluteFilePath(_file_shaft);

    osg::ref_ptr<osg::Node> node = cgi::Models::get(file.toStdString());
    if ( node.valid() )
    {
        _shaft->addChild(node.get());
    }
}

void Rotor::createBlades()
{
    if ( !_shaft.valid() ) return;
    if ( _file_blade.length() == 0 ) return;

    QDir proj_dir = QFileInfo(_proj_file).absoluteDir();
    QString file = proj_dir.absoluteFilePath(_file_blade);

    osg::ref_ptr<osg::Node> node = cgi::Models::get(file.toStdString());
    if ( !node.valid() ) return;

    osg::ref_ptr<osg::Group> blades = new osg::Group();
    blades->setName("Blades");
    _shaft->addChild(blades.get());

    const double step = (_direction == Direction::CCW ? -1.0 : 1.0)*2.0*M_PI/((double)_blades_no);

    for ( unsigned int i = 0; i < _blades_no; i++ )
    {
        double azimuth = i*step + M_PI;

        while ( azimuth < 0.0        ) azimuth += 2.0 * M_PI;
        while ( azimuth > 2.0 * M_PI ) azimuth -= 2.0 * M_PI;

        double offset_x = cos(azimuth) * _hinge_offset;
        double offset_y = sin(azimuth) * _hinge_offset;

        osg::ref_ptr<osg::PositionAttitudeTransform> pat_flap = new osg::PositionAttitudeTransform();
        pat_flap->setName("Flap");
        pat_flap->addChild(node.get());

        osg::ref_ptr<osg::PositionAttitudeTransform> pat_feather = new osg::PositionAttitudeTransform();
        pat_feather->setName("Feather");
        pat_feather->setAttitude(osg::Quat(azimuth, osg::Vec3(0.0,0.0,1.0)));
        pat_feather->setPosition(osg::Vec3(offset_x, offset_y, 0.0));
        pat_feather->addChild(pat_flap.get());

        blades->addChild(pat_feather.get());
        _blades.push_back(pat_flap.get());
    }
}

} // namespace pro
} // namespace mc
