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
#ifndef MC_SIM_EDITOR_GUI_TREESCENE_H_
#define MC_SIM_EDITOR_GUI_TREESCENE_H_

#include <QTreeWidget>

namespace mc {
namespace gui {

class TreeScene : public QTreeWidget
{
    Q_OBJECT

public:

    explicit TreeScene(QWidget *parent = nullptr);

    ~TreeScene();

    QModelIndex getIndexOfItem(QTreeWidgetItem* item, int column = 0) const;
};

} // namespace gui
} // namespace mc

#endif // MC_SIM_EDITOR_GUI_TREESCENE_H_
