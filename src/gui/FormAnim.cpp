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

#include <gui/FormAnim.h>
#include <ui_FormAnim.h>

#include <gui/Utils.h>

namespace mc {
namespace gui {

FormAnim::FormAnim(QWidget* parent)
    : QWidget(parent)
    , _ui(new Ui::FormAnim)
{
    _ui->setupUi(this);

    _timer_id = startTimer(1000.0 / 60.0);
    _timer.start();
}

FormAnim::~FormAnim()
{
    if ( _timer_id ) killTimer(_timer_id);

    if ( _ui ) { delete _ui; } _ui = nullptr;
}

void FormAnim::setProject(std::shared_ptr<pro::Project> proj)
{
    _proj = proj;
    updateAnimation();
}

void FormAnim::timerEvent(QTimerEvent* event)
{
    ///////////////////////////
    QWidget::timerEvent(event);
    ///////////////////////////

    // always restart timer to avoid buildup of time step
    double dt = static_cast<double>(_timer.restart()) / 1000.0;

    if ( _ui->pushButtonPlay->isChecked() )
    {
        _currentTime += _ui->spinBoxSpeed->value() * dt;

        if ( _currentTime > _ui->spinBoxEndTime->value() )
        {
            _currentTime -= (_ui->spinBoxEndTime->value() - _ui->spinBoxStartTime->value());
        }

        emit(projectChanged());

        double frac = (_currentTime - _ui->spinBoxStartTime->value())
                    / (_ui->spinBoxEndTime->value() - _ui->spinBoxStartTime->value());
        _ui->sliderTime->setValue(100*frac);
        _ui->spinBoxCurrentTime->setValue(_currentTime);
    }
}

void FormAnim::updateAnimation()
{
    if ( !_proj.expired() )
    {
        std::shared_ptr<pro::Project> proj = _proj.lock();

        _currentTime = proj->GetPlayback()->current_time();

        Utils::setNoEmitValue(_ui->spinBoxStartTime, proj->GetPlayback()->time_start());
        Utils::setNoEmitValue(_ui->spinBoxEndTime, proj->GetPlayback()->time_end());
        Utils::setNoEmitValue(_ui->spinBoxSpeed, proj->GetPlayback()->speed());
        Utils::setNoEmitValue(_ui->spinBoxCurrentTime, _currentTime);

        double frac = (_currentTime - _ui->spinBoxStartTime->value())
                    / (_ui->spinBoxEndTime->value() - _ui->spinBoxStartTime->value());
        Utils::setNoEmitValue(_ui->sliderTime, 100*frac);
    }
}

void FormAnim::on_pushButtonPlay_toggled(bool checked)
{
    if ( checked )
    {
        _ui->pushButtonPlay->setText(tr("Pause"));
    }
    else
    {
        _ui->pushButtonPlay->setText(tr("Play"));
    }
}

void FormAnim::on_sliderTime_valueChanged(int value)
{
    if ( !_ui->pushButtonPlay->isChecked() )
    {
        _currentTime = _ui->spinBoxStartTime->value()
                + (static_cast<double>(value) / 100.0)
                * (_ui->spinBoxEndTime->value() - _ui->spinBoxStartTime->value());
        _ui->spinBoxCurrentTime->setValue(_currentTime);
        if ( !_proj.expired() )
        {
            std::shared_ptr<pro::Project> proj = _proj.lock();
            proj->GetPlayback()->set_current_time(_currentTime);
        }
        emit(projectChanged());
    }
}

void FormAnim::on_spinBoxStartTime_valueChanged(double arg1)
{
    _ui->spinBoxEndTime->setMinimum(arg1);
    if ( !_proj.expired() )
    {
        std::shared_ptr<pro::Project> proj = _proj.lock();
        proj->GetPlayback()->set_time_start(arg1);
    }
    emit(projectChanged());
}

void FormAnim::on_spinBoxEndTime_valueChanged(double arg1)
{
    _ui->spinBoxStartTime->setMaximum(arg1);
    if ( !_proj.expired() )
    {
        std::shared_ptr<pro::Project> proj = _proj.lock();
        proj->GetPlayback()->set_time_end(arg1);
    }
    emit(projectChanged());
}

void FormAnim::on_spinBoxSpeed_valueChanged(double arg1)
{
    if ( !_proj.expired() )
    {
        std::shared_ptr<pro::Project> proj = _proj.lock();
        proj->GetPlayback()->set_speed(arg1);
    }
    emit(projectChanged());
}

} // namespace gui
} // namespace mc
