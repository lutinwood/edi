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

#include "myedit.h"

using namespace std;

MyEdit::MyEdit(QWidget * parent) : QLineEdit(parent)
{
	m_listCmd = new QVector<QString>;
	m_nb= 0;
}


MyEdit::MyEdit(const QString & contents, QWidget * parent): QLineEdit(contents,parent)
{
	m_listCmd = new QVector<QString>;
	m_nb= 0;
}


MyEdit::~MyEdit(){}

void MyEdit::keyPressEvent ( QKeyEvent * event )
{
	if (event->key()==Qt::Key_Up)
	{
		//cout << "up" << endl;
		clear();
		//if (!m_listCmd->empty()) cout << "nb listCmd " << m_listCmd->count() << endl;
		//cout << "m_nb "<< m_nb << endl;
		if (m_nb>0) 
		{
			m_nb--;
			setText(m_listCmd->at(m_nb));
			//selectAll();
		}
		else if ((!m_listCmd->empty())&&(m_nb==0))
		{
			setText(m_listCmd->at(m_nb));
		}
	}
	else if (event->key()==Qt::Key_Down) 
	{	
		//cout << "down" << endl;
		clear();
		//if (!m_listCmd->empty()) cout << "nb listCmd " << m_listCmd->count() << endl;
		//cout << "m_nb "<< m_nb << endl;
		//if (!m_listCmd->empty())
		if (m_listCmd->count()>(m_nb+1))
		{	
			m_nb++;		
			setText(m_listCmd->at(m_nb));
			//selectAll();
		}
	}
	else
	{
		QLineEdit::keyPressEvent ( event );
	}
}


void MyEdit::enter(QString ch)
{
	//cout << "enter" << endl;
	//cout << "ch " << ch.compare(tr("\n")) << endl;
	/*if (ch.compare(tr("\n"))!=0)*/ m_listCmd->push_back(ch);
// 	clear();
	m_nb=m_listCmd->count();
	/*cout << "m_nb "  << m_nb << endl;
	cout << "m_listeCmd" << endl;
	for (int i=0;i<m_listCmd->count();i++)
	{
		cout << m_listCmd->at(i).toStdString() << endl;
	}
        */
}

