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

#include <gui/FormScene.h>
#include <ui_FormScene.h>

#include <QMessageBox>

#include <pro/Components.h>

namespace mc {
namespace gui {

FormScene::FormScene(QWidget* parent)
    : QWidget(parent)
    , _ui(new Ui::FormScene)
{
    _ui->setupUi(this);

    _shortcut_remove = new QShortcut(QKeySequence(Qt::Key_Delete) , this, SLOT(actionRemove_triggered()));

    createSceneMenu();
    populateComboBoxTypes();
}

FormScene::~FormScene()
{
    if ( _ui ) { delete _ui; } _ui = nullptr;
}

std::shared_ptr<pro::Component> FormScene::getMarkedComponent()
{
    return getComponentByIndex(_marked_index);
}

void FormScene::setProject(std::shared_ptr<pro::Project> proj)
{
    _proj = proj;
    updateTreeWidgetScene();
    _ui->buttonAdd->setEnabled(false);
}

std::shared_ptr<pro::Component> FormScene::getComponentByIndex(QModelIndex index)
{
    std::shared_ptr<pro::Component> comp;

    if ( !_proj.expired() )
    {
        std::shared_ptr<pro::Project> proj = _proj.lock();
        comp = proj->getAssembly()->getRoot();
        if ( comp )
        {
            std::vector<int> indecies;
            indecies.clear();
            QModelIndex temp = index;
            while ( temp.parent().isValid() )
            {
                indecies.push_back(temp.row());
                temp = temp.parent();
            }

            for ( int i = indecies.size() - 1; i >= 0; i-- )
            {
                std::shared_ptr<pro::Group> group = std::dynamic_pointer_cast<pro::Group>(comp);

                if ( group )
                {
                    comp = group->getChild(indecies.at(i));
                }
                else
                {
                    break;
                }
            }
        }
    }

    return comp;
}

void FormScene::addComponent()
{
    int index = _ui->comboTypes->currentIndex();
    pro::Components::Type type = pro::Components::instance()->types().at(index);
    std::shared_ptr<pro::Component> component = type.component->clone();
    std::shared_ptr<pro::Component> selected = getComponentByIndex(_ui->treeScene->currentIndex());

    std::shared_ptr<pro::Group> parent = std::dynamic_pointer_cast<pro::Group>(selected);

    if ( parent )
    {
        parent->addChild(component);
        updateTreeWidgetScene();
        emit(projectChanged());
    }
}

void FormScene::removeComponent()
{
    QModelIndex index = _ui->treeScene->currentIndex();
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);

    if ( comp )
    {
        if ( comp->getParent().expired() )
        {
            return;
        }

        std::shared_ptr<pro::Component> parent = comp->getParent().lock();

        if ( parent )
        {
            std::shared_ptr<pro::Group> group = std::dynamic_pointer_cast<pro::Group>(parent);

            if ( group )
            {
                group->removeChild(comp);
                updateTreeWidgetScene();
                emit(projectChanged());
            }
        }
    }
}

void FormScene::duplicateComponent()
{
    QModelIndex index = _ui->treeScene->currentIndex();
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);

    if ( comp )
    {
        if ( comp->getParent().expired() )
        {
            return;
        }

        std::shared_ptr<pro::Component> parent = comp->getParent().lock();

        if ( parent )
        {
            std::shared_ptr<pro::Group> group = std::dynamic_pointer_cast<pro::Group>(parent);

            if ( group )
            {
                std::shared_ptr<pro::Component> duplicate = comp->clone();
                group->addChild(duplicate);
                updateTreeWidgetScene();
                emit(projectChanged());
            }
        }
    }
}

void FormScene::markComponent()
{
    _marked_index = _ui->treeScene->currentIndex();
}

void FormScene::moveComponent(std::shared_ptr<pro::Group> new_parent)
{
    QModelIndex index = _ui->treeScene->currentIndex();
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);

    if ( comp )
    {
        if ( comp->getParent().expired() )
        {
            return;
        }

        std::shared_ptr<pro::Component> parent = comp->getParent().lock();
        if ( parent )
        {
            std::shared_ptr<pro::Group> group = std::dynamic_pointer_cast<pro::Group>(parent);

            if ( group )
            {
                group->removeChild(comp);
            }
        }

        new_parent->addChild(comp);
        updateTreeWidgetScene();
        emit(projectChanged());
    }
}

void FormScene::addTreeWidgetSceneItem(pro::Component* comp, int index,
                                       QTreeWidgetItem* parent)
{
    if ( comp )
    {
        QTreeWidgetItem* item = 0;
        if ( parent )
        {
            item = new QTreeWidgetItem(parent);
        }
        else
        {
            item = new QTreeWidgetItem(_ui->treeScene);
        }

        item->setFlags(item->flags()|Qt::ItemIsEditable);
        item->setText(0, comp->getNode()->getName().c_str());

        pro::Group* group = dynamic_cast<pro::Group*>(comp);

        if ( group )
        {
            for ( size_t i = 0; i < group->getChildrenCount(); ++i )
            {
                std::shared_ptr<pro::Component> comp = group->getChild(i);
                addTreeWidgetSceneItem(comp.get(), index, item);
            }
        }

        if ( parent )
        {
            parent->insertChild(index, item);
            _ui->treeScene->expandItem(parent);
        }
        else
        {
            _ui->treeScene->insertTopLevelItem(0, item);
        }
    }
}

void FormScene::createSceneMenu()
{
    _scene_menu = new QMenu(this);

    _action_rename = new QAction(tr("Rename (F2)"), this);
    _action_remove = new QAction(tr("Remove (Del)"), this);
    connect(_action_rename, SIGNAL(triggered()), this, SLOT(actionRename_triggered()));
    connect(_action_remove, SIGNAL(triggered()), this, SLOT(actionRemove_triggered()));
    _scene_menu->addAction(_action_rename);
    _scene_menu->addAction(_action_remove);

    _action_duplicate = new QAction(tr("Duplicate"), this);
    connect(_action_duplicate, SIGNAL(triggered()), this, SLOT(actionDuplicate_triggered()));
    _scene_menu->addAction(_action_duplicate);

    _action_mark = new QAction(tr("Mark for multiply"), this);
    connect(_action_mark, SIGNAL(triggered()), this, SLOT(actionMark_triggered()));
    _scene_menu->addAction(_action_mark);

    _scene_menu->addSeparator();
    _action_move = new QAction(tr("Move"), this);
    _action_move->setEnabled(false);
    connect(_action_move, SIGNAL(triggered()), this, SLOT(actionMove_triggered()));
    _scene_menu->addAction(_action_move);

    _scene_menu->addSeparator();
    _action_anims_turn_on  = new QAction(tr("Enable all children animations"  ), this);
    _action_anims_turn_off = new QAction(tr("Disable all children animations" ), this);
    connect(_action_anims_turn_on  , SIGNAL(triggered()), this, SLOT(actionAnimsOn_triggered()));
    connect(_action_anims_turn_off , SIGNAL(triggered()), this, SLOT(actionAnimsOff_triggered()));
    _scene_menu->addAction(_action_anims_turn_on);
    _scene_menu->addAction(_action_anims_turn_off);
}

void FormScene::populateComboBoxTypes()
{
    for ( auto type : pro::Components::instance()->types() )
    {
        if ( type.extra )
        {
            _ui->comboTypes->addItem(QString("+ ") + type.name);
        }
        else
        {
            _ui->comboTypes->addItem(type.name);
        }
    }
}

void FormScene::populateMoveMenu(QMenu* menu, std::shared_ptr<pro::Group> group)
{
    if ( group )
    {
        QMenu* group_menu = new QMenu(group->getName(), menu);
        menu->addMenu(group_menu);

        QAction* action = new MoveAction(group, tr("Move here."), group_menu);
        group_menu->addAction(action);
        connect(action, SIGNAL(triggered(MoveAction*)), SLOT(moveHere_triggered(MoveAction*)));

        int count = 0;
        for ( int i = 0; i < group->getChildrenCount(); ++i )
        {
            std::shared_ptr<pro::Component> child = group->getChild(i);
            std::shared_ptr<pro::Group> cg = std::dynamic_pointer_cast<pro::Group>(child);

            if ( cg )
            {
                if ( count == 0 )
                {
                    group_menu->addSeparator();
                }
                count++;
            }

            populateMoveMenu(group_menu, cg);
        }
    }
}

void FormScene::updateTreeWidgetScene()
{
    _ui->treeScene->blockSignals(true);

    _ui->treeScene->clear();

    if ( !_proj.expired() )
    {
        std::shared_ptr<pro::Project> proj = _proj.lock();
        std::shared_ptr<pro::Component> comp = proj->getAssembly()->getRoot();
        addTreeWidgetSceneItem(comp.get());
    }

    _ui->treeScene->setCurrentIndex(QModelIndex());

    _ui->treeScene->blockSignals(false);
}

void FormScene::actionRename_triggered()
{
    QTreeWidgetItem* item = _ui->treeScene->currentItem();
    _ui->treeScene->editItem(item);
}

void FormScene::actionRemove_triggered()
{
    QString title = windowTitle();
    QString text = tr("Remove?");
    QMessageBox::StandardButton result = QMessageBox::question(this, title, text,
                                                               QMessageBox::Yes|QMessageBox::No,
                                                               QMessageBox::No);

    if ( result == QMessageBox::Yes )
    {
        removeComponent();
    }
}

void FormScene::actionDuplicate_triggered()
{
    duplicateComponent();
}

void FormScene::actionMark_triggered()
{
    markComponent();
}

void FormScene::actionMove_triggered()
{
    QMenu menu(this);

    if ( !_proj.expired() )
    {
        std::shared_ptr<pro::Project> proj = _proj.lock();
        std::shared_ptr<pro::Component> root = proj->getAssembly()->getRoot();
        std::shared_ptr<pro::Group> rg = std::dynamic_pointer_cast<pro::Group>(root);
        populateMoveMenu(&menu, rg);
    }

    menu.exec(QCursor::pos());
}

void FormScene::actionAnimsOn_triggered()
{
    QModelIndex index = _ui->treeScene->currentIndex();
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);
    if ( comp )
    {
        comp->setChildrenAnimationState(true);
    }
}

void FormScene::actionAnimsOff_triggered()
{
    QModelIndex index = _ui->treeScene->currentIndex();
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);
    if ( comp )
    {
        comp->setChildrenAnimationState(false);
    }
}

void FormScene::on_buttonAdd_clicked()
{
    addComponent();
}

void FormScene::on_treeScene_currentItemChanged(QTreeWidgetItem*,
                                                QTreeWidgetItem*)
{
    _ui->buttonAdd->setEnabled(false);

    std::shared_ptr<pro::Component> comp = getComponentByIndex(_ui->treeScene->currentIndex());
    _ui->buttonAdd->setEnabled(comp->canBeParent());

    _action_move->setEnabled(false);
    if ( !comp->isRoot() )
    {
        _action_move->setEnabled(true);
    }

    emit(componentChanged(comp));
}

void FormScene::on_treeScene_customContextMenuRequested(const QPoint& pos)
{
    _scene_menu->exec(_ui->treeScene->mapToGlobal(pos));
}

void FormScene::on_treeScene_itemChanged(QTreeWidgetItem* item, int column)
{
    QModelIndex index = _ui->treeScene->getIndexOfItem(item, column);
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);
    if ( comp )
    {
        comp->setName(item->text(0));
        updateTreeWidgetScene();
        emit(projectChanged());
    }
}

void FormScene::moveHere_triggered(MoveAction* action)
{
    if ( !action->group().expired() )
    {
        std::shared_ptr<pro::Group> group = action->group().lock();
        moveComponent(group);
    }
}

} // namespace gui
} // namespace mc
