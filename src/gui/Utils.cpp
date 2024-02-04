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

#include <gui/Utils.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui { namespace Utils
{

////////////////////////////////////////////////////////////////////////////////

void setNoEmitChecked(QCheckBox* checkBox, bool checked)
{
    checkBox->blockSignals(true);
    checkBox->setChecked(checked);
    checkBox->blockSignals(false);
}

////////////////////////////////////////////////////////////////////////////////

void setNoEmitChecked(QRadioButton* button, bool checked)
{
    button->blockSignals(true);
    button->setChecked(checked);
    button->blockSignals(false);
}

////////////////////////////////////////////////////////////////////////////////

void setNoEmitText(QLineEdit* lineEdit, QString text)
{
    lineEdit->blockSignals(true);
    lineEdit->setText(text);
    lineEdit->blockSignals(false);
}

////////////////////////////////////////////////////////////////////////////////

void setNoEmitValue(QDoubleSpinBox* spinBox, double value)
{
    spinBox->blockSignals(true);
    spinBox->setValue(value);
    spinBox->blockSignals(false);
}

////////////////////////////////////////////////////////////////////////////////

void setNoEmitValue(QSpinBox* spinBox, int value)
{
    spinBox->blockSignals(true);
    spinBox->setValue(value);
    spinBox->blockSignals(false);
}

////////////////////////////////////////////////////////////////////////////////

void setVisibility(QToolButton* button, QWidget* widget, bool visibility)
{
    button->setArrowType(!visibility ? Qt::ArrowType::RightArrow : Qt::ArrowType::DownArrow);
    widget->setHidden(!visibility);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace Utils
} // namespace gui
} // namespace mc
