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
#ifndef MC_SIM_EDITOR_GUI_FORMSCENE_H_
#define MC_SIM_EDITOR_GUI_FORMSCENE_H_

////////////////////////////////////////////////////////////////////////////////

#include <QAction>
#include <QMenu>
#include <QShortcut>
#include <QTreeWidgetItem>
#include <QWidget>

#include <pro/Component.h>
#include <pro/Project.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
class FormScene;
} // namespace Ui

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

/** */
class FormScene : public QWidget
{
    Q_OBJECT

public:

    explicit FormScene(QWidget* parent = nullptr);

    ~FormScene();

public slots:

    void setProject(std::shared_ptr<pro::Project> proj);

signals:

    void projectChanged();
    void componentChanged(std::shared_ptr<pro::Component>);

private:

    Ui::FormScene* ui_ = nullptr;

    QMenu* scene_menu_ = nullptr;

    QAction* action_rename_ = nullptr;
    QAction* action_remove_ = nullptr;

    QAction* action_anims_turn_on_  = nullptr;
    QAction* action_anims_turn_off_ = nullptr;

    QShortcut* shortcut_remove_ = nullptr;

    std::weak_ptr<pro::Project> proj_;

    std::shared_ptr<pro::Component> getComponentByIndex(QModelIndex index);

    void addComponent();
    void removeComponent();

    void addTreeWidgetSceneItem(pro::Component* comp, int index = 0,
                                QTreeWidgetItem* parent = nullptr);

    void createSceneMenu();

    void populateComboBoxTypes();

    void updateTreeWidgetScene();

private slots:

    void actionRename_triggered();
    void actionRemove_triggered();

    void actionAnimsOn_triggered();
    void actionAnimsOff_triggered();

    void on_buttonAdd_clicked();

    void on_treeScene_currentItemChanged(QTreeWidgetItem* current,
                                         QTreeWidgetItem* previous);
    void on_treeScene_customContextMenuRequested(const QPoint& pos);
    void on_treeScene_itemChanged(QTreeWidgetItem* item, int column);
};

} // namespace gui
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_GUI_FORMSCENE_H_
