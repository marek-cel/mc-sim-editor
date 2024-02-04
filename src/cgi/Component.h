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
#ifndef MC_SIM_EDITOR_CGI_COMPONENT_H_
#define MC_SIM_EDITOR_CGI_COMPONENT_H_

////////////////////////////////////////////////////////////////////////////////

#include <memory>

#include <osg/Group>

#include <defs.h>

#include <cgi/Data.h>

#include <pro/Project.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

/**
 * @brief CGI component base class.
 */
class Component
{
public:

    using List = std::vector<std::shared_ptr<Component>>;

    /**
     * @brief Constructor.
     * @param parent parent module
     */
    Component(std::shared_ptr<Data> data);

    /** @brief Destructor. */
    virtual ~Component();

    /** @brief Adds child to the component. */
    virtual void AddChild(std::shared_ptr<Component> child);

    /** @brief Updates component and all its children. */
    virtual void Update();

    virtual void SetProject(std::shared_ptr<pro::Project> proj);

    /** @brief Returns component root node.  */
    inline osg::ref_ptr<osg::Group> root() { return root_; }

protected:

    std::weak_ptr<Data> data_;              ///< data

    osg::ref_ptr<osg::Group> root_;         ///< OSG module root node
    List children_;                         ///< children nodes

private:

    /** Removes all children. */
    void RemoveAllChildren();
};

} // namespace cgi
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_CGI_COMPONENT_H_
