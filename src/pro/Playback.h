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

////////////////////////////////////////////////////////////////////////////////

#include <QDomElement>

#include <Result.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

////////////////////////////////////////////////////////////////////////////////

class Playback
{
public:

    Result Read(const QDomElement* node);

    Result Save(QDomDocument* doc, QDomElement* parent);

    inline double time_start() const { return time_start_; }
    inline double time_end() const { return time_end_; }
    inline double current_time() const { return current_time_; }
    inline double speed() const { return speed_; }

    inline void set_time_start(double time_start) { time_start_ = time_start; }
    inline void set_time_end(double time_end) { time_end_ = time_end; }
    inline void set_current_time(double current_time) { current_time_ = current_time; }
    inline void set_speed(double speed) { speed_ = speed; }

private:

    double time_start_   = 0.0;
    double time_end_     = 1.0;
    double current_time_ = 0.0;
    double speed_        = 1.0;
};

////////////////////////////////////////////////////////////////////////////////

} // namespace pro
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_PRO_PLAYBACK_H_
