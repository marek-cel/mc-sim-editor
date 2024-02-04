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

#include <cgi/Grid.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>

#include <cgi/Colors.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

////////////////////////////////////////////////////////////////////////////////

Grid::Grid(std::shared_ptr<Data> data)
    : Component(data)
{
    osg::ref_ptr<osg::StateSet> rootStateSet = root_->getOrCreateStateSet();
    rootStateSet->setMode(GL_LIGHT0   , osg::StateAttribute::OFF);
    rootStateSet->setMode(GL_LIGHT1   , osg::StateAttribute::OFF);
    rootStateSet->setMode(GL_LIGHTING , osg::StateAttribute::OFF);

    switch_ = new osg::Switch();
    root_->addChild(switch_.get());

    CreateGrid();
}

////////////////////////////////////////////////////////////////////////////////

void Grid::Update()
{
    ////////////////////
    Component::Update();
    ////////////////////

    if ( !data_.expired() )
    {
        std::shared_ptr<Data> data = data_.lock();

        if ( data->grid_visible != grid_visible_ )
        {
            if ( data->grid_visible )
            {
                switch_->setAllChildrenOn();
            }
            else
            {
                switch_->setAllChildrenOff();
            }
        }

        grid_visible_ = data->grid_visible;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Grid::CreateGrid()
{
    CreateGridAuxLines();
    CreateGridMainLines();
}

////////////////////////////////////////////////////////////////////////////////

void Grid::CreateGridAuxLines()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    switch_->addChild(geode.get());

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::Vec4 color(0.0, 0.0, 0.0, 0.2);
    //osg::Vec4 color(1.0, 1.0, 1.0, 0.2);

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    for ( int i = -size_; i <= size_; ++i )
    {
        if ( i != 0 )
        {
            osg::Vec3d lat_b(-size_, i * step_, 0.0);
            osg::Vec3d lat_e( size_, i * step_, 0.0);

            v->push_back(lat_b);
            v->push_back(lat_e);

            osg::Vec3d lon_b(i * step_, -size_, 0.0);
            osg::Vec3d lon_e(i * step_,  size_, 0.0);

            v->push_back(lon_b);
            v->push_back(lon_e);
        }
    }

    n->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
    c->push_back(color);

    geometry->setVertexArray(v.get());
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, v->size()));
    geometry->setNormalArray(n.get());
    geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
    geometry->setColorArray(c.get());
    geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    geode->addDrawable(geometry.get());

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth(1.0f);

    geode->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
}

////////////////////////////////////////////////////////////////////////////////

void Grid::CreateGridMainLines()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    switch_->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    osg::Vec3d lon_b( 0.0, -size_, 0.0 );
    osg::Vec3d lon_e( 0.0,  size_, 0.0 );

    v->push_back( lon_b );
    v->push_back( lon_e );
    c->push_back( osg::Vec4( Colors::lime, 1.0 ) );
    c->push_back( osg::Vec4( Colors::lime, 1.0 ) );

    osg::Vec3d lat_b( -size_, 0.0, 0.0 );
    osg::Vec3d lat_e(  size_, 0.0, 0.0 );
    c->push_back( osg::Vec4( Colors::red, 1.0 ) );
    c->push_back( osg::Vec4( Colors::red, 1.0 ) );

    v->push_back( lat_b );
    v->push_back( lat_e );

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );
    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 1.0 );

    geode->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

} // namespace cgi
} // namespace mc
