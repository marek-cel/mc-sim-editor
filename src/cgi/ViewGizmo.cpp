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

#include <cgi/ViewGizmo.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>

#include <cgi/Colors.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

////////////////////////////////////////////////////////////////////////////////

ViewGizmo::ViewGizmo(std::shared_ptr<Data> data)
    : Component(data)
{
    osg::ref_ptr<osg::StateSet> rootStateSet = root_->getOrCreateStateSet();
    rootStateSet->setMode(GL_LIGHT0   , osg::StateAttribute::OFF);
    rootStateSet->setMode(GL_LIGHT1   , osg::StateAttribute::OFF);
    rootStateSet->setMode(GL_LIGHTING , osg::StateAttribute::OFF);

    CreateGizmo();
}

////////////////////////////////////////////////////////////////////////////////

void ViewGizmo::Update()
{
    ////////////////////
    Component::Update();
    ////////////////////

    if ( !data_.expired() )
    {
        std::shared_ptr<Data> data = data_.lock();

        osg::Quat q(data->camera_x,
                    data->camera_y,
                    data->camera_z,
                    data->camera_w);

        double w2h = static_cast<double>(data->win_width)
                   / static_cast<double>(data->win_height);

        double x = -w2h * CGI_HUD_Y_2 + offset_x_;
        double y = -CGI_HUD_Y_2 + offset_y_;

        q = q.inverse();

        pat_->setAttitude(q);
        pat_->setPosition(osg::Vec3d(x, y, 0.0));
    }
}

////////////////////////////////////////////////////////////////////////////////

void ViewGizmo::CreateGizmo()
{
    constexpr double len = 10.0;

    pat_ = new osg::PositionAttitudeTransform();
    root_->addChild(pat_.get());

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    pat_->addChild(geode.get());

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    v->push_back(osg::Vec3d(0.0, 0.0, 0.0));
    v->push_back(osg::Vec3d(len, 0.0, 0.0));
    c->push_back(osg::Vec4(Colors::red, 1.0));
    c->push_back(osg::Vec4(Colors::red, 1.0));

    v->push_back(osg::Vec3d(0.0, 0.0, 0.0));
    v->push_back(osg::Vec3d(0.0, len, 0.0));
    c->push_back(osg::Vec4(Colors::lime, 1.0));
    c->push_back(osg::Vec4(Colors::lime, 1.0));

    v->push_back(osg::Vec3d(0.0, 0.0, 0.0));
    v->push_back(osg::Vec3d(0.0, 0.0, len));
    c->push_back(osg::Vec4(Colors::blue, 1.0));
    c->push_back(osg::Vec4(Colors::blue, 1.0));

    n->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

    geometry->setVertexArray(v.get());
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, v->size()));
    geometry->setNormalArray(n.get());
    geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
    geometry->setColorArray(c.get());
    geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    geode->addDrawable(geometry.get());

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth(2.0f);

    geode->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace cgi
} // namespace mc
