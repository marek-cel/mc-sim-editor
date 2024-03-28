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
#ifndef MC_SIM_PRO_PLAYBACK_H_
#define MC_SIM_PRO_PLAYBACK_H_

#include <QDomElement>

#include <Result.h>

namespace mc {
namespace pro {

class Playback
{
public:

    Result read(const QDomElement* node);

    Result save(QDomDocument* doc, QDomElement* parent);

    inline double time_start() const { return _time_start; }
    inline double time_end() const { return _time_end; }
    inline double current_time() const { return _current_time; }
    inline double speed() const { return _speed; }

    inline void setTimeStart(double time_start) { _time_start = time_start; }
    inline void setTimeEnd(double time_end) { _time_end = time_end; }
    inline void setCurrentTime(double current_time) { _current_time = current_time; }
    inline void setSpeed(double speed) { _speed = speed; }

private:

    double _time_start   = 0.0;
    double _time_end     = 1.0;
    double _current_time = 0.0;
    double _speed        = 1.0;
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_PLAYBACK_H_
