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
#ifndef APPLICATIONDISCUSS_H
#define APPLICATIONDISCUSS_H

#include <QtCore>
#include "globalDiscuss.h"

/**
*Classe de dialogue entre l'interface et l'application
*@author MANIVONG Vilayvanh & COURCELLE Julien 
*/
class ApplicationDiscuss : public GlobalDiscuss
{
	Q_OBJECT

	public:
            /**
            *Constructeur
            *@param parent widget parent
            **/
	   ApplicationDiscuss(QObject *parent);
            /** Destructeur **/
	   virtual ~ApplicationDiscuss();
            /** Recherche si l'application utilis�e est pr�sente sur la machine **/
	   bool Exist();
            /** M�thode qui tue l'application **/
	   void kill();
			
	public slots:
            /** M�thode servant � �crire la sortie d'erreur dans le buffer de sortie d'erreur **/
	   void writeErrorStream();
            /** M�thode servant � �crire la sortie standard dans le buffer de sortie standard **/
	   void writeOutputStream();
            /** M�thode servant � lancer l'application **/
           void Execute(QString);
           void displayOutputStream(int,QProcess::ExitStatus);
            /** M�thode affichant les erreurs li�es � l'application lanc�e **/
           void errorProcess(QProcess::ProcessError);
            /** M�thode affichant les changement d'�tat de l'application **/
	   void stateChanged ( QProcess::ProcessState newState );
            /** M�thode affichant la fin de l'application **/
	   void finished ( int exitCode, QProcess::ExitStatus exitStatus);
            /** M�thode qui affiche un message au lancement de l'application **/
           void started();

        signals:
            /** Signal �mis lorsque l'on veut afficher le flux de sortie standard **/
            void displayStream(QString);
            void setApplicationEnabled(bool);
};

#endif
