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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <iostream>

#include "helpwindow.h"

using namespace std;

//constructeur
HelpWindow::HelpWindow(QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
        #if defined(Q_OS_WIN)
            m_OS=true;
        #else 
            m_OS=false;
        #endif
	this->setWindowTitle(tr("Aide"));
	this->resize(640,480);
	m_nextAct = new QAction(QIcon(":/images/next.png"),tr("Suivant"),this);
	m_nextAct->setShortcut(tr("Alt+right"));
	m_nextAct->setStatusTip(tr("Suivant"));
	m_previousAct = new QAction(QIcon(":/images/previous.png"),tr("Précédent"),this);
	m_previousAct->setShortcut(tr("Alt+Left"));
	m_previousAct->setStatusTip(tr("Précédent"));
	m_refreshAct = new QAction(QIcon(":/images/refresh.png"),tr("Actualiser"),this);
	m_refreshAct->setShortcut(tr("F5"));
	m_refreshAct->setStatusTip(tr("Actualiser"));
	m_homeAct = new QAction(QIcon(":/images/home.png"),tr("Accueil"),this);	
	m_homeAct->setStatusTip(tr("Accueil"));
	m_toolBar = addToolBar(tr("Navigation"));
	m_toolBar->addAction(m_previousAct);
	m_toolBar->addAction(m_nextAct);
	m_toolBar->addAction(m_refreshAct);
	m_toolBar->addAction(m_homeAct);
	m_textBrowser = new QTextBrowser(this);
	//recherche du path pour la doc
        QStringList listPath;
        if (QFile::exists(QString("conf.ini"))) 
        {
           QFile *file = new QFile(QString("conf.ini"));
            file->open(QIODevice::ReadOnly);
            if (!m_OS)
            {//si l'OS est linux
                while (!file->atEnd())
                {
                    QByteArray buffer(file->readLine());	
                    QString path(buffer);
                    if (path.contains("DOC_PATH_X11="))
                    {//si le path est trouvé
                        QString ch(path.section("DOC_PATH_X11=",1,-1));
                        m_PATH= new QString(ch.simplified());
                    }
                    if (path.contains("DOC_INDEX="))
                    {//si le path est trouvé
                        QString ch(path.section("DOC_INDEX=",1,-1));
                        m_INDEX= new QString(ch.simplified());
                    }
                }
            }
            else{//si l'OS est Windows
                while (!file->atEnd())
                {
                    QByteArray buffer(file->readLine());	
                    QString path(buffer);
                    if (path.contains("DOC_PATH_WIN32="))
                    {//si le path est trouvé
                        QString ch(path.section("DOC_PATH_WIN32=",1,-1));
                        m_PATH= new QString(ch.simplified());
                    }
                    if (path.contains("DOC_INDEX="))
                    {//si le path est trouvé
                        QString ch(path.section("DOC_INDEX=",1,-1));
                        m_INDEX= new QString(ch.simplified());
                    }
                }
            }
            file->close();
        }
        else {//si le fichier de config n'existe pas, path définit par défaut
            m_INDEX = new QString("ref.html");
            if (!m_OS)
                m_PATH = new QString("doc/");
            else m_PATH = new QString(QLatin1String("doc\\"));
        }
	listPath << (*m_PATH) ;
	m_textBrowser->setSearchPaths(listPath);
	m_textBrowser->setSource(*m_INDEX);
	this->setCentralWidget(m_textBrowser);
        statusBar()->showMessage(tr("Prêt"));
	connect(m_textBrowser,SIGNAL(highlighted(QString)),this,SLOT(showMessage(QString)));
	connect(m_nextAct,SIGNAL(triggered()),m_textBrowser,SLOT(forward()));
	connect(m_previousAct,SIGNAL(triggered()),m_textBrowser,SLOT(backward()));
	connect(m_refreshAct,SIGNAL(triggered()),m_textBrowser,SLOT(reload()));
	connect(m_homeAct,SIGNAL(triggered()),m_textBrowser,SLOT(home()));
}

//destructeur
HelpWindow::~HelpWindow(){}

//slots

//affichage d'un message dans la barre d'état
void HelpWindow::showMessage(QString str)
{
	statusBar()->showMessage(str);
}
