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
#if defined Q_OS_WIN               
TERM_PATH = new QString("TERM_PATH_WIN32=");
CONSOLE_PATH  = new QString("C:\\WINDOWS\\system32\\cmd.exe");
#elif defined Q_OS_LINUX
QString TERM_PATH = "TERM_PATH_X11=";

#endif
                    if (path.contains(TERM_PATH))
                    {//si le path est trouvé
                        QString ch(path.section(TERM_PATH,1,-1));
                        consoleList = QStringList((ch.simplified()).split(","));
                        find = true;
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
#if defined Q_OS_WIN
                QString console("C:\\WINDOWS\\system32\\cmd.exe");
                if (QFile::exists(console))
                    m_console = new QString(console);
            
#elif defined Q_OS_LINUX

                QStringList consoleList;
                consoleList 	<< "/usr/bin/konsole"	<< "/usr/bin/gnome-terminal" 
				<< "/usr/bin/gnome-eterm" 	<< "/usr/bin/xterm";
                //on teste maintenant si une des consoles est installée
                QStringList::Iterator it;
                for (it = consoleList.begin() ; it != consoleList.end() ; it++)
                {
                    if (QFile::exists((*it)))
                        m_console = new QString((*it));
                }
#endif
            }
        
}

bool GlobalDiscuss::isRunning()
{
    if (m_prog !=NULL)
        return true;
    else return false;
}
