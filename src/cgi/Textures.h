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
#ifndef MC_SIM_EDITOR_CGI_TEXTURES_H_
#define MC_SIM_EDITOR_CGI_TEXTURES_H_

#include <memory>

#include <osg/Texture2D>

namespace mc {
namespace cgi {

/** Textures. */
class Textures
{
public:

    using List = std::map<std::string, osg::ref_ptr<osg::Texture2D>>;

    static std::shared_ptr<Textures> Instance();

    /** */
    static osg::Texture2D* Get(std::string file, double maxAnisotropy = 1.0);

    static void Reset();

private:

    static std::shared_ptr<Textures> instance_;

    List list_;

    Textures() = default;
    Textures(const Textures&) = delete;
    Textures(Textures&&) = delete;
};

} // namespace cgi
} // namespace mc

#endif // MC_SIM_EDITOR_CGI_TEXTURES_H_
