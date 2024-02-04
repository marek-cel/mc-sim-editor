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

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

////////////////////////////////////////////////////////////////////////////////

Rotor::Rotor()
    : PAT()
{
    SetName("Rotor");
}

////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Component> Rotor::Clone() const
{
    std::unique_ptr<Rotor> rotor = std::make_unique<Rotor>();
    rotor->SetName(GetName());
    return rotor;
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::SetBladesNo(int blades_no)
{
    Clear();
    blades_no_ = std::max(2, blades_no);
    Create();
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::SetHingeOffset(double offset)
{
    Clear();
    hinge_offset_ = fabs(offset);
    Create();
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::SetDirection(Direction direction)
{
    Clear();
    direction_ = direction;
    Create();
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::SetFileBlade(const QString& file)
{
    Clear();
    file_blade_ = file;
    Create();
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::SetFileShaft(const QString& file)
{
    Clear();
    file_shaft_ = file;
    Create();
}

////////////////////////////////////////////////////////////////////////////////

Result Rotor::ReadParameters(const QDomElement* node)
{
    //////////////////////////////////////////
    Result result = PAT::ReadParameters(node);
    //////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    Clear();

    file_blade_ = node->attribute("file_blade");
    file_shaft_ = node->attribute("file_shaft");

    blades_no_ = std::max(2, node->attribute("blades_no").toInt());
    hinge_offset_ = fabs(node->attribute("hinge_offset").toDouble());

    direction_ = static_cast<int>(Direction::CW) == node->attribute("direction").toInt() ? Direction::CW : Direction::CCW;

    Create();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Result Rotor::SaveParameters(QDomDocument* doc, QDomElement* node)
{
    ///////////////////////////////////////////////
    Result result = PAT::SaveParameters(doc, node);
    ///////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QDomAttr node_file_blade = doc->createAttribute("file_blade");
    QDomAttr node_file_shaft = doc->createAttribute("file_shaft");

    node_file_blade.setValue(GetFileBlade());
    node_file_shaft.setValue(GetFileShaft());

    node->setAttributeNode(node_file_blade);
    node->setAttributeNode(node_file_shaft);

    QDomAttr node_blades_no = doc->createAttribute("blades_no");
    node_blades_no.setValue(QString::number(GetBladesNo()));
    node->setAttributeNode(node_blades_no);

    QDomAttr node_hinge_offset = doc->createAttribute("hinge_offset");
    node_hinge_offset.setValue(QString::number(GetHingeOffset()));
    node->setAttributeNode(node_hinge_offset);

    QDomAttr node_direction = doc->createAttribute("direction");
    node_direction.setValue(QString::number(static_cast<int>(GetDirection())));
    node->setAttributeNode(node_direction);

    return result;
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::Clear()
{
    if ( shaft_.valid() )
    {
        shaft_->removeChild(0, shaft_->getNumChildren());
    }
    blades_.clear();
    shaft_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::Create()
{
    CreateShaft();
    CreateBlades();
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::CreateShaft()
{
    shaft_ = new osg::PositionAttitudeTransform();
    shaft_->setName("Shaft");
    pat_->addChild(shaft_.get());

    if ( file_shaft_.length() == 0 ) return;

    QDir proj_dir = QFileInfo(proj_file_).absoluteDir();
    QString file = proj_dir.absoluteFilePath(file_shaft_);

    osg::ref_ptr<osg::Node> node = cgi::Models::Get(file.toStdString());
    if ( node.valid() )
    {
        shaft_->addChild(node.get());
    }
}

////////////////////////////////////////////////////////////////////////////////

void Rotor::CreateBlades()
{
    if ( !shaft_.valid() ) return;
    if ( file_blade_.length() == 0 ) return;

    QDir proj_dir = QFileInfo(proj_file_).absoluteDir();
    QString file = proj_dir.absoluteFilePath(file_blade_);

    osg::ref_ptr<osg::Node> node = cgi::Models::Get(file.toStdString());
    if ( !node.valid() ) return;

    osg::ref_ptr<osg::Group> blades = new osg::Group();
    blades->setName("Blades");
    shaft_->addChild(blades.get());

    const double step = (direction_ == Direction::CCW ? -1.0 : 1.0)*2.0*M_PI/((double)blades_no_);

    for ( unsigned int i = 0; i < blades_no_; i++ )
    {
        double azimuth = i*step + M_PI;

        while ( azimuth < 0.0        ) azimuth += 2.0 * M_PI;
        while ( azimuth > 2.0 * M_PI ) azimuth -= 2.0 * M_PI;

        double offset_x = cos(azimuth) * hinge_offset_;
        double offset_y = sin(azimuth) * hinge_offset_;

        osg::ref_ptr<osg::PositionAttitudeTransform> pat_flap = new osg::PositionAttitudeTransform();
        pat_flap->setName("Flap");
        pat_flap->addChild(node.get());

        osg::ref_ptr<osg::PositionAttitudeTransform> pat_feather = new osg::PositionAttitudeTransform();
        pat_feather->setName("Feather");
        pat_feather->setAttitude(osg::Quat(azimuth, osg::Vec3(0.0,0.0,1.0)));
        pat_feather->setPosition(osg::Vec3(offset_x, offset_y, 0.0));
        pat_feather->addChild(pat_flap.get());

        blades->addChild(pat_feather.get());
        blades_.push_back(pat_flap.get());
    }
}

////////////////////////////////////////////////////////////////////////////////

} // namespace pro
} // namespace mc
