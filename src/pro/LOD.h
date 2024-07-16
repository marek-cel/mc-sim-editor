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
#ifndef MC_SIM_PRO_LOD_H_
#define MC_SIM_PRO_LOD_H_

#include <osg/LOD>

#include <pro/Group.h>

namespace mc {
namespace pro {

class LOD : public Group
{
public:

    using Intervals = std::vector<double>;

    static constexpr char kTagName[] = { "lod" };

    LOD(osg::LOD* lod = nullptr);

    std::shared_ptr<Component> clone() const override;

    inline const char* getTagName() const override { return kTagName; }

    virtual Result addChild(std::shared_ptr<Component> child) override;

    void addInterval(double value);
    void editInterval(int index, double value);
    double getInterval(int index) const;
    int getIntervalsCount() const;
    void removeInterval(int index);

protected:

    osg::ref_ptr<osg::LOD> _lod;
    Intervals _intervals;

    void inflateLOD();

    virtual Result readParameters(const QDomElement* node) override;
    virtual Result saveParameters(QDomDocument* doc, QDomElement* node) override;
};

} // namespace pro
} // namespace mc

#endif // MC_SIM_PRO_LOD_H_
