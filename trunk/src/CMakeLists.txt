# Version actuelle lors de la première édition de ce fichier
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.9)

# Nom du projet
PROJECT (LightEdiPascal)

###		SOURCES
###		
# Assignation de la variable SRC
# avec les fichiers sources
FILE(GLOB SRC *.cpp)

# Assignation de la variable DEF 
# avec les fichiers de définition
FILE(GLOB DEF *.h)

###		OPTIONS
###		
# Ajout des Warning à la compilation
ADD_DEFINITIONS( -Wall -c )

# Ajout du repertoire engine
ADD_SUBDIRECTORY (engine)

###		MODULES
###		
# Module Qscintilla
FIND_PACKAGE(Qscintilla REQUIRED)
# Module FindQt.cmake
# /usr/share/Modules
FIND_PACKAGE(Qt4 4.8.3 REQUIRED QtCore QtGui QtXml)

###		MOC FILES
###		
# Génération de Fichier Moc 
# nécessaire pour les programme QT
QT4_WRAP_CPP( MOC_SRCS ${DEF})

# Fichier Xml ressources (images) 
QT4_ADD_RESOURCES( RSRC application.qrc )
 
###		INCLUDES
###		 
# ${QT_USE_FILE} définie par FIND_PACKAGE
INCLUDE(${QT_USE_FILE})
 
INCLUDE_DIRECTORIES(${QSCINTILLA_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(${QT_INCLUDE})

###		EXE
###		
IF(UNIX)
	ADD_EXECUTABLE(LEP ${SRC} ${MOC_SRCS} ${RSRC} ${Moc_Eng} ${engine})
	TARGET_LINK_LIBRARIES (LEP ${QSCINTILLA_LIBRARY} ${QT_LIBARIES} engine ${QT_QTEST_LIBRARY} QtTest)
ENDIF()

IF(WINDOWS)
	ADD_EXECUTABLE(lep.exe ${SRC} ${MOC_SRCS} ${RSRC} ${Moc_Eng} ${engine})
	TARGET_LINK_LIBRARIES (lep.exe ${QSCINTILLA_LIBRARY} ${QT_LIBARIES} engine ${QT_QTEST_LIBRARY} QtTest)
ENDIF()

#ADD_EXECUTABLE(LEP ${SRC} ${MOC_SRCS} ${RSRC} ${Moc_Eng} ${engine})
INSTALL(PROGRAMS ../build/LEP DESTINATION sbin)
###		LIBRARY
###		
# Liaisons des librairies
#TARGET_LINK_LIBRARIES (LEP ${QSCINTILLA_LIBRARY} ${QT_LIBARIES} engine ${QT_QTEST_LIBRARY} QtTest)

IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
INCLUDE(InstallRequiredSystemLibraries)
 
SET(CPACK_SET_DESTDIR "on")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
SET(CPACK_GENERATOR "DEB")

SET(MAJOR_VERSION 1)
 
SET(CPACK_PACKAGE_DESCRIPTION "Light EDI 4 PASCAL")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DEsigned 4 educationnal purposes")
SET(CPACK_PACKAGE_VENDOR "UA")
SET(CPACK_PACKAGE_CONTACT "Thierry FOrest ")
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}")
 
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-dbus (>= 4.5.0), libqtcore4 (>= 4.5.0), libqtgui4 (>= 4.5.0), libqscintilla2-8, fpc ")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "editors")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
 
SET(CPACK_COMPONENTS_ALL Libraries ApplicationData)
INCLUDE(CPack)
 
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
