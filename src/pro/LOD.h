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

////////////////////////////////////////////////////////////////////////////////

#include <osg/LOD>

#include <pro/Group.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

class LOD : public Group
{
public:

    using Intervals = std::vector<double>;

    static constexpr char kTagName[] = { "lod" };

    LOD(osg::LOD* lod = nullptr);

    std::unique_ptr<Component> Clone() const override;

    inline const char* GetTagName() const override { return kTagName; }

    virtual Result AddChild(std::shared_ptr<Component> child) override;

    void AddInterval(double value);
    void EditInterval(int index, double value);
    double GetInterval(int index) const;
    int GetIntervalsCount() const;
    void RemoveInterval(int index);

protected:

    osg::ref_ptr<osg::LOD> lod_;

    Intervals intervals_;

    void InflateLOD();

    virtual Result ReadParameters(const QDomElement* node) override;
    virtual Result SaveParameters(QDomDocument* doc, QDomElement* node) override;
};

} // namespace pro
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_PRO_LOD_H_
