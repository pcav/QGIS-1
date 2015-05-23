/***************************************************************************
                          qgsservercontext.cpp
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

#include "qgsservercontext.h"

QgsServerContext::QgsServerContext()
  : mMapRenderer( new QgsMapRenderer )
  , mQgsApplication ( NULL )
#ifdef HAVE_SERVER_PYTHON_PLUGINS
  , mServerInterface ( &mCapabilitiesCache )
#endif
{
}

QgsServerContext::~QgsServerContext()
{
  delete mQgsApplication;
}

#ifdef HAVE_SERVER_PYTHON_PLUGINS
void QgsServerContext::initPluginFilters( )
{
  mPluginFilters = mServerInterface.filters();
}
#endif

void QgsServerContext::setConfigFilePath( QString const configFilePath )
{
  mConfigFilePath = configFilePath;
#ifdef HAVE_SERVER_PYTHON_PLUGINS
  mServerInterface.setConfigFilePath( configFilePath );
#endif
}

void QgsServerContext::initQgsApplication( int & argc, char ** argv, const char* display )
{
  mQgsApplication = new QgsApplication( argc, argv, display );
}
