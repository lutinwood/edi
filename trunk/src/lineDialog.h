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
#ifndef LINEDIALOG_H
#define LINEDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

/**
*Fen�tre de dialogue permettant d'interagir avec l'utilisateur lorsqu'il souhaite d�placer le curseur dans l'�diteur de texte
*@author MANIVONG Vilayvanh et COURCELLE Julien
*/
class LineDialog : public QDialog
{
    Q_OBJECT
    
    public:
	/**
	*Constructeur
	*/
        LineDialog(QWidget *parent);
        /**
	*Fonction permettant d'effacer le contenu de le lineEdit
	*/
	void clear();

    signals:
       /**
	*Signal �mis pour d�placer le curseur dans l'�diteur de texte
	*@param int num�ro de la ligne 
	*@param int num�ro de la colonne
	*/
       void goTo(int,int);
	/**
	*Signal �mis lorsque l'utilisateur entre un caract�re diff�rent d'un chiffre
	*/
       void warningNumber();

    private slots:
	/**
	*Fonction permettant de g�rer le clique sur le bouton "go", cette fonction est chargé d'envoyé les diff�rents signals
	*/
       void goClicked();
	/**
	*Fonction permettant de d�sactiver le bouton go lorsque la lineEdit est vide 
	*/
       void enableOkButton(const QString &text);

    private:
       QLabel *m_label;
       QLineEdit *m_lineEdit;
       QPushButton *m_goButton;
       QPushButton *m_closeButton;
};
#endif
