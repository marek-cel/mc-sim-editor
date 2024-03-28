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

namespace mc {
namespace gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _shortcut_save   = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(on_actionSave_triggered()));
    _shortcut_reload = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this, SLOT(on_actionReload_triggered()));

    connect(this, SIGNAL(projectCreated(std::shared_ptr<pro::Project>)), _ui->widgetComp  , SLOT(setProject(std::shared_ptr<pro::Project>)));
    connect(this, SIGNAL(projectCreated(std::shared_ptr<pro::Project>)), _ui->widgetScene , SLOT(setProject(std::shared_ptr<pro::Project>)));
    connect(this, SIGNAL(projectCreated(std::shared_ptr<pro::Project>)), _ui->widgetCGI   , SLOT(setProject(std::shared_ptr<pro::Project>)));
    connect(this, SIGNAL(projectCreated(std::shared_ptr<pro::Project>)), _ui->widgetAnim  , SLOT(setProject(std::shared_ptr<pro::Project>)));

    connect(_ui->widgetComp  , SIGNAL(projectChanged()), this, SLOT(projectChanged()));
    connect(_ui->widgetScene , SIGNAL(projectChanged()), this, SLOT(projectChanged()));
    connect(_ui->widgetAnim  , SIGNAL(projectChanged()), this, SLOT(projectChanged()));

    connect(_ui->widgetScene, SIGNAL(componentChanged(std::shared_ptr<pro::Component>)), _ui->widgetComp, SLOT(setComponent(std::shared_ptr<pro::Component>)));

    settingsRead();
    newProject();
}

MainWindow::~MainWindow()
{
    settingsSave();

    if ( _ui ) { delete _ui; } _ui = nullptr;
}

void MainWindow::openFileFromCommandLine(QString filename)
{
    readProject(filename);
}

void MainWindow::projectChanged()
{
    _saved = false;
    _ui->widgetCGI->setAnimationTime(_ui->widgetAnim->getCurrentTime());
    updateWindowTitle();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    askIfSave();

    ///////////////////////////////
    QMainWindow::closeEvent(event);
    ///////////////////////////////
}

void MainWindow::addRecentFile(QString file)
{
    QStringList recent_files;
    for ( auto action : _recent_actions )
    {
        action->disconnect();
        recent_files.push_back(action->file());
    }

    _recent_actions.clear();
    _ui->menuRecentFiles->clear();

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

    for ( int i = 0; i < recent_files.size() && i < _recent_files_max; ++i )
    {
        RecentAction* action = new RecentAction(recent_files.at(i), _ui->menuRecentFiles);
        _recent_actions.push_back(action);

        connect(action, SIGNAL(triggered(RecentAction*)), SLOT(recentFile_triggered(RecentAction*)));
        _ui->menuRecentFiles->addAction(action);
    }
}

void MainWindow::askIfSave()
{
    if ( !_saved )
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

void MainWindow::newProject()
{
    askIfSave();
    _proj = std::make_shared<pro::Project>();
    _saved = true;
    emit projectCreated(_proj);
    updateWindowTitle();
}

void MainWindow::openProject()
{
    askIfSave();

    QString caption = tr("Open...");
    QString dir = ( _proj->getFile().length() > 0 ) ? QFileInfo(_proj->getFile()).absolutePath() : "";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "(*.mcedit)";

    QString file = QFileDialog::getOpenFileName(this, caption, dir, filter, &selectedFilter);

    if ( file.length() > 0 )
    {
        readProject(file);
    }

    updateWindowTitle();
}

void MainWindow::saveProject()
{
    if ( _proj->getFile().length() > 0 )
    {
        saveProject(_proj->getFile());
    }
    else
    {
        saveProjectAs();
    }
}

void MainWindow::saveProjectAs()
{
    QString caption = tr("Save as...");
    QString dir = ( _proj->getFile().length() > 0 ) ? QFileInfo(_proj->getFile()).absolutePath() : ".";
    QString filter;
    QString selectedFilter;

    filter += selectedFilter = "(*.mcedit)";

    QString newFile = QFileDialog::getSaveFileName(this, caption, dir, filter, &selectedFilter);

    if ( newFile.length() > 0 )
    {
        saveProject(newFile);
    }
}

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

void MainWindow::readProject(QString file)
{
    QFileInfo fileInfo(file);

    if ( file.length() > 0 && fileInfo.suffix() == QString("mcedit") )
    {
        QDir proj_dir = fileInfo.absoluteDir();
        QString fileFullPath = proj_dir.absoluteFilePath(fileInfo.fileName());

        std::shared_ptr<pro::Project> proj_temp = std::make_shared<pro::Project>();
        if ( Result::Success == proj_temp->read(fileFullPath) )
        {
            _proj = proj_temp;
            _saved = true;
            emit projectCreated(_proj);
            updateWindowTitle();
            addRecentFile(_proj->getFile());
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

void MainWindow::saveProject(QString file)
{
    if ( Result::Success == _proj->save(file) )
    {
        addRecentFile(_proj->getFile());
        _saved = true;
    }
    else
    {
        QMessageBox::warning(this, tr(APP_TITLE),
                             tr("Cannot save file %1.").arg(file));
    }

    updateWindowTitle();
}

void MainWindow::exportModel(QString file)
{
    if ( Result::Failure == _proj->getAssembly()->exportModel(file) )
    {
        QMessageBox::warning(this, tr(APP_TITLE),
                             tr("Cannot export file %1.").arg(file));
    }
}

void MainWindow::settingsRead()
{
    QSettings settings( ORG_NAME, APP_NAME );

    settings.beginGroup("main_window");

    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());

    _ui->splitterHorModel->restoreState(settings.value("splitter_hor_state").toByteArray());
    _ui->splitterVerModel->restoreState(settings.value("splitter_ver_state").toByteArray());

    bool grid_visible = settings.value("grid_visible" , 1).toBool();
    _ui->actionShowGrid->setChecked(grid_visible);

    settingsRead_RecentFiles(settings);

    settings.endGroup();
}

void MainWindow::settingsRead_RecentFiles(QSettings& settings)
{
    QStringList recent_files = settings.value("recent_files").toStringList();
    for ( auto file : recent_files )
    {
        RecentAction* action = new RecentAction(file, _ui->menuRecentFiles);
        _recent_actions.push_back(action);
        connect(action, SIGNAL(triggered(RecentAction*)), SLOT(recentFile_triggered(RecentAction*)));
        _ui->menuRecentFiles->addAction(action);
    }
}

void MainWindow::settingsSave()
{
    QSettings settings(ORG_NAME, APP_NAME);

    settings.beginGroup("main_window");

    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());

    settings.setValue("splitter_hor_state", _ui->splitterHorModel->saveState());
    settings.setValue("splitter_ver_state", _ui->splitterVerModel->saveState());

    settings.setValue("grid_visible" , _ui->actionShowGrid ->isChecked() ? 1 : 0);

    settingsSave_RecentFiles(settings);

    settings.endGroup();
}

void MainWindow::settingsSave_RecentFiles(QSettings& settings)
{
    QStringList recent_files;
    for ( auto action : _recent_actions )
    {
        recent_files.push_back(action->file());
    }

    settings.setValue("recent_files", recent_files);
}

void MainWindow::updateWindowTitle()
{
    QString title;

    if ( _proj->getFile().length() > 0 )
    {
        title = QFileInfo(_proj->getFile()).fileName();
    }
    else
    {
        title = tr("Untitled");
    }

    if ( !_saved ) title += " (*)";
    title += " - ";
    title += APP_TITLE;

    setWindowTitle(title);
}

void MainWindow::on_actionNew_triggered()
{
    newProject();
}

void MainWindow::on_actionOpen_triggered()
{
    openProject();
}

void MainWindow::on_actionClearRecent_triggered()
{
    for ( auto action : _recent_actions )
    {
        action->disconnect(this, SLOT(recentFile_triggered(RecentAction*)));
    }
    _recent_actions.clear();
    _ui->menuRecentFiles->clear();
}

void MainWindow::on_actionSave_triggered()
{
    saveProject();
}

void MainWindow::on_actionSaveAs_triggered()
{
    saveProjectAs();
}

void MainWindow::on_actionExport_triggered()
{
    exportModel();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionReload_triggered()
{
    cgi::Models::reset();
    cgi::Textures::reset();
    readProject(_proj->getFile());
}

void MainWindow::on_actionShowGrid_toggled(bool checked)
{
    _ui->widgetCGI->getManagerCGI()->setGridVisibility(checked);
}

void MainWindow::on_actionViewDefault_triggered()
{
    _ui->widgetCGI->resetView();
}

void MainWindow::on_actionViewTop_triggered()
{
    _ui->widgetCGI->topView();
}

void MainWindow::on_actionViewBottom_triggered()
{
    _ui->widgetCGI->bottomView();
}

void MainWindow::on_actionViewBack_triggered()
{
    _ui->widgetCGI->backView();
}

void MainWindow::on_actionViewFront_triggered()
{
    _ui->widgetCGI->frontView();
}

void MainWindow::on_actionViewLeft_triggered()
{
    _ui->widgetCGI->leftView();
}

void MainWindow::on_actionViewRight_triggered()
{
    _ui->widgetCGI->rightView();
}

void MainWindow::on_actionViewOrbit_triggered()
{
    _ui->widgetCGI->setCameraManipulatorOrbit();
    _ui->widgetCGI->resetView();
}

void MainWindow::on_actionViewTrack_triggered()
{
    _ui->widgetCGI->setCameraManipulatorTrack();
    _ui->widgetCGI->resetView();
}

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

void MainWindow::recentFile_triggered(RecentAction* action)
{
    askIfSave();
    readProject(action->file());
}

} // namespace gui
} // namespace mc
