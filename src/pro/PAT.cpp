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
    _pat = dynamic_cast<osg::PositionAttitudeTransform*>(_node.get());
    setName("PAT");
}

std::unique_ptr<Component> PAT::clone() const
{
    std::unique_ptr<PAT> pat = std::make_unique<PAT>();
    pat->setName(getName());
    pat->cloneChildren(&_children);
    return pat;
}

void PAT::setPx(double px)
{
    _px = px;
    updatePositionAndAttitude();
}

void PAT::setPy(double py)
{
    _py = py;
    updatePositionAndAttitude();
}

void PAT::setPz(double pz)
{
    _pz = pz;
    updatePositionAndAttitude();
}

void PAT::setAx(double ax)
{
    _ax = ax;
    updatePositionAndAttitude();
}

void PAT::setAy(double ay)
{
    _ay = ay;
    updatePositionAndAttitude();
}

void PAT::setAz(double az)
{
    _az = az;
    updatePositionAndAttitude();
}

void PAT::setConvention(Convention convention)
{
    convention_ = convention;
    updatePositionAndAttitude();
}

Result PAT::readParameters(const QDomElement* node)
{
    ///////////////////////////////////////////////
    Result result = Animated::readParameters(node);
    ///////////////////////////////////////////////

    if ( result == Result::Failure ) return result;

    _px = node->attribute("px").toDouble();
    _py = node->attribute("py").toDouble();
    _pz = node->attribute("pz").toDouble();

    _ax = node->attribute("ax").toDouble();
    _ay = node->attribute("ay").toDouble();
    _az = node->attribute("az").toDouble();

    if ( node->attribute("convention").toInt() == 0 )
    {
        convention_ = Convention::XYZ;
    }
    else
    {
        convention_ = Convention::ZYX;
    }

    updatePositionAndAttitude();

    return result;
}

Result PAT::saveParameters(QDomDocument* doc, QDomElement* node)
{
    ////////////////////////////////////////////////////
    Result result = Animated::saveParameters(doc, node);
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

void PAT::updatePositionAndAttitude()
{
    osg::Vec3d pos(_px, _py, _pz);
    osg::Quat att;

    if ( convention_ == Convention::ZYX )
    {
        att = osg::Quat(osg::DegreesToRadians(_az), osg::Z_AXIS,
                        osg::DegreesToRadians(_ay), osg::Y_AXIS,
                        osg::DegreesToRadians(_ax), osg::X_AXIS);
    }
    else
    {
        att = osg::Quat(osg::DegreesToRadians(_ax), osg::X_AXIS,
                        osg::DegreesToRadians(_ay), osg::Y_AXIS,
                        osg::DegreesToRadians(_az), osg::Z_AXIS);
    }

    _pat->setPosition(pos);
    _pat->setAttitude(att);
}

} // namespace pro
} // namespace mc
