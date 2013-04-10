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
#ifndef MYEDIT_H
#define MYEDIT_H

#include <QtGui>
#include <QtCore>
#include <iostream>

/**
*  Widget servant � �crire une commande dans le d�bogueur.
*  @author MANIVONG Vilayvanh et Julien Courcelle
*/
class MyEdit : public QLineEdit
{
	Q_OBJECT
	private:
	QVector<QString> *m_listCmd;
	int m_nb;
	
	public:
	/**Constructeur, initialise un lineedit � vide
	*/
	MyEdit( QWidget * parent = 0 );
	/**Constructeur, initialise un Lineedit contenant la chaine contents
	*/
	MyEdit( const QString & contents, QWidget * parent = 0 );
	/**Destructeur
	*/
	~MyEdit();
	/**Fonction g�rant l'�v�nement "keyEvent"*/
	void keyPressEvent ( QKeyEvent * event );

	public slots:
	/**Fonction permettant de garder en m�moire les derni�res commandes entr�es par l'utilisateur */
	void enter(QString);


};

#endif
