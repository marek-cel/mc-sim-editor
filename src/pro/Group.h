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

    inline bool canBeAnimated() const override { return false; }

    inline bool canBeParent() const override { return true; }

    std::shared_ptr<Component> clone() const override;

    inline const char* getTagName() const override { return kTagName; }

    virtual void setProjFile(QString proj_file) override;

    virtual void setAnimationTime(double time) override;

    virtual void setChildrenAnimationState(bool enabled) override;

    Result read(const QDomElement* node) override;
    Result save(QDomDocument* doc, QDomElement* parent) override;

    void update() override;

    virtual Result addChild(std::shared_ptr<Component> child);
    std::shared_ptr<Component> getChild(unsigned int i) { return _children.at(i); }
    inline unsigned int getChildrenCount() { return _children.size(); }
    Result removeChild(std::shared_ptr<Component> child);

protected:

    osg::ref_ptr<osg::Group> _group;
    Children _children;

    virtual void cloneChildren(const Children* children);

    virtual Result readChild(QDomElement* node);

    virtual Result saveChildren(QDomDocument* doc, QDomElement* parent);
    virtual Result saveParameters(QDomDocument* doc, QDomElement* node) override;
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_GROUP_H_
