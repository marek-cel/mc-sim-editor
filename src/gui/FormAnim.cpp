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

namespace mc {
namespace gui {

FormAnim::FormAnim(QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::FormAnim)
{
    ui_->setupUi(this);

    timer_id_ = startTimer(1000.0 / 60.0);
    _timer.start();
}

FormAnim::~FormAnim()
{
    if ( timer_id_ ) killTimer(timer_id_);

    if ( ui_ ) { delete ui_; } ui_ = nullptr;
}

void FormAnim::setProject(std::shared_ptr<pro::Project> proj)
{
    proj_ = proj;
    updateAnimation();
}

void FormAnim::timerEvent(QTimerEvent* event)
{
    ///////////////////////////
    QWidget::timerEvent(event);
    ///////////////////////////

    // always restart timer to avoid buildup of time step
    double dt = static_cast<double>(_timer.restart()) / 1000.0;

    if ( ui_->pushButtonPlay->isChecked() )
    {
        currentTime_ += ui_->spinBoxSpeed->value() * dt;

        if ( currentTime_ > ui_->spinBoxEndTime->value() )
        {
            currentTime_ -= (ui_->spinBoxEndTime->value() - ui_->spinBoxStartTime->value());
        }

        emit(projectChanged());

        double frac = (currentTime_ - ui_->spinBoxStartTime->value())
                    / (ui_->spinBoxEndTime->value() - ui_->spinBoxStartTime->value());
        ui_->sliderTime->setValue(100*frac);
        ui_->spinBoxCurrentTime->setValue(currentTime_);
    }
}

void FormAnim::updateAnimation()
{
    if ( !proj_.expired() )
    {
        std::shared_ptr<pro::Project> proj = proj_.lock();

        currentTime_ = proj->GetPlayback()->current_time();

        ui_->spinBoxStartTime->setValue(proj->GetPlayback()->time_start());
        ui_->spinBoxEndTime->setValue(proj->GetPlayback()->time_end());
        ui_->spinBoxSpeed->setValue(proj->GetPlayback()->speed());
        ui_->spinBoxCurrentTime->setValue(currentTime_);

        double frac = (currentTime_ - ui_->spinBoxStartTime->value())
                    / (ui_->spinBoxEndTime->value() - ui_->spinBoxStartTime->value());
        ui_->sliderTime->setValue(100*frac);
    }
}

void FormAnim::on_pushButtonPlay_toggled(bool checked)
{
    if ( checked )
    {
        ui_->pushButtonPlay->setText(tr("Pause"));
    }
    else
    {
        ui_->pushButtonPlay->setText(tr("Play"));
    }
}

void FormAnim::on_sliderTime_valueChanged(int value)
{
    if ( !ui_->pushButtonPlay->isChecked() )
    {
        currentTime_ = ui_->spinBoxStartTime->value()
                + (static_cast<double>(value) / 100.0)
                * (ui_->spinBoxEndTime->value() - ui_->spinBoxStartTime->value());
        ui_->spinBoxCurrentTime->setValue(currentTime_);
        if ( !proj_.expired() )
        {
            std::shared_ptr<pro::Project> proj = proj_.lock();
            proj->GetPlayback()->set_current_time(currentTime_);
        }
        emit(projectChanged());
    }
}

void FormAnim::on_spinBoxStartTime_valueChanged(double arg1)
{
    ui_->spinBoxEndTime->setMinimum(arg1);
    if ( !proj_.expired() )
    {
        std::shared_ptr<pro::Project> proj = proj_.lock();
        proj->GetPlayback()->set_time_start(arg1);
    }
    emit(projectChanged());
}

void FormAnim::on_spinBoxEndTime_valueChanged(double arg1)
{
    ui_->spinBoxStartTime->setMaximum(arg1);
    if ( !proj_.expired() )
    {
        std::shared_ptr<pro::Project> proj = proj_.lock();
        proj->GetPlayback()->set_time_end(arg1);
    }
    emit(projectChanged());
}

void FormAnim::on_spinBoxSpeed_valueChanged(double arg1)
{
    if ( !proj_.expired() )
    {
        std::shared_ptr<pro::Project> proj = proj_.lock();
        proj->GetPlayback()->set_speed(arg1);
    }
    emit(projectChanged());
}

} // namespace gui
} // namespace mc
