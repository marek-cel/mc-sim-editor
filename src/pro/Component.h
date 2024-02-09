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

////////////////////////////////////////////////////////////////////////////////

#include <memory>

#include <osg/Node>

#include <QDomElement>

#include <Result.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

class Component : public std::enable_shared_from_this<Component>
{
public:

    Component(osg::Node *node);
    Component(const Component& component) = delete;
    Component(Component&&) = delete;

    /** */
    virtual bool CanBeAnimated() const = 0;

    /** */
    virtual bool CanBeParent() const = 0;

    /** */
    virtual std::unique_ptr<Component> Clone() const = 0;

    /** */
    virtual const char* GetTagName() const = 0;

    /** */
    virtual Result Read(const QDomElement* node);

    /** */
    virtual Result Save(QDomDocument* doc, QDomElement* parent);

    /** */
    virtual void Update();

    virtual inline QString GetName() const { return name_; }

    virtual inline std::weak_ptr<Component> GetParent() const { return parent_; }

    virtual void SetName(QString name);

    virtual void SetParent(std::shared_ptr<Component> parent);

    virtual void SetProjFile(QString proj_file) { proj_file_ = proj_file; }

    virtual void SetAnimationTime(double time);

    inline osg::ref_ptr<osg::Node> GetNode() { return node_; }

    inline int  GetDepthSortedBinValue() const { return depth_sorted_bin_value_; }
    inline bool GetDepthSortedBinState() const { return depth_sorted_bin_state_; }

    void SetDepthSortedBinValue(int value);
    void SetDepthSortedBinState(bool state);

    virtual void SetChildrenAnimationState(bool enabled) {}

    Component& operator=(const Component& component) = delete;
    Component& operator=(Component&&) = delete;

protected:

    osg::ref_ptr<osg::Node> node_;  ///< OSG node

    QString name_;

    std::weak_ptr<Component> parent_;

    QString proj_file_;

    int  depth_sorted_bin_value_ = 1;       ///<
    bool depth_sorted_bin_state_ = false;   ///< specifies if transparency is enabled

    virtual Result ReadParameters(const QDomElement* node);
    virtual Result SaveParameters(QDomDocument* doc, QDomElement* node);

    void SetTransparencyMode(osg::Node* node);
};

} // namespace pro
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_PRO_COMPONENT_H_
