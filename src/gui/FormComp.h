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
#ifndef MC_SIM_EDITOR_GUI_FORMCOMP_H_
#define MC_SIM_EDITOR_GUI_FORMCOMP_H_

////////////////////////////////////////////////////////////////////////////////

#include <QToolButton>
#include <QWidget>

#include <pro/Component.h>
#include <pro/Project.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
class FormComp;
} // namespace Ui

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

class FormComp : public QWidget
{
    Q_OBJECT

public:

    explicit FormComp(QWidget* parent = nullptr);

    ~FormComp();

public slots:

    void setComponent(std::shared_ptr<pro::Component> comp);
    void setProject(std::shared_ptr<pro::Project> proj);

signals:

    void projectChanged();

private:

    Ui::FormComp* ui_ = nullptr;

    std::weak_ptr<pro::Component> comp_;
    std::weak_ptr<pro::Project> proj_;

    QString getFile(QString file = "");

    void hideAllProperties();

    void updateParameters(std::shared_ptr<pro::Component> comp);
    void updateParametersAnim(std::shared_ptr<pro::Component> comp);
    void updateParametersComp(std::shared_ptr<pro::Component> comp);
    void updateParametersFile(std::shared_ptr<pro::Component> comp);
    void updateParametersLOD(std::shared_ptr<pro::Component> comp);
    void updateParametersPAT(std::shared_ptr<pro::Component> comp);
    void updateParametersRotor(std::shared_ptr<pro::Component> comp);
    void updateParametersSwitch(std::shared_ptr<pro::Component> comp);
    void updateParametersTrans(std::shared_ptr<pro::Component> comp);

private slots:

    void on_buttonAnim_toggled(bool checked);
    void on_buttonComp_toggled(bool checked);
    void on_buttonFile_toggled(bool checked);
    void on_buttonLOD_toggled(bool checked);
    void on_buttonPAT_toggled(bool checked);
    void on_buttonRotor_toggled(bool checked);
    void on_buttonSwitch_toggled(bool checked);
    void on_buttonTrans_toggled(bool checked);

    // slots specific for Component
    void on_checkBoxDepthSortedBin_toggled(bool checked);
    void on_spinBoxDepthSortedBin_valueChanged(int arg1);

    // slots specific for PAT
    void on_radioXYZ_toggled(bool checked);
    void on_spinBoxPx_valueChanged(double arg1);
    void on_spinBoxPy_valueChanged(double arg1);
    void on_spinBoxPz_valueChanged(double arg1);
    void on_spinBoxAx_valueChanged(double arg1);
    void on_spinBoxAy_valueChanged(double arg1);
    void on_spinBoxAz_valueChanged(double arg1);

    // slots specific for LOD
    void on_listIntervals_currentRowChanged(int currentRow);
    void on_pushButtonIntervalAdd_clicked();
    void on_pushButtonIntervalSave_clicked();
    void on_pushButtonIntervalRemove_clicked();

    // slots specific for File
    void on_toolButtonBrowseFiles_clicked();
    void on_lineEditFilePath_textChanged(const QString &arg1);

    // slots specific for Switch
    void on_checkBoxSwitchVisible_toggled(bool checked);

    // slots specific for Rotor
    void on_toolButtonBrowseBladeFiles_clicked();
    void on_toolButtonBrowseShaftFiles_clicked();
    void on_lineEditBladeFile_textChanged(const QString &arg1);
    void on_lineEditShaftFile_textChanged(const QString &arg1);
    void on_spinBoxBladesNo_valueChanged(int arg1);
    void on_spinBoxHingeOffset_valueChanged(double arg1);
    void on_radioButtonCW_toggled(bool checked);

    // slots specific for Trans
    void on_spinBoxTransPx_valueChanged(double arg1);
    void on_spinBoxTransPy_valueChanged(double arg1);
    void on_spinBoxTransPz_valueChanged(double arg1);
    void on_spinBoxTransAx_valueChanged(double arg1);
    void on_spinBoxTransAy_valueChanged(double arg1);
    void on_spinBoxTransAz_valueChanged(double arg1);
    void on_spinBoxAnimTimeMin_valueChanged(double arg1);
    void on_spinBoxAnimTimeMax_valueChanged(double arg1);

    // slots specific for Animation
    void on_listKeyframes_currentRowChanged(int currentRow);
    void on_pushButtonAnimAdd_clicked();
    void on_pushButtonAnimSave_clicked();
    void on_pushButtonAnimRemove_clicked();
};

} // namespace gui
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_GUI_FORMCOMP_H_
