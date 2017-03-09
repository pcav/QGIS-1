/***************************************************************************
    passwordmanagerbridge.h
    -------------------
    begin                : Nov 21, 2016
    copyright            : (C) 2016 Boundless Spatial Inc.
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

#ifndef AuthPasswordHelper_H
#define AuthPasswordHelper_H

//QT includes
#include <QObject>

//QGIS includes
#include "qgis_core.h"

// QtKeyChain library
#include "qtkeychain/keychain.h"


/**
* \class QgsAuthPasswordHelper
* \brief Keeps master password in sync with the OS password manager
* @note added in QGIS 3
*/
class CORE_EXPORT QgsAuthPasswordHelper: public QObject
{
    Q_OBJECT

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
    QString errorMessage() { return mErrorMessage; }

    //! Use wallet  getter
    bool useWallet() { return mUseWallet; }

    //! Use wallet setter
    void setUseWallet( bool useWallet ) { mUseWallet = useWallet; }

    //! Logging getter
    bool loggingEnabled() { return mLoggingEnabled; }

    //! Logging setter
    void setLoggingEnabled( bool loggingEnabled ) { mLoggingEnabled = loggingEnabled; }


  private:

    //! Return name for logging
    const QString name();

    //! Print a debug message in QGIS
    void debug( QString msg );

    //! Read settings
    void readSettings();

    //! Write settings
    void writeSettings();

    //! Error message setter
    void setErrorMessage( QString errorMessage ) { mErrorMessage = errorMessage; }

    //! Clear error code and message
    void clearErrors();

    //! Error code setter
    void setErrorCode( QKeychain::Error errorCode ) { mErrorCode = errorCode; }

    //! Error code getter
    QKeychain::Error errorCode() { return mErrorCode; }

    //! Dirty flag setter
    void setIsDirty( bool dirty ) { mIsDirty = dirty; }

    //! Dirty flag getter
    bool isDirty( ) { return mIsDirty; }

    //! Ask the user, and then store
    void saveMasterPassword();

    //! Process the error: show it and/or disable the wallet system in case of
    //! access denied or no backend
    void processError();



    //! The user has chosen of using this plugin to store and retrieve the master pwd from his wallet
    bool mUseWallet;

    //! The cached master password
    QString mMasterPassword;

    //! Master password verification has failed
    bool mVerificationError;

    //! Store last error message
    QString mErrorMessage;

    //! Store last error code (enum)
    QKeychain::Error mErrorCode;

    //! Master password in memory is not in sync with the wallet
    //! This could be for several reasons: error in reading, empty password, wrong password etc.
    bool mIsDirty;

    //! Enable logging
    bool mLoggingEnabled;

    //! The display name of the wallet (platform dependent)
    static const QString sWalletDisplayName;

    //! Master password name in the wallets
    static const QLatin1String sMasterPasswordName;

    //! Wallet folder in the wallets
    static const QLatin1String sWalletFolderName;

    //! Whether the plugin failed to initialize
    bool mFailedInit;
};

#endif //AuthPasswordHelper_H
