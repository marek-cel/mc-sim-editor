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

#include <pro/Keyframe.h>

namespace mc {
namespace pro {

Keyframe::Keyframe() {}

Result Keyframe::Read(const QDomElement* node)
{
    Result result = Result::Success;

    if ( node->tagName() != kTagName ) return Result::Failure;

    t_ = node->attribute("t").toDouble();

    px_ = node->attribute("px").toDouble();
    py_ = node->attribute("py").toDouble();
    pz_ = node->attribute("pz").toDouble();

    ax_ = node->attribute("ax").toDouble();
    ay_ = node->attribute("ay").toDouble();
    az_ = node->attribute("az").toDouble();

    return result;
}

Result Keyframe::Save(QDomDocument* doc, QDomElement* parent)
{
    Result result = Result::Success;

    QDomElement node = doc->createElement(kTagName);
    parent->appendChild(node);

    QDomAttr node_t = doc->createAttribute("t");

    QDomAttr node_px = doc->createAttribute("px");
    QDomAttr node_py = doc->createAttribute("py");
    QDomAttr node_pz = doc->createAttribute("pz");

    QDomAttr node_ax = doc->createAttribute("ax");
    QDomAttr node_ay = doc->createAttribute("ay");
    QDomAttr node_az = doc->createAttribute("az");

    node_t.setValue(QString::number(t(), 'f', 4));

    node_px.setValue(QString::number(px(), 'f', 6));
    node_py.setValue(QString::number(py(), 'f', 6));
    node_pz.setValue(QString::number(pz(), 'f', 6));

    node_ax.setValue(QString::number(ax(), 'f', 12));
    node_ay.setValue(QString::number(ay(), 'f', 12));
    node_az.setValue(QString::number(az(), 'f', 12));

    node.setAttributeNode(node_t);

    node.setAttributeNode(node_px);
    node.setAttributeNode(node_py);
    node.setAttributeNode(node_pz);

    node.setAttributeNode(node_ax);
    node.setAttributeNode(node_ay);
    node.setAttributeNode(node_az);

    return result;
}

} // namespace pro
} // namespace mc
