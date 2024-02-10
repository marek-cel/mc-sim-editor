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

#include "TreeScene.h"

#include <QKeyEvent>

namespace mc {
namespace gui {

TreeScene::TreeScene(QWidget *parent)
    : QTreeWidget(parent)
{}

TreeScene::~TreeScene() {}

QModelIndex TreeScene::getIndexOfItem(QTreeWidgetItem* item, int column) const
{
    return indexFromItem(item, column);
}

} // namespace gui
} // namespace mc
