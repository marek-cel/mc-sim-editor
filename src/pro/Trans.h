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
#ifndef MC_SIM_PRO_TRANS_H_
#define MC_SIM_PRO_TRANS_H_

#include <osg/MatrixTransform>

#include <pro/Animated.h>

namespace mc {
namespace pro {

class Trans : public Animated
{
public:

    static constexpr char kTagName[] = { "matrix_trans" };

    Trans(osg::MatrixTransform* mt = nullptr);

    std::shared_ptr<Component> clone() const override;

    inline const char* getTagName() const override { return kTagName; }

    void setPx(double px);
    void setPy(double py);
    void setPz(double pz);

    void setAx(double ax);
    void setAy(double ay);
    void setAz(double az);

    inline double px() const { return _px; }
    inline double py() const { return _py; }
    inline double pz() const { return _pz; }

    inline double ax() const { return _ax; }
    inline double ay() const { return _ay; }
    inline double az() const { return _az; }

protected:

    osg::ref_ptr<osg::MatrixTransform> _mt;

    double _px = 0.0;
    double _py = 0.0;
    double _pz = 0.0;

    double _ax = 0.0;
    double _ay = 0.0;
    double _az = 0.0;

    virtual Result readParameters(const QDomElement* node) override;
    virtual Result saveParameters(QDomDocument* doc, QDomElement* node) override;

    void updateTransformMatrix();
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_TRANS_H_
