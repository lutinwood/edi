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

#include <iostream>

#include "listwidget.h"

using namespace std;

//constructeur
ListWidget::ListWidget(QWidget *parent) : QListWidget(parent)
{
	createMenu();
	m_exitDisplay = NULL;
        
	connect(this,SIGNAL(itemDoubleClicked(QListWidgetItem *)),
	parent,SLOT(itemDoubleClicked(QListWidgetItem *)));
        
	connect(parent,SIGNAL(message(int)),this,SLOT(message(int)));
}

//destructeur
ListWidget::~ListWidget(){}


//méthodes

void ListWidget::createMenu()
{
	m_courtAct = new QAction(QString("Message court"),this);
	m_courtAct->setCheckable(true);
     	m_moyenAct = new QAction(QString("Message moyen"),this);
	m_moyenAct->setCheckable(true);
	m_longAct = new QAction(QString("Message long"),this);
	m_longAct->setCheckable(true);
	m_moyenAct->setChecked(true);
	m_clearAct = new QAction(QString("effacer"),this);
	m_messageActGr = new QActionGroup(this);
    	m_messageActGr->addAction(m_courtAct);
    	m_messageActGr->addAction(m_moyenAct);
    	m_messageActGr->addAction(m_longAct);   	
	m_menu = new QMenu(this);	
	m_menu->addAction(m_courtAct);
        m_menu->addAction(m_moyenAct);
	m_menu->addAction(m_longAct);
	m_menu->addAction(m_clearAct); 	
	connect(m_courtAct, SIGNAL(triggered()), this, SLOT(shortMessage()) );
	connect(m_moyenAct, SIGNAL(triggered()), this, SLOT(mediumMessage()) );
	connect(m_longAct, SIGNAL(triggered()), this, SLOT(longMessage()) );
	connect(m_clearAct, SIGNAL(triggered()), this, SLOT(clear()) );
}


void ListWidget::contextMenuEvent(QContextMenuEvent * e) 
{
         m_menu->exec(e->globalPos());
}

bool ListWidget::contains(QString str)
{
    int it;
    QStringList strList;
    for (it = 0 ; it <= this->count()-1 ; it++)
    {
        strList << (this->item(it))->text();
    }
    return (strList.join("\n")).contains(str);
}

bool ListWidget::vectorContains(int numLigneError)
{
    std::vector<erreur>::iterator it;
    for(it = m_listError.begin(); it != m_listError.end() ; it++)
    {
        if ((*it).numLigneError == numLigneError)
            return true;
    }
    return false;
}

void ListWidget::displayVector()
{
    std::vector<erreur>::iterator it;
    for(it = m_listError.begin(); it != m_listError.end() ; it++)
        cout 	<< "(" << (*it).numLigneList 
		<< " , " << (*it).numLigneError 
		<< " , " << (*it).numPosError 
		<< " , " << (*it).numError << ")" << endl;
}

int ListWidget::maxNumError()
{
    int maxError = 0;
    std::vector<erreur>::iterator it;
    for(it = m_listError.begin(); it != m_listError.end() ; it++)
    {
        if ((*it).numError > maxError)
            maxError = (*it).numError;
    }
    return maxError;
}

bool ListWidget::containsError()
{
    if (m_exitDisplay != NULL)
        return m_exitDisplay->contains("compilation aborted",Qt::CaseInsensitive);
    return false;
}

//slots

void ListWidget::shortMessage()
{
	emit getShortMessage();
}

void ListWidget::mediumMessage()
{
	emit getMediumMessage();
}

void ListWidget::longMessage()
{
	emit getLongMessage();
}


void ListWidget::changeMessage(int numType)
{
	switch (numType)
	{
	case 0:
	m_courtAct->setChecked(true);
	break;
	case 1:
	m_moyenAct->setChecked(true);
	break;
	case 2:
	m_longAct->setChecked(true);
	break;
	}
	
}

void ListWidget::searchNextError()
{
    if (this->contains(tr("aborted")) || this->contains(tr("échoué")))
    {
        int numError;
        std::vector<erreur>::iterator it;
        for(it = m_listError.begin(); it != m_listError.end() ; it++)
        {
            if ((*it).numLigneList == this->currentRow())
            {
                numError = (*it).numError;
            }
        }
        numError++;
        if (numError >= maxNumError())
        {
            numError=1;
        }
        for(it = m_listError.begin(); it != m_listError.end() ; it++)
        {
            if ((*it).numError == numError)
            {
                this->setCurrentRow((*it).numLigneList);
                break;
            }
        }
        emit showError(this->currentItem());
    }
}

void ListWidget::searchPreviousError()
{
    if (this->contains(tr("aborted")) || this->contains(tr("échoué")))
    {
        int numError;
        std::vector<erreur>::iterator it;
        for(it = m_listError.begin(); it != m_listError.end() ; it++)
        {
            if ((*it).numLigneList == this->currentRow())
            {
                numError = (*it).numError;
            }
        }
        numError--;
        if (numError == 0)
        {
            numError = maxNumError()-1;
        }
        for(it = m_listError.begin(); it != m_listError.end() ; it++)
        {
            if ((*it).numError == numError)
            {
                this->setCurrentRow((*it).numLigneList);
                break;
            }
        }
        emit showError(this->currentItem());
    }
}

void ListWidget::loadErrorList(QStringList strList)
{
    m_listError.erase(m_listError.begin(),m_listError.end());
    QStringList::Iterator it;
    int numLigneListe = 0;
    int numError = 0;
    for (it = strList.begin() ; it != strList.end() ; it++ , numLigneListe++)
    {
        if 	((*it).contains("Fatal",Qt::CaseInsensitive) 
	|| 
		(*it).contains("Error",Qt::CaseInsensitive) 
	|| 
		(*it).contains("Warning",Qt::CaseInsensitive))
	{
            erreur e;
            e.numLigneList = numLigneListe;
            QString ch((*it).section('(', 1, 1));
            QString ch2(ch.section(',',0,0));
            QString ch3(ch.section(',',1,1));
            QString ch4(ch3.section(')',0,0));
            int numLigneError = ch2.toInt(0,10);
            int numPosError = ch4.toInt(0,10);
            e.numLigneError = numLigneError;
            e.numPosError = numPosError;
            if (!vectorContains(numLigneError))
                numError++;
            e.numError = numError;
            m_listError.push_back(e);
        }
    }
    //displayVector();
}


void ListWidget::selectErrorItem(int numLigneError)
{
    std::vector<erreur>::iterator it;
    for(it = m_listError.begin(); it != m_listError.end() ; it++)
    {
        if ((*it).numLigneError == numLigneError)
        {
            this->setCurrentRow((*it).numLigneList);
            break;
        }
    }
}

void ListWidget::displayCompilerList(QString str)
{
    if (m_exitDisplay != NULL)
        delete m_exitDisplay;
    m_exitDisplay = new QString(str);
    this->clear();
    if (m_courtAct->isChecked())
        showShortMessage();
    if (m_moyenAct->isChecked())
        showMediumMessage();
    if (m_longAct->isChecked())
        showLongMessage();
}

void ListWidget::showShortMessage()
{
    this->clear();
    if (m_exitDisplay != NULL)
    {
        QStringList strList;
        QStringList current = m_exitDisplay->split("\n");
        QStringList::Iterator it;
        for (it = current.begin() ; it != current.end() ; it++)
        {
            if 		((*it).contains("Fatal",Qt::CaseInsensitive) 
		|| 
			(*it).contains("Error",Qt::CaseInsensitive))
            {
                if ((*it).contains("compilation aborted",Qt::CaseInsensitive) )
                    strList << QString("La compilation a échoué");
                else if ((*it).contains("returned an error exitcode",Qt::CaseInsensitive)){}
                    else strList << (*it);
            }
            if 		((*it).contains("lines compiled",Qt::CaseInsensitive) 
		|| 
			(*it).contains("line compiled",Qt::CaseInsensitive)  )
                
				strList << QString("Compilation réussie");
        }
        loadErrorList(strList);
        this->addItems(strList);
        std::vector< erreur >::iterator it2;
        int firstErrorItem = -1;
        for (it2 = m_listError.begin() ; it2 != m_listError.end() ; it2++)
        {
            if (firstErrorItem == -1)
                firstErrorItem = (*it2).numLigneList;
            this->setCurrentRow((*it2).numLigneList);
            emit itemDoubleClicked(this->currentItem());
        }
        if (firstErrorItem != -1)
        {
            this->setCurrentRow(firstErrorItem);
            emit itemDoubleClicked(this->currentItem());
        }
    }
}

void ListWidget::showMediumMessage()
{
    this->clear();
    if (m_exitDisplay != NULL)
    {
        QStringList strList;
        QStringList current = m_exitDisplay->split("\n");
        QStringList::Iterator it;
        for (it = current.begin() ; it != current.end() ; it++)
        {
            if 		((*it).contains("Fatal",Qt::CaseInsensitive) 
		|| 
			(*it).contains("Error",Qt::CaseInsensitive) 
		|| 
			(*it).contains("Warning",Qt::CaseInsensitive) 
		|| 
			(*it).contains("Compiling",Qt::CaseInsensitive))
            {
                if ((*it).contains("compilation aborted",Qt::CaseInsensitive) )
                    strList << QString("La compilation a échoué");
                else if ((*it).contains("returned an error exitcode",Qt::CaseInsensitive)){}
                    else strList << (*it);
            }
            if 		((*it).contains("lines compiled",Qt::CaseInsensitive) 
		|| 
			(*it).contains("line compiled",Qt::CaseInsensitive))
            {
                strList << (*it);
                strList << QString("Compilation réussie");
            }
        }
        loadErrorList(strList);
        this->addItems(strList);
        std::vector< erreur >::iterator it2;
        int firstErrorItem = -1;
        for (it2 = m_listError.begin() ; it2 != m_listError.end() ; it2++)
        {
            if (firstErrorItem == -1)
                firstErrorItem = (*it2).numLigneList;
            this->setCurrentRow((*it2).numLigneList);
            emit itemDoubleClicked(this->currentItem());
        }
        if (firstErrorItem != -1)
        {
            this->setCurrentRow(firstErrorItem);
            emit itemDoubleClicked(this->currentItem());
        }
    }
}

void ListWidget::showLongMessage()
{
    this->clear();
    if (m_exitDisplay != NULL)
    {
        loadErrorList(m_exitDisplay->split("\n"));
        this->addItems(m_exitDisplay->split("\n"));
        std::vector< erreur >::iterator it2;
        int firstErrorItem = -1;
        for (it2 = m_listError.begin() ; it2 != m_listError.end() ; it2++)
        {
            if (firstErrorItem == -1)
                firstErrorItem = (*it2).numLigneList;
            this->setCurrentRow((*it2).numLigneList);
            emit itemDoubleClicked(this->currentItem());
        }
        if (firstErrorItem != -1)
        {
            this->setCurrentRow(firstErrorItem);
            emit itemDoubleClicked(this->currentItem());
        }
    }
}

void ListWidget::message(int length)
{
    switch(length)
    {
        case 0:
            m_courtAct->setChecked(true);
            showShortMessage();
            break;
        case 1:
            m_moyenAct->setChecked(true);
            showMediumMessage();
            break;
        case 2:
            m_longAct->setChecked(true);
            showLongMessage();
            break;
    }
}
