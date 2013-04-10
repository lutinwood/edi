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
#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
/**
*Fen�tre de dialogue permettant d'interagir avec l'utilisateur lorsqu'il souhaite trouver une chaine de caratère dans l'�diteur de texte
*@author MANIVONG Vilayvanh et COURCELLE Julien
*/
class FindDialog : public QDialog
{
    Q_OBJECT
    
    public:
	/**
	*Constructeur
	*/
        FindDialog(QWidget *parent);

	bool m_findBefore, m_findAfter;

    signals:
       /**
       *Signal �mis lorsque l'utilisateur demande de trouver la prochaine occurence de la chaine de caract�re.
       */
       void findNext(const QString &str, Qt::CaseSensitivity cs);
       /**
       *Signal �mis lorsque l'utilisateur demande de trouver l'occurence pr�c�dente de la chaine de caract�re.
       */	
       void findPrevious(const QString &str, Qt::CaseSensitivity cs);
   
    private slots:
       /**
       *Fonction permettant de placer le curseur sur la prochaine ou pr�c�dente occurence de la chaine de caract�re.
       */	
       void findClicked();
       /**
       *Fonction permettant de d�sactiver le bouton "rechercher"
       *@param QString texte contenu dans le lineEdit
       */			
       void enableFindButton(const QString &text);
       /**
       *fonction permettant d'initialiser les variables lors d'un changement d'�tat des cases � cocher
       */
       void stateChanged(int);
    
    private:
       QLabel *m_label;
       QLineEdit *m_lineEdit;
       QCheckBox *m_caseCheckBox;
       QCheckBox *m_backwardCheckBox;
       QPushButton *m_findButton;
       QPushButton *m_closeButton;
};

#endif
