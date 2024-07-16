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

namespace mc {
namespace pro {

Trans::Trans(osg::MatrixTransform* mt)
    : Animated(mt ? mt : new osg::MatrixTransform())
{
    _mt = dynamic_cast<osg::MatrixTransform*>(_node.get());
    setName("Matrix Transform");
}

std::shared_ptr<Component> Trans::clone() const
{
    std::shared_ptr<Trans> mt = std::make_shared<Trans>();
    mt->setName(getName());
    mt->cloneChildren(&_children);
    return mt;
}

void Trans::setPx(double px)
{
    _px = px;
    updateTransformMatrix();
}

void Trans::setPy(double py)
{
    _py = py;
    updateTransformMatrix();
}

void Trans::setPz(double pz)
{
    _pz = pz;
    updateTransformMatrix();
}

void Trans::setAx(double ax)
{
    _ax = ax;
    updateTransformMatrix();
}

void Trans::setAy(double ay)
{
    _ay = ay;
    updateTransformMatrix();
}

void Trans::setAz(double az)
{
    _az = az;
    updateTransformMatrix();
}

Result Trans::readParameters(const QDomElement* node)
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

    updateTransformMatrix();

    return result;
}

Result Trans::saveParameters(QDomDocument* doc, QDomElement* node)
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

void Trans::updateTransformMatrix()
{
    osg::Matrix mr;
    mr.makeRotate(osg::DegreesToRadians(_az), osg::Z_AXIS,
                  osg::DegreesToRadians(_ay), osg::Y_AXIS,
                  osg::DegreesToRadians(_ax), osg::X_AXIS);

    osg::Matrix mt;
    mt.makeTranslate(_px, _py, _pz);

    _mt->setMatrix(mr*mt);
}

} // namespace pro
} // namespace mc
