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
#ifndef QSCILEXERPASCAL_H
#define QSCILEXERPASCAL_H

#include <qobject.h>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexercpp.h>


//! \brief La classe QsciLexerPascal permet la mise en place d'un lexer Pascal Scintilla.
class QSCINTILLA_EXPORT QsciLexerPascal : public QsciLexerCPP
{
   Q_OBJECT

public:
    //! Constructeur
    QsciLexerPascal(QObject *parent = 0, bool caseInSensitiveKeyWords = true);

    //! Destructeur
    virtual ~QsciLexerPascal();

    //! Retourne le nom du langage.
    const char *language() const;

    //! Retourne l'ensemble des mots clés \a set ensemble de mots clés
    const char *keywords(int set) const;

private:
    QsciLexerPascal(const QsciLexerPascal &);
    QsciLexerPascal &operator=(const QsciLexerPascal &);
};

#endif
