# Version actuelle lors de la première édition de ce fichier
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

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

#:FIND_PACKAGE(Qt4 4.8.1 REQUIRED QtCore QtGui QtXml)

# Check directory exist
IF(UNIX)
	SET(QSCI_DIR "/opt/qt_static_4_8_4")
    SET(QSCINTILLA_INCLUDE_DIR ${QSCI_DIR}/include)   
	SET(QSCINTILLA_LIBRARY ${QSCI_DIR}/lib/libqscintilla2.a)
ENDIF()
IF(WIN32)
	SET(QSCI_DIR "c:\\Qt_static")
    SET(QSCINTILLA_INCLUDE_DIR ${QSCI_DIR}\\include)   
	SET(QSCINTILLA_LIBRARY ${QSCI_DIR}\\lib\\libqscintilla2.a)
ENDIF()

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
if(UNIX)
MESSAGE("UNIX systeme")
endif()

if(WIN32)
MESSAGE("WinDOWS")
endif()


IF(UNIX)
Message("UNIX BASED")
	SET(qt_include_dir /opt/qt_static_4_8_4/include/)
	MESSAGE(" INCLUDE FOLDER ${qt_include_dir}")
	include_directories(${qt_include_dir})
	link_directories(/opt/qt_static_4_8_4/lib)
	ADD_EXECUTABLE(edi ${SRC} 
	${MOC_SRCS} ${RSRC} ${Moc_Eng} ${engine})
	TARGET_LINK_LIBRARIES (
		edi ${QSCINTILLA_LIBRARY} ${QT_LIBARIES} 
		engine ${QT_QTEST_LIBRARY} 
		QtGui QtTest QtCore pthread X11 xcb Xrender Xext rt png12
		SM fontconfig glib-2.0 freetype z gobject-2.0 ICE Xdmcp 
		 pcre Xau expat dl Xi c stdc++ 
				)

ENDIF()

IF(WIN32)
	SET(qt_install_path "c:/Qt_static")
	SET(qt_include_dir  ${qt_install_path}/include)
	set(qt_lib_dir ${qt_install_path}/lib)
	
	SET(mingw_install_path "c:/MinGW")
	SET(mingw_include_dir ${mingw_install_path}/include)
	SET(mingw_lib_dir ${mingw_install_path}/lib)
	
	include_directories(${qt_include_dir} ${mingw_include_dir})
	link_directories(${qt_lib_dir} ${mingw_lib_dir})
	
	ADD_EXECUTABLE(edi 
	${SRC} ${MOC_SRCS} ${RSRC} ${Moc_Eng} ${engine})
	TARGET_LINK_LIBRARIES (edi 
	${QSCINTILLA_LIBRARY} ${QT_LIBARIES}
	engine ${QT_QTEST_LIBRARY} QtGui QtCore ws2_32 
	winmm Imm32 mingw32)
ENDIF()