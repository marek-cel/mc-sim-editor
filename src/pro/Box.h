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
#ifndef MC_SIM_PRO_BOX_H_
#define MC_SIM_PRO_BOX_H_

#include <osg/Geode>

#include <Result.h>

#include <pro/Group.h>

namespace mc {
namespace pro {

class Box : public Group
{
public:

    static constexpr char kTagName[] = { "box" };

    Box();

    inline bool CanBeAnimated() const override { return false; }

    inline bool CanBeParent() const override { return false; }

    std::unique_ptr<Component> Clone() const override;

    inline const char* GetTagName() const override { return kTagName; }

protected:

    osg::ref_ptr<osg::Geode> geode_;
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_BOX_H_
