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
            /** Recherche si l'application utilis�e est pr�sente sur la machine **/
	   bool Exist();
            /** Charge les options du fichier de configuration **/
	   void FileConf();

            /** Processus g�rant un terminal auxiliaire pour linux **/
           QProcess *m_terminal;
            /** PID du processus fils lanc� par le bash **/
           int m_childPid;
			
	public slots:
            /** M�thode servant � �crire la sortie d'erreur dans le buffer de sortie d'erreur **/
	   void writeErrorStream();
            /** M�thode servant � �crire la sortie standard dans le buffer de sortie standard **/
	   void writeOutputStream();
            /** M�thode servant � lancer l'application **/
           void Execute(QString);
            /** M�thode qui affiche le buffer de sortie standard **/
           void displayOutputStream();
           /** M�thode affichant les erreurs li�es � l'application lanc�e **/
           void errorProcess(QProcess::ProcessError);
            /** M�thode affichant les changement d'�tat de l'application **/
	   void stateChanged ( QProcess::ProcessState newState );
            /** M�thode affichant la fin de l'application **/
	   void finished ( int exitCode, QProcess::ExitStatus exitStatus);
            /** M�thode qui affiche un message au lancement de l'application **/
           void started();
            /** M�thode affichant le nombre de bits envoy�s � l'application **/
           void send(qint64 bytes);
            /** M�thode qui tue l'application **/
           void kill();
            /** M�thode qui permet d'acc�der � une �tape suivante dans le debug **/
           void next();
            /** M�thode qui permet de lancer l'application **/
           void run();
            /** M�thode qui permet d'�crire une commande dans le d�bogueur**/
            void writeCmd(QString str);

        signals:
            /** Signal �mis lorsque l'on veut afficher le flux de sortie standard **/
            void displayOutputStream(QString);
            /** Signal �mis lorsque l'application n'a pas �t� trouv�e sur l'ordinateur de l'utilisateur**/
          //  void notFound(QString);
            /** Signal �mis lorsque le d�bogueur est pr�t � recevoir les breakpoints **/
            void writeBreakPoints();
            /** Signal �mis lorsque le d�bogueur est tu� pour r�ins�rer des break points **/
            void connectBreakPoints();
            void setDebugEnabled(bool);
};

#endif
