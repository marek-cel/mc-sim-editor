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

#include <cgi/CGI.h>

#include <osg/Light>
#include <osg/LightSource>

namespace mc {
namespace cgi {

CGI::CGI(std::shared_ptr<Data> data)
    : Component(data)
{
    _root->setName("SceneRoot");

    osg::ref_ptr<osg::StateSet> rootStateSet = _root->getOrCreateStateSet();
    rootStateSet->setMode(GL_RESCALE_NORMAL , osg::StateAttribute::ON);
    rootStateSet->setMode(GL_LIGHT0         , osg::StateAttribute::ON);
    rootStateSet->setMode(GL_LIGHT1         , osg::StateAttribute::ON);
    rootStateSet->setMode(GL_LIGHTING       , osg::StateAttribute::ON);
    rootStateSet->setMode(GL_BLEND          , osg::StateAttribute::ON);
    rootStateSet->setMode(GL_ALPHA_TEST     , osg::StateAttribute::ON);
    rootStateSet->setMode(GL_DEPTH_TEST     , osg::StateAttribute::ON);
    rootStateSet->setRenderBinDetails(1, "DepthSortedBin");

    createLight();
}

void CGI::createLight()
{
    _lightSwitch = new osg::Switch();

    osg::ref_ptr<osg::LightSource> lightSourceSun = new osg::LightSource();
    _lightSwitch->addChild(lightSourceSun.get());

    osg::ref_ptr<osg::Light> lightSun = new osg::Light();

    lightSun->setLightNum(1);
    lightSun->setPosition(osg::Vec4d(5000.0, 5000.0, 5000.0, 0.0));

    lightSun->setAmbient(  osg::Vec4(1.0, 1.0, 1.0, 1.0) );
    lightSun->setDiffuse(  osg::Vec4(1.0, 1.0, 1.0, 1.0) );
    lightSun->setSpecular( osg::Vec4(1.0, 1.0, 1.0, 1.0) );

    lightSun->setConstantAttenuation(1.0);

    lightSourceSun->setLight(lightSun.get());

    lightSourceSun->setLocalStateSetModes(osg::StateAttribute::ON);
    lightSourceSun->setStateSetModes(*_root->getOrCreateStateSet(), osg::StateAttribute::ON);
}

} // namespace cgi
} // namespace mc
