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
*Widget servant à l'affichage de la compilation sous forme de liste
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
	*Fonction qui crée le menu (obtenu par l'évènement "clic droit") et l'initialise 
	*/
	void createMenu();
	/**
	*Fonction gèrant l'évènement "clic droit"
	*@param QContextMenuEvent e evenement "clic droit"
	*/
	void contextMenuEvent(QContextMenuEvent * e);
	
	/**
	*Fonction permettant de chercher une chaine dans la listWidget
	*/
	bool contains(QString);

        /**
        *Recherche dans le vecteur si une ligne avec une erreur est déjà présente
        *@param int numéro de la ligne à chercher
        *@return renvoie vrai si trouvé et faux sinon
        **/
        bool vectorContains(int numLigne);

        /** Affiche l'état du vecteur **/
        void displayVector();

        /** Retourne le numéro maximum de l'erreur trouvée dans le vecteur **/
        int maxNumError();

        /** Reherche si le résultat de la compilation contient des erreurs ou non **/
        bool containsError();

        /** Chaîne de caractères contenant la sortie à afficher **/
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
	*Fonction permettant de gérer le cochage du message courant dans le menu contextuelle
	*/
	void changeMessage(int);
	
	/**
	*Fonction permettant d'accèder à l'erreur suivante
	*/
	void searchNextError();
        
	/**
	*Fonction permettant d'accèder à l'erreur précèdente
	*/
	void searchPreviousError();

        /**
        *Charge le vecteur d'erreur
        *@param QStringList liste à afficher dans le QListWidget (sortie de compilation)
        **/
        void loadErrorList(QStringList strList);

        /**
        *Séléctionne l'item de la liste en rapport avec l'erreur
        *@param int numéros de ligne de l'erreur dans l'éditeur de code
        **/
        void selectErrorItem(int numLigneError);

        /**
        *Affiche le résultat de la compilation sous forme de liste et met en place les markers
        *@param QString résultat de la compilation
        **/
        void displayCompilerList(QString);
        /** Affiche le résultat de la compilation sous forme courte **/
        void showShortMessage();
        /** Affiche le résultat de la compilation sous forme moyenne **/
        void showMediumMessage();
        /** Affiche le résultat de la compilation sous forme longue **/
        void showLongMessage();
        /** 
        *Traite un changement de type d'affichage des messages de compilation
        *@param int type de messages de compilation (court, moyen ou long)
        **/
        void message(int);


	signals:
	/**
	*Signal émis pour obtenir un message court
	*/
	void getShortMessage();
	
	/**
	*Signal émis pour obtenir un message moyen
	*/
	void getMediumMessage();
	
	/**
	*Signal émis pour obtenir un message long
	*/
	void getLongMessage();
	
	/**
	*Signal émis pour obtenir le flux d'erreur
	*/
	void showError(QListWidgetItem*);

    //    void itemDoubleClicked(QListWidgetItem * item);
};


#endif
