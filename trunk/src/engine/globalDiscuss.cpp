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
#include "globalDiscuss.h"
#include <iostream>

using namespace std;

GlobalDiscuss::GlobalDiscuss(QObject *parent) : QObject(parent)
{
#if defined(Q_OS_WIN)
	m_OS=true;
#else 
    	m_OS=false;
#endif

	m_ErrorStream=NULL;	
	m_OutputStream=NULL;
        m_prog = NULL;
        m_console = NULL;
        m_PATH = NULL;
        loadConsole();
}


GlobalDiscuss::~GlobalDiscuss(){}


QString GlobalDiscuss::getOutputStream()
{
	return *m_OutputStream;
}

QString GlobalDiscuss::getErrorStream()
{
	return *m_ErrorStream;
} 

void GlobalDiscuss::addOutputStream(QString str)
{
	if (m_OutputStream == NULL)
	    m_OutputStream = new QString(str);
        else (*m_OutputStream) += str;
}


void GlobalDiscuss::addErrorStream(QString str)
{
	if (m_ErrorStream == NULL)
	    m_ErrorStream = new QString(str);
        else (*m_ErrorStream) += str;
} 

void GlobalDiscuss::loadConsole()
{
        bool find = false;
        QStringList consoleList;
        //on regarde si le fichier de configuration est présent sur la machine
        if (QFile::exists("conf.ini"))
        {
            QFile file("conf.ini");
            file.open(QIODevice::ReadOnly);
            while (!file.atEnd() && !find)
            {
                QByteArray buffer(file.readLine());	
                QString path(buffer);
                if (m_OS)
                {//si l'OS est windows
                    if (path.contains("TERM_PATH_WIN32="))
                    {//si le path est trouvé
                        QString ch(path.section("TERM_PATH_WIN32=",1,-1));
                        consoleList = QStringList((ch.simplified()).split(","));
                        find = true;
                    }
                }
                else{//si l'OS est linux
                    if (path.contains("TERM_PATH_X11="))
                    {//si le path est trouvé
                        QString ch(path.section("TERM_PATH_X11=",1,-1));
                        consoleList = QStringList((ch.simplified()).split(","));
                        find = true;
                    }
                }
            }
            file.close();
            //fin de la recherche dans le fichier de configuration
            //on teste maintenant si une des consoles est installée
            QStringList::Iterator it;
            for (it = consoleList.begin() ; it != consoleList.end() ; it++)
            {
                if (QFile::exists((*it)))
                    m_console = new QString((*it));
            }
        }
        else {//le fichier de configuration n'existe pas
            //on charge alors les paramètres par défaut
            if (m_OS)
            {//si l'OS est windows
                QString console("C:\\WINDOWS\\system32\\cmd.exe");
                if (QFile::exists(console))
                    m_console = new QString(console);
            }
            else{//si l'OS est linux
                QStringList consoleList;
                consoleList << "/usr/bin/konsole" << "/usr/bin/eterm" << "/usr/bin/xterm" << "/usr/bin/gnome-terminal";
                //on teste maintenant si une des consoles est installée
                QStringList::Iterator it;
                for (it = consoleList.begin() ; it != consoleList.end() ; it++)
                {
                    if (QFile::exists((*it)))
                        m_console = new QString((*it));
                }
            }
        }
}

bool GlobalDiscuss::isRunning()
{
    if (m_prog !=NULL)
        return true;
    else return false;
}
