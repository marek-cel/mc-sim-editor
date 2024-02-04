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

#include <cgi/Geometry.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi { namespace Geometry
{

////////////////////////////////////////////////////////////////////////////////

void CreateLine(osg::Geometry* geom, const osg::Vec3& b, const osg::Vec3& e,
                const osg::Vec4& color)
{
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    v->push_back(b);
    v->push_back(e);

    n->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
    c->push_back(color);

    geom->setVertexArray(v.get());
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, v->size()));
    geom->setNormalArray(n.get());
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    geom->setColorArray(c.get());
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace Geometry
} // namespace cgi
} // namespace mc
