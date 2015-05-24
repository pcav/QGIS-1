/***************************************************************************
                          qgsserver.h
  Qgis Server main class.
                        -------------------
  begin                : June 05, 2015
  copyright            : (C) 2015 by Alessandro Pasotti
  email                : a dot pasotti at itopen dot it

  Based on previous work from:

  begin                : July 04, 2006
  copyright            : (C) 2006 by Marco Hugentobler & Ionut Iosifescu Enescu
  email                : marco dot hugentobler at karto dot baug dot ethz dot ch

  ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QGSSERVER_H
#define QGSSERVER_H

#include <QFileInfo>
#include "qgsrequesthandler.h"
#include "qgsservercontext.h"

class  SERVER_EXPORT QgsServer
{
  public:
    QgsServer();
    ~QgsServer();
    /** Server initialisation: intialise QGIS ang QT core application.
     * This method is automatically called by handleRequest if it wasn't
     * explicitly called before */
    void init(int & argc, char ** argv);
    // TODO: if HAVE_SERVER_PYTHON
    // The following is used by python bindings, that do not pass argc/argv
    void init();

    /** Handles the request. The output is normally printed trough FCGI printf
     * by the request handler or, in case the server has been invoked from python
     * bindings, a flag is set that capures all the output headers abd body instead
     * of printing it returns the output as a QByteArray.
     * When calling handleRequest() from python bindings a additional argument
     * specify if we only want the headers or the body back, this is mainly useful
     * for testing purposes.
     * The query string is normally read from environment
     * but can be also passed in and in this case overrides the environment
     * variable */
    QByteArray handleRequest( const QString queryString = QString( ) );
    // TODO: if HAVE_SERVER_PYTHON
    QByteArray handleRequest( const QString queryString,
                              const bool returnBody,
                              const bool returnHeaders );
    QByteArray handleRequestGetBody( const QString queryString = QString( ) );
    QByteArray handleRequestGetHeaders( const QString queryString = QString( ) );

  private:
    // All functions that where previously in the main file are now
    // static methods of this class
    static QString configPath( const QString& defaultConfigPath,
                                   const QMap<QString, QString>& parameters );
    // Mainly for debug
    static void dummyMessageHandler( QtMsgType type, const char *msg );
    // Mainly for debug
    static void printRequestInfos();
    // Mainly for debug
    static void printRequestParameters(
          const QMap< QString, QString>& parameterMap,
          int logLevel );
    static QFileInfo defaultProjectFile();
    static QFileInfo defaultAdminSLD();
    static void setupNetworkAccessManager();
    /** Created and returns a request handler instance */
    static QgsRequestHandler* createRequestHandler(
          const bool captureOutput = FALSE );

    // Instance status    
    bool mInitialised;
    QgsServerContext mServerContext;
    QString mServerName;
    char* mArgv[1];
    int mArgc;
    bool mCaptureOutput;
};
#endif // QGSSERVER_H

