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

#include <gui/FormComp.h>
#include <ui_FormComp.h>

#include <QFileDialog>

#include <gui/Utils.h>

#include <pro/File.h>
#include <pro/LOD.h>
#include <pro/PAT.h>
#include <pro/Rotor.h>
#include <pro/Switch.h>
#include <pro/Trans.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

////////////////////////////////////////////////////////////////////////////////

FormComp::FormComp(QWidget* parent) :
    QWidget(parent),
    ui_(new Ui::FormComp)
{
    ui_->setupUi(this);
    hideAllProperties();
}

////////////////////////////////////////////////////////////////////////////////

FormComp::~FormComp()
{
    if ( ui_ ) { delete ui_; } ui_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::setComponent(std::shared_ptr<pro::Component> comp)
{
    comp_ = comp;
    updateParameters(comp);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::setProject(std::shared_ptr<pro::Project> proj)
{
    proj_ = proj;
    comp_ = std::weak_ptr<pro::Component>();
    hideAllProperties();
}

////////////////////////////////////////////////////////////////////////////////

QString FormComp::getFile(QString file)
{
    if ( proj_.expired() ) return QString();

    std::shared_ptr<pro::Project> proj = proj_.lock();
    QDir proj_dir = QFileInfo(proj->GetFile()).absoluteDir();
    QString proj_path = proj_dir.absolutePath();

    QString dir = ".";
    if ( proj_path.length() > 0 )
    {
        dir = proj_path;
    }
    if ( file.length() > 0 )
    {
        QString old_file = proj_dir.absoluteFilePath(file);
        dir = QFileInfo(old_file).path();
    }

    QString caption = "Browse";
    QString filter;
    QString selected_filter;

    filter += "AC3D (*.ac)";
    filter += ";;";
    filter += "3D Studio (*.3ds)";
    filter += ";;";
    filter += "COLLADA (*.dae)";
    filter += ";;";
    filter += selected_filter = "OpenSceneGraph (*.osg *.osga *.osgb *.osgt *.ive)";
    filter += ";;";
    filter += "Wavefront OBJ (*.obj)";
    filter += ";;";
    filter += "STL (*.stl)";

    QString new_file = QFileDialog::getOpenFileName(this, caption, dir, filter, &selected_filter);

    return proj_dir.relativeFilePath(new_file);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::hideAllProperties()
{
    ui_->widgetWrapAnim->hide();
    ui_->widgetWrapComp->hide();
    ui_->widgetWrapFile->hide();
    ui_->widgetWrapLOD->hide();
    ui_->widgetWrapPAT->hide();
    ui_->widgetWrapRotor->hide();
    ui_->widgetWrapSwitch->hide();
    ui_->widgetWrapTrans->hide();
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParameters(std::shared_ptr<pro::Component> comp)
{
    hideAllProperties();

    updateParametersAnim(comp);
    updateParametersComp(comp);
    updateParametersFile(comp);
    updateParametersLOD(comp);
    updateParametersPAT(comp);
    updateParametersRotor(comp);
    updateParametersSwitch(comp);
    updateParametersTrans(comp);
#   ifdef MCSIM_EDITOR_EXTRA_COMPONENTS
#   endif // MCSIM_EDITOR_EXTRA_COMPONENTS
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParametersAnim(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);

    if ( anim )
    {
        ui_->widgetWrapAnim->show();

        ui_->spinBoxAnimTimeMin->setValue(anim->GetAnimation()->GetTimeMin());
        ui_->spinBoxAnimTimeMax->setValue(anim->GetAnimation()->GetTimeMax());

        ui_->listKeyframes->clear();
        pro::Animation::Keyframes keyframes = anim->GetAnimation()->GetKeyframes();
        int index = 0;
        for ( int i = 0; i < keyframes.size(); ++i )
        {
            std::shared_ptr<pro::Keyframe> keyframe = keyframes.at(i);

            QString text;
            text +=   "t="  + QString::number(keyframe->t() ,'f',4);
            text += "; x="  + QString::number(keyframe->px(),'f',4);
            text += "; y="  + QString::number(keyframe->py(),'f',4);
            text += "; z="  + QString::number(keyframe->pz(),'f',4);
            text += "; rx=" + QString::number(keyframe->ax(),'f',4);
            text += "; ry=" + QString::number(keyframe->ay(),'f',4);
            text += "; rz=" + QString::number(keyframe->az(),'f',4);

            QListWidgetItem* item = new QListWidgetItem(ui_->listKeyframes);
            item->setText(text);
            ui_->listKeyframes->insertItem(i, item);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParametersComp(std::shared_ptr<pro::Component> comp)
{
    if ( comp )
    {
        ui_->widgetWrapComp->show();

        Utils::setNoEmitChecked(ui_->checkBoxDepthSortedBin, comp->GetDepthSortedBinState());

        ui_->labelDepthSortedBin->setEnabled(comp->GetDepthSortedBinState());
        ui_->spinBoxDepthSortedBin->setEnabled(comp->GetDepthSortedBinState());

        Utils::setNoEmitValue(ui_->spinBoxDepthSortedBin, comp->GetDepthSortedBinValue());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParametersFile(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::File> file = std::dynamic_pointer_cast<pro::File>(comp);

    if ( file )
    {
        ui_->widgetWrapFile->show();

        Utils::setNoEmitText(ui_->lineEditFilePath, file->GetFile());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParametersLOD(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);

    if ( lod )
    {
        ui_->widgetWrapLOD->show();

        ui_->listIntervals->clear();
        for ( int i = 0; i < lod->GetIntervalsCount(); ++i )
        {
            QListWidgetItem* item = new QListWidgetItem(ui_->listIntervals);
            item->setText(QString::number(lod->GetInterval(i), 'f', 2));
            ui_->listIntervals->insertItem(i, item);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParametersPAT(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);

    if ( pat )
    {
        ui_->widgetWrapPAT->show();

        Utils::setNoEmitChecked(ui_->radioXYZ, pro::PAT::Convention::XYZ == pat->convention());
        Utils::setNoEmitChecked(ui_->radioZYX, pro::PAT::Convention::ZYX == pat->convention());

        Utils::setNoEmitValue(ui_->spinBoxPx, pat->px());
        Utils::setNoEmitValue(ui_->spinBoxPy, pat->py());
        Utils::setNoEmitValue(ui_->spinBoxPz, pat->pz());

        Utils::setNoEmitValue(ui_->spinBoxAx, pat->ax());
        Utils::setNoEmitValue(ui_->spinBoxAy, pat->ay());
        Utils::setNoEmitValue(ui_->spinBoxAz, pat->az());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParametersRotor(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);

    if ( rotor )
    {
        ui_->widgetWrapRotor->show();

        Utils::setNoEmitText(ui_->lineEditBladeFile, rotor->GetFileBlade());
        Utils::setNoEmitText(ui_->lineEditShaftFile, rotor->GetFileShaft());

        Utils::setNoEmitValue(ui_->spinBoxBladesNo, rotor->GetBladesNo());
        Utils::setNoEmitValue(ui_->spinBoxHingeOffset, rotor->GetHingeOffset());

        Utils::setNoEmitChecked(ui_->radioCW  , pro::Rotor::Direction::CW  == rotor->GetDirection());
        Utils::setNoEmitChecked(ui_->radioCCW , pro::Rotor::Direction::CCW == rotor->GetDirection());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParametersSwitch(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::Switch> sw = std::dynamic_pointer_cast<pro::Switch>(comp);

    if ( sw )
    {
        ui_->widgetWrapSwitch->show();

        Utils::setNoEmitChecked(ui_->checkBoxSwitchVisible, sw->GetVisible());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::updateParametersTrans(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);

    if ( trans )
    {
        ui_->widgetWrapTrans->show();

        Utils::setNoEmitValue(ui_->spinBoxTransPx, trans->px());
        Utils::setNoEmitValue(ui_->spinBoxTransPy, trans->py());
        Utils::setNoEmitValue(ui_->spinBoxTransPz, trans->pz());

        Utils::setNoEmitValue(ui_->spinBoxTransAx, trans->ax());
        Utils::setNoEmitValue(ui_->spinBoxTransAy, trans->ay());
        Utils::setNoEmitValue(ui_->spinBoxTransAz, trans->az());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_buttonComp_toggled(bool checked)
{
    Utils::setVisibility(ui_->buttonComp, ui_->widgetComp, checked);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_buttonFile_toggled(bool checked)
{
    Utils::setVisibility(ui_->buttonFile, ui_->widgetFile, checked);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_buttonLOD_toggled(bool checked)
{
    Utils::setVisibility(ui_->buttonLOD, ui_->widgetLOD, checked);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_buttonPAT_toggled(bool checked)
{
    Utils::setVisibility(ui_->buttonPAT, ui_->widgetPAT, checked);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_buttonRotor_toggled(bool checked)
{
    Utils::setVisibility(ui_->buttonRotor, ui_->widgetRotor, checked);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_buttonSwitch_toggled(bool checked)
{
    Utils::setVisibility(ui_->buttonSwitch, ui_->widgetSwitch, checked);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_buttonTrans_toggled(bool checked)
{
    Utils::setVisibility(ui_->buttonTrans, ui_->widgetTrans, checked);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_buttonAnim_toggled(bool checked)
{
    Utils::setVisibility(ui_->buttonAnim, ui_->widgetAnim, checked);
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_checkBoxDepthSortedBin_toggled(bool checked)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    comp->SetDepthSortedBinState(checked);

    ui_->labelDepthSortedBin->setEnabled(checked);
    ui_->spinBoxDepthSortedBin->setEnabled(checked);

    if ( !checked )
    {
        ui_->spinBoxDepthSortedBin->setValue(1);
    }

    emit(projectChanged());
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxDepthSortedBin_valueChanged(int arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    comp->SetDepthSortedBinValue(arg1);
    emit(projectChanged());
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_radioXYZ_toggled(bool checked)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetConvention(checked ? pro::PAT::Convention::XYZ : pro::PAT::Convention::ZYX);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxPx_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetPx(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxPy_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetPy(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxPz_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetPz(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxAx_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetAx(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxAy_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetAy(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxAz_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetAz(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_listIntervals_currentRowChanged(int currentRow)
{
    ui_->pushButtonIntervalSave->setEnabled(false);
    ui_->pushButtonIntervalRemove->setEnabled(false);

    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);
    if ( lod )
    {
        if ( currentRow >= 0 && currentRow < ui_->listIntervals->count() )
        {
            ui_->pushButtonIntervalSave->setEnabled(true);
            ui_->pushButtonIntervalRemove->setEnabled(ui_->listIntervals->count() > 1);

            ui_->spinBoxInterval->setValue(lod->GetInterval(currentRow));
        }
        else
        {
            ui_->spinBoxInterval->setValue(0.0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_pushButtonIntervalAdd_clicked()
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);
    if ( lod )
    {
        lod->AddInterval(ui_->spinBoxInterval->value());
        updateParametersLOD(comp);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_pushButtonIntervalSave_clicked()
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);
    if ( lod )
    {
        lod->EditInterval(ui_->listIntervals->currentRow(), ui_->spinBoxInterval->value());
        updateParametersLOD(comp);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_pushButtonIntervalRemove_clicked()
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);
    if ( lod )
    {
        lod->RemoveInterval(ui_->listIntervals->currentRow());
        updateParametersLOD(comp);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_toolButtonBrowseFiles_clicked()
{
    QString new_file = getFile(ui_->lineEditFilePath->text());
    if ( new_file.length() > 0 )
    {
        ui_->lineEditFilePath->setText(new_file);
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_lineEditFilePath_textChanged(const QString &arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::File> file = std::dynamic_pointer_cast<pro::File>(comp);
    if ( file )
    {
        file->SetFile(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_checkBoxSwitchVisible_toggled(bool checked)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Switch> sw = std::dynamic_pointer_cast<pro::Switch>(comp);
    if ( sw )
    {
        sw->SetVisible(checked);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_toolButtonBrowseBladeFiles_clicked()
{
    QString new_file = getFile(ui_->lineEditBladeFile->text());
    if ( new_file.length() > 0 )
    {
        ui_->lineEditBladeFile->setText(new_file);
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_toolButtonBrowseShaftFiles_clicked()
{
    QString new_file = getFile(ui_->lineEditShaftFile->text());
    if ( new_file.length() > 0 )
    {
        ui_->lineEditShaftFile->setText(new_file);
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_lineEditBladeFile_textChanged(const QString &arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetFileBlade(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_lineEditShaftFile_textChanged(const QString &arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetFileShaft(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxBladesNo_valueChanged(int arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetBladesNo(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxHingeOffset_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetHingeOffset(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_radioButtonCW_toggled(bool checked)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetDirection(checked ? pro::Rotor::Direction::CW : pro::Rotor::Direction::CCW);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxTransPx_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetPx(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxTransPy_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetPy(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxTransPz_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetPz(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxTransAx_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetAx(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxTransAy_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetAy(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxTransAz_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetAz(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxAnimTimeMin_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        anim->GetAnimation()->SetTimeMin(arg1);
        ui_->spinBoxAnimTimeMax->setMinimum(arg1);
        ui_->spinBoxAnimT->setMinimum(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_spinBoxAnimTimeMax_valueChanged(double arg1)
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        anim->GetAnimation()->SetTimeMax(arg1);
        ui_->spinBoxAnimTimeMin->setMaximum(arg1);
        ui_->spinBoxAnimT->setMaximum(arg1);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_listKeyframes_currentRowChanged(int currentRow)
{
    ui_->pushButtonAnimSave->setEnabled(false);
    ui_->pushButtonAnimRemove->setEnabled(false);

    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        if ( currentRow >= 0 && currentRow < anim->GetAnimation()->GetKeyframes().size() )
        {
            ui_->pushButtonAnimSave->setEnabled(true);
            ui_->pushButtonAnimRemove->setEnabled(true);

            std::shared_ptr<pro::Keyframe> keyframe = anim->GetAnimation()->GetKeyframes().at(currentRow);

            ui_->spinBoxAnimT->setValue(keyframe->t());
            ui_->spinBoxAnimT->setMinimum(anim->GetAnimation()->GetTimeMin());
            ui_->spinBoxAnimT->setMaximum(anim->GetAnimation()->GetTimeMax());

            ui_->spinBoxAnimPx->setValue(keyframe->px());
            ui_->spinBoxAnimPy->setValue(keyframe->py());
            ui_->spinBoxAnimPz->setValue(keyframe->pz());

            ui_->spinBoxAnimAx->setValue(keyframe->ax());
            ui_->spinBoxAnimAy->setValue(keyframe->ay());
            ui_->spinBoxAnimAz->setValue(keyframe->az());
        }
        else
        {
            ui_->spinBoxAnimT->setValue(0.0);
            ui_->spinBoxAnimT->setMinimum(anim->GetAnimation()->GetTimeMin());
            ui_->spinBoxAnimT->setMaximum(anim->GetAnimation()->GetTimeMax());

            ui_->spinBoxAnimPx->setValue(0.0);
            ui_->spinBoxAnimPy->setValue(0.0);
            ui_->spinBoxAnimPz->setValue(0.0);

            ui_->spinBoxAnimAx->setValue(0.0);
            ui_->spinBoxAnimAy->setValue(0.0);
            ui_->spinBoxAnimAz->setValue(0.0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_pushButtonAnimAdd_clicked()
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        std::shared_ptr<pro::Keyframe> keyframe = std::make_shared<pro::Keyframe>();

        keyframe->SetT(ui_->spinBoxAnimT->value());

        keyframe->SetPx(ui_->spinBoxAnimPx->value());
        keyframe->SetPy(ui_->spinBoxAnimPy->value());
        keyframe->SetPz(ui_->spinBoxAnimPz->value());

        keyframe->SetAx(ui_->spinBoxAnimAx->value());
        keyframe->SetAy(ui_->spinBoxAnimAy->value());
        keyframe->SetAz(ui_->spinBoxAnimAz->value());

        anim->GetAnimation()->AddKeyframe(keyframe);

        updateParametersAnim(comp);

        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_pushButtonAnimSave_clicked()
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        int index = ui_->listKeyframes->currentRow();
        if ( index >= 0 && index < anim->GetAnimation()->GetKeyframes().size() )
        {
            std::shared_ptr<pro::Keyframe> keyframe = anim->GetAnimation()->GetKeyframes().at(index);

            keyframe->SetT(ui_->spinBoxAnimT->value());

            keyframe->SetPx(ui_->spinBoxAnimPx->value());
            keyframe->SetPy(ui_->spinBoxAnimPy->value());
            keyframe->SetPz(ui_->spinBoxAnimPz->value());

            keyframe->SetAx(ui_->spinBoxAnimAx->value());
            keyframe->SetAy(ui_->spinBoxAnimAy->value());
            keyframe->SetAz(ui_->spinBoxAnimAz->value());

            anim->GetAnimation()->SetKeyframe(index, keyframe);
            updateParametersAnim(comp);
            emit(projectChanged());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void FormComp::on_pushButtonAnimRemove_clicked()
{
    if ( comp_.expired() ) return;

    std::shared_ptr<pro::Component> comp = comp_.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        anim->GetAnimation()->RemoveKeyframe(ui_->listKeyframes->currentRow());
        updateParametersAnim(comp);
        emit(projectChanged());
    }
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gui
} // namespace mc
