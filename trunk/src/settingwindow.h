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
*  Fenêtre de préférence de l'application
*  @author MANIVONG Vilayvanh et Julien Courcelle
*/
class SettingWindow : public QMainWindow
{	
    Q_OBJECT

    public slots:
        /*

	*/
	void setPath();
        /* Méthode qui charge les options enregistrées dans le fichier de configuration
        **/
        void loadFile();
        /**
        * Méthode qui sauvegarde les options cochées dans le fichier de configuration
        **/
        void saveFile();
        /**
        * Méthode qui charge les options par défault
        **/
        void loadDefaultSetting();
        /**
        * Méthode qui coche les options présentes dans la variable m_args
        **/
        void loadCheckOption();
        /**
        *Méthode qui annule les changements effectués
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
	
        //options avancées
        QCheckBox *m_vwOption;
        QCheckBox *m_vnOption;
        QCheckBox *m_vhOption;
        QCheckBox *m_viOption;
        QCheckBox *m_vxOption;
        QCheckBox *m_debugOption;
        //options de génération de code
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
        //méthodes
        /**
        *Méthode de création du widget associé au préférence du compilateur
        **/
        void createCompilerWidget();
        /**
        *Méthode de création du widget contenant le chemin d'accès aux applications
        **/
        void createPathWidget();
        /**
        *Méthode de création du widget contenant les options avancées du compilateur
        **/
        void createAdvanceWidget();
        /**
        *Méthode de création du widget contenant les options d'amélioration du code
        **/
        void createCodeWidget();
        /**
        *Méthode de création du widget contenant les onglets des différents menus
        **/
        void createTabWidget();
        /**
        *Méthode de création du widget contenant l'ensemble des composants de la fenêtre
        **/
        void createGlobalWidget();
};

#endif
