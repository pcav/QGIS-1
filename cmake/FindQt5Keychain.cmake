# Find Qt5Keychain
# ~~~~~~~~~~~~~~~
# Copyright (c) 2016, Larry Shaffer lshaffer-at-boundlessgeo-com
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# CMake module to search for Qt5Keychain library from:
#    https://github.com/frankosterfeld/qtkeychain
#
# If it's found it sets QT5KEYCHAIN_FOUND to TRUE
# and following variables are set:
#    QT5KEYCHAIN_INCLUDE_DIR
#    QT5KEYCHAIN_LIBRARY

FIND_PATH(QT5KEYCHAIN_INCLUDE_DIR qt5keychain/keychain.h
  PATHS
  ${LIB_DIR}/include
  "$ENV{LIB_DIR}/include"
  $ENV{INCLUDE}
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(QT5KEYCHAIN_LIBRARY NAMES qt5keychain
  PATHS
  ${LIB_DIR}
  "$ENV{LIB_DIR}"
  $ENV{LIB}
  /usr/local/lib
  /usr/lib
)


IF (QT5KEYCHAIN_INCLUDE_DIR AND QT5KEYCHAIN_LIBRARY)
   SET(QT5KEYCHAIN_FOUND TRUE)
ENDIF (QT5KEYCHAIN_INCLUDE_DIR AND QT5KEYCHAIN_LIBRARY)

IF (QT5KEYCHAIN_FOUND)
   IF (NOT QT5KEYCHAIN_FIND_QUIETLY)
      MESSAGE(STATUS "Found Qt5Keychain: ${QT5KEYCHAIN_LIBRARY}")
   ENDIF (NOT QT5KEYCHAIN_FIND_QUIETLY)
ELSE (QT5KEYCHAIN_FOUND)
   IF (QT5KEYCHAIN_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Qt5Keychain")
   ENDIF (QT5KEYCHAIN_FIND_REQUIRED)
ENDIF (QT5KEYCHAIN_FOUND)
