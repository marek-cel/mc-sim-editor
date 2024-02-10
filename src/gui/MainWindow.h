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
#ifndef MC_SIM_EDITOR_GUI_MAINWINDOW_H_
#define MC_SIM_EDITOR_GUI_MAINWINDOW_H_

#include <memory>

#include <QMainWindow>
#include <QSettings>
#include <QShortcut>

#include <gui/RecentAction.h>

#include <pro/Project.h>

namespace Ui {
class MainWindow;
} // namespace Ui

namespace mc {
namespace gui {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    using RecentActions = QVector<RecentAction*>;

    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

    void openFileFromCommandLine(QString filename);

public slots:

    void projectChanged();

signals:

    void projectCreated(std::shared_ptr<pro::Project> project);

protected:

    /** */
    virtual void closeEvent(QCloseEvent* event) override;

private:

    Ui::MainWindow* ui_ = nullptr;

    QShortcut* shortcut_save_ = nullptr;    ///< save shortcut

    RecentActions recent_actions_;          ///< recent action items

    std::shared_ptr<pro::Project> proj_;    ///< current project

    int recent_files_max_ = 10;

    bool saved_ = true;                     ///< specifies if current file is saved or if there are unsaved changes

    void addRecentFile(QString file);

    void askIfSave();

    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();
    void exportModel();

    void readProject(QString file);
    void saveProject(QString file);
    void exportModel(QString file);

    void settingsRead();
    void settingsRead_RecentFiles(QSettings& settings);

    void settingsSave();
    void settingsSave_RecentFiles(QSettings& settings);

    void updateWindowTitle();

private slots:

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionClearRecent_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExport_triggered();
    void on_actionExit_triggered();

    void on_actionRefresh_triggered();
    void on_actionShowGrid_toggled(bool checked);
    void on_actionViewDefault_triggered();
    void on_actionViewTop_triggered();
    void on_actionViewBottom_triggered();
    void on_actionViewBack_triggered();
    void on_actionViewFront_triggered();
    void on_actionViewLeft_triggered();
    void on_actionViewRight_triggered();
    void on_actionViewOrbit_triggered();
    void on_actionViewTrack_triggered();

    void on_actionAbout_triggered();

    void recentFile_triggered(RecentAction* action);
};

} // namespace gui
} // namespace mc

#endif // MC_SIM_EDITOR_GUI_MAINWINDOW_H_
