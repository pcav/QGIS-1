/***************************************************************************
                          qgsserverconfig.h
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

#ifndef QGSSERVERCONFIG_H
#define QGSSERVERCONFIG_H

#include "qgsconfig.h"
#include "qgsmaprenderer.h"
#include "qgsconfigcache.h"
#include "qgscapabilitiescache.h"

#ifdef HAVE_SERVER_PYTHON_PLUGINS
#include "qgsserverplugins.h"
#include "qgsserverfilter.h"
#include "qgsserverinterfaceimpl.h"
#endif

class SERVER_EXPORT QgsServerConfig
{
  public:
    QgsServerConfig();
    ~QgsServerConfig();
    void setConfigFilePath( QString const configFilePath );
    QString configFilePath( ) { return mConfigFilePath; }
    QgsCapabilitiesCache& capabilitiesCache( ) { return mCapabilitiesCache; }
    QgsMapRenderer* mapRenderer( ) { return mMapRenderer.data(); }
#ifdef HAVE_SERVER_PYTHON_PLUGINS
    QgsServerInterfaceImpl& serverInterface( ) { return mServerInterface; }
    QMultiMap<int, QgsServerFilter*> pluginFilters( ) { return mPluginFilters; }
    void initPluginFilters( );
#endif

  private:
    QString mConfigFilePath;
    QgsCapabilitiesCache mCapabilitiesCache;
    QScopedPointer< QgsMapRenderer > mMapRenderer;
#ifdef HAVE_SERVER_PYTHON_PLUGINS
    QgsServerInterfaceImpl mServerInterface;
    QMultiMap<int, QgsServerFilter*> mPluginFilters;
#endif
};

#endif // QGSSERVERCONFIG_H
