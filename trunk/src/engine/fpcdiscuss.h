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
#ifndef FPCDISCUSS_H
#define FPCDISCUSS_H

#include <QtCore>
#include "globalDiscuss.h"

/**
*Classe de dialogue entre l'interface et Fpc
*@author MANIVONG Vilayvanh & COURCELLE Julien 
*/
class FpcDiscuss : public GlobalDiscuss
{
	Q_OBJECT

	public:
	   /**
	   *Constructeur
	   */
	   FpcDiscuss(QObject *parent);
	   /**
	   *Destructeur
	   */
	   virtual ~FpcDiscuss();
	   /**
	   *Fonction permettant de savoir si le fpc est présent dans le chemin indiqué par *m_PATH
	   *@return bool vrai s'il est présent et faux sinon
           */
	   bool Exist();
	   /**	
	   *Fonction permettant de stopper le programme
	   */
	   void kill();
           /**
	   *Fonction permettant de charger le fichier "conf" contenant le chemin de fpc, ainsi que d'autres informations
           */
	   void FileConf();


	public slots:
	   /**
           *Fonction permettant d'écrire le flux d'erreurs du programme exÃ©cutÃ© dans m_ErrorStream
	   */
	   void writeErrorStream();
	   /**
	   *Fonction permettant d'écrire le flux de sortie du programme dabs m_OutputStream
	   */
	   void writeOutputStream();
	   
	   /**
	   *Fonction permettant de lancer la compilation du fichier courant
	   *@param QString nom du fichier courant
           */
           void Execute(QString);

            /** Méthode affichant la fin de l'application **/
	   void finished ( int exitCode, QProcess::ExitStatus exitStatus);

        signals:
	    /**
	    *Signal émis pour envoyer le message qui sera affiché dans l'onglet compilateur
	    *@param QString message 
	    */
            void displayStream(QString);
	    /**
	    *Signal émis pour envoyer le message qui sera affiché dans l'onglet compilateur
	    *@param bool vrai pour activer les composants graphiques liés à la compilation ou faux sinon
	    */
            void setCompileEnabled(bool);
};

#endif
