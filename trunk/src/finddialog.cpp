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
#include <QtGui>

#include <iostream>


#include "finddialog.h"

//constructeur
FindDialog::FindDialog(QWidget *parent) : QDialog(parent)
{
    m_label = new QLabel(tr("Rechercher :"));
    m_lineEdit = new QLineEdit;
    m_label->setBuddy(m_lineEdit);
    m_caseCheckBox = new QCheckBox(tr("Respecter la &casse"));
    m_backwardCheckBox = new QCheckBox(tr("Rechercher en &arrière"));
    m_findButton = new QPushButton(tr("&Rechercher"));
    m_findButton->setDefault(true);
    m_findButton->setEnabled(false);
    m_closeButton = new QPushButton(tr("Quitter"));
    connect(m_lineEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(enableFindButton(const QString &)));
    connect(m_findButton, SIGNAL(clicked()),
        this, SLOT(findClicked()));
    connect(m_closeButton, SIGNAL(clicked()),
        this, SLOT(close()));
    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(m_label);
    topLeftLayout->addWidget(m_lineEdit);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(m_caseCheckBox);
    leftLayout->addWidget(m_backwardCheckBox);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(m_findButton);
    rightLayout->addWidget(m_closeButton);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Rechercher"));
    setFixedHeight(sizeHint().height());
    m_findBefore = false;
    m_findAfter = false;
    connect(m_backwardCheckBox,SIGNAL(stateChanged(int)),this,SLOT(stateChanged(int)));
    connect(m_caseCheckBox,SIGNAL(stateChanged(int)),this,SLOT(stateChanged(int)));
}

//slots

//bouton de recherche cliqué
void FindDialog::findClicked()
{
    QString text = m_lineEdit->text();
    Qt::CaseSensitivity cs =
          m_caseCheckBox->isChecked() ? Qt::CaseSensitive
                                    : Qt::CaseInsensitive;
    if (m_backwardCheckBox->isChecked()) {
       emit findPrevious(text, cs);
		m_findBefore = true;
		m_findAfter = false;
	} else {
    	emit findNext(text, cs);
		m_findBefore = false;	
		m_findAfter = true; 
	}
}

//rend le bouton de recherche actif lorsqu'une expresion est rentré dans la boîte de dialogue
void FindDialog::enableFindButton(const QString &text)
{
    m_findButton->setEnabled(!text.isEmpty());
}

//fonction qui sert à initialiser les variables lors d'un changement d'état des cases à cocher
void FindDialog::stateChanged(int state)
{
    m_findBefore = false;
    m_findAfter = false;
}
