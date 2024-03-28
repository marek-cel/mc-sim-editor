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

namespace mc {
namespace gui {

FormComp::FormComp(QWidget* parent)
    : QWidget(parent)
    , _ui(new Ui::FormComp)
{
    _ui->setupUi(this);
    hideAllProperties();
}

FormComp::~FormComp()
{
    if ( _ui ) { delete _ui; } _ui = nullptr;
}

void FormComp::setComponent(std::shared_ptr<pro::Component> comp)
{
    _comp = comp;
    updateParameters(comp);
}

void FormComp::setProject(std::shared_ptr<pro::Project> proj)
{
    _proj = proj;
    _comp = std::weak_ptr<pro::Component>();
    hideAllProperties();
}

QString FormComp::getFile(QString file)
{
    if ( _proj.expired() ) return QString();

    std::shared_ptr<pro::Project> proj = _proj.lock();
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

void FormComp::hideAllProperties()
{
    _ui->widgetWrapAnim->hide();
    _ui->widgetWrapComp->hide();
    _ui->widgetWrapFile->hide();
    _ui->widgetWrapLOD->hide();
    _ui->widgetWrapPAT->hide();
    _ui->widgetWrapRotor->hide();
    _ui->widgetWrapSwitch->hide();
    _ui->widgetWrapTrans->hide();
}

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

void FormComp::updateParametersAnim(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);

    if ( anim )
    {
        _ui->widgetWrapAnim->show();

        Utils::setNoEmitChecked(_ui->checkBoxAnimEnabled, anim->GetAnimationEnabled());

        Utils::setNoEmitValue(_ui->spinBoxAnimTimeMin, anim->GetAnimation()->GetTimeMin());
        Utils::setNoEmitValue(_ui->spinBoxAnimTimeMax, anim->GetAnimation()->GetTimeMax());

        _ui->listKeyframes->clear();
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

            QListWidgetItem* item = new QListWidgetItem(_ui->listKeyframes);
            item->setText(text);
            _ui->listKeyframes->insertItem(i, item);
        }
    }
}

void FormComp::updateParametersComp(std::shared_ptr<pro::Component> comp)
{
    if ( comp )
    {
        _ui->widgetWrapComp->show();

        Utils::setNoEmitChecked(_ui->checkBoxDepthSortedBin, comp->GetDepthSortedBinState());

        _ui->labelDepthSortedBin->setEnabled(comp->GetDepthSortedBinState());
        _ui->spinBoxDepthSortedBin->setEnabled(comp->GetDepthSortedBinState());

        Utils::setNoEmitValue(_ui->spinBoxDepthSortedBin, comp->GetDepthSortedBinValue());
    }
}

void FormComp::updateParametersFile(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::File> file = std::dynamic_pointer_cast<pro::File>(comp);

    if ( file )
    {
        _ui->widgetWrapFile->show();

        Utils::setNoEmitText(_ui->lineEditFilePath, file->GetFile());
    }
}

void FormComp::updateParametersLOD(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);

    if ( lod )
    {
        _ui->widgetWrapLOD->show();

        _ui->listIntervals->clear();
        for ( int i = 0; i < lod->GetIntervalsCount(); ++i )
        {
            QListWidgetItem* item = new QListWidgetItem(_ui->listIntervals);
            item->setText(QString::number(lod->GetInterval(i), 'f', 2));
            _ui->listIntervals->insertItem(i, item);
        }
    }
}

void FormComp::updateParametersPAT(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);

    if ( pat )
    {
        _ui->widgetWrapPAT->show();

        Utils::setNoEmitChecked(_ui->radioXYZ, pro::PAT::Convention::XYZ == pat->convention());
        Utils::setNoEmitChecked(_ui->radioZYX, pro::PAT::Convention::ZYX == pat->convention());

        Utils::setNoEmitValue(_ui->spinBoxPx, pat->px());
        Utils::setNoEmitValue(_ui->spinBoxPy, pat->py());
        Utils::setNoEmitValue(_ui->spinBoxPz, pat->pz());

        Utils::setNoEmitValue(_ui->spinBoxAx, pat->ax());
        Utils::setNoEmitValue(_ui->spinBoxAy, pat->ay());
        Utils::setNoEmitValue(_ui->spinBoxAz, pat->az());
    }
}

void FormComp::updateParametersRotor(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);

    if ( rotor )
    {
        _ui->widgetWrapRotor->show();

        Utils::setNoEmitText(_ui->lineEditBladeFile, rotor->GetFileBlade());
        Utils::setNoEmitText(_ui->lineEditShaftFile, rotor->GetFileShaft());

        Utils::setNoEmitValue(_ui->spinBoxBladesNo, rotor->GetBladesNo());
        Utils::setNoEmitValue(_ui->spinBoxHingeOffset, rotor->GetHingeOffset());

        Utils::setNoEmitChecked(_ui->radioCW  , pro::Rotor::Direction::CW  == rotor->GetDirection());
        Utils::setNoEmitChecked(_ui->radioCCW , pro::Rotor::Direction::CCW == rotor->GetDirection());
    }
}

void FormComp::updateParametersSwitch(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::Switch> sw = std::dynamic_pointer_cast<pro::Switch>(comp);

    if ( sw )
    {
        _ui->widgetWrapSwitch->show();

        Utils::setNoEmitChecked(_ui->checkBoxSwitchVisible, sw->GetVisible());
    }
}

void FormComp::updateParametersTrans(std::shared_ptr<pro::Component> comp)
{
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);

    if ( trans )
    {
        _ui->widgetWrapTrans->show();

        Utils::setNoEmitValue(_ui->spinBoxTransPx, trans->px());
        Utils::setNoEmitValue(_ui->spinBoxTransPy, trans->py());
        Utils::setNoEmitValue(_ui->spinBoxTransPz, trans->pz());

        Utils::setNoEmitValue(_ui->spinBoxTransAx, trans->ax());
        Utils::setNoEmitValue(_ui->spinBoxTransAy, trans->ay());
        Utils::setNoEmitValue(_ui->spinBoxTransAz, trans->az());
    }
}

void FormComp::on_buttonComp_toggled(bool checked)
{
    Utils::setVisibility(_ui->buttonComp, _ui->widgetComp, checked);
}

void FormComp::on_buttonFile_toggled(bool checked)
{
    Utils::setVisibility(_ui->buttonFile, _ui->widgetFile, checked);
}

void FormComp::on_buttonLOD_toggled(bool checked)
{
    Utils::setVisibility(_ui->buttonLOD, _ui->widgetLOD, checked);
}

void FormComp::on_buttonPAT_toggled(bool checked)
{
    Utils::setVisibility(_ui->buttonPAT, _ui->widgetPAT, checked);
}

void FormComp::on_buttonRotor_toggled(bool checked)
{
    Utils::setVisibility(_ui->buttonRotor, _ui->widgetRotor, checked);
}

void FormComp::on_buttonSwitch_toggled(bool checked)
{
    Utils::setVisibility(_ui->buttonSwitch, _ui->widgetSwitch, checked);
}

void FormComp::on_buttonTrans_toggled(bool checked)
{
    Utils::setVisibility(_ui->buttonTrans, _ui->widgetTrans, checked);
}

void FormComp::on_buttonAnim_toggled(bool checked)
{
    Utils::setVisibility(_ui->buttonAnim, _ui->widgetAnim, checked);
}

void FormComp::on_checkBoxDepthSortedBin_toggled(bool checked)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    comp->SetDepthSortedBinState(checked);

    _ui->labelDepthSortedBin->setEnabled(checked);
    _ui->spinBoxDepthSortedBin->setEnabled(checked);

    if ( !checked )
    {
        _ui->spinBoxDepthSortedBin->setValue(1);
    }

    emit(projectChanged());
}

void FormComp::on_spinBoxDepthSortedBin_valueChanged(int arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    comp->SetDepthSortedBinValue(arg1);
    emit(projectChanged());
}

void FormComp::on_radioXYZ_toggled(bool checked)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetConvention(checked ? pro::PAT::Convention::XYZ : pro::PAT::Convention::ZYX);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxPx_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetPx(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxPy_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetPy(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxPz_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetPz(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxAx_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetAx(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxAy_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetAy(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxAz_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::PAT> pat = std::dynamic_pointer_cast<pro::PAT>(comp);
    if ( pat )
    {
        pat->SetAz(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_listIntervals_currentRowChanged(int currentRow)
{
    _ui->pushButtonIntervalSave->setEnabled(false);
    _ui->pushButtonIntervalRemove->setEnabled(false);

    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);
    if ( lod )
    {
        if ( currentRow >= 0 && currentRow < _ui->listIntervals->count() )
        {
            _ui->pushButtonIntervalSave->setEnabled(true);
            _ui->pushButtonIntervalRemove->setEnabled(_ui->listIntervals->count() > 1);

            _ui->spinBoxInterval->setValue(lod->GetInterval(currentRow));
        }
        else
        {
            _ui->spinBoxInterval->setValue(0.0);
        }
    }
}

void FormComp::on_pushButtonIntervalAdd_clicked()
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);
    if ( lod )
    {
        lod->AddInterval(_ui->spinBoxInterval->value());
        updateParametersLOD(comp);
        emit(projectChanged());
    }
}

void FormComp::on_pushButtonIntervalSave_clicked()
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);
    if ( lod )
    {
        lod->EditInterval(_ui->listIntervals->currentRow(), _ui->spinBoxInterval->value());
        updateParametersLOD(comp);
        emit(projectChanged());
    }
}

void FormComp::on_pushButtonIntervalRemove_clicked()
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::LOD> lod = std::dynamic_pointer_cast<pro::LOD>(comp);
    if ( lod )
    {
        lod->RemoveInterval(_ui->listIntervals->currentRow());
        updateParametersLOD(comp);
        emit(projectChanged());
    }
}

void FormComp::on_toolButtonBrowseFiles_clicked()
{
    QString new_file = getFile(_ui->lineEditFilePath->text());
    if ( new_file.length() > 0 )
    {
        _ui->lineEditFilePath->setText(new_file);
    }
}

void FormComp::on_lineEditFilePath_textChanged(const QString &arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::File> file = std::dynamic_pointer_cast<pro::File>(comp);
    if ( file )
    {
        file->SetFile(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_checkBoxSwitchVisible_toggled(bool checked)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Switch> sw = std::dynamic_pointer_cast<pro::Switch>(comp);
    if ( sw )
    {
        sw->SetVisible(checked);
        emit(projectChanged());
    }
}

void FormComp::on_toolButtonBrowseBladeFiles_clicked()
{
    QString new_file = getFile(_ui->lineEditBladeFile->text());
    if ( new_file.length() > 0 )
    {
        _ui->lineEditBladeFile->setText(new_file);
    }
}

void FormComp::on_toolButtonBrowseShaftFiles_clicked()
{
    QString new_file = getFile(_ui->lineEditShaftFile->text());
    if ( new_file.length() > 0 )
    {
        _ui->lineEditShaftFile->setText(new_file);
    }
}

void FormComp::on_lineEditBladeFile_textChanged(const QString &arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetFileBlade(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_lineEditShaftFile_textChanged(const QString &arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetFileShaft(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxBladesNo_valueChanged(int arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetBladesNo(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxHingeOffset_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetHingeOffset(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_radioButtonCW_toggled(bool checked)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Rotor> rotor = std::dynamic_pointer_cast<pro::Rotor>(comp);
    if ( rotor )
    {
        rotor->SetDirection(checked ? pro::Rotor::Direction::CW : pro::Rotor::Direction::CCW);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxTransPx_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetPx(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxTransPy_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetPy(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxTransPz_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetPz(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxTransAx_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetAx(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxTransAy_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetAy(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxTransAz_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Trans> trans = std::dynamic_pointer_cast<pro::Trans>(comp);
    if ( trans )
    {
        trans->SetAz(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_checkBoxAnimEnabled_toggled(bool checked)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        anim->SetAnimationEnabled(checked);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxAnimTimeMin_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        anim->GetAnimation()->SetTimeMin(arg1);
        _ui->spinBoxAnimTimeMax->setMinimum(arg1);
        _ui->spinBoxAnimT->setMinimum(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_spinBoxAnimTimeMax_valueChanged(double arg1)
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        anim->GetAnimation()->SetTimeMax(arg1);
        _ui->spinBoxAnimTimeMin->setMaximum(arg1);
        _ui->spinBoxAnimT->setMaximum(arg1);
        emit(projectChanged());
    }
}

void FormComp::on_listKeyframes_currentRowChanged(int currentRow)
{
    _ui->pushButtonAnimSave->setEnabled(false);
    _ui->pushButtonAnimRemove->setEnabled(false);

    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        if ( currentRow >= 0 && currentRow < anim->GetAnimation()->GetKeyframes().size() )
        {
            _ui->pushButtonAnimSave->setEnabled(true);
            _ui->pushButtonAnimRemove->setEnabled(true);

            std::shared_ptr<pro::Keyframe> keyframe = anim->GetAnimation()->GetKeyframes().at(currentRow);

            _ui->spinBoxAnimT->setValue(keyframe->t());
            _ui->spinBoxAnimT->setMinimum(anim->GetAnimation()->GetTimeMin());
            _ui->spinBoxAnimT->setMaximum(anim->GetAnimation()->GetTimeMax());

            _ui->spinBoxAnimPx->setValue(keyframe->px());
            _ui->spinBoxAnimPy->setValue(keyframe->py());
            _ui->spinBoxAnimPz->setValue(keyframe->pz());

            _ui->spinBoxAnimAx->setValue(keyframe->ax());
            _ui->spinBoxAnimAy->setValue(keyframe->ay());
            _ui->spinBoxAnimAz->setValue(keyframe->az());
        }
        else
        {
            _ui->spinBoxAnimT->setValue(0.0);
            _ui->spinBoxAnimT->setMinimum(anim->GetAnimation()->GetTimeMin());
            _ui->spinBoxAnimT->setMaximum(anim->GetAnimation()->GetTimeMax());

            _ui->spinBoxAnimPx->setValue(0.0);
            _ui->spinBoxAnimPy->setValue(0.0);
            _ui->spinBoxAnimPz->setValue(0.0);

            _ui->spinBoxAnimAx->setValue(0.0);
            _ui->spinBoxAnimAy->setValue(0.0);
            _ui->spinBoxAnimAz->setValue(0.0);
        }
    }
}

void FormComp::on_pushButtonAnimAdd_clicked()
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        std::shared_ptr<pro::Keyframe> keyframe = std::make_shared<pro::Keyframe>();

        keyframe->SetT(_ui->spinBoxAnimT->value());

        keyframe->SetPx(_ui->spinBoxAnimPx->value());
        keyframe->SetPy(_ui->spinBoxAnimPy->value());
        keyframe->SetPz(_ui->spinBoxAnimPz->value());

        keyframe->SetAx(_ui->spinBoxAnimAx->value());
        keyframe->SetAy(_ui->spinBoxAnimAy->value());
        keyframe->SetAz(_ui->spinBoxAnimAz->value());

        anim->GetAnimation()->AddKeyframe(keyframe);

        updateParametersAnim(comp);

        emit(projectChanged());
    }
}

void FormComp::on_pushButtonAnimSave_clicked()
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        int index = _ui->listKeyframes->currentRow();
        if ( index >= 0 && index < anim->GetAnimation()->GetKeyframes().size() )
        {
            std::shared_ptr<pro::Keyframe> keyframe = anim->GetAnimation()->GetKeyframes().at(index);

            keyframe->SetT(_ui->spinBoxAnimT->value());

            keyframe->SetPx(_ui->spinBoxAnimPx->value());
            keyframe->SetPy(_ui->spinBoxAnimPy->value());
            keyframe->SetPz(_ui->spinBoxAnimPz->value());

            keyframe->SetAx(_ui->spinBoxAnimAx->value());
            keyframe->SetAy(_ui->spinBoxAnimAy->value());
            keyframe->SetAz(_ui->spinBoxAnimAz->value());

            anim->GetAnimation()->SetKeyframe(index, keyframe);
            updateParametersAnim(comp);
            emit(projectChanged());
        }
    }
}

void FormComp::on_pushButtonAnimRemove_clicked()
{
    if ( _comp.expired() ) return;

    std::shared_ptr<pro::Component> comp = _comp.lock();
    std::shared_ptr<pro::Animated> anim = std::dynamic_pointer_cast<pro::Animated>(comp);
    if ( anim )
    {
        anim->GetAnimation()->RemoveKeyframe(_ui->listKeyframes->currentRow());
        updateParametersAnim(comp);
        emit(projectChanged());
    }
}

} // namespace gui
} // namespace mc
