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
#ifndef MC_SIM_EDITOR_GUI_UTILS_H_
#define MC_SIM_EDITOR_GUI_UTILS_H_

////////////////////////////////////////////////////////////////////////////////

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QSpinBox>
#include <QToolButton>
#include <QWidget>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui { namespace Utils
{

void setNoEmitChecked(QCheckBox* checkBox, bool checked);

void setNoEmitChecked(QRadioButton* button, bool checked);

void setNoEmitText(QLineEdit* lineEdit, QString text);

void setNoEmitValue(QDoubleSpinBox* spinBox, double value);

void setNoEmitValue(QSpinBox* spinBox, int value);

void setVisibility(QToolButton* button, QWidget* widget, bool visibility);

} // namespace Utils
} // namespace gui
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_GUI_UTILS_H_
