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
#ifndef GLOBALDISCUSS_H
#define GLOBALDISCUSS_H
#include <QtGui>
#include <QtCore>

#define CONFIG_FILE "conf.ini"

/**
*Classe abstraite permettant à l'interface et à un programme extèrieur de dialoguer
*/
class GlobalDiscuss : public QObject 
{
	Q_OBJECT
	
        public :
	   /**
	   *Constructeur abstrait
	   */
	   GlobalDiscuss(QObject *parent);
	   /**
	   *Destructeur
           */
	   ~GlobalDiscuss();
	   virtual bool Exist()=0;
	   virtual void Execute(QString appName)=0;
	   virtual void kill()=0;
           /**
	   *Fonction retournant m_OutputStream
           *@return QString
	   */
	   QString getOutputStream();
	   /**
	   *Fonction retournant m_ErrorStream
           *@return QString
	   */
	   QString getErrorStream();
	   /**
	   *Fonction permettant de mettre à  jour m_OutputStream
	   *@param QString str
           */
	   void addOutputStream(QString);
	   /**
	   *Fonction permettant de mettre à jour m_ErrorStream
	   *@param QString str
           */
	   void addErrorStream(QString);
	   /**
	   *Fonction permettant de tester l'existence sur le système des différentes consoles installées sous Linux et d'en choisir une
	   */
	   void loadConsole();
             /** Renvoie vrai si le gdb tourne et faux sinon **/
           bool isRunning();
	
	   bool m_OS;
           QString *m_console;
	   QString *m_PATH;
	   QString *m_OutputStream;
	   QString *m_ErrorStream;
	   QString *m_name;
	   QProcess *m_prog;


        signals:
	   /**
	   *Signal émis lorque le programme n'est pas présent dans le système au *m_PATH
	   *@param QString chemin absolu du programme
	   */
	   void notFound(QString);
};



#endif
