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

#include <pro/FLOLS.h>

#include <cgi/Colors.h>
#include <cgi/Textures.h>

namespace mc {
namespace pro {

const double FLOLS::kSectorDegFrom = osg::DegreesToRadians(70.0f);
const double FLOLS::kSectorDegUnto = osg::DegreesToRadians(110.0f);

FLOLS::FLOLS()
    : PAT()
{
    SetName("FLOLS");
    Create();
}

std::unique_ptr<Component> FLOLS::Clone() const
{
    std::unique_ptr<FLOLS> rotor = std::make_unique<FLOLS>();
    rotor->SetName(GetName());
    return rotor;
}

void FLOLS::Create()
{
    osg::ref_ptr<osgSim::LightPointNode> lpn = new osgSim::LightPointNode();

    CreateIFLOLS(lpn);

    osg::ref_ptr<osg::Texture2D> texture = cgi::Textures::Get("../data/lightpoint.png");
    if ( texture.valid() )
    {
        osg::ref_ptr<osg::StateSet> ss = lpn->getOrCreateStateSet();
        lpn->setPointSprite();
        ss->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
    }

    pat_->addChild(lpn.get());
}

void FLOLS::CreateIFLOLS(osgSim::LightPointNode* lpn)
{
    CreateIFLOLS_Datum(lpn);
    CreateIFLOLS_Ball(lpn);
    CreateIFLOLS_WaveOff(lpn);
    CreateIFLOLS_CutOff(lpn);
}

void FLOLS::CreateIFLOLS_Datum(osgSim::LightPointNode* lpn)
{
    const double radius    = 4.0 * 0.3;
    const double intensity = 1.0;
    const double dist      = 0.3;

    osg::ref_ptr<osgSim::Sector> sector =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(  5.0),
                                            osg::DegreesToRadians(-10.0));

    for ( int i = 0; i < 10; i++ )
    {
        float y = ( i + 5 ) * dist;

        osgSim::LightPoint lpl(osg::Vec3(0.0, -y, 0.0), osg::Vec4(cgi::Colors::lime, 1.0));
        osgSim::LightPoint lpr(osg::Vec3(0.0,  y, 0.0), osg::Vec4(cgi::Colors::lime, 1.0));

        lpl._intensity = intensity;
        lpr._intensity = intensity;

        lpl._radius = radius;
        lpr._radius = radius;

        lpl._sector = sector;
        lpr._sector = sector;

        lpn->addLightPoint(lpl);
        lpn->addLightPoint(lpr);
    }
}

void FLOLS::CreateIFLOLS_Ball(osgSim::LightPointNode* lpn)
{
    const double radius    = 4.0 * 0.3;
    const double intensity = 5.0;
    const double dist      = 0.3;

    osg::ref_ptr<osgSim::Sector> sector_0 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(-5.00),
                                            osg::DegreesToRadians(1.55));

    osg::ref_ptr<osgSim::Sector> sector_1 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(1.45),
                                            osg::DegreesToRadians(2.30));
    osg::ref_ptr<osgSim::Sector> sector_2 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(2.20),
                                            osg::DegreesToRadians(3.05));
    osg::ref_ptr<osgSim::Sector> sector_3 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(2.95),
                                            osg::DegreesToRadians(3.80));
    osg::ref_ptr<osgSim::Sector> sector_4 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(3.70),
                                            osg::DegreesToRadians(4.55));
    osg::ref_ptr<osgSim::Sector> sector_5 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(4.45),
                                            osg::DegreesToRadians(5.30));
    osg::ref_ptr<osgSim::Sector> sector_6 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(5.20),
                                            osg::DegreesToRadians(6.05));
    osg::ref_ptr<osgSim::Sector> sector_7 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(5.95),
                                            osg::DegreesToRadians(6.80));
    osg::ref_ptr<osgSim::Sector> sector_8 =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians( 6.70),
                                            osg::DegreesToRadians(10.00));

    osgSim::LightPoint lp_0a(osg::Vec3(0.0, 0.0, -4.5 * dist), osg::Vec4(cgi::Colors::orangeRed, 1.0));
    osgSim::LightPoint lp_0b(osg::Vec3(0.0, 0.0, -3.5 * dist), osg::Vec4(cgi::Colors::orangeRed, 1.0));

    osgSim::LightPoint lp_1(osg::Vec3(0.0, 0.0, -2.5 * dist), osg::Vec4(cgi::Colors::yellow, 1.0));
    osgSim::LightPoint lp_2(osg::Vec3(0.0, 0.0, -1.5 * dist), osg::Vec4(cgi::Colors::yellow, 1.0));
    osgSim::LightPoint lp_3(osg::Vec3(0.0, 0.0, -0.5 * dist), osg::Vec4(cgi::Colors::yellow, 1.0));
    osgSim::LightPoint lp_4(osg::Vec3(0.0, 0.0,  0.5 * dist), osg::Vec4(cgi::Colors::yellow, 1.0));
    osgSim::LightPoint lp_5(osg::Vec3(0.0, 0.0,  1.5 * dist), osg::Vec4(cgi::Colors::yellow, 1.0));
    osgSim::LightPoint lp_6(osg::Vec3(0.0, 0.0,  2.5 * dist), osg::Vec4(cgi::Colors::yellow, 1.0));
    osgSim::LightPoint lp_7(osg::Vec3(0.0, 0.0,  3.5 * dist), osg::Vec4(cgi::Colors::yellow, 1.0));
    osgSim::LightPoint lp_8(osg::Vec3(0.0, 0.0,  4.5 * dist), osg::Vec4(cgi::Colors::yellow, 1.0));

    lp_0a._intensity = intensity;
    lp_0b._intensity = intensity;

    lp_1._intensity = intensity;
    lp_2._intensity = intensity;
    lp_3._intensity = intensity;
    lp_4._intensity = intensity;
    lp_5._intensity = intensity;
    lp_6._intensity = intensity;
    lp_7._intensity = intensity;
    lp_8._intensity = intensity;

    lp_0a._radius = radius;
    lp_0b._radius = radius;

    lp_1._radius = radius;
    lp_2._radius = radius;
    lp_3._radius = radius;
    lp_4._radius = radius;
    lp_5._radius = radius;
    lp_6._radius = radius;
    lp_7._radius = radius;
    lp_8._radius = radius;

    lp_0a._sector = sector_0;
    lp_0b._sector = sector_0;

    lp_1._sector = sector_1;
    lp_2._sector = sector_2;
    lp_3._sector = sector_3;
    lp_4._sector = sector_4;
    lp_5._sector = sector_5;
    lp_6._sector = sector_6;
    lp_7._sector = sector_7;
    lp_8._sector = sector_8;

    osg::ref_ptr<osgSim::BlinkSequence> blinkSequence = new osgSim::BlinkSequence;

    blinkSequence->addPulse(0.5f, osg::Vec4(cgi::Colors::orangeRed, 1.0));
    blinkSequence->addPulse(0.5f, osg::Vec4(0.0, 0.0, 0.0, 0.0));

    lp_0a._blinkSequence = blinkSequence;
    lp_0b._blinkSequence = blinkSequence;

    lpn->addLightPoint(lp_0a);
    lpn->addLightPoint(lp_0b);
    lpn->addLightPoint(lp_1);
    lpn->addLightPoint(lp_2);
    lpn->addLightPoint(lp_3);
    lpn->addLightPoint(lp_4);
    lpn->addLightPoint(lp_5);
    lpn->addLightPoint(lp_6);
    lpn->addLightPoint(lp_7);
    lpn->addLightPoint(lp_8);
}

void FLOLS::CreateIFLOLS_WaveOff(osgSim::LightPointNode* lpn)
{
    const double radius    = 4.0 * 0.3;
    const double intensity = 5.0;
    const double dist      = 0.3;

    double y1 = 5.0 * dist;
    double y2 = 7.0 * dist;

    double z[] =
    {
        -1.5 * dist,
         2.0 * dist,
         4.0 * dist
    };

    osg::ref_ptr<osgSim::Sector> sector [] =
    {
        new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                        osg::DegreesToRadians(-5.00),
                                        osg::DegreesToRadians( 1.30)),
        new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                        osg::DegreesToRadians(-5.00),
                                        osg::DegreesToRadians( 1.45)),
        new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                        osg::DegreesToRadians(-5.00),
                                        osg::DegreesToRadians( 1.50))
    };

    osg::ref_ptr<osgSim::BlinkSequence> blinkSequence = new osgSim::BlinkSequence();

    blinkSequence->addPulse(0.5, osg::Vec4(cgi::Colors::orangeRed, 1.0));
    blinkSequence->addPulse(0.5, osg::Vec4(0.0, 0.0, 0.0, 0.0));

    for ( int i = 0; i < 3; ++i )
    {
        osgSim::LightPoint lp_L1(osg::Vec3(0.0, -y1,  z[i]), osg::Vec4(cgi::Colors::orangeRed, 1.0));
        osgSim::LightPoint lp_L2(osg::Vec3(0.0, -y2,  z[i]), osg::Vec4(cgi::Colors::orangeRed, 1.0));
        osgSim::LightPoint lp_R1(osg::Vec3(0.0,  y1,  z[i]), osg::Vec4(cgi::Colors::orangeRed, 1.0));
        osgSim::LightPoint lp_R2(osg::Vec3(0.0,  y2,  z[i]), osg::Vec4(cgi::Colors::orangeRed, 1.0));

        lp_L1._intensity = intensity;
        lp_L2._intensity = intensity;
        lp_R1._intensity = intensity;
        lp_R2._intensity = intensity;

        lp_L1._radius = radius;
        lp_L2._radius = radius;
        lp_R1._radius = radius;
        lp_R2._radius = radius;

        lp_L1._sector = sector[i];
        lp_L2._sector = sector[i];
        lp_R1._sector = sector[i];
        lp_R2._sector = sector[i];

        lp_L1._blinkSequence = blinkSequence;
        lp_L2._blinkSequence = blinkSequence;
        lp_R1._blinkSequence = blinkSequence;
        lp_R2._blinkSequence = blinkSequence;

        lpn->addLightPoint(lp_L1);
        lpn->addLightPoint(lp_L2);
        lpn->addLightPoint(lp_R1);
        lpn->addLightPoint(lp_R2);
    }
}

void FLOLS::CreateIFLOLS_CutOff(osgSim::LightPointNode* lpn)
{
    const double radius    = 4.0 * 0.3;
    const double intensity = 5.0;
    const double dist      = 0.3;

    double y1 = 2.0 * dist;
    double y2 = 4.0 * dist;

    double z = 2.5 * dist;

    osg::ref_ptr<osgSim::Sector> sector =
            new osgSim::AzimElevationSector(kSectorDegFrom, kSectorDegUnto,
                                            osg::DegreesToRadians(6.70),
                                            osg::DegreesToRadians(10.00));

    osgSim::LightPoint lp_L1(osg::Vec3(0.0, -y1, z), osg::Vec4(cgi::Colors::lime, 1.0));
    osgSim::LightPoint lp_L2(osg::Vec3(0.0, -y2, z), osg::Vec4(cgi::Colors::lime, 1.0));
    osgSim::LightPoint lp_R1(osg::Vec3(0.0,  y1, z), osg::Vec4(cgi::Colors::lime, 1.0));
    osgSim::LightPoint lp_R2(osg::Vec3(0.0,  y2, z), osg::Vec4(cgi::Colors::lime, 1.0));

    lp_L1._intensity = intensity;
    lp_L2._intensity = intensity;
    lp_R1._intensity = intensity;
    lp_R2._intensity = intensity;

    lp_L1._radius = radius;
    lp_L2._radius = radius;
    lp_R1._radius = radius;
    lp_R2._radius = radius;

    lp_L1._sector = sector;
    lp_L2._sector = sector;
    lp_R1._sector = sector;
    lp_R2._sector = sector;

    lpn->addLightPoint(lp_L1);
    lpn->addLightPoint(lp_L2);
    lpn->addLightPoint(lp_R1);
    lpn->addLightPoint(lp_R2);
}

} // namespace pro
} // namespace mc
