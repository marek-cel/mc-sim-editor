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

////////////////////////////////////////////////////////////////////////////////

#include <memory>

#include <osg/Node>
#include <osg/AnimationPath>

#include <QDomElement>

#include <Result.h>
#include <pro/Keyframe.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

class Animation
{
public:

    using Keyframes = std::vector<std::shared_ptr<Keyframe>>;

    static constexpr char kTagName[] = { "animation" };

    Animation(osg::ref_ptr<osg::Node> node);

    /** */
    virtual Result Read(const QDomElement* node);

    /** */
    virtual Result Save(QDomDocument* doc, QDomElement* parent);

    void AddKeyframe(std::shared_ptr<Keyframe> keyframe);

    void RemoveKeyframe(int index);

    void SetKeyframe(int index, std::shared_ptr<Keyframe> keyframe);

    Keyframes GetKeyframes() { return keyframes_; }

    double GetTimeMin() const { return t_min_; }
    double GetTimeMax() const { return t_max_; }

    void SetTimeMin(double t_min);
    void SetTimeMax(double t_max);

    void SetTime(double time);

protected:

    osg::ref_ptr<osg::AnimationPathCallback> apcb_;

    Keyframes keyframes_;

    double t_min_ = 0.0;
    double t_max_ = 1.0;

    osg::ref_ptr<osg::Node> node_;

    Result ReadKeyframe(const QDomElement* node);

    void SortKeyframes();

    void UpdateAnimationPath();
};

} // namespace pro
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_PRO_ANIMATION_H_
