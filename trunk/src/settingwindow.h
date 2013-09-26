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


#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H
#include <QtGui>
#include <QtCore>

#define CONFIG_FILE "conf.ini"
/**
*  Fen�tre de pr�f�rence de l'application
*  @author MANIVONG Vilayvanh et Julien Courcelle
*/
class SettingWindow : public QMainWindow
{	
    Q_OBJECT

    public slots:
        /*

	*/
	void setPath();
        /* M�thode qui charge les options enregistr�es dans le fichier de configuration
        **/
        void loadFile();
        /**
        * M�thode qui sauvegarde les options coch�es dans le fichier de configuration
        **/
        void saveFile();
        /**
        * M�thode qui charge les options par d�fault
        **/
        void loadDefaultSetting();
        /**
        * M�thode qui coche les options pr�sentes dans la variable m_args
        **/
        void loadCheckOption();
        /**
        *M�thode qui annule les changements effectu�s
        **/
        void cancel();

    signals:
        void setDebugActivated(bool);

    public:
        /**
        * Constructeur
        * initialise toutes les variables
        * @see SettingWindow
        **/
	SettingWindow(QWidget * parent, Qt::WFlags f = 0);
        /** Destructeur **/
	~SettingWindow();
		
	//attributs
        /** Liste des arguments pour le compilateur fpc **/
        QStringList *m_args;
        /** variable d'OS, renvoie vrai si windows et faux si linux**/
        bool m_OS;
        QString *m_debugDisplay;
        QTabWidget *m_tabWidget;
        QWidget *m_compilerWidget;
        QWidget *m_pathWidget;
        QWidget *m_advanceWidget;
        QWidget *m_codeWidget;
        QWidget *m_globalWidget;
        QPushButton *m_defaultSettingButton;
        QPushButton *m_okButton;
        QPushButton *m_cancelButton;
        //options de compilation
        QCheckBox *m_SdOption;
        QCheckBox *m_ScOption;
        QCheckBox *m_SeOption;
        QCheckBox *m_SgOption;
        QCheckBox *m_SoOption;
        QCheckBox *m_StOption;
        QCheckBox *m_ShOption;
        QRadioButton *m_XSOption;
        QRadioButton *m_XDOption;
        QRadioButton *m_XXOption;
        QRadioButton *m_XDefaultOption;
        QCheckBox *m_XsOption;
        //options des chemins
        QLineEdit *m_fpcPathX11;
        QLineEdit *m_fpcPathWIN32;
        QLineEdit *m_gdbPathX11;
        QLineEdit *m_gdbPathWIN32;
        QLineEdit *m_docPathX11;
        QLineEdit *m_docPathWIN32;
        QLineEdit *m_docIndexX11;
        QLineEdit *m_docIndexWIN32;

	QLineEdit *m_fpcPathtemp;
	QLineEdit *m_gdbPathtemp;
	QLineEdit *m_docPathtemp;
	QLineEdit *m_docIndextemp;
	
        //options avanc�es
        QCheckBox *m_vwOption;
        QCheckBox *m_vnOption;
        QCheckBox *m_vhOption;
        QCheckBox *m_viOption;
        QCheckBox *m_vxOption;
        QCheckBox *m_debugOption;
        //options de g�n�ration de code
        QCheckBox *m_CrOption;
        QCheckBox *m_CtOption;
        QCheckBox *m_CiOption;
        QCheckBox *m_CoOption;
        QRadioButton *m_O1Option;
        QRadioButton *m_O2Option;
        QRadioButton *m_OGOption;
        QRadioButton *m_OgOption;
        QRadioButton *m_00Option;
        QRadioButton *m_OG0Option;
        QHBoxLayout *bottomLayout;
        //m�thodes
        /**
        *M�thode de cr�ation du widget associ� au pr�f�rence du compilateur
        **/
        void createCompilerWidget();
        /**
        *M�thode de cr�ation du widget contenant le chemin d'acc�s aux applications
        **/
        void createPathWidget();
        /**
        *M�thode de cr�ation du widget contenant les options avanc�es du compilateur
        **/
        void createAdvanceWidget();
        /**
        *M�thode de cr�ation du widget contenant les options d'am�lioration du code
        **/
        void createCodeWidget();
        /**
        *M�thode de cr�ation du widget contenant les onglets des diff�rents menus
        **/
        void createTabWidget();
        /**
        *M�thode de cr�ation du widget contenant l'ensemble des composants de la fen�tre
        **/
        void createGlobalWidget();
};

#endif
