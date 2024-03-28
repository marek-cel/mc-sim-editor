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

#include <pro/Animation.h>
#include <pro/Group.h>

namespace mc {
namespace pro {

class Animated : public Group
{
public:

    Animated(osg::Group* group);

    inline bool canBeAnimated() const override { return true; }

    Result read(const QDomElement* node) override;

    Result save(QDomDocument* doc, QDomElement* parent) override;

    void update() override;

    std::shared_ptr<Animation> getAnimation() { return _animation; }

    bool getAnimationEnabled() const { return _enabled; }

    virtual void setAnimationTime(double time) override;

    inline void setAnimationEnabled(bool enabled) { _enabled = enabled; }

protected:

    double _time = 0.0;
    bool _enabled = true;

    std::shared_ptr<Animation> _animation;

    Result readAnimation(const QDomElement* node);

    Result saveAnimation(QDomDocument* doc, QDomElement* parent);

    virtual Result readParameters(const QDomElement* node) override;
    virtual Result saveParameters(QDomDocument* doc, QDomElement* node) override;
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_ANIMATED_H_
