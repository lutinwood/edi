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
#ifndef GDBDISCUSS_H
#define GDBDISCUSS_H

#include <QtCore>
#include "globalDiscuss.h"

/**
*Classe de dialogue entre l'interface et Gdb
*@author MANIVONG Vilayvanh & COURCELLE Julien 
*/
class GdbDiscuss : public GlobalDiscuss
{
	Q_OBJECT

	public:
            /**
            *Constructeur
            *@param parent widget parent
            */
	   GdbDiscuss(QObject *parent);
            /** Destructeur **/
	   virtual ~GdbDiscuss();
            /** Recherche si l'application utilisée est présente sur la machine **/
	   bool Exist();
            /** Charge les options du fichier de configuration **/
	   void FileConf();

            /** Processus gérant un terminal auxiliaire pour linux **/
           QProcess *m_terminal;
            /** PID du processus fils lancé par le bash **/
           int m_childPid;
			
	public slots:
            /** Méthode servant à écrire la sortie d'erreur dans le buffer de sortie d'erreur **/
	   void writeErrorStream();
            /** Méthode servant à écrire la sortie standard dans le buffer de sortie standard **/
	   void writeOutputStream();
            /** Méthode servant à lancer l'application **/
           void Execute(QString);
            /** Méthode qui affiche le buffer de sortie standard **/
           void displayOutputStream();
           /** Méthode affichant les erreurs liées à l'application lancée **/
           void errorProcess(QProcess::ProcessError);
            /** Méthode affichant les changement d'état de l'application **/
	   void stateChanged ( QProcess::ProcessState newState );
            /** Méthode affichant la fin de l'application **/
	   void finished ( int exitCode, QProcess::ExitStatus exitStatus);
            /** Méthode qui affiche un message au lancement de l'application **/
           void started();
            /** Méthode affichant le nombre de bits envoyés à l'application **/
           void send(qint64 bytes);
            /** Méthode qui tue l'application **/
           void kill();
            /** Méthode qui permet d'accéder à une étape suivante dans le debug **/
           void next();
            /** Méthode qui permet de lancer l'application **/
           void run();
            /** Méthode qui permet d'écrire une commande dans le débogueur**/
            void writeCmd(QString str);

        signals:
            /** Signal émis lorsque l'on veut afficher le flux de sortie standard **/
            void displayOutputStream(QString);
            /** Signal émis lorsque l'application n'a pas été trouvée sur l'ordinateur de l'utilisateur**/
          //  void notFound(QString);
            /** Signal émis lorsque le débogueur est prêt à recevoir les breakpoints **/
            void writeBreakPoints();
            /** Signal émis lorsque le débogueur est tué pour réinsérer des break points **/
            void connectBreakPoints();
            void setDebugEnabled(bool);
};

#endif
