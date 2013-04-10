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
#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

/**
*  Bo�te de dialogue pour le remplacement de cha�nes de caract�res.
*  @author MANIVONG Vilayvanh et Julien Courcelle
*/
class ReplaceDialog : public QDialog
{
    Q_OBJECT
    
    public:
        /**
        *Constructeur
        *@param parent widget parent
        **/
        ReplaceDialog(QWidget *parent);
        /** Variable de recherche (avant ou apr�s) **/
	bool findBefore, findAfter;

    signals:
        /**
        *Signal �mis lors de la recherche du mot suivant.
        *@param str mot � rechercher
        *@param cs sensibilit� � la casse
        **/
       void findNext(const QString &str, Qt::CaseSensitivity cs);
        /**
        *Signal �mis lors de la recherche du mot pr�c�dent.
        *@param str mot � rechercher
        *@param cs sensibilit� � la casse
        **/
       void findPrevious(const QString &str, Qt::CaseSensitivity cs);
        /**
        *Signal �mis lors du remplacement d'un mot.
        *@param str mot � rechercher
        **/
       void replace(QString);
   
    private slots:
        /** G�re les actions lorsque l'on clique sur le bouton de recherche **/
       void findClicked();
        /** G�re les actions lorsque l'on clique su le bouton remplacer **/
       void replaceClicked();
        /** G�re l'activit� du bouton de recherche **/
       void enableFindButton(const QString &text);
        /** G�re l'activit� du bouton remplacer **/
       void enableReplaceButton(const QString &text);

    private:
       QLabel *m_label;
       QLabel *m_labelReplace;
       QLineEdit *m_lineEdit;
       QLineEdit *m_lineEditReplace;
       QCheckBox *m_caseCheckBox;
       QCheckBox *m_backwardCheckBox;
       QPushButton *m_findButton;
       QPushButton *m_replaceButton;
       QPushButton *m_closeButton;
};
#endif
