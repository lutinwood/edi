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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtCore>
#include <Qsci/qsciscintilla.h>
#include <vector>
#include "finddialog.h"
#include "replaceDialog.h"
#include "lineDialog.h"
#include "engine/fpcdiscuss.h"
#include "engine/applicationdiscuss.h"
#include "engine/gdbdiscuss.h"
#include "helpwindow.h"
#include "listwidget.h"
#include "settingwindow.h"
#include "myedit.h"

#define CONFIG_FILE "conf.ini"

/**
*  Fenêtre principale de l'application
*  @author MANIVONG Vilayvanh et Julien Courcelle
*/
class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
      /**
      * Constructeur
      * initialise toutes les variables
      * @see QMainWindow
      */
	MainWindow();

	protected:
	/**
	*Fonction appelée lors de l'évènement "event" de fermeture de la fenêtre principale 
	*@param event
	*/
	void closeEvent ( QCloseEvent *event );

        signals:
	/**
	*Signal envoyé permettant de déplacer le curseur de sa place courante à la prochaine occurence d'une chaine.
	*/
        void findNext();
	
	/**
	*Signal envoyé permettant de sélectionner tout le texte contenu dans m_textEdit (bool pour tout sélectionner)
	*@param bool vrai pour tout sélectionner, faux sinon.
	*/
        void selectAll(bool);
	
	/**
	*Signal envoyé permettant d'interagir avec la classe FpcDiscuss
	*@param QString chemin du complet fichier (*.PAS) qu'on souhaite compiler
	*/
        void compile(QString);
	
	/**
	*Signal envoyé permettant de lancer un éxécutable
	*@param QString chemin complet du fichier (*.PAS) qu'on souhaite exécuter (le ".PAS" est retirer dans le fpcDiscuss)
	*/
        void run(QString);
	
	/** 
        *Signal envoyé permettant de déboguer l'application
        *@param QString Chemin entier de l'application
        **/
        void debug(QString);

	/**
	*Signal envoyé permettant de changer le message de l'onglet débogeur de l'interface
	*@param int 0,1,2 correspondant à message court, moyen et long qu'on souhaite afficher dans l'onglet compilateur de l'interface
	*/
        void message(int);
        
	/**
	*Signal envoyé permettant de récupérer le message par le module "compilateur", sert notamment dans le mise en place de l'historique des commandes, entré par l'utilisateur
	*@param QString commande entrée par l'utilisateur 
	*/
	void cmdHistory(QString);
    
        /** 
        *Signal qui transmet une commande au débogueur
        *@param QString commmande
        **/
        void writeCmdDebug(QString);

        /**
        *Signal qui demande un affichage de la liste de compilation
        *@param QString résultat de la compilation
        **/
        void displayCompilerList(QString);

	private slots:
	/**
	*Fonction permettant de créer un nouveau document 
	*/
	void newFile();
	
	/**
	*Fonction permettant d'ouvrir un document
	*/
	void open();
	
	/**
	*Fonction permettant de sauvegarder le document courant
	*/
	bool save();
	
	/**
	*Fonction permettant de sauvegarder le document courant dans le répertoire souhaité
	*/
	bool saveAs();
		
	/**
	*Fonction permettant d'afficher la fenêtre FindDialog
	*/	
	void find();

	/**
	*Fonction permettant d'afficher la fenêtre replaceDialog
	*/
	void replace();

	/**
	*Fonction permettant de replacer le texte selectionné
	*/
	void replaceText ( QString );

	/**
	*Fonction permettant d'afficher la fenêtre lineDialog
	*/
	void goLine();

	/**
	*Fonction permettant d'afficher la fenêtre setting
	*/	
	void setting();
	
	/**
	*Fonction permettant d'afficher la fenêtre "apropos"
	*/
	void about();

	/**
	*Fonction permettant d'afficher la fenêtre d'aide
	*/
        void help();

	/**
	*Fonction permettant de savoir si le document a été modifié
	*/
	void documentWasModified();
	
	/**
	*Fonction permettant de rechercher l'occurence précédente de str dans le m_textEdit en respectant la case ou pas (cs = vrai ou faux) 
	*@param QString str 
	*@param Qt::CaseSensitivity cs 
	*/
	void searchBefore ( const QString &str, Qt::CaseSensitivity cs );

	/**
	*Fonction permettant de rechercher la prochaine occurence de str dans le m_textEdit en respectant la case ou pas (cs = vrai ou faux) 
	*@param QString str 
	*@param Qt::CaseSensitivity cs 
	*/
	void searchAfter ( const QString &str, Qt::CaseSensitivity cs );
        
	/**
	*Fonction permettant de chercher l'occurence suivante d'un mot déjà recherché
	*/
	void searchNext();

	/**
	*Fonction permettant de sélectionner tout le texte dans le m_textEdit
	*/
        void selectAll();
        
	/**
	*Fonction permettant d'enlever la ToolBar
	*/
        void removeToolBar();
        
	/**
	*Fonction permettant d'enlever la EditBar
	*/
        void removeEditBar();
        
	/**
	*Fonction permettant d'enlever la BuildBar
	*/
        void removeBuildBar();

	/**
	*Fonction permettant d'enlever la FileBar
	*/
        void removeFileBar();

	/**
	*Fonction permettant d'enlever la DebugBar
	*/
	void removeDebugBar();	
	
	/**
	*Fonction permettant d'afficher une fenêtre d'avertissement lorsqu'un caractère différent d'un chiffre est entré
        *dans la boîte de dialogue "Aller à la ligne"
	*/
        void warningLine();
        
        /**
	*Fonction permettant d'afficher une fenêtre d'avertissement lorsque l'un des outils (compilateur ou debogeur) n'est pas trouvé par l'application
	*@param QString str
	*/
        void appNotFound(QString);
         
	/**
	*Fonction permettant de lancer la compilation
	*/
        void compile();
	
	/**
	*Fonction permettant de compiler et lancer le programme courant 
	*/
        void compileAndRun();
        
	/**
	*Focntion permettant de lancer le débogeur
	*/
        void debug();
        
	/**
	*Fonction permettant à l'utilisateur d'envoyer une commande avec le débogeur
	*/
        void runCmdDebug();
	
	/**
	*Fonction permettant d'afficher le message venant du compilateur dans l'onglet compilateur de l'interface
	*@param QStringList str
	*/
        void displayCompile(QString);
        
	/**
	*Fonction permettant d'afficher le statut des programmes extérieurs lancés par l'application
	*@param QString str
	*/
        void displayApplication(QString);

	/**
	*Focntion permettant d'afficher le message venant du débogeur dans l'onglet débogeur dans l'interface
	*/
        void displayDebug(QString);

	/**
	*Fonction permettant de créer un marqueur lors du double clic de l'utilisateur sur un message d'erreur
	*/
        void itemDoubleClicked ( QListWidgetItem * item );

	/**
	*Fonction permettant d'afficher la version courte des messages obtenus du compilateur
	*/
        void shortMessageShow();
	
	/**
	*Fonction permettant d'afficher la version moyenne des messages obtenus du compilateur
	*/
        void mediumMessageShow();
        
	/**
	*Fonction permettant d'afficher la version longue des messages obtenus du compilateur
	*/
        void longMessageShow();
	
	/**
	*Fonction permettant de placer des marqueurs de débogue lors d'un clic gauche sur la marge	
	*@param int margin
	*@param int line ligne sur lequel l'utilisateur place un marqueur	
	*/
        void marginClicked(int margin, int line, Qt::KeyboardModifiers state);

	/**
	*Fonction permettant d'envoyer aux lignes sur lesquelles l'utilisateur a placé un marqueur de débogue
	*/
        void writeBreakPoints();
        
	/**
	*Fonction permettant de connecter l'évènement "clic gauche sur la marge" à la création de marqueurs de débogue
	*/
        void connectBreakPoints();
	/**
	*Fonction permettant à l'utilisateur d'imprimer le document
	*/
        void print();

        /**
        *Traite l'évènement lorsque l'utilisateur bouge le curseur de l'éditeur de code
        *@param int ligne où est positionné le curseur
        *@param int position du curseur sur la ligne
        **/
        void cursorPositionChanged (int line, int pos);

        /**
        *Active ou non les composants graphiques liés à la compilation.
        *@param bool vrai pour activer et faux sinon
        **/
        void setCompileEnabled(bool);
        /**
        *Active ou non les composants graphiques liés au lancement de l'application.
        *@param bool vrai pour activer et faux sinon
        **/
        void setApplicationEnabled(bool);
        /**
        *Active ou non les composants graphiques liés au déboguage.
        *@param bool vrai pour activer et faux sinon
        **/
        void setDebugEnabled(bool);

        void setDebugActivated(bool);

	private:

        /** Crée un fichier de configuration si il n'existe pas **/
        void initFileconf();

	/**
	*Fonction permettant de créer et initialiser le qscintilla
	*/
	void initScintilla();
	/**
	*Fonction permettant de créer les actions de la fenêtre principale 
	*/
	void createActions();
	
	/**
	*Fonction permettant de créer les menus de la fenêtre principale
	*/
	void createMenus();

	/**
	*Fonction permettant de créer les barres d'outils de la fenêtre principale
	*/
	void createToolBars();
	
	/**
	*Fonction permettant de créer les actions de la fenêtre principale
	*/
	void createStatusBar();

	/**
	*Fonction permettant la lecture des propriétés de l'application
	*/
	void readSettings();
	
	/**
	*Fonction permettant l'écriture des propriétés de l'application
	*/
	void writeSettings();

	/**
	*Fonction permettant de rechercher si le composant Scintilla a été sauvegardé depuis les modifications
	*@return bool vrai si le composant Scintilla a été sauvegardé depuis les modifications, faux sinon 
	*/	
	bool maybeSave();

	/**
	*Fonction permettant de charger un fichier
	*@param QString nom du fichier que l'on souhaite charger
	*/
	void loadFile ( const QString &fileName );
	
	/**
	*Fonction permettant de sauvegarder le fichier
	*@param QString nom du fichier que l'on souhaite donner à la sauvegarde
	*/
	bool saveFile ( const QString &fileName );
	
	/**
	*Fonction affichant le nom du fichier courant dans la fenêtre principale
	*/
	void setCurrentFile ( const QString &fileName );

    bool eventFilter(QObject* watched, QEvent *event);


    /**bool MainWindow::eventFilter(QObject* watched, QEvent *event)
	*
	*/
	QString strippedName ( const QString &fullFileName );

        QLabel *m_rowLabel;
        QLabel *m_colLabel;
        QsciScintilla *m_textEdit;
        std::vector<int> m_debugMarkerVect;
	QString m_curFile;
        //QString m_version;
        ListWidget *m_compilerOutput;
        QTextEdit *m_debugOutput;
        QTextEdit *m_applicationOutput;
        QTabWidget *m_tabWidget;
        QDockWidget *m_dockWidget;
        QWidget *m_debugWidget;
        FpcDiscuss *m_compiler;
        ApplicationDiscuss *m_application;
        GdbDiscuss *m_debug;
        MyEdit *m_cmdDebug;

	FindDialog *m_findDialog;
	ReplaceDialog *m_replaceDialog;
	LineDialog *m_lineDialog;
        HelpWindow *m_helpWindow;
        SettingWindow *m_settingWindow;

        QMenuBar *m_menuBar;
	QMenu *m_fileMenu;
	QMenu *m_editMenu;
	QMenu *m_researchMenu;
       	QMenu *m_buildMenu;
        QMenu *m_debugMenu;
        QMenu *m_displayMenu;
        QMenu *m_removeMenu;
	QMenu *m_helpMenu;
        QMenu *m_compileMenu;
	QToolBar *m_fileToolBar;
	QToolBar *m_editToolBar;
        QToolBar *m_buildToolBar;
        QToolBar *m_debugToolBar;
	QAction *m_newAct;
	QAction *m_openAct;
	QAction *m_saveAct;
	QAction *m_saveAsAct;
	QAction *m_researchAct;
        QAction *m_researchNextAct;
	QAction *m_replaceAct;
	QAction *m_exitAct;
	QAction *m_cutAct;
	QAction *m_copyAct;
	QAction *m_pasteAct;
        QAction *m_selectAllAct;
        /*QAction *m_zoomInAct;
	QAction *m_zoomOutAct;*/
        QAction *m_removeAct;
	QAction *m_removeFileToolBarAct;
	QAction *m_removeEditToolBarAct;
	QAction *m_removeBuildToolBarAct;
        QAction *m_removeDebugToolBarAct;
	QAction *m_undoAct;
	QAction *m_redoAct;
	QAction *m_lineAct;
        QAction *m_toolBarDisplayAct;
        QAction *m_compileAct;
        QAction *m_compileRunAct;
        QAction *m_settingCompileAct;
        QAction *m_debugAct;
        QAction *m_debugRunAct;
        QAction *m_debugNextAct;
        QAction *m_killAct;
	QAction *m_aboutAct;
	QAction *m_aboutQtAct;
        QAction *m_helpAct;
        QAction *m_longMessageAct;
        QAction *m_mediumMessageAct;
        QAction *m_shortMessageAct;
        QActionGroup *m_messageActGr;
        QAction *m_nextCompileErrorAct;
        QAction *m_previousCompileErrorAct;
        QAction *m_printAct;
};

#endif
