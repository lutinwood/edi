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
#include "gdbdiscuss.h"
#include <iostream>
#if defined(Q_OS_WIN)
#else
    #include <QtTest/qtest.h>
    #include <QtTest/qtestcase.h>
    #include <QtTest/qtestsystem.h>
    #include <sys/types.h>
#endif


using namespace std;

GdbDiscuss::GdbDiscuss(QObject *parent) : GlobalDiscuss(parent)
{
	FileConf();
        m_terminal = NULL;
        m_childPid = -1;
        connect(this,SIGNAL(notFound(QString)),parent,SLOT(appNotFound(QString)));
        if (!Exist())
        {
            QString str("Le débogueur gdb n'est pas present");
            emit notFound(str);
        }
        else {
           connect(this,SIGNAL(displayOutputStream(QString)),parent,SLOT(displayDebug(QString)));
           connect(this,SIGNAL(writeBreakPoints()),parent,SLOT(writeBreakPoints()));
           connect(this,SIGNAL(connectBreakPoints()),parent,SLOT(connectBreakPoints()));
        }
}

//destructeur
GdbDiscuss::~GdbDiscuss(){}

//test si l'application existe sur la machine
bool GdbDiscuss::Exist(){return QFile::exists(*m_PATH);}

//charge le chemin du path à partir d'un fichier conf
void GdbDiscuss::FileConf()
{
    bool find = false;
    if (QFile::exists(QString("conf.ini"))) 
    {
        QFile *file = new QFile(QString("conf.ini"));
        file->open(QIODevice::ReadOnly);
#if defined Q_OS_LINUX
      QString  gdb_path = "GDB_PATH_X11=";
#elif defined Q_OS_WIN
      gdb_path = new QString("GDB_PATH_WIN32=");
#endif
            while (!file->atEnd() && !find)
            {
                QByteArray buffer(file->readLine());	
                QString path(buffer);
                if (path.contains(gdb_path))
                {//si le path est trouvé
                    QString ch(path.section(gdb_path,1,-1));
                    m_PATH= new QString(ch.simplified());
                    find = true;
                }
            }
        
        file->close();
    }
    else {//si le fichier de config n'existe pas, path définit par défaut
#if defined Q_OS_LINUX
	m_PATH = new QString("/usr/bin/gdb");
#elif defined Q_OS_WIN
	m_PATH = new QString(QLatin1String("C:\\MinGw\\bin\\gdb.exe"));
#endif
    }
}


//écriture du flux d'erreur
void GdbDiscuss::writeErrorStream()
{
        QString strOutput;
	if (m_OutputStream==NULL)
        {
            strOutput = QString(m_prog->readAllStandardError());
	    m_OutputStream = new QString(strOutput);
        }
        else {
            strOutput = QString(m_prog->readAllStandardError());
            (*m_OutputStream) += strOutput;
        }
        emit displayOutputStream((*m_OutputStream));
}


void GdbDiscuss::writeOutputStream()
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
        emit displayOutputStream((*m_OutputStream));
}


//redéfinition de la fonction Execute
void GdbDiscuss::Execute(QString appName)
{
    if ( m_console!=NULL && Exist() && m_prog == NULL)
    {
        //on récupère la chaîne sans le .pas
        QString str_tmp(appName.leftJustified(appName.length()-4,'.',true));
 #if defined Q_OS_WIN
            str_tmp += tr(".exe");
#endif
        m_prog = new QProcess(this);
            if (m_terminal == NULL)
                m_terminal = new QProcess(this);
       
	 connect(m_prog,
		SIGNAL( readyReadStandardError()),
		this,SLOT(writeErrorStream()));
        connect(m_prog,
		SIGNAL( readyReadStandardOutput()),
		this,SLOT(writeOutputStream()));
        connect(m_prog,
		SIGNAL(finished(int,QProcess::ExitStatus)),
		this,SLOT(finished(int,QProcess::ExitStatus)));
        connect(m_prog,
		SIGNAL(started()),
		this,SLOT(started()));
        connect(m_prog,
		SIGNAL(bytesWritten(qint64)),
		this,SLOT(send(qint64)));
        if (appName!="")
        {
            emit setDebugEnabled(true);
            QStringList listArg;
#if defined Q_OS_LINUX
                if (QFile::exists(("/tmp/montty_xxxxx")))
                {
                    QFile fileRemoveTty("/tmp/montty_xxxxx");
                    fileRemoveTty.remove();
                    fileRemoveTty.close();
                }
                if (QFile::exists(("/tmp/monpid_xxxxx")))
                {
                    QFile fileRemovePid("/tmp/monpid_xxxxx");
                    fileRemovePid.remove();
                    fileRemovePid.close();
                }
                listArg << str_tmp << "--interpreter=mi";
                m_prog->start((*m_PATH),listArg);
                //on met le gdb en mode pascal
                m_prog->write("set language pascal\n");
                //on démarre un bash qui servira à redirriger la sortie standard de gdb
                m_terminal->start("/bin/sh");
                cout << "pid : " << m_terminal->pid() << endl;
                cout << (*m_console).toStdString() << endl;
                if ((*m_console).contains("konsole"))
                    m_terminal->write((tr(	
		"konsole -caption 'Terminal' -e sh -c\
		'tty >> /tmp/montty_xxxxx; ps >> /tmp/monpid_xxxxx;\
		 trap INT QUIT TSTP; exec<&-; exec>&-; while :; do sleep 3600;\
		 done '\n")).toLatin1());
	
                if ((*m_console).contains("gnome-terminal"))
                    m_terminal->write(((*m_console)+tr(
		" --command=\"sh -c 'tty >> /tmp/montty_xxxxx; ps >> /tmp/monpid_xxxxx;\
		 trap INT QUIT TSTP; exec<&-;exec>&-; while :;do sleep 3600;\
		done'\"\n")).toLatin1());
	
                if ((*m_console).contains("xterm"))
                    m_terminal->write((tr(
		"xterm -e sh -c 'tty >> /tmp/montty_xxxxx; \
		ps >> /tmp/monpid_xxxxx; trap INT QUIT TSTP; exec<&-; exec>&-; while :; \
		do sleep 3600; done '\n")).toLatin1());
            
                int i = 0;
                    while (!QFile::exists("/tmp/montty_xxxxx") 
			&& !QFile::exists("/tmp/monpid_xxxxx") && i++ < 8)
                        
			QTest::qWait(500);
                
                //on récupère le tty de la commande lancée
                if (QFile::exists("/tmp/montty_xxxxx"))
                {
                    QString ch("");
                    while(ch.size() == 0)
                    {
                        QFile fileOpenTty("/tmp/montty_xxxxx");
                        fileOpenTty.open(QIODevice::ReadOnly);
                        fileOpenTty.waitForReadyRead(3000);
                        QByteArray buffer(fileOpenTty.readLine());
                        ch = QString(buffer);
                        //cout << ch.toStdString() << endl;
                        fileOpenTty.close();
                    }
                    m_prog->write((tr("tty ")+ch.simplified()+tr("\n")).toLatin1());
                    emit writeBreakPoints();
                }
                //on récupère le pid du terminal lancé pour pouvoir le tuer après
                if (QFile::exists("/tmp/monpid_xxxxx"))
                {
                    QString str("");
                    while (str.size() == 0)
                    {
                        QFile fileOpenPid("/tmp/monpid_xxxxx");
                        fileOpenPid.open(QIODevice::ReadOnly);
                        fileOpenPid.waitForReadyRead(3000);
                        fileOpenPid.readLine();
                        str = QString(((
			QString(fileOpenPid.readLine())).simplified()).section("pts",0,0));
                        bool ok;
                        m_childPid = (str.simplified()).toInt(&ok,10);
//                         cout << "pid fils : " << str.toStdString() << endl;
                        fileOpenPid.close();
                    }
                }
            
#elif defined Q_OS_WIN
                listArg << str_tmp << "--interpreter=mi";
                m_prog->start((*m_console));
                m_prog->write(QByteArray(
			QString((*m_PATH)+tr(" ")+str_tmp+"\n").toLatin1()));
                //on met le gdb en mode pascal
                m_prog->write("set language pascal\n");
#endif
                emit writeBreakPoints();

            
        }
    }
}

void GdbDiscuss::displayOutputStream()
{
    emit displayOutputStream(*m_OutputStream);
}

void GdbDiscuss::errorProcess(QProcess::ProcessError error)
{
    QString str("Erreur de type : ");
    str += error;
    if (m_OutputStream == NULL)
        m_OutputStream = new QString(str);
    else (*m_OutputStream) += str;
    //emit displayOutputStream(*m_OutputStream);
}

void GdbDiscuss::stateChanged ( QProcess::ProcessState newState )
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
            //une fois le gdb quitter on reconnecte les break points
            emit connectBreakPoints();
            kill();
            break;
    }
    if (m_OutputStream == NULL){
        m_OutputStream = new QString(str);
    }else {(*m_OutputStream) += str;
    }
	emit displayOutputStream(*m_OutputStream);
}


void GdbDiscuss::finished ( int exitCode, QProcess::ExitStatus exitStatus)
{
    kill();
    QString str("Fin de l'application.");
//     str += exitCode;
    if (m_OutputStream == NULL){
        	m_OutputStream = new QString(str);
    }else{
 		(*m_OutputStream) += str;
	}
    emit displayOutputStream(*m_OutputStream);
    emit connectBreakPoints();
    emit setDebugEnabled(false);
}

void GdbDiscuss::started()
{
    QString str("Démarrage de l'application.\n");
    if (m_OutputStream == NULL)
        m_OutputStream = new QString(str);
    else (*m_OutputStream) += str;
    emit displayOutputStream(*m_OutputStream);
}

void GdbDiscuss::send(qint64 bytes)
{
  /*  cout << "send : " << bytes << endl;*/
}

//tue l'application en cours
void GdbDiscuss::kill()
{
    #if defined(Q_OS_WIN)
    #else
        if (m_childPid != -1 && m_childPid != 0)
        {//si le pid c'est affiché dans le fichier on kill le processus
            QString str(tr("kill ")+QString::number(m_childPid,10));
            cout << str.toStdString() << endl;
            system(str.toStdString().c_str());
            m_childPid = -1;
        }
        if (m_terminal != NULL)
        {
            m_terminal->kill();
            m_terminal->waitForFinished(3000);
            delete m_terminal;
            m_terminal = NULL;
        }
    #endif
    if (m_prog != NULL)
    {
        m_prog->kill();
        m_prog->waitForFinished(3000);
        delete m_prog;
        m_prog = NULL;
    }
    emit setDebugEnabled(false);
}

//passe à l'étape suivante lors du debug
void GdbDiscuss::next()
{
    if (m_prog != NULL)
        m_prog->write("next\n");
    if (m_OutputStream !=NULL)
    {
        (*m_OutputStream) += tr("next\n");
        emit displayOutputStream(*m_OutputStream);
    }
}

void GdbDiscuss::run()
{
    if (m_prog != NULL)
        m_prog->write("run\n");
    if (m_OutputStream !=NULL)
    {
        (*m_OutputStream) += tr("run\n");
        emit displayOutputStream(*m_OutputStream);
    }
}

void GdbDiscuss::writeCmd(QString str)
{
    if (m_prog != NULL)
    {
        QByteArray buffer(QString(str+tr("\n")).toLatin1());
        m_prog->write(buffer);
        if ((this->m_OutputStream) != NULL)
            (*(this->m_OutputStream)) += str+tr("\n");
        this->displayOutputStream();
        #if defined(Q_OS_WIN)
        if (str.contains("quit"))
        {
            m_prog->waitForBytesWritten(3000);
            m_prog->write("exit\n");
        }
        #endif
    }
}
