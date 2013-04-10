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


#include "qscilexerpascal.h"


// constructeur
QsciLexerPascal::QsciLexerPascal(QObject *parent,bool caseInSensitiveKeyWords)
    : QsciLexerCPP(parent,caseInSensitiveKeyWords)
{
}


// destructeur
QsciLexerPascal::~QsciLexerPascal()
{
}


// retourne le nom du langage
const char *QsciLexerPascal::language() const
{
    return "Pascal";
}


// retourne l'ensemble des mots clés
const char *QsciLexerPascal::keywords(int set) const
{
    if (set != 1)
        return 0;

    return "absolute abstract alias all and and_then array as asm asmname assembler attribute \
	begin bindable break case cdecl class const contructor continue cppclass cvar default destructor \
	dispose div do downto dynamic else end end. except exit exports external fail false far file finalization \
	finally for forward function goto if implementation import in index inherited initialization inline interface \
	internconst interrupt iocheck is label library message mod module name near new nil nodefault not object \
	of on only openstring operator or or_else otherwise out overlay overload override packed pascal popstack \
	pow private procedure process program property protected public published qualified raise read record \
	register reintroduce repeat resident resourcestring restricted result safecall saveregisters segment self \
	separate set shl shortstring shr static stdcall stored string syscall system then threadvar to true try type \
	unit univ until uses value var view virutal volatile while with write xor";
}
