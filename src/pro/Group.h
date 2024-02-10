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
#ifndef MC_SIM_PRO_GROUP_H_
#define MC_SIM_PRO_GROUP_H_

#include <osg/Group>

#include <Result.h>

#include <pro/Component.h>

namespace mc {
namespace pro {

class Group : public Component
{
public:

    using Children = std::vector<std::shared_ptr<Component>>;

    static constexpr char kTagName[] = { "group" };

    Group(osg::Group* group = nullptr);

    inline bool CanBeAnimated() const override { return false; }

    inline bool CanBeParent() const override { return true; }

    std::unique_ptr<Component> Clone() const override;

    inline const char* GetTagName() const override { return kTagName; }

    virtual void SetProjFile(QString proj_file) override;

    virtual void SetAnimationTime(double time) override;

    virtual void SetChildrenAnimationState(bool enabled) override;

    Result Read(const QDomElement* node) override;
    Result Save(QDomDocument* doc, QDomElement* parent) override;

    void Update() override;

    virtual Result AddChild(std::shared_ptr<Component> child);
    std::shared_ptr<Component> GetChild(unsigned int i) { return children_.at(i); }
    inline unsigned int GetChildrenCount() { return children_.size(); }
    Result RemoveChild(std::shared_ptr<Component> child);

protected:

    osg::ref_ptr<osg::Group> group_;

    Children children_;

    virtual void CloneChildren(const Children* children);

    virtual Result ReadChild(QDomElement* node);

    virtual Result SaveChildren(QDomDocument* doc, QDomElement* parent);
    virtual Result SaveParameters(QDomDocument* doc, QDomElement* node) override;
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_GROUP_H_
