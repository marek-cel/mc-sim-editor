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
#ifndef MC_SIM_PRO_ROTOR_H_
#define MC_SIM_PRO_ROTOR_H_

#include <pro/PAT.h>

namespace mc {
namespace pro {

class Rotor : public PAT
{
public:

    using Blades = std::vector<osg::ref_ptr<osg::PositionAttitudeTransform>>;

    enum class Direction
    {
        CW  = 0,
        CCW = 1
    };

    static constexpr char kTagName[] = { "rotor" };

    Rotor();

    inline bool CanBeAnimated() const override { return false; }

    inline bool CanBeParent() const override { return false; }

    std::unique_ptr<Component> Clone() const override;

    inline const char* GetTagName() const override { return kTagName; }

    void SetBladesNo(int blades_no);
    void SetHingeOffset(double offset);
    void SetDirection(Direction direction);
    void SetFileBlade(const QString& file);
    void SetFileShaft(const QString& file);

    inline unsigned int GetBladesNo()    const { return blades_no_; }
    inline double       GetHingeOffset() const { return hinge_offset_; }
    inline Direction    GetDirection()   const { return direction_; }
    inline QString      GetFileBlade()   const { return file_blade_; }
    inline QString      GetFileShaft()   const { return file_shaft_; }

protected:

    osg::ref_ptr<osg::PositionAttitudeTransform> shaft_;

    Blades blades_;

    int blades_no_ = 2;
    double hinge_offset_ = 0.0;
    Direction direction_ = Direction::CW;

    QString file_blade_;
    QString file_shaft_;

    virtual Result ReadParameters(const QDomElement* node) override;
    virtual Result SaveParameters(QDomDocument* doc, QDomElement* node) override;

    void Clear();
    void Create();
    void CreateShaft();
    void CreateBlades();
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_ROTOR_H_
