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

#include <cgi/Intersections.h>

#include <iostream>

#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/View>

namespace mc {
namespace cgi {

Intersections::Intersections(osg::ref_ptr<osg::Node> assembly)
    : _assembly(assembly)
{}

bool Intersections::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    if ( ea.getEventType() == osgGA::GUIEventAdapter::PUSH
         &&
         ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON
         &&
         (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL) )
    {
        osg::Vec3d v_win = getIntersection(ea.getX(), ea.getY());
        osg::Vec3d v = v_win;

        osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&us);
        if ( viewer )
        {
            osg::Camera* camera = viewer->getCamera();
            if ( camera )
            {
                osg::Matrixd transmat
                    = camera->getViewMatrix()
                    * camera->getProjectionMatrix()
                    * camera->getViewport()->computeWindowMatrix();
                transmat.invert(transmat);
                osg::Quat q = transmat.getRotate();

                osg::Vec3d v = osg::Vec3(ea.getX(), ea.getY(), 0.0) * transmat;
                osg::Vec3d n = q* osg::Vec3(0.0, 0.0, 1.0);
                n.normalize();

//                std::cout << std::endl;
//                std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;
//                std::cout << n.x() << " " << n.y() << " " << n.z() << std::endl;

                osg::Vec3d b = v - n * 1.0e5;
                osg::Vec3d e = v + n * 1.0e5;

//                std::cout << std::endl;
//                std::cout << b.x() << " " << b.y() << " " << b.z() << std::endl;
//                std::cout << e.x() << " " << e.y() << " " << e.z() << std::endl;

                v_win = getIntersection(b, e);
                v = v_win;
                std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;

            }
        }

        //std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;
        return true;
    }

    return false;
}

osg::Vec3d Intersections::getIntersection(const osg::Vec3d& b, const osg::Vec3d& e)
{
    osg::Vec3d result;

    osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
            new osgUtil::LineSegmentIntersector( b, e );

    osgUtil::IntersectionVisitor iv(intersector.get());
    iv.setLODSelectionMode( osgUtil::IntersectionVisitor::USE_HIGHEST_LEVEL_OF_DETAIL );
    iv.setTraversalMode( osgUtil::IntersectionVisitor::TRAVERSE_ACTIVE_CHILDREN );
    _assembly->accept( iv );

    if ( intersector->containsIntersections() )
    {
        osgUtil::LineSegmentIntersector::Intersection intersection = intersector->getFirstIntersection();
        osg::Vec3d v = intersection.getWorldIntersectPoint();
        result = v;
        //result = v * osg::computeLocalToWorld(intersection.nodePath);
    }

    return result;
}

osg::Vec3d Intersections::getIntersection(double x, double y)
{
    osg::Vec3d result;

    osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
            new osgUtil::LineSegmentIntersector(osgUtil::Intersector::MODEL, x, y);

    osgUtil::IntersectionVisitor iv(intersector.get());
//    iv.setLODSelectionMode( osgUtil::IntersectionVisitor::USE_HIGHEST_LEVEL_OF_DETAIL );
//    iv.setTraversalMode( osgUtil::IntersectionVisitor::TRAVERSE_ACTIVE_CHILDREN );
    _assembly->accept(iv);

    if ( intersector->containsIntersections() )
    {
        osgUtil::LineSegmentIntersector::Intersection intersection = intersector->getFirstIntersection();
        osg::Vec3d v = intersection.getWorldIntersectPoint();
        //result = v;
        result = v * osg::computeLocalToWorld(intersection.nodePath);
    }

    return result;
}

} // namespace cgi
} // namespace mc
