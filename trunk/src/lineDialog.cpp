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

#include "lineDialog.h"

using namespace std;

//constructeur
LineDialog::LineDialog(QWidget *parent) : QDialog(parent)
{
    m_label = new QLabel(tr("Aller à  :"));
    m_lineEdit = new QLineEdit;
    m_label->setBuddy(m_lineEdit);
    m_goButton = new QPushButton(tr("&Ok"));
    m_goButton->setDefault(true);
    m_goButton->setEnabled(false);
    m_closeButton = new QPushButton(tr("Quitter"));
    connect(m_lineEdit, SIGNAL(textChanged(const QString &)),
        this, SLOT(enableOkButton(const QString &)));
	
	connect(m_goButton, SIGNAL(clicked()),
        this, SLOT(goClicked()));
    connect(m_closeButton, SIGNAL(clicked()),
        this, SLOT(close()));
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_label);
    topLayout->addWidget(m_lineEdit);
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_goButton);
    bottomLayout->addWidget(m_closeButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Aller à "));
    setFixedHeight(sizeHint().height());
}

//methodes 

//supprime le contenu de la boîte de dialogue
void LineDialog::clear()
{
	m_lineEdit->clear();
}


//slots

//bouton OK cliqué
void LineDialog::goClicked()
{
    QString str(m_lineEdit->text());
    int i=0;
    bool ok=true;
    while ((i<str.length())&&(ok==true))
    { 
	if (!str.at(i).isNumber()) 
	{	
		emit warningNumber();
		ok =false;
 	}
	i++; 
    }	
    if (ok==true) 
    {
	emit goTo(str.toInt()-1,0);
    	close();
    }
}

//rend le bouton OK actif lorsque l'on rentre un nombre dans la boîte de dialogue
void LineDialog::enableOkButton(const QString &text)
{
    m_goButton->setEnabled(!text.isEmpty());
}

