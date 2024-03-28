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
#ifndef MC_SIM_PRO_ANIMATION_H_
#define MC_SIM_PRO_ANIMATION_H_

#include <memory>

#include <osg/Node>
#include <osg/AnimationPath>

#include <QDomElement>

#include <Result.h>
#include <pro/Keyframe.h>

namespace mc {
namespace pro {

class Animation
{
public:

    using Keyframes = std::vector<std::shared_ptr<Keyframe>>;

    static constexpr char kTagName[] = { "animation" };

    Animation(osg::ref_ptr<osg::Node> node);

    virtual Result read(const QDomElement* node);
    virtual Result save(QDomDocument* doc, QDomElement* parent);

    void addKeyframe(std::shared_ptr<Keyframe> keyframe);

    void removeKeyframe(int index);

    void setKeyframe(int index, std::shared_ptr<Keyframe> keyframe);

    Keyframes getKeyframes() { return _keyframes; }

    double getTimeMin() const { return _t_min; }
    double getTimeMax() const { return _t_max; }

    void setTimeMin(double t_min);
    void setTimeMax(double t_max);

    void setTime(double time);

protected:

    osg::ref_ptr<osg::AnimationPathCallback> _apcb;

    Keyframes _keyframes;

    double _t_min = 0.0;
    double _t_max = 1.0;

    osg::ref_ptr<osg::Node> _node;

    Result readKeyframe(const QDomElement* node);

    void sortKeyframes();

    void updateAnimationPath();
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_ANIMATION_H_
