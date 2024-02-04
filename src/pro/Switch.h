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
#ifndef MC_SIM_PRO_SWITCH_H_
#define MC_SIM_PRO_SWITCH_H_

////////////////////////////////////////////////////////////////////////////////

#include <osg/Switch>

#include <pro/Group.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

class Switch : public Group
{
public:

    static constexpr char kTagName[] = { "switch" };

    Switch(osg::Switch* sw = nullptr);

    std::unique_ptr<Component> Clone() const override;

    inline const char* GetTagName() const override { return kTagName; }

    inline bool GetVisible() const { return visible_; }

    void SetVisible(bool visible);

protected:

    osg::ref_ptr<osg::Switch> switch_;

    bool visible_ = true;

    virtual Result ReadParameters(const QDomElement* node) override;
    virtual Result SaveParameters(QDomDocument* doc, QDomElement* node) override;
};

} // namespace pro
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_PRO_SWITCH_H_
