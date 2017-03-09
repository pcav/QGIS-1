/***************************************************************************
  passwordmanagerbridge.cpp

  Keychain authentication bridge

  This is much more like a pen test for the auth system than a well
  designed plugin, but hooking into the authentication system,
  that was designed to be secure in the first place is not easy.

  -------------------
  begin                : Nov 21, 2016
  copyright            : (C) 2016 Boundless Spatial Inc.
  author               : Alessandro Pasotti
  email                : apasotti@boundlessgeo.com

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//
// QGIS Specific includes
//


#include "qgsauthpasswordhelper.h"

//
// Qt4 Related Includes
//

#include <QAction>
#include <QToolBar>
#include <QMessageBox>
#include <QSettings>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QTimer>
#include <QStackedWidget>
#include <QEventLoop>

// QtKeyChain library
#include "qtkeychain/keychain.h"

// QGIS classes
#include "qgsmessagelog.h"



// Timeout for the messagebar info messages, in seconds
const int MESSAGE_BAR_INFO_TIMEOUT = 10;

const QLatin1String QgsAuthPasswordHelper::sMasterPasswordName( "QGIS-Master-Password" );
const QLatin1String QgsAuthPasswordHelper::sWalletFolderName( "QGIS" );

#if defined(Q_OS_MAC)
const QString QgsAuthPasswordHelper::sWalletDisplayName( "KeyChain" );
static const QString sDescription = QObject::tr( "Master Password <-> KeyChain storage plugin. Store and retrieve your master password in your KeyChain" );
#elif defined(Q_OS_WIN)
const QString QgsAuthPasswordHelper::sWalletDisplayName( "Password Manager" );
static const QString sDescription = QObject::tr( "Master Password <-> Password Manager storage plugin. Store and retrieve your master password in your Password Manager" );
#elif defined(Q_OS_LINUX)
const QString QgsAuthPasswordHelper::sWalletDisplayName( "Wallet/KeyRing" );
static const QString sDescription = QObject::tr( "Master Password <-> Wallet/KeyRing storage plugin. Store and retrieve your master password in your Wallet/KeyRing" );
#else
const QString QgsAuthPasswordHelper::sWalletDisplayName( "Password Manager" );
static const QString sDescription = QObject::tr( "Master Password <-> KeyChain storage plugin. Store and retrieve your master password in your Wallet/KeyChain/Password Manager" );
#endif


/**
 * Constructor
 */
QgsAuthPasswordHelper::QgsAuthPasswordHelper( ):
  mUseWallet( true ),
  mMasterPassword( "" ),
  mVerificationError( false ),
  mErrorMessage( "" ),
  mErrorCode( QKeychain::NoError ),
  mIsDirty( true ),
  mLoggingEnabled( false ),
  mFailedInit( false )
{
  // Read settings
  readSettings();
}

QgsAuthPasswordHelper::~QgsAuthPasswordHelper()
{
  writeSettings();
}

const QString QgsAuthPasswordHelper::name()
{
  return QStringLiteral( "Password Helper" );
}

bool QgsAuthPasswordHelper::deleteMasterPassword()
{
  debug( "Opening wallet for DELETE ..." );
  QKeychain::DeletePasswordJob job( sWalletFolderName );
  job.setAutoDelete( false );
  job.setKey( sMasterPasswordName );
  QEventLoop loop;
  job.connect( &job, SIGNAL( finished( QKeychain::Job * ) ), &loop, SLOT( quit() ) );
  job.start();
  loop.exec();
  if ( job.error() )
  {
    setErrorCode( job.error() );
    setErrorMessage( QString( tr( "Delete password failed: %1." ) ).arg( job.errorString() ) );
    setIsDirty( true );
    return false;
  }
  else
  {
    setIsDirty( false );
    clearErrors();
    return true;
  }
}



void QgsAuthPasswordHelper::debug( QString msg )
{
  if ( loggingEnabled( ) )
  {
    QgsMessageLog::logMessage( msg, name() );
  }
}

void QgsAuthPasswordHelper::readSettings()
{
  QSettings settings;
  setUseWallet( settings.value( QString( "%1/useWallet" ).arg( name() ), true ).toBool() );
  setLoggingEnabled( settings.value( QString( "%1/loggingEnabled" ).arg( name() ), false ).toBool() );
}

void QgsAuthPasswordHelper::writeSettings()
{
  QSettings settings;
  settings.setValue( QString( "%1/useWallet" ).arg( name() ), useWallet( ) );
  settings.setValue( QString( "%1/loggingEnabled" ).arg( name() ), loggingEnabled( ) );
}


QString QgsAuthPasswordHelper::readMasterPassword()
{
  // Retrieve it!
  QString password( "" );
  debug( "Opening wallet for READ ..." );
  QKeychain::ReadPasswordJob job( sWalletFolderName );
  job.setAutoDelete( false );
  job.setKey( sMasterPasswordName );
  QEventLoop loop;
  job.connect( &job, SIGNAL( finished( QKeychain::Job * ) ), &loop, SLOT( quit() ) );
  job.start();
  loop.exec();
  if ( job.error() )
  {
    setErrorCode( job.error() );
    setErrorMessage( QString( tr( "Retrieving password from the %1 failed: %2." ) ).arg( job.errorString(), sWalletDisplayName ) );
  }
  else
  {
    password = job.textData();
    // Password is there but it is empty, treat it like if it were not found
    if ( password.isEmpty() )
    {
      setErrorCode( QKeychain::EntryNotFound );
      setErrorMessage( tr( "Empty password retrieved from the %1." ).arg( sWalletDisplayName ) );
    }
    else
    {
      clearErrors();
    }
  }
  return password;
}

bool QgsAuthPasswordHelper::storeMasterPassword( QString password )
{

  Q_ASSERT( !password.isEmpty() );
  debug( "Opening wallet for WRITE ..." );
  QKeychain::WritePasswordJob job( sWalletFolderName );
  job.setAutoDelete( false );
  job.setKey( sMasterPasswordName );
  job.setTextData( password );
  QEventLoop loop;
  job.connect( &job, SIGNAL( finished( QKeychain::Job * ) ), &loop, SLOT( quit() ) );
  job.start();
  loop.exec();
  if ( job.error() )
  {
    setErrorCode( job.error() );
    setErrorMessage( QString( tr( "Storing password in the %1 failed: %2." ) ).arg( sWalletDisplayName, job.errorString() ) );
    setIsDirty( true );
    return false;
  }
  else
  {
    setIsDirty( false );
    clearErrors();
    return true;
  }
}

void QgsAuthPasswordHelper::clearErrors()
{
  setErrorCode( QKeychain::NoError );
  setErrorMessage( "" );
}


/*
 * Here it is the storage control logic, the real writing in the wallet
 * is delegated to storeMasterPassword()
 *
void QgsAuthPasswordHelper::saveMasterPassword()
{
  if ( ! mAuthManager->masterPasswordIsSet() || ! passwordIsSame( masterPassword() ) )
  {
    mVerificationError = true; // Prevent the password being inserted from the wallet if it's already there
    mAuthManager->clearMasterPassword();
    mAuthManager->setMasterPassword( true );
  }
  if ( ! masterPassword().isEmpty() )
  {
    storeMasterPassword( masterPassword() );
    if ( errorCode() == QKeychain::NoError )
    {
      showInfo( tr( "Master password has been successfully stored in your %1!" ).arg( sWalletDisplayName ) );
    }
    else
    {
      processError();
    }
  }
  else
  {
    setErrorMessage( tr( "Master password is empty: nothing to store." ) );
    showWarning( );
  }
}
*/

// If the error is permanent or the user denied access to the wallet
// we also want to disable the wallet system to prevent annoying
// notification on each subsequent access try.
void QgsAuthPasswordHelper::processError()
{
  if ( errorCode() == QKeychain::AccessDenied ||
       errorCode() == QKeychain::AccessDeniedByUser ||
       errorCode() == QKeychain::NoBackendAvailable ||
       errorCode() == QKeychain::NotImplemented )
  {
    setUseWallet( false );
    setErrorMessage( QString( tr( "There was an error and the %1 system has been disabled, you can re-enable it at any time through the menus. %2" ).arg( sWalletDisplayName ).arg( errorMessage( ) ) ) );
  }
}

