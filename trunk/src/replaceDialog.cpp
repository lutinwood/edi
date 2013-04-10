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

#include "replaceDialog.h"

using namespace std;

//constructeur
ReplaceDialog::ReplaceDialog(QWidget *parent) : QDialog(parent)
{
    m_label = new QLabel(tr("Rechercher :"));
    m_labelReplace = new QLabel(tr("Remplacer par :"));
    m_lineEdit = new QLineEdit;
    m_lineEditReplace = new QLineEdit;
    m_label->setBuddy(m_lineEdit);
    m_labelReplace->setBuddy(m_lineEditReplace);
    m_caseCheckBox = new QCheckBox(tr("Respecter la &casse"));
    m_backwardCheckBox = new QCheckBox(tr("Rechercher en &arrière"));
    m_findButton = new QPushButton(tr("&Rechercher"));
    m_findButton->setDefault(true);
    m_findButton->setEnabled(false);
    m_replaceButton = new QPushButton(tr("&Remplacer"));
    m_replaceButton->setDefault(true);
    m_replaceButton->setEnabled(false);
    m_closeButton = new QPushButton(tr("Quitter"));
    connect(m_lineEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(enableFindButton(const QString &)));
    connect(m_lineEditReplace, SIGNAL(textChanged(const QString &)),
        this, SLOT(enableReplaceButton(const QString &)));
    connect(m_findButton, SIGNAL(clicked()),
        this, SLOT(findClicked()));
    connect(m_replaceButton, SIGNAL(clicked()),
        this, SLOT(replaceClicked()));
    connect(m_closeButton, SIGNAL(clicked()),
        this, SLOT(close()));
    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(m_label);
    topLeftLayout->addWidget(m_lineEdit);
    QHBoxLayout *topLeftLayout2 = new QHBoxLayout;
    topLeftLayout2->addWidget(m_labelReplace);
    topLeftLayout2->addWidget(m_lineEditReplace);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(topLeftLayout2);
    leftLayout->addWidget(m_caseCheckBox);
    leftLayout->addWidget(m_backwardCheckBox);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(m_findButton);
    rightLayout->addWidget(m_replaceButton);
    rightLayout->addWidget(m_closeButton);
    rightLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Remplacer"));
    setFixedHeight(sizeHint().height());
	findBefore = false;
	findAfter = false;
}

//slots

//bouton Recherche cliqué
void ReplaceDialog::findClicked()
{
    QString text = m_lineEdit->text();
    Qt::CaseSensitivity cs =
          m_caseCheckBox->isChecked() ? Qt::CaseSensitive
                                    : Qt::CaseInsensitive;
    if (m_backwardCheckBox->isChecked()) {
       emit findPrevious(text, cs);
		findBefore = true;
		findAfter = false;
	} else {
    	emit findNext(text, cs);
		findBefore = false;	
		findAfter = true; 
	}
}

//Bouton Remplacer cliqué
void ReplaceDialog::replaceClicked()
{
	emit replace(m_lineEditReplace->text());
}

//rend le bouton Recherche actif lorsque l'on rentre une expression dans la boîte de dialogue 
void ReplaceDialog::enableFindButton(const QString &text)
{
    m_findButton->setEnabled(!text.isEmpty());
}

//rend le bouton Remplacer actif lorsque l'on rentre une expression dans la boîte de dialogue 
void ReplaceDialog::enableReplaceButton(const QString &text)
{
    m_replaceButton->setEnabled(!text.isEmpty());
}
