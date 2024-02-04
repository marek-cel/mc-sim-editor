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
#ifndef MC_SIM_PRO_FILE_H_
#define MC_SIM_PRO_FILE_H_

////////////////////////////////////////////////////////////////////////////////

#include <osg/Group>

#include <Result.h>

#include <pro/Component.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

class File : public Component
{
public:

    static constexpr char kTagName[] = { "file" };

    File();

    inline bool CanBeAnimated() const override { return false; }

    inline bool CanBeParent() const override { return false; }

    std::unique_ptr<Component> Clone() const override;

    inline const char* GetTagName() const override { return kTagName; }

    inline QString GetFile() const { return file_; }

    void SetFile(QString file);

    virtual void SetProjFile(QString proj_file) override;

protected:

    osg::ref_ptr<osg::Group> group_;
    osg::ref_ptr<osg::Node> node_;

    QString file_;

    void ReloadFile();

    virtual Result ReadParameters(const QDomElement* node) override;
    virtual Result SaveParameters(QDomDocument* doc, QDomElement* node) override;
};

} // namespace pro
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_PRO_FILE_H_
