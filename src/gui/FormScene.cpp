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
    , ui_(new Ui::FormScene)
{
    ui_->setupUi(this);

    shortcut_remove_ = new QShortcut(QKeySequence(Qt::Key_Delete) , this, SLOT(actionRemove_triggered()));

    createSceneMenu();
    populateComboBoxTypes();
}

FormScene::~FormScene()
{
    if ( ui_ ) { delete ui_; } ui_ = nullptr;
}

void FormScene::setProject(std::shared_ptr<pro::Project> proj)
{
    proj_ = proj;
    updateTreeWidgetScene();
    ui_->buttonAdd->setEnabled(false);
}

std::shared_ptr<pro::Component> FormScene::getComponentByIndex(QModelIndex index)
{
    std::shared_ptr<pro::Component> comp;

    if ( !proj_.expired() )
    {
        std::shared_ptr<pro::Project> proj = proj_.lock();
        comp = proj->GetAssembly()->GetRoot();
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
                    comp = group->GetChild(indecies.at(i));
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
    int index = ui_->comboTypes->currentIndex();
    pro::Components::Type type = pro::Components::Instance()->types().at(index);
    std::shared_ptr<pro::Component> component = type.component->Clone();
    std::shared_ptr<pro::Component> selected = getComponentByIndex(ui_->treeScene->currentIndex());

    std::shared_ptr<pro::Group> parent = std::dynamic_pointer_cast<pro::Group>(selected);

    if ( parent )
    {
        parent->AddChild(component);
        updateTreeWidgetScene();
        emit(projectChanged());
    }
}

void FormScene::removeComponent()
{
    QModelIndex index = ui_->treeScene->currentIndex();
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);

    if ( comp )
    {
        if ( comp->GetParent().expired() )
        {
            return;
        }

        std::shared_ptr<pro::Component> parent = comp->GetParent().lock();

        if ( parent )
        {
            std::shared_ptr<pro::Group> group = std::dynamic_pointer_cast<pro::Group>(parent);

            if ( group )
            {
                group->RemoveChild(comp);
                updateTreeWidgetScene();
                emit(projectChanged());
            }
        }
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
            item = new QTreeWidgetItem(ui_->treeScene);
        }

        item->setFlags(item->flags()|Qt::ItemIsEditable);
        item->setText(0, comp->GetNode()->getName().c_str());

        pro::Group* group = dynamic_cast<pro::Group*>(comp);

        if ( group )
        {
            for ( size_t i = 0; i < group->GetChildrenCount(); ++i )
            {
                std::shared_ptr<pro::Component> comp = group->GetChild(i);
                addTreeWidgetSceneItem(comp.get(), index, item);
            }
        }

        if ( parent )
        {
            parent->insertChild(index, item);
            ui_->treeScene->expandItem(parent);
        }
        else
        {
            ui_->treeScene->insertTopLevelItem(0, item);
        }
    }
}

void FormScene::createSceneMenu()
{
    scene_menu_ = new QMenu(this);

    action_rename_ = new QAction(tr("Rename (F2)"), this);
    action_remove_ = new QAction(tr("Remove (Del)"), this);
    connect(action_rename_, SIGNAL(triggered()), this, SLOT(actionRename_triggered()));
    connect(action_remove_, SIGNAL(triggered()), this, SLOT(actionRemove_triggered()));
    scene_menu_->addAction(action_rename_);
    scene_menu_->addAction(action_remove_);

//    scene_menu_->addSeparator();
//    action_change_parent_ = new QAction(tr("Change parent"), this);
//    action_change_parent_->setEnabled(false);
//    connect(action_change_parent_, SIGNAL(triggered()), this, SLOT(actionChangeParent_triggered()));
//    scene_menu_->addAction(action_change_parent_);

    scene_menu_->addSeparator();
    action_anims_turn_on_  = new QAction(tr("Enable all children animations"  ), this);
    action_anims_turn_off_ = new QAction(tr("Disable all children animations" ), this);
    connect(action_anims_turn_on_  , SIGNAL(triggered()), this, SLOT(actionAnimsOn_triggered()));
    connect(action_anims_turn_off_ , SIGNAL(triggered()), this, SLOT(actionAnimsOff_triggered()));
    scene_menu_->addAction(action_anims_turn_on_);
    scene_menu_->addAction(action_anims_turn_off_);
}

void FormScene::populateComboBoxTypes()
{
    for ( auto type : pro::Components::Instance()->types() )
    {
        if ( type.extra )
        {
            ui_->comboTypes->addItem(QString("+ ") + type.name);
        }
        else
        {
            ui_->comboTypes->addItem(type.name);
        }
    }
}

void FormScene::updateTreeWidgetScene()
{
    ui_->treeScene->blockSignals(true);

    ui_->treeScene->clear();

    if ( !proj_.expired() )
    {
        std::shared_ptr<pro::Project> proj = proj_.lock();
        std::shared_ptr<pro::Component> comp = proj->GetAssembly()->GetRoot();
        addTreeWidgetSceneItem(comp.get());
    }

    ui_->treeScene->setCurrentIndex(QModelIndex());

    ui_->treeScene->blockSignals(false);
}

void FormScene::actionRename_triggered()
{
    QTreeWidgetItem* item = ui_->treeScene->currentItem();
    ui_->treeScene->editItem(item);
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

void FormScene::actionChangeParent_triggered()
{
    // TODO
}

void FormScene::actionAnimsOn_triggered()
{
    QModelIndex index = ui_->treeScene->currentIndex();
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);
    if ( comp )
    {
        comp->SetChildrenAnimationState(true);
    }
}

void FormScene::actionAnimsOff_triggered()
{
    QModelIndex index = ui_->treeScene->currentIndex();
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);
    if ( comp )
    {
        comp->SetChildrenAnimationState(false);
    }
}

void FormScene::on_buttonAdd_clicked()
{
    addComponent();
}

void FormScene::on_treeScene_currentItemChanged(QTreeWidgetItem*,
                                                QTreeWidgetItem*)
{
    ui_->buttonAdd->setEnabled(false);

    std::shared_ptr<pro::Component> comp = getComponentByIndex(ui_->treeScene->currentIndex());
    ui_->buttonAdd->setEnabled(comp->CanBeParent());

    emit(componentChanged(comp));
}

void FormScene::on_treeScene_customContextMenuRequested(const QPoint& pos)
{
    scene_menu_->exec(ui_->treeScene->mapToGlobal(pos));
}

void FormScene::on_treeScene_itemChanged(QTreeWidgetItem* item, int column)
{
    QModelIndex index = ui_->treeScene->getIndexOfItem(item, column);
    std::shared_ptr<pro::Component> comp = getComponentByIndex(index);
    if ( comp )
    {
        comp->SetName(item->text(0));
        updateTreeWidgetScene();
        emit(projectChanged());
    }
}

} // namespace gui
} // namespace mc
