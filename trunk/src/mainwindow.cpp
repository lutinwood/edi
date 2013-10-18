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
#include <Qsci/qsciprinter.h>

#include "qscilexerpascal.h"
#include "mainwindow.h"

using namespace std;

//constructeur
MainWindow::MainWindow()
{
    initFileconf();

    initScintilla();

    //QString m_version("13.10");
    m_findDialog = NULL;
    m_replaceDialog = NULL;
    m_lineDialog = NULL;
    m_helpWindow = NULL;
    m_settingWindow = NULL;

    m_compilerOutput = new ListWidget(this);
    connect(m_compilerOutput,
            SIGNAL(getShortMessage()), 
			this, SLOT(shortMessageShow()) );
    connect(m_compilerOutput,
            SIGNAL(getMediumMessage()), 
			this, SLOT(mediumMessageShow()) );
    connect(m_compilerOutput,
            SIGNAL(getLongMessage()), 
			this, SLOT(longMessageShow()) );

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
   
    m_debugWidget = new QWidget(this);
    m_debugOutput = new QTextEdit(m_debugWidget);
    m_debugOutput->setReadOnly(true);
    m_cmdDebug = new MyEdit(m_debugWidget);
    m_cmdDebug->setDisabled(true);
    QVBoxLayout *m_layout = new QVBoxLayout(m_debugWidget);
    m_layout->addWidget(m_debugOutput);
    m_layout->addWidget(m_cmdDebug);
    connect(m_cmdDebug,SIGNAL(returnPressed()),
	this,SLOT(runCmdDebug()));
    connect(this,SIGNAL(cmdHistory(QString)),
	m_cmdDebug,SLOT(enter(QString)));
    
    m_applicationOutput = new QTextEdit(this);
    m_applicationOutput->setReadOnly(true);
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(m_applicationOutput,tr("Application"));
    m_tabWidget->addTab(m_compilerOutput,tr("Compilateur"));
    m_tabWidget->addTab(m_debugWidget,tr("Débogueur"));
    //insertion de l'objet dock servant à avoir une 
	//barre d'outils modulable
    m_dockWidget = new QDockWidget(tr("Outils"),this,0);
    m_dockWidget->setWidget(m_tabWidget);
    m_dockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    setCentralWidget(m_textEdit);
    this->addDockWidget(Qt::BottomDockWidgetArea,
	m_dockWidget,Qt::Horizontal);

//QApplication:->installEventFilter(this);

    readSettings();

    connect(m_textEdit,
            SIGNAL(textChanged()),this, SLOT(documentWasModified()));
    connect(this,
            SIGNAL(findNext()),this,SLOT(searchNext()));
    connect(m_compilerOutput, 
	SIGNAL(itemClicked(QListWidgetItem *)),
		this, 
	SLOT(itemDoubleClicked(QListWidgetItem *)));
    
//    QString path;
    // TODO MOdifier to home
    setCurrentFile("");
    m_textEdit->setFocus();

    m_compiler = new FpcDiscuss(this);
    m_application = new ApplicationDiscuss(this);
    m_debug = new GdbDiscuss(this);
    connect(this,SIGNAL(compile(QString)),
	m_compiler,SLOT(Execute(QString)));
    connect(m_compiler,SIGNAL(setCompileEnabled(bool)),
	this,SLOT(setCompileEnabled(bool)));
    connect(this,SIGNAL(run(QString)),
	m_application,SLOT(Execute(QString)));
    connect(m_application,SIGNAL(setApplicationEnabled(bool)),
	this,SLOT(setApplicationEnabled(bool)));
    connect(this,SIGNAL(debug(QString)),
	m_debug,SLOT(Execute(QString)));
    connect(m_debug,SIGNAL(setDebugEnabled(bool)),
	this,SLOT(setDebugEnabled(bool)));
    connect(m_debugNextAct,SIGNAL(triggered()),
	m_debug,SLOT(next()));
    connect(m_debugRunAct,SIGNAL(triggered()),
	m_debug,SLOT(run()));
    connect(this,SIGNAL(writeCmdDebug(QString)),
	m_debug,SLOT(writeCmd(QString)));
    connect(this,SIGNAL(displayCompilerList(QString)),
	m_compilerOutput,SLOT(displayCompilerList(QString)));
    //affichage ou non du composant de débogue
    QFile file(CONFIG_FILE);
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        while (!file.atEnd())
        {
            QByteArray buffer(file.readLine());
            QString path(buffer);
            if (path.contains("DEBUG_DISPLAY="))
            {//si le path est trouvé
                QString ch(path.section("DEBUG_DISPLAY=",1,-1));
                if (ch.contains("1"))
                    setDebugActivated(true);
                else setDebugActivated(false);
            }
        }
        file.close();
    }
}


//gère les évènements lors de la fermeture de l'application
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
        m_compiler->kill();
        m_application->kill();
        m_debug->kill();
    } else {
        event->ignore();
    }
}


//slots

//création d'un nouveau fichier
void MainWindow::newFile()
{
    if (maybeSave()) {
        m_textEdit->clear();
        setCurrentFile("");
    }
}

//ouverture d'un fichier
void MainWindow::open()
{
    if (maybeSave()){ 
#if defined Q_OS_LINUX
    QString fileName =
            QFileDialog::getOpenFileName(
                this,tr("Ouvrir un fichier"),
				QDir::homePath()+"/Bureau","Pascal (*.pas)");
#elif defined Q_OS_WIN
        QString fileName =
                QFileDialog::getOpenFileName(
                    this,"Ouvrir un fichier","","Pascal (*.pas)");
#endif
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

//sauvegarde d'un fichier
bool MainWindow::save()
{
    if (m_curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(m_curFile);
    }
}

//sauvegarde d'un fichier à un emplacement choisit 
//par l'utilisateur
bool MainWindow::saveAs()
{
 QString fileName = QFileDialog::getSaveFileName(
			this,tr("Sauvegarder sous..."),
			"untitled.pas",tr("Pascal (*.pas)"));
	
	if (fileName.isEmpty())
        	return false;
    
	return saveFile(fileName);
}

//recherche d'une expression régulière dans le composant Scintilla
void MainWindow::find()
{
    if (!m_findDialog) 
	{
    	m_findDialog = new FindDialog(NULL);
	m_findDialog->setModal(true);

	connect(m_findDialog, 
		SIGNAL(findNext(QString,Qt::CaseSensitivity)), 
		this, 
		SLOT(searchAfter(QString,Qt::CaseSensitivity)));

    	connect(m_findDialog, 
		SIGNAL(findPrevious(QString,Qt::CaseSensitivity)), 
		this, 
		SLOT(searchBefore(QString,Qt::CaseSensitivity)));
	}

     m_findDialog->show();
}

//remplacement d'une expression régulière dans le 
//composant Scintilla 
void MainWindow::replace()
{
    if (!m_replaceDialog) 
	{
    	m_replaceDialog = new ReplaceDialog(NULL);
		m_replaceDialog->setModal(true);

	connect(m_replaceDialog, 
		SIGNAL(findNext(QString,Qt::CaseSensitivity)), 
		this, 
		SLOT(searchAfter(QString,Qt::CaseSensitivity)));
    	
	connect(m_replaceDialog, 
		SIGNAL(findPrevious(QString,Qt::CaseSensitivity)), 
		this, 
		SLOT(searchBefore(QString,Qt::CaseSensitivity)));
	
	connect(m_replaceDialog,
		SIGNAL(replace(QString)),
		this,
		SLOT(replaceText(QString)));
	}

     m_replaceDialog->show();
}

//remplace le texte séléctionner dans le composant Scintilla 
//par la chaîne str
void MainWindow::replaceText(QString str)
{
	m_textEdit->replace(str);
}

//Accède à un ligne du composant Scintilla
void MainWindow::goLine()
{
    if (!m_lineDialog) 
	{
    	m_lineDialog = new LineDialog(NULL);
	m_lineDialog->setModal(true);
	
	connect(m_lineDialog, 
		SIGNAL(goTo(int,int)), 
		m_textEdit, 
		SLOT(setCursorPosition(int,int)));
	
        connect(m_lineDialog, 
		SIGNAL(warningNumber()), 
		this, 
		SLOT(warningLine()));
    	}
     m_lineDialog->show();
}

//affiche le composant de préférence
void MainWindow::setting()
{
    if(!m_settingWindow)
        m_settingWindow = new SettingWindow(this);
    m_settingWindow->show();
}

//fenêtre à propos
void MainWindow::about()
{
 QMessageBox::about(this, tr("A Propos"),
    tr(	"Version 32bits 13.10 \n\n"
        "Auteurs: \n"
        "Vilayvanh MANIVONG, Julien COURCELLE \n\n"
        "Département Informatique de l'Université d'Angers"));
}

//fenêtre d'aide
void MainWindow::help()
{
	if (!m_helpWindow)
        	m_helpWindow = new HelpWindow(this);
   
	 m_helpWindow->show();
}

//modification du composant Scintilla
void MainWindow::documentWasModified()
{
    setWindowModified(m_textEdit->isModified());
}

//recherche l'expression régulière de la droite vers la gauche
void MainWindow::searchBefore(const QString &str, 
Qt::CaseSensitivity cs)
{
	if (m_findDialog->m_findBefore == false)	
	{	
		m_textEdit->findFirst(str,true,cs,true,true,false,-1,-1,true);
	}
	else m_textEdit->findNext();
}

//recherche l'expression régulière de la gauche vers la droite
void MainWindow::searchAfter(const QString &str, Qt::CaseSensitivity cs)
{
	if (m_findDialog->m_findAfter == false)
	{
		m_textEdit->findFirst(str,true,cs,true,true,true,-1,-1,true);
	}
	else m_textEdit->findNext();
}

//cherche l'occurence  suivante d'un mot déjà recherché
void MainWindow::searchNext()
{
    m_textEdit->findNext();
}

//séléctionne tout le composant Scintilla
void MainWindow::selectAll()
{
    emit selectAll(true);
}

//affiche ou cache la barre d'outil pour la 
//compilation et le debugueur
void MainWindow::removeToolBar()
{
    if (!(m_toolBarDisplayAct->isChecked()))
        m_dockWidget->hide();
    else {
        m_dockWidget->show();
    }
}

//affiche ou cache la barre d'outil d'edition de texte
void MainWindow::removeEditBar()
{
    if (!(m_removeEditToolBarAct->isChecked()))
        m_editToolBar->hide();
    else {
        m_editToolBar->show();
    }
}

void MainWindow::removeBuildBar()
{
    if (!(m_removeBuildToolBarAct->isChecked()))
        m_buildToolBar->hide();
    else {
        m_buildToolBar->show();
    }
}

void MainWindow::removeFileBar()
{
    if (!(m_removeFileToolBarAct->isChecked()))
        m_fileToolBar->hide();
    else {
        m_fileToolBar->show();
    }
}

void MainWindow::removeDebugBar()
{
    if (!(m_removeDebugToolBarAct->isChecked()))
        m_debugToolBar->hide();
    else {
        m_debugToolBar->show();
    }
}


//affiche une fenêtre si l'utilisateur ne rentre pas un nombre
void MainWindow::warningLine()
{
 	m_lineDialog->setModal(false);
	m_lineDialog->clear();
	QMessageBox::warning(NULL,QObject::tr("attention"),
		QObject::tr("Veuillez entrer des nombres"));	
}

//émission d'un message si une application n'est pas trouvée
void MainWindow::appNotFound(QString str)
{
    QMessageBox::warning(NULL,
	QObject::tr("Application non trouvée"),str);
    m_compileAct->setDisabled(true);
    m_compileRunAct->setDisabled(true);
    m_debugAct->setDisabled(true);
}

//compile le fichier courant
void MainWindow::compile()
{

    if (!m_debug->isRunning() && !m_application->isRunning())
    {
        if (maybeSave())
        {//si le fichier courrant est sauvegardé
            //on envoie le signal de compilation
            emit compile(m_curFile);
           
		 connect(m_compilerOutput,
			SIGNAL(showError(QListWidgetItem*)),
			this,
			SLOT(itemDoubleClicked(QListWidgetItem*)));
        }
    }
}

//compile et lance le programme pascal édité
void MainWindow::compileAndRun()
{
    if (!m_debug->isRunning() && !m_application->isRunning())
    {
        compile();
        if ((m_compiler->m_prog) != NULL)
            (m_compiler->m_prog)->waitForFinished(-1);
        if (m_compiler->m_OutputStream !=NULL)
        {
            if (!(m_compiler->m_OutputStream)->
			contains("compilation aborted",Qt::CaseInsensitive))
            {//si la compilation à réussie
                emit run(m_curFile);
            }
            else QMessageBox::warning(NULL,
		QObject::tr("Attention"),
		QObject::tr(
		"La compilation a échoué. \
		Le programe ne peut donc pas être lancé. \
		Veuillez corriger les erreurs de votre programme puis \
		réitérer l'opération."
		));
        }
    }
}

//lance le débogueur de l'application
void MainWindow::debug()
{
    if (!m_debug->isRunning())
    {
        if (m_application->isRunning())
            m_application->kill();
        compile();
        if ((m_compiler->m_prog) != NULL)
            (m_compiler->m_prog)->waitForFinished(-1);
        if (m_compiler->m_OutputStream !=NULL)
        {
            if (!(m_compiler->m_OutputStream)
			->contains("compilation aborted",Qt::CaseInsensitive))
            {//si la compilation à réussie
                m_cmdDebug->setEnabled(true);
                if ((m_debug->m_OutputStream) != NULL )
                    *(m_debug->m_OutputStream) = "";
                if ((m_debug->m_ErrorStream) != NULL )
                    *(m_debug->m_ErrorStream) = "";
                emit debug(m_curFile);
                connect(m_killAct,SIGNAL(triggered()),
				m_debug,SLOT(kill()));
                disconnect(	m_textEdit,
				SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)),
				this,SLOT(marginClicked
				(int,int,Qt::KeyboardModifiers)));
            }
            else QMessageBox::warning(NULL,
		QObject::tr("Attention"),
		QObject::tr(
		"La compilation a échoué. \
		Le programe ne peut donc pas être débogué. \
		Veuillez corriger les erreurs de votre programme \
		puis réitérer l'opération."
		));
        }
    }
}

void MainWindow::runCmdDebug()
{
    QString str(m_cmdDebug->text());
    if (m_cmdDebug->isModified()) 
	emit cmdHistory(m_cmdDebug->text());
    m_cmdDebug->clear();
    emit writeCmdDebug(str);
}

//affiche dans la barre d'outil le résultat de la compilation
void MainWindow::displayCompile(QString str)
{
    //on affiche la barre d'outil si elle n'est pas déjà présente
    if (!m_removeAct->isChecked())
    {
        m_removeAct->setDisabled(false);
        m_dockWidget->show();
    }
    //on enlève tout les markers de compilation
    m_textEdit->markerDeleteAll(1);
    //si du texte à été selectionné, on le déséléctionne
    if (m_textEdit->hasSelectedText())
        m_textEdit->selectAll(false);
    
    emit displayCompilerList(str);
    m_tabWidget->setCurrentWidget(m_compilerOutput);
    if (m_compilerOutput->containsError())
    {
        m_nextCompileErrorAct->setEnabled(true);
        m_previousCompileErrorAct->setEnabled(true);
        m_textEdit->setCaretLineBackgroundColor
		(QColor(255, 198, 190));
    }
    else{
        m_nextCompileErrorAct->setEnabled(false);
        m_previousCompileErrorAct->setEnabled(false);
        m_textEdit->setCaretLineBackgroundColor
		(QColor(244, 244, 255));
    }
}

//affiche dans la barre d'outil le résultat du flux 
//de sortie de l'application pascal
void MainWindow::displayApplication(QString str)
{
    m_applicationOutput->clear();
    //on affiche la barre d'outil si elle n'est pas déjà présente
    if (!m_removeAct->isChecked())
    {
        m_removeAct->setDisabled(false);
        m_dockWidget->show();
    }
    m_tabWidget->setCurrentWidget
	(m_applicationOutput);
    m_applicationOutput->setText(str);
}

void MainWindow::displayDebug(QString str)
{
    //on affiche la barre d'outil si elle n'est pas déjà présente
    if (!m_removeAct->isChecked())
    {
        m_removeAct->setDisabled(false);
        m_dockWidget->show();
    }
    m_debugRunAct->setEnabled(true);
    m_debugNextAct->setEnabled(true);
    m_killAct->setEnabled(true);
    m_debugOutput->clear();
    m_tabWidget->setCurrentWidget(m_debugWidget);
    m_debugOutput->setPlainText(str);
    m_debugOutput->moveCursor
	(QTextCursor::End,QTextCursor::MoveAnchor);
    m_cmdDebug->setFocus();
}

//récupère le numéros de la ligne où se situe l'erreur et l'affiche sur 
//le composant scintilla
void MainWindow::itemDoubleClicked ( QListWidgetItem * item )
{
    QString str(item->text());
    if (	str.contains("Fatal",Qt::CaseInsensitive) 
			|| 
		str.contains("Error",Qt::CaseInsensitive) 
			|| 
		str.contains("Warning",Qt::CaseInsensitive))
    {
        QString ch(str.section('(', 1, 1));
        QString ch2(ch.section(',',0,0));
        QString ch3(ch.section(',',1,1));
        QString ch4(ch3.section(')',0,0));
        int numPosError = ch4.toInt(0,10);
        int numLigne = ch2.toInt(0,10);
        m_textEdit->setCursorPosition(numLigne-1,numPosError);
        m_textEdit->markerAdd(numLigne-1,1);
        m_textEdit->setFocus();
    }
}

void MainWindow::shortMessageShow()
{

    //on enlève tout les markers de compilation
    m_textEdit->markerDeleteAll(1);
    if ( (m_compiler->m_OutputStream) != NULL )
	m_shortMessageAct->setChecked(true);
    emit message(0);
}

void MainWindow::mediumMessageShow()
{
    //on enlève tout les markers de compilation
    m_textEdit->markerDeleteAll(1);
    if ( (m_compiler->m_OutputStream) != NULL )
	m_mediumMessageAct->setChecked(true);
    emit message(1);
}

void MainWindow::longMessageShow()
{
    //on enlève tout les markers de compilation
    m_textEdit->markerDeleteAll(1);
    if ( (m_compiler->m_OutputStream) != NULL )
        m_longMessageAct->setChecked(true);
    emit message(2);
}

void MainWindow::marginClicked(int, int line, 
Qt::KeyboardModifiers)
{
    m_textEdit->markerAdd(line,2);
    std::vector<int>::iterator it;
    bool find = false;
    for (it = m_debugMarkerVect.begin() ; 
	it != m_debugMarkerVect.end() ; it++)
    {
        if (*it == line)
        {
            find = true;
            if (m_debugMarkerVect.size() == 1)
            {//on regarde si il ne reste qu'un 
			//seul objet dans le vector
                m_debugMarkerVect.pop_back();
                break;
            }
            else if (m_debugMarkerVect.at
			(m_debugMarkerVect.size()-1) == (*it))
                {//on regarde si l'objet trouvé 
				//est le dernier dans le vector
                    m_debugMarkerVect.pop_back();
                    break;
                }
                else m_debugMarkerVect.erase(it);
        }
    }
    if (!find)
        m_debugMarkerVect.push_back(line);
    else m_textEdit->markerDelete(line,2);
}

void MainWindow::writeBreakPoints()
{
    //cout << "ecriture des breakpoints" << endl;
    std::vector<int>::iterator it;
    for (it = m_debugMarkerVect.begin() ; 
	it != m_debugMarkerVect.end() ; it++)
    {
        //cout << "break " << *it << endl;
        int line = (*it)+1;
        QString cmd(tr("break ")+QString::number(line,10));
        emit writeCmdDebug(cmd);
    }
}

void MainWindow::connectBreakPoints()
{
     connect(m_textEdit,SIGNAL(marginClicked
	 (int,int,Qt::KeyboardModifiers)),
	 this,SLOT(marginClicked(int,int,Qt::KeyboardModifiers)));
}

void MainWindow::print()
{
    QTextDocument *document = new QTextDocument(m_textEdit->text());
    QsciPrinter printer;
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() != QDialog::Accepted)
        return;
    document->print(&printer);
    statusBar()->showMessage(tr("Prêt"), 2000);
}

void MainWindow::cursorPositionChanged (int line,int col)
{
    if (m_textEdit->markersAtLine(line) == 2)
    {
        m_compilerOutput->selectErrorItem(line+1);
    }
    QString rowStr(tr(" Ligne : ")+
	QString::number(line+1,10)+tr(" "));
    m_rowLabel->setText(rowStr);
    QString colStr(tr(" Colonne : ")+
	QString::number(col,10)+tr(" "));
    m_colLabel->setText(colStr);
}

void MainWindow::setCompileEnabled(bool b)
{
    if (b)
    {
        m_compilerOutput->setEnabled(b);
        m_compileAct->setEnabled(!b);
        m_applicationOutput->setEnabled(!b);
        m_compileRunAct->setEnabled(!b);
        m_debugOutput->setEnabled(!b);
        m_cmdDebug->setEnabled(!b);
        m_debugAct->setEnabled(!b);
        m_debugRunAct->setEnabled(!b);
        m_debugNextAct->setEnabled(!b);
        m_killAct->setEnabled(!b);
    }
    else{
        m_compilerOutput->setEnabled(!b);
        m_compileAct->setEnabled(!b);
        m_applicationOutput->setEnabled(b);
        m_compileRunAct->setEnabled(!b);
        m_debugOutput->setEnabled(b);
        m_cmdDebug->setEnabled(b);
        m_debugAct->setEnabled(!b);
        m_debugRunAct->setEnabled(b);
        m_debugNextAct->setEnabled(b);
        m_killAct->setEnabled(b);
    }
}

void MainWindow::setApplicationEnabled(bool b)
{
    if (b)
    {
        m_compilerOutput->setEnabled(b);
        m_compileAct->setEnabled(!b);
        m_applicationOutput->setEnabled(b);
        m_compileRunAct->setEnabled(!b);
        m_debugOutput->setEnabled(!b);
        m_cmdDebug->setEnabled(!b);
        m_debugAct->setEnabled(!b);
        m_debugRunAct->setEnabled(!b);
        m_debugNextAct->setEnabled(!b);
        m_killAct->setEnabled(!b);
    }
    else{
        m_compilerOutput->setEnabled(!b);
        m_compileAct->setEnabled(!b);
        m_applicationOutput->setEnabled(b);
        m_compileRunAct->setEnabled(!b);
        m_debugOutput->setEnabled(b);
        m_cmdDebug->setEnabled(b);
        m_debugAct->setEnabled(!b);
        m_debugRunAct->setEnabled(b);
        m_debugNextAct->setEnabled(b);
        m_killAct->setEnabled(b);
    }
}

void MainWindow::setDebugEnabled(bool b)
{
    if (b)
    {
        m_compilerOutput->setEnabled(b);
        m_compileAct->setEnabled(!b);
        m_applicationOutput->setEnabled(!b);
        m_compileRunAct->setEnabled(!b);
        m_debugOutput->setEnabled(b);
        m_cmdDebug->setEnabled(b);
        m_debugAct->setEnabled(!b);
        m_debugRunAct->setEnabled(b);
        m_debugNextAct->setEnabled(b);
        m_killAct->setEnabled(b);
    }
    else{
        m_compilerOutput->setEnabled(!b);
        m_compileAct->setEnabled(!b);
        m_applicationOutput->setEnabled(b);
        m_compileRunAct->setEnabled(!b);
        m_debugOutput->setEnabled(!b);
        m_cmdDebug->setEnabled(b);
        m_debugAct->setEnabled(!b);
        m_debugRunAct->setEnabled(b);
        m_debugNextAct->setEnabled(b);
        m_killAct->setEnabled(b);
    }
}


void MainWindow::setDebugActivated(bool b)
{
    if (b)
    {
        menuBar()->clear();
        m_fileMenu = menuBar()->addMenu(tr("&Fichier"));
        m_fileMenu->addAction(m_newAct);
        m_fileMenu->addAction(m_openAct);
        m_fileMenu->addAction(m_saveAct);
        m_fileMenu->addAction(m_saveAsAct);
        m_fileMenu->addSeparator();
        m_fileMenu->addAction(m_printAct);
        m_fileMenu->addSeparator();
        m_fileMenu->addAction(m_exitAct);

        m_editMenu = menuBar()->addMenu(tr("&Edition"));
        m_editMenu->addAction(m_undoAct);
        m_editMenu->addAction(m_redoAct);
        m_editMenu->addSeparator();
        m_editMenu->addAction(m_cutAct);
        m_editMenu->addAction(m_copyAct);
        m_editMenu->addAction(m_pasteAct);
        m_editMenu->addSeparator();
        m_editMenu->addAction(m_selectAllAct);
        m_editMenu->addSeparator();
        m_editMenu->addAction(m_removeAct);

        m_researchMenu = menuBar()->addMenu(tr("&Rechercher"));
        m_researchMenu->addAction(m_researchAct);
        m_researchMenu->addAction(m_researchNextAct);
        m_researchMenu->addAction(m_replaceAct);
        m_researchMenu->addSeparator();
        m_researchMenu->addAction(m_lineAct);

        m_displayMenu = menuBar()->addMenu(tr("&Affichage"));
        m_removeMenu = new QMenu(tr("&Barres"));
        m_removeMenu->addAction(m_removeFileToolBarAct);
        m_removeMenu->addAction(m_removeEditToolBarAct);
        m_removeMenu->addAction(m_removeBuildToolBarAct);
        m_removeMenu->addAction(m_removeDebugToolBarAct);
        m_removeMenu->addAction(m_toolBarDisplayAct);
        m_displayMenu->addMenu(m_removeMenu);
        m_compileMenu = new QMenu(tr("&Compilateur"),this);
        m_compileMenu->addAction(m_shortMessageAct);
        m_compileMenu->addAction(m_mediumMessageAct);
        m_compileMenu->addAction(m_longMessageAct);
        m_displayMenu->addMenu(m_compileMenu);

        m_buildMenu = menuBar()->addMenu(tr("&Construction"));
        m_buildMenu->addAction(m_compileAct);
        m_buildMenu->addAction(m_compileRunAct);
        m_buildMenu->addSeparator();
        m_buildMenu->addAction(m_nextCompileErrorAct);
        m_buildMenu->addAction(m_previousCompileErrorAct);
        m_buildMenu->addSeparator();
        m_buildMenu->addAction(m_settingCompileAct);

        m_debugMenu = menuBar()->addMenu(tr("&Débogueur"));
        m_debugMenu->addAction(m_debugAct);
        m_debugMenu->addSeparator();
        m_debugMenu->addAction(m_debugRunAct);
        m_debugMenu->addAction(m_debugNextAct);

        menuBar()->addSeparator();

        m_helpMenu = menuBar()->addMenu(tr("&Aide"));
        m_helpMenu->addAction(m_helpAct);
        m_helpMenu->addAction(m_aboutAct);
        m_helpMenu->addAction(m_aboutQtAct);
        m_debugToolBar->show();
        if (m_tabWidget->count() == 2)
            m_tabWidget->addTab(m_debugWidget,tr("Débogueur"));
    }
    else{
        menuBar()->clear();
        m_fileMenu = menuBar()->addMenu(tr("&Fichier"));
        m_fileMenu->addAction(m_newAct);
        m_fileMenu->addAction(m_openAct);
        m_fileMenu->addAction(m_saveAct);
        m_fileMenu->addAction(m_saveAsAct);
        m_fileMenu->addSeparator();
        m_fileMenu->addAction(m_printAct);
        m_fileMenu->addSeparator();
        m_fileMenu->addAction(m_exitAct);

        m_editMenu = menuBar()->addMenu(tr("&Edition"));
        m_editMenu->addAction(m_undoAct);
        m_editMenu->addAction(m_redoAct);
        m_editMenu->addSeparator();
        m_editMenu->addAction(m_cutAct);
        m_editMenu->addAction(m_copyAct);
        m_editMenu->addAction(m_pasteAct);
        m_editMenu->addSeparator();
        m_editMenu->addAction(m_selectAllAct);
        m_editMenu->addSeparator();
        m_editMenu->addAction(m_removeAct);

        m_researchMenu = menuBar()->addMenu(tr("&Rechercher"));
        m_researchMenu->addAction(m_researchAct);
        m_researchMenu->addAction(m_researchNextAct);
        m_researchMenu->addAction(m_replaceAct);
        m_researchMenu->addSeparator();
        m_researchMenu->addAction(m_lineAct);

        m_displayMenu = menuBar()->addMenu(tr("&Affichage"));
        m_removeMenu = new QMenu(tr("&Barres"));
        m_removeMenu->addAction(m_removeFileToolBarAct);
        m_removeMenu->addAction(m_removeEditToolBarAct);
        m_removeMenu->addAction(m_removeBuildToolBarAct);
        m_removeMenu->addAction(m_toolBarDisplayAct);
        m_displayMenu->addMenu(m_removeMenu);
        m_compileMenu = new QMenu(tr("&Compilateur"),this);
        m_compileMenu->addAction(m_shortMessageAct);
        m_compileMenu->addAction(m_mediumMessageAct);
        m_compileMenu->addAction(m_longMessageAct);
        m_displayMenu->addMenu(m_compileMenu);

        m_buildMenu = menuBar()->addMenu(tr("&Construction"));
        m_buildMenu->addAction(m_compileAct);
        m_buildMenu->addAction(m_compileRunAct);
        m_buildMenu->addSeparator();
        m_buildMenu->addAction(m_nextCompileErrorAct);
        m_buildMenu->addAction(m_previousCompileErrorAct);
        m_buildMenu->addSeparator();
        m_buildMenu->addAction(m_settingCompileAct);

        menuBar()->addSeparator();

        m_helpMenu = menuBar()->addMenu(tr("&Aide"));
        m_helpMenu->addAction(m_helpAct);
        m_helpMenu->addAction(m_aboutAct);
        m_helpMenu->addAction(m_aboutQtAct);
        m_debugToolBar->hide();
        if (m_tabWidget->count() == 3)
            m_tabWidget->removeTab(2);
    }
}

//méthodes

void MainWindow::initFileconf()
{
    if (!QFile::exists(CONFIG_FILE))
    {
        QFile file(CONFIG_FILE);

        file.open(QIODevice::WriteOnly);
#if defined Q_OS_LINUX
file.write("#Version : 32bits 13.10 ") ;
file.write("--> Chemin menant au compilateur FPC sous Linux <--\n");
//modifier par un appel system `which fpc` qui retourne 
//le chemin de l'executable
file.write("FPC_PATH_X11=/usr/bin/fpc\n");
file.write("--> Arguments du compilateur FPC <--\n");
// Modifier par un appel system `fpc -iV` 
//retourne le numero de  la version de fpc 
file.write("FPC_ARGS=-g -Fr/usr/lib/fpc/2.4.4/msg/errorf.msg \n");
file.write("--> Chemin menant au débogueur GDB sous Linux <--\n");
file.write("GDB_PATH_X11=/usr/bin/gdb\n");
file.write("--> Chemin menant aux différents terminaux sous Linux <--\n");
file.write("TERM_PATH_X11=/usr/bin/konsole,/usr/bin/xterm,/usr/bin/gnome-terminal\n");
file.write("--> Chemin du dossier contenant la documentation sous Linux <--\n");
file.write("DOC_PATH_X11=/usr/share/doc/fpc-2.4.4/user_doc/ref\n");

#elif defined Q_OS_WIN32
file.write("--> Chemin menant au compilateur FPC sous Windows <--\n");
file.write("FPC_PATH_WIN32=C:\\FPC\\2.6.2\\bin\\i386-win32\\fpc.exe\n");
file.write("--> Chemin menant au compilateur GDB sous Windows <--\n");
file.write("GDB_PATH_WIN32=C:\\FPC\\2.6.2\\bin\\i386-win32\\gdb.exe\n");
file.write("--> Chemin menant au terminal de Windows <--\n");
file.write("TERM_PATH_WIN32=C:\\WINDOWS\\system32\\cmd.exe\n");
file.write("--> Chemin du dossier contenant la documentation sous Windows <--\n");
//a modifier
file.write("DOC_PATH_WIN32=doc\\fpdocs-2.0.4\\\n");

#endif
        
	file.write("--> Nom du fichier index de la documentation <--\n");
        file.write("DOC_INDEX=ref.html\n");
        file.write("--> Affichage des composants de debogue <--\n");
        file.write("DEBUG_DISPLAY=0\n");
        file.close();
    }
}

//initilaisation du composant Scintilla
void MainWindow::initScintilla()
{
    m_textEdit = new QsciScintilla;
    //création de la marge avec les numéros de lignes
    m_textEdit->setMarginLineNumbers(0,true);
    //création d'un marge à  droite
    m_textEdit->setMarginWidth(0,40);
    //on sensibilise la marge de gauche
    m_textEdit->setMarginSensitivity(1,true);
    //séléction du lexique Pascal
    QsciLexerPascal *lexPascal = new QsciLexerPascal(0);
    m_textEdit->setLexer(lexPascal);
    //définition du "marker" de compilation
    m_textEdit->markerDefine (QsciScintilla::MarkerSymbol(0),1);
    m_textEdit->setMarkerBackgroundColor(Qt::red,1);
    //définition du "marker" de debug
    m_textEdit->markerDefine (QsciScintilla::MarkerSymbol(2),2);
    m_textEdit->setMarkerBackgroundColor(Qt::black,2);
    //mise en place de la ligne de surbrillance
    m_textEdit->setCaretLineVisible(true);
    m_textEdit->setCaretLineBackgroundColor(QColor(244, 244, 255));
    connect(m_textEdit,
	SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)),
	this,SLOT(marginClicked(int,int,Qt::KeyboardModifiers)));
    connect(m_textEdit,
	SIGNAL(cursorPositionChanged(int,int)),
	this,SLOT(cursorPositionChanged(int,int)));
}

//créations des différentes actions possibles
void MainWindow::createActions()
{
    m_newAct = new QAction(QIcon(":/images/new.png"), 
	tr("&Nouveau"), this);
    m_newAct->setShortcut(tr("Ctrl+N"));
    m_newAct->setStatusTip(tr("Créer un nouveau fichier"));
    connect(m_newAct, SIGNAL(triggered()), 
	this, SLOT(newFile()));

    m_openAct = new QAction(QIcon(":/images/open.png"), 
	tr("&Ouvrir..."), this);
    m_openAct->setShortcut(tr("Ctrl+O"));
    m_openAct->setStatusTip(tr("Ouvrir un fichier existant"));
    connect(m_openAct, SIGNAL(triggered()), this, SLOT(open()));

    m_saveAct = new QAction(QIcon(":/images/save.png"), 
	tr("&Sauvegarder"), this);
    m_saveAct->setShortcut(tr("Ctrl+S"));
    m_saveAct->setStatusTip(
	tr("Sauvegarder le document sur le disque"));
    connect(m_saveAct, SIGNAL(triggered()), 
	this, SLOT(save()));

    m_saveAsAct = new QAction(QIcon(":/images/save-as.png"),
	tr("Sa&uvegarder sous..."), this);
    m_saveAsAct->setStatusTip(
	tr("Sauvegarde le document sous un nouveau nom"));
    connect(m_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    m_exitAct = new QAction(QIcon(":/images/exit.png"),
	tr("&Quitter"), this);
    m_exitAct->setShortcut(tr("Ctrl+Q"));
    m_exitAct->setStatusTip(tr("Quitter l'application"));
    connect(m_exitAct, SIGNAL(triggered()), 
	this, SLOT(close()));

    m_cutAct = new QAction(QIcon(":/images/cut.png"), 
	tr("Cou&per"), this);
    m_cutAct->setShortcut(tr("Ctrl+X"));
    m_cutAct->setStatusTip(tr("Couper la séléction courante"));
    connect(m_cutAct, SIGNAL(triggered()),
	m_textEdit, SLOT(cut()));

    m_copyAct = new QAction(QIcon(":/images/copy.png"), 
	tr("&Copier"), this);
    m_copyAct->setShortcut(tr("Ctrl+C"));
    m_copyAct->setStatusTip(tr("Copier la séléction courante"));
    connect(m_copyAct, SIGNAL(triggered()), 
	m_textEdit, SLOT(copy()));

    m_pasteAct = new QAction(QIcon(":/images/paste.png"),
	tr("Co&ller"), this);
    m_pasteAct->setShortcut(tr("Ctrl+V"));
    m_pasteAct->setStatusTip(tr("Coller sur la zone de texte"));
    connect(m_pasteAct, SIGNAL(triggered()), 
	m_textEdit, SLOT(paste()));

    m_aboutAct = new QAction(QIcon(":/images/info.png"),
	tr("A &Propos"), this);
    m_aboutAct->setStatusTip(tr("Montre la fenêtre à  propos"));
    connect(m_aboutAct, SIGNAL(triggered()), 
	this, SLOT(about()));

    m_aboutQtAct = new QAction(QIcon(":/images/info.png"),
	tr("A Propos de &Qt"), this);
    m_aboutQtAct->setStatusTip(
	tr("Show the Qt library's About box"));
    connect(m_aboutQtAct, SIGNAL(triggered()), 
	qApp, SLOT(aboutQt()));

    m_helpAct = new QAction(QIcon(":/images/help.png"),
	tr("&Aide"), this);
    m_helpAct->setShortcut(tr("F1"));
    m_helpAct->setStatusTip(tr("Aide"));
    connect(m_helpAct, SIGNAL(triggered()), 
	this, SLOT(help()));

    m_cutAct->setEnabled(false);
    m_copyAct->setEnabled(false);
    connect(m_textEdit, SIGNAL(copyAvailable(bool)), 
	m_cutAct, SLOT(setEnabled(bool)));
    connect(m_textEdit, SIGNAL(copyAvailable(bool)), 
	m_copyAct, SLOT(setEnabled(bool)));
	
    m_researchAct = new QAction(QIcon(":/images/find.png"),
	tr("&Rechercher"),this);
    m_researchAct->setShortcut(tr("Ctrl+F"));
    m_researchAct->setStatusTip(tr("Rechercher"));
    connect(m_researchAct,SIGNAL(triggered()),this,SLOT(find()));

    m_researchNextAct = new QAction(QIcon(":/images/find.png"),
	tr("Rechercher &suivant"),this);
    m_researchNextAct->setShortcut(tr("F3"));
    m_researchNextAct->setStatusTip(tr("Rechercher suivant"));
    connect(m_researchNextAct,SIGNAL(triggered()),
	this,SLOT(searchNext()));


    m_replaceAct = new QAction(QIcon(":/images/find-replace.png"),
	tr("Rem&placer"),this);
    m_replaceAct->setShortcut(tr("Ctrl+R"));
    m_replaceAct->setStatusTip(tr("Remplacer"));
    connect(m_replaceAct,SIGNAL(triggered()),
	this,SLOT(replace()));	

    m_undoAct = new QAction(QIcon(":/images/undo.png"),
	tr("&Défaire"),this);
    m_undoAct->setShortcut(tr("Ctrl+Z"));
    m_undoAct->setStatusTip(tr("Défaire"));
    connect(m_undoAct,SIGNAL(triggered()),
	m_textEdit,SLOT(undo()));

    m_redoAct = new QAction(QIcon(":/images/redo.png"),
	tr("&Refaire"),this);
    m_redoAct->setShortcut(tr("Ctrl+Y"));
    m_redoAct->setStatusTip(tr("Refaire"));
    connect(m_redoAct,SIGNAL(triggered()),m_textEdit,
	SLOT(redo()));

    m_removeAct = new QAction(QIcon(":/images/delete.png"),
	tr("&Supprimer"),this);
    m_removeAct->setStatusTip(tr("Supprimer"));
    connect(m_removeAct,SIGNAL(triggered()),m_textEdit,
	SLOT(removeSelectedText()));

    m_lineAct = new QAction(QIcon(":/images/go-to-line.png"),
	tr("&Aller à "),this);
    m_lineAct->setShortcut(tr("Ctrl+G"));
    m_lineAct->setStatusTip(tr("Aller à "));
    connect(m_lineAct,SIGNAL(triggered()),this,SLOT(goLine()));

    m_selectAllAct = new QAction(QIcon(":/images/select-all.png"),
	tr("Sélectionner &tout"),this);
    m_selectAllAct->setShortcut(tr("Ctrl+A"));
    m_selectAllAct->setStatusTip(tr("Sélectionner tout"));
    connect(m_selectAllAct,SIGNAL(triggered()),
	this,SLOT(selectAll()));
    connect(this,SIGNAL(selectAll(bool)),
	m_textEdit,SLOT(selectAll(bool)));

    m_compileAct = new QAction(QIcon(":/images/compile.png"),
	tr("&Compiler"),this);
    m_compileAct->setShortcut(tr("F8"));
    m_compileAct->setStatusTip(tr("Compiler"));
    connect(m_compileAct,SIGNAL(triggered()),
	this,SLOT(compile()));

    m_compileRunAct = new QAction(
	QIcon(":/images/compile-run.png"),
	tr("Compiler et &exécuter"),this);
    m_compileRunAct->setShortcut(tr("Shift+F9"));
    m_compileRunAct->setStatusTip(tr("Compiler et exécuter"));
    connect(m_compileRunAct,SIGNAL(triggered()),
	this,SLOT(compileAndRun()));

    m_settingCompileAct = new QAction(tr("Pré&férences"),this);
    m_settingCompileAct->setStatusTip(
	tr("Préférences pour le compilateur"));
    connect(m_settingCompileAct,
	SIGNAL(triggered()),this,SLOT(setting()));

    m_debugAct = new QAction(QIcon(":/images/debug.png"),
	tr("&Déboguer"),this);
    m_debugAct->setShortcut(tr("F9"));
    m_debugAct->setStatusTip(tr("Déboguer"));
    connect(m_debugAct,SIGNAL(triggered()),
	this,SLOT(debug()));
    
    m_debugRunAct = new QAction(QIcon(":/images/debug-run.png"),
	tr("&Lancer l'application"),this);
    m_debugRunAct->setShortcut(tr("F10"));
    m_debugRunAct->setEnabled(false);
    m_debugRunAct->setStatusTip(tr("Lancer l'application"));

    m_debugNextAct = new QAction(QIcon(":/images/next.png"),
	tr("&Etape suivante"),this);
    m_debugNextAct->setShortcut(tr("F5"));
    m_debugNextAct->setEnabled(false);
    m_debugNextAct->setStatusTip(tr("Etape suivante"));

    m_killAct = new QAction(QIcon(":/images/process-stop.png"),
	tr("&Kill"),this);
    m_killAct->setStatusTip(tr("Kill"));
    m_killAct->setEnabled(false);

    m_toolBarDisplayAct = new QAction(tr("&Barre d'outils"),
	this);
    m_toolBarDisplayAct->setStatusTip(tr("Barre d'outils"));
    m_toolBarDisplayAct->setCheckable(true);
    m_toolBarDisplayAct->setChecked(true);
    connect(m_toolBarDisplayAct,SIGNAL(triggered()),
	this,SLOT(removeToolBar()));
    
    m_removeFileToolBarAct = new QAction(tr("Barre de fichier"),
	this);
    m_removeFileToolBarAct->setCheckable(true);
    m_removeFileToolBarAct->setChecked(true);
    connect(m_removeFileToolBarAct,SIGNAL(triggered()),
	this,SLOT(removeFileBar()));

    m_removeEditToolBarAct = new QAction(tr("Barre d'edition"),
	this);	
    m_removeEditToolBarAct->setCheckable(true);
    m_removeEditToolBarAct->setChecked(true);
    connect(m_removeEditToolBarAct,SIGNAL(triggered()),
	this,SLOT(removeEditBar()));
	
    m_removeBuildToolBarAct = new QAction(
	tr("Barre de construction"),this);
    m_removeBuildToolBarAct->setCheckable(true);
    m_removeBuildToolBarAct->setChecked(true);	
    connect(m_removeBuildToolBarAct,SIGNAL(triggered()),
	this,SLOT(removeBuildBar()));

    m_removeDebugToolBarAct = new QAction(
	tr("Barre du débogueur"),this);
    m_removeDebugToolBarAct->setCheckable(true);
    m_removeDebugToolBarAct->setChecked(true);
    connect(m_removeDebugToolBarAct,SIGNAL(triggered()),
	this,SLOT(removeDebugBar()));

    m_longMessageAct = new QAction(tr("Message long"),this);
    m_longMessageAct->setCheckable(true);
    connect(m_longMessageAct,SIGNAL(triggered()),
	this,SLOT(longMessageShow()));

    m_mediumMessageAct = new QAction(tr("Message moyen "),
	this);
    m_mediumMessageAct->setCheckable(true);
    m_mediumMessageAct->setChecked(true);
    connect(m_mediumMessageAct,SIGNAL(triggered()),
	this,SLOT(mediumMessageShow()));

    m_shortMessageAct = new QAction(tr("Message court"),
	this);
    m_shortMessageAct->setCheckable(true);
    connect(m_shortMessageAct,SIGNAL(triggered()),
	this,SLOT(shortMessageShow()));
    
    //on regroupe les trois actions afin de ne pas 
	//pouvoir en faire 2 en même temps
    m_messageActGr = new QActionGroup(this);
    m_messageActGr->addAction(m_shortMessageAct);
    m_messageActGr->addAction(m_mediumMessageAct);
    m_messageActGr->addAction(m_longMessageAct);

    m_nextCompileErrorAct = new QAction(
	QIcon(":/images/next.png"),tr("Erreur &suivante"),this);
    m_nextCompileErrorAct->setShortcut(tr("F4"));
    m_nextCompileErrorAct->setStatusTip(tr("Erreur suivante"));
    m_nextCompileErrorAct->setEnabled(false);
    connect(m_nextCompileErrorAct,SIGNAL(triggered()),
	m_compilerOutput,SLOT(searchNextError()));

    m_previousCompileErrorAct = new QAction(
	QIcon(":/images/previous.png"),tr("Erreur &précédente"),this);
    m_previousCompileErrorAct->setShortcut(tr("Shift+F4"));
    m_previousCompileErrorAct->setStatusTip(
	tr("Erreur précédente"));
    m_previousCompileErrorAct->setEnabled(false);
    connect(m_previousCompileErrorAct,SIGNAL(triggered()),
	m_compilerOutput,SLOT(searchPreviousError()));
    
    m_printAct = new QAction(QIcon(":/images/print.png"),
	tr("&Imprimer"),this);
    m_printAct->setShortcut(tr("Ctrl+P"));
    m_printAct->setStatusTip(tr("Imprimer"));
    connect(m_printAct,SIGNAL(triggered()),this,SLOT(print()));
}

//création de la barre de menu
void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&Fichier"));
    m_fileMenu->addAction(m_newAct);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_printAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);

    m_editMenu = menuBar()->addMenu(tr("&Edition"));
    m_editMenu->addAction(m_undoAct);
    m_editMenu->addAction(m_redoAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_cutAct);
    m_editMenu->addAction(m_copyAct);
    m_editMenu->addAction(m_pasteAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_selectAllAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_removeAct);

    m_researchMenu = menuBar()->addMenu(tr("&Rechercher"));
    m_researchMenu->addAction(m_researchAct);
    m_researchMenu->addAction(m_researchNextAct);
    m_researchMenu->addAction(m_replaceAct);
    m_researchMenu->addSeparator();
    m_researchMenu->addAction(m_lineAct);

    m_displayMenu = menuBar()->addMenu(tr("&Affichage"));
    m_removeMenu = new QMenu(tr("&Barres"));
    m_removeMenu->addAction(m_removeFileToolBarAct);
    m_removeMenu->addAction(m_removeEditToolBarAct);
    m_removeMenu->addAction(m_removeBuildToolBarAct);
    m_removeMenu->addAction(m_removeDebugToolBarAct);
    m_removeMenu->addAction(m_toolBarDisplayAct);
    m_displayMenu->addMenu(m_removeMenu);
    m_compileMenu = new QMenu(tr("&Compilateur"),this);
    m_compileMenu->addAction(m_shortMessageAct);
    m_compileMenu->addAction(m_mediumMessageAct);
    m_compileMenu->addAction(m_longMessageAct);
    m_displayMenu->addMenu(m_compileMenu);

    m_buildMenu = menuBar()->addMenu(tr("&Construction"));
    m_buildMenu->addAction(m_compileAct);
    m_buildMenu->addAction(m_compileRunAct);
    m_buildMenu->addSeparator();
    m_buildMenu->addAction(m_nextCompileErrorAct);
    m_buildMenu->addAction(m_previousCompileErrorAct);
    m_buildMenu->addSeparator();
    m_buildMenu->addAction(m_settingCompileAct);

    m_debugMenu = menuBar()->addMenu(tr("&Débogueur"));
    m_debugMenu->addAction(m_debugAct);
    m_debugMenu->addSeparator();
    m_debugMenu->addAction(m_debugRunAct);
    m_debugMenu->addAction(m_debugNextAct);

    menuBar()->addSeparator();

    m_helpMenu = menuBar()->addMenu(tr("&Aide"));
    m_helpMenu->addAction(m_helpAct);
    m_helpMenu->addAction(m_aboutAct);
    m_helpMenu->addAction(m_aboutQtAct);
}


//création de la barre d'outil située sous le menu
void MainWindow::createToolBars()
{
    m_fileToolBar = addToolBar(tr("Fichier"));
    m_fileToolBar->addAction(m_newAct);
    m_fileToolBar->addAction(m_openAct);
    m_fileToolBar->addAction(m_saveAct);

    m_editToolBar = addToolBar(tr("Edition"));
    m_editToolBar->addAction(m_undoAct);
    m_editToolBar->addAction(m_redoAct);
    m_editToolBar->addAction(m_cutAct);
    m_editToolBar->addAction(m_copyAct);
    m_editToolBar->addAction(m_pasteAct);

    m_buildToolBar = addToolBar(tr("Construction"));
    m_buildToolBar->addAction(m_compileAct);
    m_buildToolBar->addAction(m_compileRunAct);
    m_buildToolBar->addAction(m_previousCompileErrorAct);
    m_buildToolBar->addAction(m_nextCompileErrorAct);

    m_debugToolBar = addToolBar(tr("Débogueur"));
    m_debugToolBar->addAction(m_debugAct);
    m_debugToolBar->addAction(m_debugRunAct);
    m_debugToolBar->addAction(m_debugNextAct);
    m_debugToolBar->addAction(m_killAct);
}

//création de la barre d'état
void MainWindow::createStatusBar()
{
    m_rowLabel = new QLabel(this);
    m_colLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_rowLabel,0);
    statusBar()->addPermanentWidget(m_colLabel,0);
    statusBar()->showMessage(tr("Prêt"));
}

//lecture des propriétés de l'application
void MainWindow::readSettings()
{
    QSettings settings("Département Informatique de \
	l'Université d'Angers", "EDI Free Pascal");
    QPoint pos = 
	settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = 
	settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

//ériture des propriétés de l'application
void MainWindow::writeSettings()
{
    QSettings settings("Département Informatique de \
	l'Université d'Angers", "EDI Free Pascal");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

//recherche si le composant Scintilla à été sauvegardé 
//depuis les modifications
bool MainWindow::maybeSave()
{
    if (m_textEdit->isModified()) {
        int ret = QMessageBox::warning(this,
		tr("EDI Free Pascal"),
        tr("Le document a été modifié.\n"
       "Voulez-vous sauvegarder les changements?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

//Charge un fichier
void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("EDI Free Pascal"),
         tr("Impossible de lire le fichier %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_textEdit->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Fichier chargé"), 2000);
 //	path = QFileInfo(fileName).path();// stock le chemin 
 //de fichier du fichier sauvegarder
}

//Sauvegarde un fichier
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("EDI Free Pascal"),
          tr("Impossible d'écrire sur le fichier %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << m_textEdit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Fichier sauvegardé"), 2000);
   // path = QFileInfo(fileName).path();// stock le 
   //chemin de fichier du fichier sauvegarder
	return true;
}

//donne un nom au fichier courrant
void MainWindow::setCurrentFile(const QString &fileName)
{
    m_curFile = fileName;
    m_textEdit->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (m_curFile.isEmpty())
        shownName = "untitled.pas";
    else
        shownName = strippedName(m_curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(
        tr("EDI FreePascal 32Bits 13.10")));
}

bool MainWindow::eventFilter(QObject* watched, QEvent *event){
    return event->type() == QEvent::ToolTip;
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
