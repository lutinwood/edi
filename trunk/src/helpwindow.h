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
#ifndef HELPWINDOW_H
#define HELPWINDOW_H
#include <QtGui>
#include <QtCore>

/**
*Classe permettant l'affichage d'une fenêtre d'aide.
*@author MANIVONG Vilayvanh & COURCELLE Julien 
*/
class HelpWindow : public QMainWindow{
	
	Q_OBJECT

	public:
		//constructeurs destructeurs
                /**
                *Constructeur
                *@param parent widget parent
                *@param f flag
                **/
		HelpWindow(QWidget * parent, Qt::WFlags f = 0);
                /**
                *destructeur
                **/
		~HelpWindow();
		
		//attributs
                /** barre d'outil de la fenêtre **/
		QToolBar *m_toolBar;
                /** chemin de recherche de la documentation **/
                QString *m_PATH;
                /** Fichier index de la documentation **/
                QString *m_INDEX;
                /** variable d'OS, vrai si windows et faux si linux **/
                bool m_OS;
                /** composant central affichant la page web **/
		QTextBrowser *m_textBrowser;
		QAction *m_nextAct;
		QAction *m_previousAct;
		QAction *m_refreshAct;
		QAction *m_homeAct;

	public slots:
                /**
                *Slot qui affiche un message dans la barre d'état
                *@param QString message à afficher
                **/
		void showMessage(QString);
};

#endif
