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

#include <gui/MainWindow.h>
#include <ui_MainWindow.h>

#include <QFileDialog>
#include <QMessageBox>

#include <defs.h>

#include <cgi/Models.h>
#include <cgi/Textures.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    shortcut_save_ = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(on_actionSave_triggered()));

    connect(this, SIGNAL(projectCreated(std::shared_ptr<pro::Project>)), ui_->widgetComp  , SLOT(setProject(std::shared_ptr<pro::Project>)));
    connect(this, SIGNAL(projectCreated(std::shared_ptr<pro::Project>)), ui_->widgetScene , SLOT(setProject(std::shared_ptr<pro::Project>)));
    connect(this, SIGNAL(projectCreated(std::shared_ptr<pro::Project>)), ui_->widgetCGI   , SLOT(setProject(std::shared_ptr<pro::Project>)));
    connect(this, SIGNAL(projectCreated(std::shared_ptr<pro::Project>)), ui_->widgetAnim  , SLOT(setProject(std::shared_ptr<pro::Project>)));

    connect(ui_->widgetComp  , SIGNAL(projectChanged()), this, SLOT(projectChanged()));
    connect(ui_->widgetScene , SIGNAL(projectChanged()), this, SLOT(projectChanged()));
    connect(ui_->widgetAnim  , SIGNAL(projectChanged()), this, SLOT(projectChanged()));

    connect(ui_->widgetScene, SIGNAL(componentChanged(std::shared_ptr<pro::Component>)), ui_->widgetComp, SLOT(setComponent(std::shared_ptr<pro::Component>)));

    settingsRead();
    newProject();
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    settingsSave();

    if ( ui_ ) { delete ui_; } ui_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::projectChanged()
{
    saved_ = false;
    ui_->widgetCGI->setAnimationTime(ui_->widgetAnim->getCurrentTime());
    updateWindowTitle();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::closeEvent(QCloseEvent* event)
{
    askIfSave();

    ///////////////////////////////
    QMainWindow::closeEvent(event);
    ///////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::addRecentFile(QString file)
{
    QStringList recent_files;
    for ( auto action : recent_actions_ )
    {
        action->disconnect();
        recent_files.push_back(action->file());
    }

    recent_actions_.clear();
    ui_->menuRecentFiles->clear();

    if ( file.length() > 0 )
    {
#       ifdef WIN32
        if ( recent_files.contains(file, Qt::CaseInsensitive) )
#       else
        if ( recent_files.contains(file, Qt::CaseSensitive) )
#       endif
        {
            recent_files.move(recent_files.indexOf(file),0);
        }
        else
        {
            recent_files.push_front(file);
        }
    }

    for ( int i = 0; i < recent_files.size() && i < 4; ++i )
    {
        RecentAction* action = new RecentAction(recent_files.at(i), ui_->menuRecentFiles);
        recent_actions_.push_back(action);

        connect(action, SIGNAL(triggered(RecentAction*)), SLOT(recentFile_triggered(RecentAction*)));
        ui_->menuRecentFiles->addAction(action);
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::askIfSave()
{
    if ( !saved_ )
    {
        QString title = windowTitle();
        QString text = tr("File have unsaved changes.");

        QMessageBox::StandardButton result = QMessageBox::question(this, title, text,
                                                                   QMessageBox::Save | QMessageBox::Discard,
                                                                   QMessageBox::Save);

        if ( result == QMessageBox::Save )
        {
            saveProject();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::newProject()
{
    askIfSave();
    proj_ = std::make_shared<pro::Project>();
    saved_ = true;
    emit projectCreated(proj_);
    updateWindowTitle();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::openProject()
{
    askIfSave();

    QString caption = tr("Open...");
    QString dir = ( proj_->GetFile().length() > 0 ) ? QFileInfo(proj_->GetFile()).absolutePath() : "";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "(*.pro)";

    QString file = QFileDialog::getOpenFileName(this, caption, dir, filter, &selectedFilter);

    if ( file.length() > 0 )
    {
        readProject(file);
    }

    updateWindowTitle();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::saveProject()
{
    if ( proj_->GetFile().length() > 0 )
    {
        saveProject(proj_->GetFile());
    }
    else
    {
        saveProjectAs();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::saveProjectAs()
{
    QString caption = tr("Save as...");
    QString dir = ( proj_->GetFile().length() > 0 ) ? QFileInfo(proj_->GetFile()).absolutePath() : ".";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "(*.pro)";

    QString newFile = QFileDialog::getSaveFileName(this, caption, dir, filter, &selectedFilter);

    if ( newFile.length() > 0 )
    {
        saveProject(newFile);
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::exportModel()
{
    QString fileName = "";

    QString caption = tr("Export 3D model as...");
    QString dir = ( fileName.length() > 0 ) ? QFileInfo(fileName).absolutePath() : ".";
    QString filter;
    QString selectedFilter;

    filter += "AC3D (*.ac)";
    filter += ";;";
    filter += "3D Studio (*.3ds)";
    filter += ";;";
    filter += "COLLADA (*.dae)";
    filter += ";;";
    filter += selectedFilter = "OpenSceneGraph (*.osg *.osga *.osgb *.osgt *.ive)";
    filter += ";;";
    filter += "Wavefront OBJ (*.obj)";

    fileName = QFileDialog::getSaveFileName(this, caption, dir, filter, &selectedFilter);

    if ( fileName.length() > 0 )
    {
        exportModel(fileName);
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::readProject(QString file)
{
    if ( file.length() > 0 && QFileInfo(file).suffix() == QString("pro") )
    {
        std::shared_ptr<pro::Project> proj_temp = std::make_shared<pro::Project>();
        if ( Result::Success == proj_temp->Read(file) )
        {
            proj_ = proj_temp;
            saved_ = true;
            emit projectCreated(proj_);
            updateWindowTitle();
            addRecentFile(proj_->GetFile());
        }
        else
        {

            QMessageBox::warning(this, tr(APP_TITLE),
                                 tr("Cannot read file %1.").arg(file));
        }
    }
    else
    {
        QMessageBox::warning(this, tr(APP_TITLE), tr("Wrong type of file.") );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::saveProject(QString file)
{
    if ( Result::Success == proj_->Save(file) )
    {
        addRecentFile(proj_->GetFile());
        saved_ = true;
    }
    else
    {
        QMessageBox::warning(this, tr(APP_TITLE),
                             tr("Cannot save file %1.").arg(file));
    }

    updateWindowTitle();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::exportModel(QString file)
{
    if ( Result::Failure == proj_->GetAssembly()->Export(file) )
    {
        QMessageBox::warning(this, tr(APP_TITLE),
                             tr("Cannot export file %1.").arg(file));
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead()
{
    QSettings settings( ORG_NAME, APP_NAME );

    settings.beginGroup("main_window");

    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());

    ui_->splitterHorModel->restoreState(settings.value("splitter_hor_state").toByteArray());
    ui_->splitterVerModel->restoreState(settings.value("splitter_ver_state").toByteArray());

    bool grid_visible = settings.value("grid_visible" , 1).toBool();
    ui_->actionShowGrid->setChecked(grid_visible);

    settingsRead_RecentFiles(settings);

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead_RecentFiles(QSettings& settings)
{
    QStringList recent_files = settings.value("recent_files").toStringList();
    for ( auto file : recent_files )
    {
        RecentAction* action = new RecentAction(file, ui_->menuRecentFiles);
        recent_actions_.push_back(action);
        connect(action, SIGNAL(triggered(RecentAction*)), SLOT(recentFile_triggered(RecentAction*)));
        ui_->menuRecentFiles->addAction(action);
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave()
{
    QSettings settings(ORG_NAME, APP_NAME);

    settings.beginGroup("main_window");

    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());

    settings.setValue("splitter_hor_state", ui_->splitterHorModel->saveState());
    settings.setValue("splitter_ver_state", ui_->splitterVerModel->saveState());

    settings.setValue("grid_visible" , ui_->actionShowGrid ->isChecked() ? 1 : 0);

    settingsSave_RecentFiles(settings);

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave_RecentFiles(QSettings& settings)
{
    QStringList recent_files;
    for ( auto action : recent_actions_ )
    {
        recent_files.push_back(action->file());
    }

    settings.setValue("recent_files", recent_files);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateWindowTitle()
{
    QString title;

    if ( proj_->GetFile().length() > 0 )
    {
        title = QFileInfo(proj_->GetFile()).fileName();
    }
    else
    {
        title = tr("Untitled");
    }

    if ( !saved_ ) title += " (*)";
    title += " - ";
    title += APP_TITLE;

    setWindowTitle(title);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionNew_triggered()
{
    newProject();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionOpen_triggered()
{
    openProject();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionClearRecent_triggered()
{
    for ( auto action : recent_actions_ )
    {
        action->disconnect(this, SLOT(recentFile_triggered(RecentAction*)));
    }
    recent_actions_.clear();
    ui_->menuRecentFiles->clear();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionSave_triggered()
{
    saveProject();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionSaveAs_triggered()
{
    saveProjectAs();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionExport_triggered()
{
    exportModel();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionExit_triggered()
{
    close();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionRefresh_triggered()
{
    cgi::Models::Reset();
    cgi::Textures::Reset();
    readProject(proj_->GetFile());
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowGrid_toggled(bool checked)
{
    ui_->widgetCGI->getManagerCGI()->SetGridVisibility(checked);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewDefault_triggered()
{
    ui_->widgetCGI->resetView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewTop_triggered()
{
    ui_->widgetCGI->topView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewBottom_triggered()
{
    ui_->widgetCGI->bottomView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewBack_triggered()
{
    ui_->widgetCGI->backView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewFront_triggered()
{
    ui_->widgetCGI->frontView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewLeft_triggered()
{
    ui_->widgetCGI->leftView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewRight_triggered()
{
    ui_->widgetCGI->rightView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewOrbit_triggered()
{
    ui_->widgetCGI->setCameraManipulatorOrbit();
    ui_->widgetCGI->resetView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewTrack_triggered()
{
    ui_->widgetCGI->setCameraManipulatorTrack();
    ui_->widgetCGI->resetView();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionAbout_triggered()
{
    QFile about_html_file(":/gui/html/about.html");

    QString about_win_title;
    QString about_info_text;

    about_win_title = tr("About");

    if ( about_html_file.open(QIODevice::ReadOnly) )
    {
        about_info_text = about_html_file.readAll();
        about_html_file.close();
    }

    QMessageBox::about(this, about_win_title, about_info_text);
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::recentFile_triggered(RecentAction* action)
{
    askIfSave();
    readProject(action->file());
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gui
} // namespace mc
