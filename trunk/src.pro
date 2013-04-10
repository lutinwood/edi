# Fichier utilisé par qmake



HEADERS = mainwindow.h
HEADERS += finddialog.h \
replaceDialog.h \
lineDialog.h \
qscilexerpascal.h \
engine/globalDiscuss.h \
engine/fpcdiscuss.h  \
engine/gdbdiscuss.h \
engine/applicationdiscuss.h \
helpwindow.h \
listwidget.h \
settingwindow.h \
myedit.h 
SOURCES = main.cpp \
                mainwindow.cpp
SOURCES += finddialog.cpp \
replaceDialog.cpp \
lineDialog.cpp \
qscilexerpascal.cpp \
engine/globalDiscuss.cpp \
engine/fpcdiscuss.cpp \
engine/gdbdiscuss.cpp \
engine/applicationdiscuss.cpp \
helpwindow.cpp \
listwidget.cpp \
settingwindow.cpp \
myedit.cpp
RESOURCES = application.qrc
LIBS += -lqscintilla2
win32 {
    CONFIG += warn_on \
	  release \
          qt
}
unix {
    CONFIG += warn_on \
	  release \
          qt \
        qtestlib
}
TARGET = ../bin/edi
