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

    inline bool canBeAnimated() const override { return false; }

    inline bool canBeParent() const override { return false; }

    std::shared_ptr<Component> clone() const override;

    inline const char* getTagName() const override { return kTagName; }

    void setBladesNo(int blades_no);
    void setHingeOffset(double offset);
    void setDirection(Direction direction);
    void setFileBlade(const QString& file);
    void setFileShaft(const QString& file);

    inline unsigned int getBladesNo()    const { return _blades_no; }
    inline double       getHingeOffset() const { return _hinge_offset; }
    inline Direction    getDirection()   const { return _direction; }
    inline QString      getFileBlade()   const { return _file_blade; }
    inline QString      getFileShaft()   const { return _file_shaft; }

protected:

    osg::ref_ptr<osg::PositionAttitudeTransform> _shaft;

    Blades _blades;

    int _blades_no = 2;
    double _hinge_offset = 0.0;
    Direction _direction = Direction::CW;

    QString _file_blade;
    QString _file_shaft;

    virtual Result readParameters(const QDomElement* node) override;
    virtual Result saveParameters(QDomDocument* doc, QDomElement* node) override;

    void clear();
    void create();
    void createShaft();
    void createBlades();
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_ROTOR_H_
