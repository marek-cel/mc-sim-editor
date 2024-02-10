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
#ifndef MC_SIM_PRO_PROJECT_H_
#define MC_SIM_PRO_PROJECT_H_

#include <memory>

#include <QString>

#include <Result.h>

#include <pro/Assembly.h>
#include <pro/Playback.h>

namespace mc {
namespace pro {

class Project
{
public:

    /** */
    Result Read(const QString &file);

    /** */
    Result Save(const QString &file);

    inline QString GetFile() const { return file_; }

    inline std::shared_ptr<Assembly> GetAssembly() { return assembly_; }
    inline std::shared_ptr<Playback> GetPlayback() { return playback_; }

    void SetAnimationTime(double time);

private:

    /** CGI model assembly */
    std::shared_ptr<Assembly> assembly_ = std::make_shared<Assembly>();
    std::shared_ptr<Playback> playback_ = std::make_shared<Playback>();

    QString file_ = "";     ///< project file path
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_PROJECT_H_
