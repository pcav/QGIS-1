/***************************************************************************
    qgsauthpasswordhelper.sip
    -------------------
    begin                : Feb, 2017
    copyright            : (C) 2017 Boundless Spatial Inc.
    author               : Alessandro Pasotti
    email                : apasotti@boundlessgeo.com

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



/**
* \class QgsAuthPasswordHelper
* \brief Keeps master password in sync with the OS password manager
*/
class QgsAuthPasswordHelper: public QObject
{
%TypeHeaderCode
#include <qgsauthpasswordhelper.h>
%End

  public:

    /**
    * Constructor
    */
    explicit QgsAuthPasswordHelper( );
    //! Destructor
    ~QgsAuthPasswordHelper();


    //! Read Master password from the wallet
    QString readMasterPassword();

    //! Delete master password from wallet
    bool deleteMasterPassword();

    //! Store Master password in the wallet
    bool storeMasterPassword( QString password );

    //! Error message getter
    QString errorMessage();

    //! Use wallet  getter
    bool useWallet();

    //! Use wallet setter
    void setUseWallet( bool useWallet );

    //! Logging getter
    bool loggingEnabled();

    //! Logging setter
    void setLoggingEnabled( bool loggingEnabled );
};
