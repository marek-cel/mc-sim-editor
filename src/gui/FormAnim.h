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
#ifndef MC_SIM_EDITOR_GUI_FORMANIM_H_
#define MC_SIM_EDITOR_GUI_FORMANIM_H_

////////////////////////////////////////////////////////////////////////////////

#include <QWidget>
#include <QElapsedTimer>

#include <pro/Project.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui {
class FormAnim;
} // namespace Ui

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

class FormAnim : public QWidget
{
    Q_OBJECT

public:

    explicit FormAnim(QWidget* parent = nullptr);
    ~FormAnim();

    inline double getCurrentTime() const { return currentTime_; }

public slots:

    void setProject(std::shared_ptr<pro::Project> proj);

signals:

    void projectChanged();

protected:

    void timerEvent(QTimerEvent* event) override;

private:

    Ui::FormAnim* ui_ = nullptr;

    QElapsedTimer _timer;

    std::weak_ptr<pro::Project> proj_;

    int timer_id_ = 0;

    double currentTime_ = 0.0;

    void updateAnimation();

private slots:

    void on_pushButtonPlay_toggled(bool checked);
    void on_sliderTime_valueChanged(int value);
    void on_spinBoxStartTime_valueChanged(double arg1);
    void on_spinBoxEndTime_valueChanged(double arg1);
    void on_spinBoxSpeed_valueChanged(double arg1);
};

} // namespace gui
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_GUI_FORMANIM_H_
