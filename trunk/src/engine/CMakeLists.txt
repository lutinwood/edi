# Module FindQT.cmake 
# /usr/share/cmake-2.8/Modules
FIND_PACKAGE(Qt4 4.8.1 REQUIRED)

# Utilisation de la library QT 
INCLUDE(${QT_USE_FILE})

# Asignation de la variable EngineSources
# avec les fichier *.cpp
FILE(GLOB EngineSources *.cpp)

# Assignation de la varible EngineDef 
# avec les fichier de définition *.h
FILE(GLOB EngineDef *.h)

# Assignation de la variable Moc_Eng 
# depuis la variables des fichiers de definition
QT4_WRAP_CPP( Moc_Eng ${EngineDef}) 
 
# Ajout des fichiers nécessaire à la librairie engine
ADD_LIBRARY(engine ${EngineSources} ${Moc_Eng} )

# Liaison de libraries QtTest
Target_Link_libraries(engine QtTest QtGui)

