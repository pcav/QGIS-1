/***************************************************************************
                          qgsserverconfig.cpp
 Configuration for Qgis Mapserver
                          -------------------
  begin                : 2015-05-20
  copyright            : (C) 2015 by Alessandro Pasotti
  email                : a dot pasotti at itopen dot it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsserverconfig.h"

QgsServerConfig::QgsServerConfig()
  : mMapRenderer( new QgsMapRenderer )
#ifdef HAVE_SERVER_PYTHON_PLUGINS
  , mServerInterface ( &mCapabilitiesCache )
#endif
{
}

QgsServerConfig::~QgsServerConfig()
{
}

#ifdef HAVE_SERVER_PYTHON_PLUGINS

void QgsServerConfig::initPluginFilters( )
{
  mPluginFilters = mServerInterface.filters();
}
#endif

void QgsServerConfig::setConfigFilePath( QString const configFilePath )
{
  mConfigFilePath = configFilePath;
#ifdef HAVE_SERVER_PYTHON_PLUGINS
  mServerInterface.setConfigFilePath( configFilePath );
#endif
}