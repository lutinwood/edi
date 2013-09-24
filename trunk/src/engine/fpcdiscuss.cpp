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
#include "fpcdiscuss.h"
#include <iostream>

using namespace std;

FpcDiscuss::FpcDiscuss(QObject *parent) : GlobalDiscuss(parent)
{
	FileConf();
        connect(this,SIGNAL(notFound(QString)),parent,SLOT(appNotFound(QString)));
        if (!Exist()) 
        {
            QString str("Le compilateur FreePascal n'est pas present");
            emit notFound(str);
        }
        else {
           connect(this,SIGNAL(displayStream(QString)),parent,SLOT(displayCompile(QString))); 
        }
}

//destructeur
FpcDiscuss::~FpcDiscuss(){}

//test si l'application existe sur la machine
bool FpcDiscuss::Exist(){return QFile::exists(*m_PATH);}

//tue l'application en cours
void FpcDiscuss::kill()
{
    if (m_prog!=NULL)
    {
        m_prog->kill();
        m_prog->waitForFinished(3000);
        delete m_prog;
        m_prog = NULL;
    }
    emit setCompileEnabled(false);
}

//charge le chemin du path à partir d'un fichier conf
void FpcDiscuss::FileConf()
{
    bool find = false;
    if (QFile::exists(QString("conf.ini"))) 
    {
        QFile *file = new QFile(QString("conf.ini"));
        file->open(QIODevice::ReadOnly);
        if (!m_OS)
       
#if defined (__linux__) 
            while (!file->atEnd() && !find)
            {
                QByteArray buffer(file->readLine());	
                QString path(buffer);
                if (path.contains("FPC_PATH_X11="))
                {//si le path est trouvé
                    QString ch(path.section("FPC_PATH_X11=",1,-1));
                    m_PATH= new QString(ch.simplified());
                    find = true;
                }
            }
#elif defined (_WIN32) ||(_WIN64) 
       
            while (!file->atEnd() && !find)
            {
                QByteArray buffer(file->readLine());	
                QString path(buffer);
                if (path.contains("FPC_PATH_WIN32="))
                {//si le path est trouvé
                    QString ch(path.section("FPC_PATH_WIN32=",1,-1));
                    m_PATH= new QString(ch.simplified());
                    find = true;
                }
            }
#endif
        file->close();
    }
    else {//si le fichier de config n'existe pas, path définit par défaut
#if defined (__linux__)
            	m_PATH = new QString("/usr/bin/fpc");
#elif defined (_WIN32) || (_WIN64)
		m_PATH = new QString("C:\\FPC\\2.1.4\\bin\\i386-win32\\fpc.exe");
#endif
	}
}

//écriture du flux d'erreur
void FpcDiscuss::writeErrorStream()
{
    this->addErrorStream(m_prog->readAllStandardError());
    emit displayStream(*m_OutputStream);
}


void FpcDiscuss::writeOutputStream()
{
    this->addOutputStream(m_prog->readAllStandardOutput());
    emit displayStream(*m_OutputStream);
}


//execute l'application de compilation
void FpcDiscuss::Execute(QString appName)
{
    if (Exist())
    {//si l'application est présente sur la machine
        if (!appName.contains(" "))
        {
            if (m_prog == NULL)
                m_prog = new QProcess(this);
            connect(m_prog,SIGNAL( readyReadStandardError()),this,SLOT(writeErrorStream()));
            connect(m_prog,SIGNAL( readyReadStandardOutput()),this,SLOT(writeOutputStream()));
            connect(m_prog,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(finished(int,QProcess::ExitStatus)));
            //on récupère le type de traitement du message de compilation
            if (appName!="")
            {
                emit setCompileEnabled(true);
                //on vide les buffers de flux de sortie standard et d'erreur
                if ((this->m_OutputStream)!=NULL)
                    (this->m_OutputStream)->clear();
                if ((this->m_ErrorStream)!=NULL)
                    (this->m_ErrorStream)->clear();
                QStringList listArg;
                bool find = false;
                if (QFile::exists("conf.ini"))
                {//si le fichier de configuration existe
                    QFile file("conf.ini");
                    file.open(QIODevice::ReadOnly);
                    while (!file.atEnd() && !find)
                    {
                        QByteArray buffer(file.readLine());	
                        QString path(buffer);
                        if (path.contains("FPC_ARGS="))
                        {
                            QString ch(path.section("FPC_ARGS=",1,-1));
                            listArg = (ch.simplified()).split(" ");
                            find = true;
                        }
                    }
                    listArg << appName;
                    file.close();
                }
                else listArg << "-g" << appName;
                m_prog->start((*m_PATH),listArg);
            }
        }
        else {
            QString strErr("Le compilateur FPC n'autorise pas de chemin de fichier contenant un espace. Veuillez enregistrer votre document sous un autre nom ou dans un autre dossier ne contenant pas d'espace.");
            QMessageBox::warning(NULL,QObject::tr("Erreur de fichier"),strErr);
            if (m_OutputStream != NULL)
            {
                delete m_OutputStream;
                m_OutputStream = NULL;
            }
        }
    }
}


void FpcDiscuss::finished (int, QProcess::ExitStatus){kill();}
