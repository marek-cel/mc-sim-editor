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

#include "Playback.h"

namespace mc {
namespace pro {

Result Playback::Read(const QDomElement* node)
{
    time_start_   = node->attribute("time_start").toDouble();
    time_end_     = node->attribute("time_end").toDouble();
    speed_        = node->attribute("speed").toDouble();
    current_time_ = node->attribute("current_time").toDouble();

    return Result::Success;
}

Result Playback::Save(QDomDocument* doc, QDomElement* parent)
{
    QDomElement node = doc->createElement("playback");
    parent->appendChild(node);

    QDomAttr node_time_start = doc->createAttribute("time_start");
    node_time_start.setValue(QString::number(time_start_, 'f', 6));
    node.setAttributeNode(node_time_start);

    QDomAttr node_time_end = doc->createAttribute("time_end");
    node_time_end.setValue(QString::number(time_end_, 'f', 6));
    node.setAttributeNode(node_time_end);

    QDomAttr node_speed = doc->createAttribute("speed");
    node_speed.setValue(QString::number(speed_, 'f', 6));
    node.setAttributeNode(node_speed);

    QDomAttr node_current_time = doc->createAttribute("current_time");
    node_current_time.setValue(QString::number(current_time_, 'f', 6));
    node.setAttributeNode(node_current_time);

    return Result::Success;
}

} // namespace pro
} // namespace mc
