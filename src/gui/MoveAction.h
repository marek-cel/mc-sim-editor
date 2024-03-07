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
#ifndef MC_SIM_EDITOR_GUI_MOVEACTION_H_
#define MC_SIM_EDITOR_GUI_MOVEACTION_H_

#include <QAction>

#include <pro/Group.h>

namespace mc {
namespace gui {

class MoveAction : public QAction
{
    Q_OBJECT

public:

    MoveAction(std::weak_ptr<pro::Group> group, QString text, QObject* parent);

    inline std::weak_ptr<pro::Group> group() { return group_; }

signals:

    void triggered(MoveAction* action);

private:

    std::weak_ptr<pro::Group> group_;

private slots:

    void trigger();
};

} // namespace gui
} // namespace mc

#endif // MC_SIM_EDITOR_GUI_MOVEACTION_H_
