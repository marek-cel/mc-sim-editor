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

#include <cgi/Textures.h>

#include <osgDB/ReadFile>

#include <mcutils/misc/MapUtils.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Textures> Textures::instance_;

////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Textures> Textures::Instance()
{
    if ( !instance_ )
    {
        Textures* instance = new Textures();
        instance_ = std::shared_ptr<Textures>(instance);
    }

    return instance_;
}

////////////////////////////////////////////////////////////////////////////////

osg::Texture2D* Textures::Get(std::string file, double maxAnisotropy)
{
    osg::ref_ptr<osg::Texture2D> texture = GetMapItemByKey(&Instance()->list_, file);

    if ( texture.valid() )
    {
        return texture.get();
    }

    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(file);

    if ( image.valid() )
    {
        texture = new osg::Texture2D();
        texture->setImage(image.get());

        texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_EDGE);
        texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_EDGE);
        texture->setWrap(osg::Texture2D::WRAP_R, osg::Texture2D::CLAMP_TO_EDGE);

        //texture->setNumMipmapLevels( 4 );
        texture->setMaxAnisotropy(maxAnisotropy);

        texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_NEAREST);
        texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

        texture->setUnRefImageDataAfterApply(false);

        AddMapItem(&Instance()->list_, file, texture);
        return texture.get();
    }
    else
    {
        std::cerr << "Error! Cannot open texture file: " << file << std::endl;
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void Textures::Reset()
{
    Instance()->list_.clear();
}

////////////////////////////////////////////////////////////////////////////////

} // namespace cgi
} // namespace mc
