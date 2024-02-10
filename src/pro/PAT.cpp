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

#include <pro/PAT.h>

namespace mc {
namespace pro {

PAT::PAT(osg::PositionAttitudeTransform* pat)
    : Animated(pat ? pat : new osg::PositionAttitudeTransform())
{
    pat_ = dynamic_cast<osg::PositionAttitudeTransform*>(node_.get());
    SetName("PAT");
}

std::unique_ptr<Component> PAT::Clone() const
{
    std::unique_ptr<PAT> pat = std::make_unique<PAT>();
    pat->SetName(GetName());
    pat->CloneChildren(&children_);
    return pat;
}

void PAT::SetPx(double px)
{
    px_ = px;
    UpdatePositionAndAttitude();
}

void PAT::SetPy(double py)
{
    py_ = py;
    UpdatePositionAndAttitude();
}

void PAT::SetPz(double pz)
{
    pz_ = pz;
    UpdatePositionAndAttitude();
}

void PAT::SetAx(double ax)
{
    ax_ = ax;
    UpdatePositionAndAttitude();
}

void PAT::SetAy(double ay)
{
    ay_ = ay;
    UpdatePositionAndAttitude();
}

void PAT::SetAz(double az)
{
    az_ = az;
    UpdatePositionAndAttitude();
}

void PAT::SetConvention(Convention convention)
{
    convention_ = convention;
    UpdatePositionAndAttitude();
}

Result PAT::ReadParameters(const QDomElement* node)
{
    ///////////////////////////////////////////////
    Result result = Animated::ReadParameters(node);
    ///////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    px_ = node->attribute("px").toDouble();
    py_ = node->attribute("py").toDouble();
    pz_ = node->attribute("pz").toDouble();

    ax_ = node->attribute("ax").toDouble();
    ay_ = node->attribute("ay").toDouble();
    az_ = node->attribute("az").toDouble();

    if ( node->attribute("convention").toInt() == 0 )
    {
        convention_ = Convention::XYZ;
    }
    else
    {
        convention_ = Convention::ZYX;
    }

    UpdatePositionAndAttitude();

    return result;
}

Result PAT::SaveParameters(QDomDocument* doc, QDomElement* node)
{
    ////////////////////////////////////////////////////
    Result result = Animated::SaveParameters(doc, node);
    ////////////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    QDomAttr node_px = doc->createAttribute("px");
    QDomAttr node_py = doc->createAttribute("py");
    QDomAttr node_pz = doc->createAttribute("pz");

    QDomAttr node_ax = doc->createAttribute("ax");
    QDomAttr node_ay = doc->createAttribute("ay");
    QDomAttr node_az = doc->createAttribute("az");

    QDomAttr node_convention = doc->createAttribute("convention");

    node_px.setValue(QString::number(px(), 'f', 6));
    node_py.setValue(QString::number(py(), 'f', 6));
    node_pz.setValue(QString::number(pz(), 'f', 6));

    node_ax.setValue(QString::number(ax(), 'f', 12));
    node_ay.setValue(QString::number(ay(), 'f', 12));
    node_az.setValue(QString::number(az(), 'f', 12));

    node_convention.setValue(QString::number(static_cast<int>(convention_)));

    node->setAttributeNode(node_px);
    node->setAttributeNode(node_py);
    node->setAttributeNode(node_pz);

    node->setAttributeNode(node_ax);
    node->setAttributeNode(node_ay);
    node->setAttributeNode(node_az);

    node->setAttributeNode(node_convention);

    return result;
}

void PAT::UpdatePositionAndAttitude()
{
    osg::Vec3d pos(px_, py_, pz_);
    osg::Quat att;

    if ( convention_ == Convention::ZYX )
    {
        att = osg::Quat(osg::DegreesToRadians(az_), osg::Z_AXIS,
                        osg::DegreesToRadians(ay_), osg::Y_AXIS,
                        osg::DegreesToRadians(ax_), osg::X_AXIS);
    }
    else
    {
        att = osg::Quat(osg::DegreesToRadians(ax_), osg::X_AXIS,
                        osg::DegreesToRadians(ay_), osg::Y_AXIS,
                        osg::DegreesToRadians(az_), osg::Z_AXIS);
    }

    pat_->setPosition(pos);
    pat_->setAttitude(att);
}

} // namespace pro
} // namespace mc
