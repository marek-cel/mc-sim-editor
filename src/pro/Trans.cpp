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

#include <pro/Trans.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

////////////////////////////////////////////////////////////////////////////////

Trans::Trans(osg::MatrixTransform* mt)
    : Animated(mt ? mt : new osg::MatrixTransform())
{
    mt_ = dynamic_cast<osg::MatrixTransform*>(node_.get());
    SetName("Matrix Transform");
}

////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Component> Trans::Clone() const
{
    std::unique_ptr<Trans> mt = std::make_unique<Trans>();
    mt->SetName(GetName());
    mt->CloneChildren(&children_);
    return mt;
}

////////////////////////////////////////////////////////////////////////////////

void Trans::SetPx(double px)
{
    px_ = px;
    UpdateTransformMatrix();
}

////////////////////////////////////////////////////////////////////////////////

void Trans::SetPy(double py)
{
    py_ = py;
    UpdateTransformMatrix();
}

////////////////////////////////////////////////////////////////////////////////

void Trans::SetPz(double pz)
{
    pz_ = pz;
    UpdateTransformMatrix();
}

////////////////////////////////////////////////////////////////////////////////

void Trans::SetAx(double ax)
{
    ax_ = ax;
    UpdateTransformMatrix();
}

////////////////////////////////////////////////////////////////////////////////

void Trans::SetAy(double ay)
{
    ay_ = ay;
    UpdateTransformMatrix();
}

////////////////////////////////////////////////////////////////////////////////

void Trans::SetAz(double az)
{
    az_ = az;
    UpdateTransformMatrix();
}

////////////////////////////////////////////////////////////////////////////////

Result Trans::ReadParameters(const QDomElement* node)
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

    UpdateTransformMatrix();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Result Trans::SaveParameters(QDomDocument* doc, QDomElement* node)
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

    node_px.setValue(QString::number(px(), 'f', 6));
    node_py.setValue(QString::number(py(), 'f', 6));
    node_pz.setValue(QString::number(pz(), 'f', 6));

    node_ax.setValue(QString::number(ax(), 'f', 12));
    node_ay.setValue(QString::number(ay(), 'f', 12));
    node_az.setValue(QString::number(az(), 'f', 12));

    node->setAttributeNode(node_px);
    node->setAttributeNode(node_py);
    node->setAttributeNode(node_pz);

    node->setAttributeNode(node_ax);
    node->setAttributeNode(node_ay);
    node->setAttributeNode(node_az);

    return result;
}

////////////////////////////////////////////////////////////////////////////////

void Trans::UpdateTransformMatrix()
{
    osg::Matrix mr;
    mr.makeRotate(osg::DegreesToRadians(az_), osg::Z_AXIS,
                  osg::DegreesToRadians(ay_), osg::Y_AXIS,
                  osg::DegreesToRadians(ax_), osg::X_AXIS);

    osg::Matrix mt;
    mt.makeTranslate(px_, py_, pz_);

    mt_->setMatrix(mr*mt);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace pro
} // namespace mc
