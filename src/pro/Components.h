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
#ifndef MC_SIM_PRO_COMPONENTS_H_
#define MC_SIM_PRO_COMPONENTS_H_

#include <memory>
#include <vector>

#include <QString>

#include <pro/Component.h>

namespace mc {
namespace pro {

class Components
{
public:

    struct Type
    {
        QString name;
        std::shared_ptr<pro::Component> component;
        bool can_be_animated = false;
        bool can_be_parent = false;
        bool extra = false;
    };

    using Types = std::vector<Type>;


    static std::shared_ptr<Components> Instance();

    void AddType(QString name, std::shared_ptr<pro::Component> comp,
                 bool extra = false);
    void AddTypes();

    Type GetComponentByTagName(QString tag_name);

    int GetIndexByTagName(QString tag_name);

    inline std::vector<Type> types() const { return types_; }

private:

    static std::shared_ptr<Components> instance_;

    std::vector<Type> types_;

    Components() = default;
    Components(const Components&) = delete;
    Components(Components&&) = delete;
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_COMPONENTS_H_
