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
#include "applicationdiscuss.h"
#include <iostream>

using namespace std;

ApplicationDiscuss::ApplicationDiscuss(QObject *parent) : GlobalDiscuss(parent)
{
        connect(this,SIGNAL(displayStream(QString)),parent,SLOT(displayApplication(QString)));
}

//destructeur
ApplicationDiscuss::~ApplicationDiscuss(){}

//test si l'application existe sur la machine
bool ApplicationDiscuss::Exist(){return QFile::exists(*m_PATH);}

//tue l'application en cours
void ApplicationDiscuss::kill()
{
    if (m_prog!=NULL)
    {
        m_prog->kill();
        m_prog->waitForFinished(3000);
        delete m_prog;
        m_prog = NULL;
    }
    emit setApplicationEnabled(false);
}

//écriture du flux d'erreur
void ApplicationDiscuss::writeErrorStream()
{
        QString strError;
	if (m_ErrorStream==NULL)
        {
            strError = QString(m_prog->readAllStandardError());
	    m_ErrorStream = new QString(strError);	
        }
        else  {
            strError = QString(m_prog->readAllStandardError());
            (*m_ErrorStream) += (*m_ErrorStream)+strError;
        }

        emit displayStream(*m_ErrorStream);//Debug
}


void ApplicationDiscuss::writeOutputStream()
{
        QString strOutput;
	if (m_OutputStream==NULL)
        {
            strOutput = QString(m_prog->readAllStandardOutput());
	    m_OutputStream = new QString(strOutput);
        }
        else {
            strOutput = QString(m_prog->readAllStandardOutput());
            (*m_OutputStream) += strOutput;
        }
        emit displayStream(*m_OutputStream); // debug
}

//redéfinition de la fonction Execute
void ApplicationDiscuss::Execute(QString appName)
{
    if (m_console != NULL)
    {//si une console existe on lance l'application
        emit setApplicationEnabled(true);
        //on récupère la chaîne sans le .pas
        QString str_tmp(appName.leftJustified(appName.length()-4,'.',true));
        //on rajoute le .exe si l'OS est windows
       // if (m_OS)
#if defined(_Win32)|| defined(__Win64)
            str_tmp += tr(".exe");
#endif
        if (m_prog == NULL)
            m_prog = new QProcess(this);
        if (m_OutputStream!=NULL)
            m_OutputStream->clear();
        connect(m_prog,SIGNAL( readyReadStandardError()),this,SLOT(writeErrorStream()));
        connect(m_prog,SIGNAL( readyReadStandardOutput()),this,SLOT(writeOutputStream()));
        connect(m_prog,SIGNAL(started()),this,SLOT(started()));
        connect(m_prog,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(finished(int,QProcess::ExitStatus)));
        QStringList list;  
      
#if defined(__linux__)
	
            list << "-e" << str_tmp ;
            m_prog->start((*m_console),list);
#elif defined(_Win32)
            m_prog->start((*m_console));
            //puis on lance l'application avec start
            m_prog->write(QString(tr("start /Q ")+str_tmp+tr("\n")).toLatin1());
            m_prog->write("exit\n");
#endif  
    }
}

void ApplicationDiscuss::displayOutputStream(int exitCode, QProcess::ExitStatus exitStatus )
{
   // QStringList str(m_OutputStream->split("\n"));

    //emit displayStream(str);//debug
}

void ApplicationDiscuss::errorProcess(QProcess::ProcessError error)
{
    QString str("Erreur de type : ");
    str += error;
    if (m_OutputStream == NULL)
        m_OutputStream = new QString(str);
    else (*m_OutputStream) += str;

    emit displayStream(*m_OutputStream);//debug
}

void ApplicationDiscuss::stateChanged ( QProcess::ProcessState newState )
{
    QString str("");
    switch (newState){
        case QProcess::Running :
            str += "Changement d'état : En cours\n";
            break;	
        case QProcess::Starting : 
            str += "Changement d'état : Démarrage\n";
            break;
        case QProcess::NotRunning : 
            str += "Changement d'état : Terminé\n";
            kill();
            break;
    }
    if (m_OutputStream == NULL)
        m_OutputStream = new QString(str);
    else (*m_OutputStream) += str;

    emit displayStream(*m_OutputStream);//debug
}

void ApplicationDiscuss::finished ( int exitCode, QProcess::ExitStatus exitStatus)
{
    QString str("Fin de l'application.\n");
    if (m_OutputStream == NULL)
        m_OutputStream = new QString(str);
    else (*m_OutputStream) += str;
    
	emit displayStream(*m_OutputStream);//debug
	    
	kill();
}

void ApplicationDiscuss::started()
{
    QString str("Démarrage de l'application\n");
    if (m_OutputStream == NULL)
        m_OutputStream = new QString(str);
    else (*m_OutputStream) += str;

    emit displayStream(*m_OutputStream);//debug
}
