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

////////////////////////////////////////////////////////////////////////////////

#include <osg/MatrixTransform>

#include <pro/Animated.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

class Trans : public Animated
{
public:

    static constexpr char kTagName[] = { "matrix_trans" };

    Trans(osg::MatrixTransform* mt = nullptr);

    std::unique_ptr<Component> Clone() const override;

    inline const char* GetTagName() const override { return kTagName; }

    void SetPx(double px);
    void SetPy(double py);
    void SetPz(double pz);

    void SetAx(double ax);
    void SetAy(double ay);
    void SetAz(double az);

    inline double px() const { return px_; }
    inline double py() const { return py_; }
    inline double pz() const { return pz_; }

    inline double ax() const { return ax_; }
    inline double ay() const { return ay_; }
    inline double az() const { return az_; }

protected:

    osg::ref_ptr<osg::MatrixTransform> mt_;

    double px_ = 0.0;
    double py_ = 0.0;
    double pz_ = 0.0;

    double ax_ = 0.0;
    double ay_ = 0.0;
    double az_ = 0.0;

    virtual Result ReadParameters(const QDomElement* node) override;
    virtual Result SaveParameters(QDomDocument* doc, QDomElement* node) override;

    void UpdateTransformMatrix();
};

} // namespace pro
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_PRO_TRANS_H_
