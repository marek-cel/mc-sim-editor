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
#ifndef MC_SIM_PRO_ASSEMBLY_H_
#define MC_SIM_PRO_ASSEMBLY_H_

#include <osg/Node>

#include <QDomElement>
#include <QString>

#include <Result.h>

#include <pro/Group.h>

namespace mc {
namespace pro {

class Assembly
{
public:

    /** */
    Result Export(QString file);

    osg::ref_ptr<osg::Node> GetScene() const;

    Result Read(const QDomElement* node);

    Result Save(QDomDocument* doc, QDomElement* parent);

    void SetProjFile(QString proj_file);

    inline std::shared_ptr<Group> GetRoot() { return root_; }

    void SetAnimationTime(double time);

private:

    std::shared_ptr<Group> root_ = std::make_shared<Group>();

    QString proj_file_;
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_ASSEMBLY_H_
