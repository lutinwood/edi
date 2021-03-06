/***************************************************************************
 *   Copyright (C) 2007 by Julien COURCELLE, Vilayvanh MANIVONG   *
 *                                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURP:q
OSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QApplication>
#include <QFont>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);
    QTranslator translator;
#if defined Q_OS_WIN
    translator.load("qt_fr","C:\\Qt_static\\translations");
#else
    translator.load("qt_fr","/opt/qt_static_4_8_4/translations");
#endif
    app.setStyle("plastique");
    app.installTranslator(&translator);

    QSplashScreen splash(QPixmap(QString
    (":/images/splashscreen.png"),
    0,Qt::AutoColor),0);

    splash.show();
    MainWindow mainWin;
    mainWin.setAttribute(Qt::WA_AlwaysShowToolTips,true);
    mainWin.show();
    splash.finish(&mainWin);
    return app.exec();
}
