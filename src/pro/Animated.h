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
#ifndef MC_SIM_PRO_ANIMATED_H_
#define MC_SIM_PRO_ANIMATED_H_

////////////////////////////////////////////////////////////////////////////////

#include <pro/Animation.h>
#include <pro/Group.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

class Animated : public Group
{
public:

    Animated(osg::Group* group);

    inline bool CanBeAnimated() const override { return true; }

    Result Read(const QDomElement* node) override;

    Result Save(QDomDocument* doc, QDomElement* parent) override;

    void Update() override;

    std::shared_ptr<Animation> GetAnimation() { return animation_; }

    bool GetAnimationEnabled() const { return enabled_; }

    virtual void SetAnimationTime(double time) override;

    inline void SetAnimationEnabled(bool enabled) { enabled_ = enabled; }

protected:

    double time_ = 0.0;

    bool enabled_ = true;

    std::shared_ptr<Animation> animation_;

    Result ReadAnimation(const QDomElement* node);

    Result SaveAnimation(QDomDocument* doc, QDomElement* parent);

    virtual Result ReadParameters(const QDomElement* node) override;
    virtual Result SaveParameters(QDomDocument* doc, QDomElement* node) override;
};

} // namespace pro
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_PRO_ANIMATED_H_
