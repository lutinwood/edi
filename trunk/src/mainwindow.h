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
*  Fen�tre principale de l'application
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
	*Fonction appel�e lors de l'�v�nement "event" de fermeture de la fen�tre principale 
	*@param event
	*/
	void closeEvent ( QCloseEvent *event );

        signals:
	/**
	*Signal envoy� permettant de d�placer le curseur de sa place courante � la prochaine occurence d'une chaine.
	*/
        void findNext();
	
	/**
	*Signal envoy� permettant de s�lectionner tout le texte contenu dans m_textEdit (bool pour tout s�lectionner)
	*@param bool vrai pour tout s�lectionner, faux sinon.
	*/
        void selectAll(bool);
	
	/**
	*Signal envoy� permettant d'interagir avec la classe FpcDiscuss
	*@param QString chemin du complet fichier (*.PAS) qu'on souhaite compiler
	*/
        void compile(QString);
	
	/**
	*Signal envoy� permettant de lancer un �x�cutable
	*@param QString chemin complet du fichier (*.PAS) qu'on souhaite ex�cuter (le ".PAS" est retirer dans le fpcDiscuss)
	*/
        void run(QString);
	
	/** 
        *Signal envoy� permettant de d�boguer l'application
        *@param QString Chemin entier de l'application
        **/
        void debug(QString);

	/**
	*Signal envoy� permettant de changer le message de l'onglet d�bogeur de l'interface
	*@param int 0,1,2 correspondant � message court, moyen et long qu'on souhaite afficher dans l'onglet compilateur de l'interface
	*/
        void message(int);
        
	/**
	*Signal envoy� permettant de r�cup�rer le message par le module "compilateur", sert notamment dans le mise en place de l'historique des commandes, entr� par l'utilisateur
	*@param QString commande entr�e par l'utilisateur 
	*/
	void cmdHistory(QString);
    
        /** 
        *Signal qui transmet une commande au d�bogueur
        *@param QString commmande
        **/
        void writeCmdDebug(QString);

        /**
        *Signal qui demande un affichage de la liste de compilation
        *@param QString r�sultat de la compilation
        **/
        void displayCompilerList(QString);

	private slots:
	/**
	*Fonction permettant de cr�er un nouveau document 
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
	*Fonction permettant de sauvegarder le document courant dans le r�pertoire souhait�
	*/
	bool saveAs();
		
	/**
	*Fonction permettant d'afficher la fen�tre FindDialog
	*/	
	void find();

	/**
	*Fonction permettant d'afficher la fen�tre replaceDialog
	*/
	void replace();

	/**
	*Fonction permettant de replacer le texte selectionn�
	*/
	void replaceText ( QString );

	/**
	*Fonction permettant d'afficher la fen�tre lineDialog
	*/
	void goLine();

	/**
	*Fonction permettant d'afficher la fen�tre setting
	*/	
	void setting();
	
	/**
	*Fonction permettant d'afficher la fen�tre "apropos"
	*/
	void about();

	/**
	*Fonction permettant d'afficher la fen�tre d'aide
	*/
        void help();

	/**
	*Fonction permettant de savoir si le document a �t� modifi�
	*/
	void documentWasModified();
	
	/**
	*Fonction permettant de rechercher l'occurence pr�c�dente de str dans le m_textEdit en respectant la case ou pas (cs = vrai ou faux) 
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
	*Fonction permettant de chercher l'occurence suivante d'un mot d�j� recherch�
	*/
	void searchNext();

	/**
	*Fonction permettant de s�lectionner tout le texte dans le m_textEdit
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
	*Fonction permettant d'afficher une fen�tre d'avertissement lorsqu'un caract�re diff�rent d'un chiffre est entr�
        *dans la bo�te de dialogue "Aller � la ligne"
	*/
        void warningLine();
        
        /**
	*Fonction permettant d'afficher une fen�tre d'avertissement lorsque l'un des outils (compilateur ou debogeur) n'est pas trouv� par l'application
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
	*Focntion permettant de lancer le d�bogeur
	*/
        void debug();
        
	/**
	*Fonction permettant � l'utilisateur d'envoyer une commande avec le d�bogeur
	*/
        void runCmdDebug();
	
	/**
	*Fonction permettant d'afficher le message venant du compilateur dans l'onglet compilateur de l'interface
	*@param QStringList str
	*/
        void displayCompile(QString);
        
	/**
	*Fonction permettant d'afficher le statut des programmes ext�rieurs lanc�s par l'application
	*@param QString str
	*/
        void displayApplication(QString);

	/**
	*Focntion permettant d'afficher le message venant du d�bogeur dans l'onglet d�bogeur dans l'interface
	*/
        void displayDebug(QString);

	/**
	*Fonction permettant de cr�er un marqueur lors du double clic de l'utilisateur sur un message d'erreur
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
	*Fonction permettant de placer des marqueurs de d�bogue lors d'un clic gauche sur la marge	
	*@param int margin
	*@param int line ligne sur lequel l'utilisateur place un marqueur	
	*/
        void marginClicked(int margin, int line, Qt::KeyboardModifiers state);

	/**
	*Fonction permettant d'envoyer aux lignes sur lesquelles l'utilisateur a plac� un marqueur de d�bogue
	*/
        void writeBreakPoints();
        
	/**
	*Fonction permettant de connecter l'�v�nement "clic gauche sur la marge" � la cr�ation de marqueurs de d�bogue
	*/
        void connectBreakPoints();
	/**
	*Fonction permettant � l'utilisateur d'imprimer le document
	*/
        void print();

        /**
        *Traite l'�v�nement lorsque l'utilisateur bouge le curseur de l'�diteur de code
        *@param int ligne o� est positionn� le curseur
        *@param int position du curseur sur la ligne
        **/
        void cursorPositionChanged (int line, int pos);

        /**
        *Active ou non les composants graphiques li�s � la compilation.
        *@param bool vrai pour activer et faux sinon
        **/
        void setCompileEnabled(bool);
        /**
        *Active ou non les composants graphiques li�s au lancement de l'application.
        *@param bool vrai pour activer et faux sinon
        **/
        void setApplicationEnabled(bool);
        /**
        *Active ou non les composants graphiques li�s au d�boguage.
        *@param bool vrai pour activer et faux sinon
        **/
        void setDebugEnabled(bool);

        void setDebugActivated(bool);

	private:

        /** Cr�e un fichier de configuration si il n'existe pas **/
        void initFileconf();

	/**
	*Fonction permettant de cr�er et initialiser le qscintilla
	*/
	void initScintilla();
	/**
	*Fonction permettant de cr�er les actions de la fen�tre principale 
	*/
	void createActions();
	
	/**
	*Fonction permettant de cr�er les menus de la fen�tre principale
	*/
	void createMenus();

	/**
	*Fonction permettant de cr�er les barres d'outils de la fen�tre principale
	*/
	void createToolBars();
	
	/**
	*Fonction permettant de cr�er les actions de la fen�tre principale
	*/
	void createStatusBar();

	/**
	*Fonction permettant la lecture des propri�t�s de l'application
	*/
	void readSettings();
	
	/**
	*Fonction permettant l'�criture des propri�t�s de l'application
	*/
	void writeSettings();

	/**
	*Fonction permettant de rechercher si le composant Scintilla a �t� sauvegard� depuis les modifications
	*@return bool vrai si le composant Scintilla a �t� sauvegard� depuis les modifications, faux sinon 
	*/	
	bool maybeSave();

	/**
	*Fonction permettant de charger un fichier
	*@param QString nom du fichier que l'on souhaite charger
	*/
	void loadFile ( const QString &fileName );
	
	/**
	*Fonction permettant de sauvegarder le fichier
	*@param QString nom du fichier que l'on souhaite donner � la sauvegarde
	*/
	bool saveFile ( const QString &fileName );
	
	/**
	*Fonction affichant le nom du fichier courant dans la fen�tre principale
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
