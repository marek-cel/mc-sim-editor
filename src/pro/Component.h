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
#ifndef MC_SIM_PRO_COMPONENT_H_
#define MC_SIM_PRO_COMPONENT_H_

#include <memory>

#include <osg/Node>

#include <QDomElement>

#include <Result.h>

namespace mc {
namespace pro {

class Component : public std::enable_shared_from_this<Component>
{
public:

    Component(osg::Node *node);
    Component(const Component& component) = delete;
    Component(Component&&) = delete;

    virtual bool canBeAnimated() const = 0;
    virtual bool canBeParent() const = 0;
    virtual std::unique_ptr<Component> clone() const = 0;
    virtual const char* getTagName() const = 0;

    virtual Result read(const QDomElement* node);
    virtual Result save(QDomDocument* doc, QDomElement* parent);
    virtual void update();
    virtual bool isRoot() const;
    virtual inline QString getName() const { return _name; }
    virtual inline std::weak_ptr<Component> getParent() const { return _parent; }
    virtual void setName(QString name);
    virtual void setParent(std::shared_ptr<Component> parent);
    virtual void setProjFile(QString proj_file) { _proj_file = proj_file; }
    virtual void setAnimationTime(double time);

    inline osg::ref_ptr<osg::Node> getNode() { return _node; }

    inline int  getDepthSortedBinValue() const { return _depth_sorted_bin_value; }
    inline bool getDepthSortedBinState() const { return _depth_sorted_bin_state; }

    void setDepthSortedBinValue(int value);
    void setDepthSortedBinState(bool state);

    virtual void setChildrenAnimationState(bool enabled) {}

    Component& operator=(const Component& component) = delete;
    Component& operator=(Component&&) = delete;

protected:

    osg::ref_ptr<osg::Node> _node;

    QString _name;

    std::weak_ptr<Component> _parent;

    QString _proj_file;

    int  _depth_sorted_bin_value = 1;       ///<
    bool _depth_sorted_bin_state = false;   ///< specifies if transparency is enabled

    virtual Result readParameters(const QDomElement* node);
    virtual Result saveParameters(QDomDocument* doc, QDomElement* node);

    void setTransparencyMode(osg::Node* node);
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_COMPONENT_H_
