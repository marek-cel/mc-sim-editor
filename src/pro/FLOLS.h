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
#ifndef MC_SIM_PRO_FLOLS_H_
#define MC_SIM_PRO_FLOLS_H_

#include <osgSim/LightPointNode>

#include <pro/PAT.h>

namespace mc {
namespace pro {

/** Improved Fresnel Lens Optical Landing System - IFLOLS */
class FLOLS : public PAT
{
public:

    static constexpr char kTagName[] = { "flols" };

    static const double kSectorDegFrom;     ///< light point sector - foreward narrow from
    static const double kSectorDegUnto;     ///< light point sector - foreward narrow unto

    FLOLS();

    inline bool CanBeAnimated() const override { return false; }

    inline bool CanBeParent() const override { return false; }

    std::unique_ptr<Component> Clone() const override;

    inline const char* GetTagName() const override { return kTagName; }

protected:

    void Create();
    void CreateIFLOLS(osgSim::LightPointNode* lpn);
    void CreateIFLOLS_Datum(osgSim::LightPointNode* lpn);
    void CreateIFLOLS_Ball(osgSim::LightPointNode* lpn);
    void CreateIFLOLS_WaveOff(osgSim::LightPointNode* lpn);
    void CreateIFLOLS_CutOff(osgSim::LightPointNode* lpn);
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_FLOLS_H_
