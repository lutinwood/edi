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
#ifndef LISTWIDGET_H
#define LISTWIDGET_H
#include <QtGui>
#include <QtCore>
#include <vector>

/**
*Widget servant � l'affichage de la compilation sous forme de liste
*@author MANIVONG Vilayvanh & COURCELLE Julien
*/

class ListWidget : public QListWidget
{
	Q_OBJECT	

	private :
        /** Menu pop up **/
	QMenu *m_menu;
	QAction *m_moyenAct, *m_courtAct, *m_longAct, *m_clearAct;
	QActionGroup *m_messageActGr;
	
	public :
	/**
	*Constructeur
	*@see QListWidget
	*/
	ListWidget(QWidget *parent=0);
	/**
	*Destructeur
	*/
	~ListWidget();
	/**
	*Fonction qui cr�e le menu (obtenu par l'�v�nement "clic droit") et l'initialise 
	*/
	void createMenu();
	/**
	*Fonction g�rant l'�v�nement "clic droit"
	*@param QContextMenuEvent e evenement "clic droit"
	*/
	void contextMenuEvent(QContextMenuEvent * e);
	
	/**
	*Fonction permettant de chercher une chaine dans la listWidget
	*/
	bool contains(QString);

        /**
        *Recherche dans le vecteur si une ligne avec une erreur est d�j� pr�sente
        *@param int num�ro de la ligne � chercher
        *@return renvoie vrai si trouv� et faux sinon
        **/
        bool vectorContains(int numLigne);

        /** Affiche l'�tat du vecteur **/
        void displayVector();

        /** Retourne le num�ro maximum de l'erreur trouv�e dans le vecteur **/
        int maxNumError();

        /** Reherche si le r�sultat de la compilation contient des erreurs ou non **/
        bool containsError();

        /** Cha�ne de caract�res contenant la sortie � afficher **/
        QString *m_exitDisplay;

        struct erreur{
            int numLigneList;
            int numLigneError;
            int numPosError;
            int numError;
        };

        /** Vecteur contenant la liste des erreurs de compilation **/
        std::vector< erreur > m_listError;


	public slots:
	/**
	*Fonction permettant d'envoyer le signal pour obtenir le message court du compilateur
	*/
	void shortMessage();
	/**
	*Fonction permettant d'envoyer le signal pour obtenir le message moyen du compilateur
	*/
	void mediumMessage();
	/**
	*Fonction permettant d'envoyer le signal pour obtenir le message long du compilateur
	*/
	void longMessage();
	/**
	*Fonction permettant de g�rer le cochage du message courant dans le menu contextuelle
	*/
	void changeMessage(int);
	
	/**
	*Fonction permettant d'acc�der �l'erreur suivante
	*/
	void searchNextError();
        
	/**
	*Fonction permettant d'acc�der � l'erreur pr�c�dente
	*/
	void searchPreviousError();

        /**
        *Charge le vecteur d'erreur
        *@param QStringList liste � afficher dans le QListWidget (sortie de compilation)
        **/
        void loadErrorList(QStringList strList);

        /**
        *S�l�ctionne l'item de la liste en rapport avec l'erreur
        *@param int num�ros de ligne de l'erreur dans l'�diteur de code
        **/
        void selectErrorItem(int numLigneError);

        /**
        *Affiche le r�sultat de la compilation sous forme de liste et met en place les markers
        *@param QString r�sultat de la compilation
        **/
        void displayCompilerList(QString);
        /** Affiche le r�sultat de la compilation sous forme courte **/
        void showShortMessage();
        /** Affiche le r�sultat de la compilation sous forme moyenne **/
        void showMediumMessage();
        /** Affiche le r�sultat de la compilation sous forme longue **/
        void showLongMessage();
        /** 
        *Traite un changement de type d'affichage des messages de compilation
        *@param int type de messages de compilation (court, moyen ou long)
        **/
        void message(int);


	signals:
	/**
	*Signal �mis pour obtenir un message court
	*/
	void getShortMessage();
	
	/**
	*Signal �mis pour obtenir un message moyen
	*/
	void getMediumMessage();
	
	/**
	*Signal �mis pour obtenir un message long
	*/
	void getLongMessage();
	
	/**
	*Signal �mis pour obtenir le flux d'erreur
	*/
	void showError(QListWidgetItem*);

    //    void itemDoubleClicked(QListWidgetItem * item);
};


#endif
