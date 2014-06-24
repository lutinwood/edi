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

#include "settingwindow.h"

using namespace std;

//constructeur
SettingWindow::SettingWindow(QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
    this->setWindowModality ( Qt::ApplicationModal );
    m_args = new QStringList();
    m_debugDisplay = NULL;
    this->setWindowTitle("Préférences");

    createCompilerWidget();
    createPathWidget();
    createAdvanceWidget();
    createCodeWidget();
    createTabWidget();
    createGlobalWidget();
    setCentralWidget(m_globalWidget);
    loadFile();
    loadCheckOption();
    connect(m_cancelButton,SIGNAL(clicked()),this,SLOT(cancel()));
    connect (m_defaultSettingButton,SIGNAL(clicked()),this,SLOT(loadDefaultSetting()));
    connect(m_okButton,SIGNAL(clicked()),this,SLOT(saveFile()));
    connect(m_debugOption,SIGNAL(clicked(bool)),parent,SLOT(setDebugActivated(bool)));
}

//destructeur
SettingWindow::~SettingWindow(){}

void SettingWindow::setPath()
{
#if defined Q_OS_LINUX
QString FPC_PATH = "FPC_PATH_X11=";
QString GDB_PATH = "GDB_PATH_X11=";
QString DOC_PATH = "DOC_PATH_X11=";
#elif defined Q_OS_WIN32
QString FPC_PATH = "FPC_PATH_WIN32=";
QString GDB_PATH = "GDB_PATH_WIN32=";
QString DOC_PATH = "DOC_PATH_WNI32=";

#endif

}

//slots
void SettingWindow::loadFile()
{
//SettingWindow::setPath;
    if (QFile::exists(QString(CONFIG_FILE))) 
    {
        QFile *file = new QFile(QString(CONFIG_FILE));
        file->open(QIODevice::ReadOnly);
 
	QString fpcPath;
    	QString gdbPath;
    	QString docPath;
    	QString docIndex;
     
       while (!file->atEnd())
            {
		QString FPC_PATH = "FPC_PATH_X11=";
QString GDB_PATH = "GDB_PATH_X11=";
QString DOC_PATH = "DOC_PATH_X11=";

                QByteArray buffer(file->readLine());	
                QString path(buffer);
                if (path.contains(FPC_PATH))
                {//si le path est trouvé
                    QString ch(path.section(FPC_PATH,1,-1));
                    m_fpcPathtemp->setText(ch.simplified());
		fpcPath = QString((m_fpcPathtemp->text()).simplified());
                }
                if (path.contains(GDB_PATH))
                {
                    QString ch(path.section(GDB_PATH,1,-1));
                    m_gdbPathtemp->setText(ch.simplified());
                gdbPath = QString((m_gdbPathtemp->text()).simplified());
		}
                if (path.contains(DOC_PATH))
                {
                    QString ch(path.section(DOC_PATH,1,-1));
                    m_docPathtemp->setText(ch.simplified()); 
                docPath = QString((m_docPathtemp->text()).simplified());
		}
                if (path.contains("DOC_INDEX="))
                {
                    QString ch(path.section("DOC_INDEX=",1,-1));
                    m_docIndextemp->setText(ch.simplified()); 
		docIndex = QString((m_docIndextemp->text()).simplified());
                }
                if (path.contains("FPC_ARGS="))
                {
                    QString ch(path.section("FPC_ARGS=",1,-1));
                    QString ch_tmp(ch.simplified());
                    (*m_args) += ch_tmp.split(" ");
                }
                if (path.contains("DEBUG_DISPLAY="))
                {
                    QString ch(path.section("DEBUG_DISPLAY=",1,-1));
		    QString ch_tmp(ch.simplified());
                    if (m_debugDisplay != NULL)
                        delete m_debugDisplay;
                    m_debugDisplay = new QString(ch_tmp);
                }
           
        }
    file->close();
    }
}

void SettingWindow::saveFile()
{
//setPath;

    QString debug;
    if (m_debugOption->isChecked())
        debug = QString("1");
    else debug = QString("0");
    //on vide les arguments
    m_args->clear();
    //rajout de la compilation en mode debug
    (*m_args) << "-g";
    //on ajoute toutes les options séléctionnées
    if (m_SdOption->isChecked())
        (*m_args) << "-Sd";
    if (m_ScOption->isChecked())
        (*m_args) << "-Sc";
    if (m_SeOption->isChecked())
        (*m_args) << "-Se";
    if (m_SgOption->isChecked())
        (*m_args) << "-Sg";
    if (m_SoOption->isChecked())
        (*m_args) << "-So";
    if (m_StOption->isChecked())
        (*m_args) << "-St";
    if (m_ShOption->isChecked())
        (*m_args) << "-Sh";
    if (m_XSOption->isChecked())
        (*m_args) << "-XS";
    if (m_XDOption->isChecked())
        (*m_args) << "-XD";
    if (m_XXOption->isChecked())
        (*m_args) << "-XX";
    if (m_XsOption->isChecked())
        (*m_args) << "-Xs";
    if (m_vwOption->isChecked())
        (*m_args) << "-vw";
    if (m_vnOption->isChecked())
        (*m_args) << "-vn";
    if (m_vhOption->isChecked())
        (*m_args) << "-vh";
    if (m_viOption->isChecked())
        (*m_args) << "-vi";
    if (m_vxOption->isChecked())
        (*m_args) << "-vx";
    if (m_CrOption->isChecked())
        (*m_args) << "-Cr";
    if (m_CtOption->isChecked())
        (*m_args) << "-Ct";
    if (m_CiOption->isChecked())
        (*m_args) << "-Ci";
    if (m_CoOption->isChecked())
        (*m_args) << "-Co";
    if (m_O1Option->isChecked())
        (*m_args) << "-O1";
    if (m_O2Option->isChecked())
        (*m_args) << "-O2";
    if (m_OGOption->isChecked())
        (*m_args) << "-OG";
    if (m_OgOption->isChecked())
        (*m_args) << "-Og";
    //on sauvegarde après les préférences dans le fichier de configuration
    if (QFile::exists(CONFIG_FILE))
    {

        QString fpcPath;
        QString gdbPath;
        QString docPath;
        QString docIndex;

	QString FPC_PATH = "FPC_PATH_X11=";
QString GDB_PATH = "GDB_PATH_X11=";
QString DOC_PATH = "DOC_PATH_X11=";

        QFile file(CONFIG_FILE);
        file.open(QIODevice::ReadWrite);
        QByteArray buffer(file.readAll());
        QString str(buffer);
        QStringList file_list(str.split("\n"));
        QStringList::Iterator it;
        //on regarde où se trouve la ligne des arguments dans le fichier texte
        for (it = file_list.begin() ; it != file_list.end() ; it++)
        {
            //sauvegarde des arguments de fpc
            if ((*it).contains("FPC_ARGS="))
            {
                (*it) = QString("FPC_ARGS=") + (*m_args).join(" ");
            }
                //sauvegarde du chemin de fpc
                if ((*it).contains(FPC_PATH))
                {
                    (*it) = QString(FPC_PATH) + fpcPath;
                }
                //sauvegarde du chemin de gdb
                if ((*it).contains(GDB_PATH))
                {
                    (*it) = QString(GDB_PATH) + gdbPath;
                }
                //sauvegarde du chemin de la doc
                if ((*it).contains(DOC_PATH))
                {
                    (*it) = QString(DOC_PATH) + docPath;
                }
                if ((*it).contains("DOC_INDEX="))
                {
                    (*it) = QString("DOC_INDEX=") + docIndex;
                }
                if ((*it).contains("DEBUG_DISPLAY="))
                {
                    (*it) = QString("DEBUG_DISPLAY=") + debug;
                }
            }
        
        file.remove();
        QFile file2(CONFIG_FILE);
        file2.open(QIODevice::WriteOnly);
        file2.write((file_list.join("\n")).toLatin1());
        file2.close();
        file.close();
    }
    this->close();
}

void SettingWindow::loadDefaultSetting()
{
    m_args->clear();
    (*m_args) << "-g";
    loadCheckOption();
}

void SettingWindow::loadCheckOption()
{
    if (m_debugDisplay != NULL)
    {
        if (m_debugDisplay->contains("1"))
            m_debugOption->setChecked(true);
        else m_debugOption->setChecked(false);
    }
    if ((*m_args).contains("-Sd",Qt::CaseSensitive))
        m_SdOption->setChecked(true);
    else m_SdOption->setChecked(false);
    if ((*m_args).contains("-Sc",Qt::CaseSensitive))
        m_ScOption->setChecked(true);
    else m_ScOption->setChecked(false);
    if ((*m_args).contains("-Se",Qt::CaseSensitive))
        m_SeOption->setChecked(true);
    else m_SeOption->setChecked(false);
    if ((*m_args).contains("-Sg",Qt::CaseSensitive))
        m_SgOption->setChecked(true);
    else m_SgOption->setChecked(false);
    if ((*m_args).contains("-So",Qt::CaseSensitive))
        m_SoOption->setChecked(true);
    else m_SoOption->setChecked(false);
    if ((*m_args).contains("-St",Qt::CaseSensitive))
        m_StOption->setChecked(true);
    else m_StOption->setChecked(false);
    if ((*m_args).contains("-Sh",Qt::CaseSensitive))
        m_ShOption->setChecked(true);
    else m_ShOption->setChecked(false);
    if ((*m_args).contains("-XS",Qt::CaseSensitive))
        m_XSOption->setChecked(true);
    else m_XSOption->setChecked(false);
    if ((*m_args).contains("-XD",Qt::CaseSensitive))
        m_XDOption->setChecked(true);
    else m_XDOption->setChecked(false);
    if ((*m_args).contains("-XX",Qt::CaseSensitive))
        m_XXOption->setChecked(true);
    else m_XXOption->setChecked(false);
    if ((*m_args).contains("-Xs",Qt::CaseSensitive))
        m_XsOption->setChecked(true);
    else m_XsOption->setChecked(false);
    if (!(*m_args).contains("-Xs",Qt::CaseSensitive) && !(*m_args).contains("-XX",Qt::CaseSensitive) && !(*m_args).contains("-XD",Qt::CaseSensitive))
        m_XDefaultOption->setChecked(true);
    else m_XDefaultOption->setChecked(false);
    if ((*m_args).contains("-vw",Qt::CaseSensitive))
        m_vwOption->setChecked(true);
    else m_vwOption->setChecked(false);
    if ((*m_args).contains("-vn",Qt::CaseSensitive))
        m_vnOption->setChecked(true);
    else m_vnOption->setChecked(false);
    if ((*m_args).contains("-vh",Qt::CaseSensitive))
        m_vhOption->setChecked(true);
    else m_vhOption->setChecked(false);
    if ((*m_args).contains("-vi",Qt::CaseSensitive))
        m_viOption->setChecked(true);
    else m_viOption->setChecked(false);
    if ((*m_args).contains("-vx",Qt::CaseSensitive))
        m_vxOption->setChecked(true);
    else m_vxOption->setChecked(false);
    if ((*m_args).contains("-Cr",Qt::CaseSensitive))
        m_CrOption->setChecked(true);
    else m_CrOption->setChecked(false);
    if ((*m_args).contains("-Ct",Qt::CaseSensitive))
        m_CtOption->setChecked(true);
    else m_CtOption->setChecked(false);
    if ((*m_args).contains("-Ci",Qt::CaseSensitive))
        m_CiOption->setChecked(true);
    else m_CiOption->setChecked(false);
    if ((*m_args).contains("-Co",Qt::CaseSensitive))
        m_CoOption->setChecked(true);
    else m_CoOption->setChecked(false);
    if ((*m_args).contains("-O1",Qt::CaseSensitive))
        m_O1Option->setChecked(true);
    else m_O1Option->setChecked(false);
    if ((*m_args).contains("-O2",Qt::CaseSensitive))
        m_O2Option->setChecked(true);
    else m_O2Option->setChecked(false);
    if (!(*m_args).contains("-O1",Qt::CaseSensitive) && !(*m_args).contains("-O2",Qt::CaseSensitive))
        m_00Option->setChecked(true);
    else m_00Option->setChecked(false);
    if ((*m_args).contains("-OG",Qt::CaseSensitive))
        m_OGOption->setChecked(true);
    else m_OGOption->setChecked(false);
    if ((*m_args).contains("-Og",Qt::CaseSensitive))
        m_OgOption->setChecked(true);
    else m_OgOption->setChecked(false);
    if (!(*m_args).contains("-Og",Qt::CaseSensitive) && !(*m_args).contains("-Og",Qt::CaseSensitive))
        m_OG0Option->setChecked(true);
    else m_OG0Option->setChecked(false);
}

void SettingWindow::cancel()
{
    this->loadCheckOption();
    QFile file(CONFIG_FILE);
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        while (!file.atEnd())
        {
            QByteArray buffer(file.readLine());
            QString str(buffer);
            if (str.contains("DEBUG_DISPLAY="))
            {//si le path est trouvé
                QString ch(str.section("DEBUG_DISPLAY=",1,-1));
                if ((ch.simplified()).contains("1"))
                {
                    m_debugOption->setChecked(false);
                    m_debugOption->click();
                }
                else{
                    m_debugOption->setChecked(true);
                    m_debugOption->click();
                }
            }
        }
        file.close();
    }
    this->close();
}

//méthodes
void SettingWindow::createCompilerWidget()
{
    m_compilerWidget = new QWidget(this);
    QVBoxLayout *globalLayout = new QVBoxLayout(m_compilerWidget);
    m_SdOption = new QCheckBox(tr("Compatibilité Delphi -Sd : essaye d'être compatible avec Delphi"),m_compilerWidget);
    m_ScOption = new QCheckBox(tr("Support de la synthaxe C -Sc : support de *=, +=, /=, &, -=, ..."),m_compilerWidget);
    m_SeOption = new QCheckBox(tr("Stoppe la première erreur -Se : stopper FPC dès la première erreur"),m_compilerWidget);
    m_SgOption = new QCheckBox(tr("Autoriser les sauts -Sg : autoriser label et goto"),m_compilerWidget);
    m_SoOption = new QCheckBox(tr("Compatibilité TP7.0 -So : essaye d'être compatible avec Turbo Pascal"),m_compilerWidget);
    m_StOption = new QCheckBox(tr("Autoriser static -St : autoriser le mot clé statique"),m_compilerWidget);
    m_ShOption = new QCheckBox(tr("ansistring -Sh : string design des ansistring au lieu de shortstring"),m_compilerWidget);
    m_XSOption = new QRadioButton(tr("Liaison static -XS"),m_compilerWidget);
    m_XDOption = new QRadioButton(tr("Liaison dynamic -XD"),m_compilerWidget);
    m_XXOption = new QRadioButton(tr("Liaison smart -XX"),m_compilerWidget);
    m_XDefaultOption = new QRadioButton(tr("Défaut"),m_compilerWidget);
    m_XDefaultOption->setChecked(true);
    m_XsOption = new QCheckBox(tr("Enlever des symboles de l'exécutable -Xs"),m_compilerWidget);
    QGroupBox *synthaxOptionGrBox = new QGroupBox(tr("Options de synthaxe"),this);
    QVBoxLayout *synthaxOptionLayout = new QVBoxLayout;
    synthaxOptionLayout->addWidget(m_SdOption);
    synthaxOptionLayout->addWidget(m_ScOption);
    synthaxOptionLayout->addWidget(m_SeOption);
    synthaxOptionLayout->addWidget(m_SgOption);
    synthaxOptionLayout->addWidget(m_SoOption);
    synthaxOptionLayout->addWidget(m_StOption);
    synthaxOptionLayout->addWidget(m_ShOption);
    synthaxOptionGrBox->setLayout(synthaxOptionLayout);
    globalLayout->addWidget(synthaxOptionGrBox);
    QGroupBox *executableGrBox = new QGroupBox(tr("Exécutable"),this);
    QVBoxLayout *executableLayout = new QVBoxLayout;
    executableLayout->addWidget(m_XSOption);
    executableLayout->addWidget(m_XDOption);
    executableLayout->addWidget(m_XXOption);
    executableLayout->addWidget(m_XDefaultOption);
    executableLayout->addWidget(m_XsOption);
    executableGrBox->setLayout(executableLayout);
    globalLayout->addWidget(executableGrBox);
}

void SettingWindow::createPathWidget()
{

    m_pathWidget = new QWidget(this);
    QVBoxLayout *globalLayout = new QVBoxLayout(m_pathWidget);


    m_fpcPathtemp = new QLineEdit(this);
    QLabel *fpctempLabel = new QLabel("Chemin de fpc : ", this);
	
    m_gdbPathtemp = new QLineEdit(this);
    QLabel *gdbtempLabel = new QLabel("Chemin de gdb : ", this);

    m_docPathtemp = new QLineEdit(this);
    QLabel *doctempLabel = new QLabel("Chemin de la documentation : ", this);

    m_docIndextemp = new QLineEdit(this);
    QLabel *doctempIndexLabel = new QLabel("Nom du fichier index de la documentation : ", this);


    QGroupBox *pathGrBox = new QGroupBox(tr("Chemins des applications"),m_pathWidget);
    QGridLayout *pathLayout = new QGridLayout;

    pathLayout->addWidget(fpctempLabel,0,0);
    pathLayout->addWidget(m_fpcPathtemp,0,1);
    pathLayout->addWidget(gdbtempLabel,2,0);
    pathLayout->addWidget(m_gdbPathtemp,2,1);
    pathLayout->addWidget(doctempLabel,4,0);
    pathLayout->addWidget(m_docPathtemp,4,1);
    pathLayout->addWidget(doctempIndexLabel,6,0);
    pathLayout->addWidget(m_docIndextemp,6,1);

    pathGrBox->setLayout(pathLayout);
    globalLayout->addWidget(pathGrBox);
}

void SettingWindow::createAdvanceWidget()
{
    m_advanceWidget = new QWidget(this);
    QVBoxLayout *globalLayout = new QVBoxLayout(m_advanceWidget);
    m_vwOption = new QCheckBox(tr("Message d'erreur -vw"),m_advanceWidget);
    m_vnOption = new QCheckBox(tr("Notes -vn"),m_advanceWidget);
    m_vhOption = new QCheckBox(tr("Conseils -vh"),m_advanceWidget);
    m_viOption = new QCheckBox(tr("Infos générales -vi"),m_advanceWidget);
    m_vxOption = new QCheckBox(tr("Info executables -vx"),m_advanceWidget);
    QGroupBox *advanceGrBox = new QGroupBox(tr("Messages du compilateurs"),m_advanceWidget);
    QVBoxLayout *advanceLayout = new QVBoxLayout;
    advanceLayout->addWidget(m_vwOption);
    advanceLayout->addWidget(m_vnOption);
    advanceLayout->addWidget(m_vhOption);
    advanceLayout->addWidget(m_viOption);
    advanceLayout->addWidget(m_vxOption);
    advanceGrBox->setLayout(advanceLayout);
    QGroupBox *advanceDebugGrBox = new QGroupBox(tr("Débogueur"),m_advanceWidget);
    QVBoxLayout *advanceDebugLayout = new QVBoxLayout;
    m_debugOption = new QCheckBox(tr("Activation du débogueur"),m_advanceWidget);
    advanceDebugLayout->addWidget(m_debugOption);
    advanceDebugGrBox->setLayout(advanceDebugLayout);
    globalLayout->addWidget(advanceGrBox);
    globalLayout->addWidget(advanceDebugGrBox);
}

void SettingWindow::createCodeWidget()
{
    m_codeWidget = new QWidget(this);
    QVBoxLayout *globalLayout = new QVBoxLayout(m_codeWidget);
    m_CrOption = new QCheckBox(tr("Range checking -Cr"),m_codeWidget);
    m_CtOption = new QCheckBox(tr("Stack checking -Ct"),m_codeWidget);
    m_CiOption = new QCheckBox(tr("I/O checking -Ci"),m_codeWidget);
    m_CoOption = new QCheckBox(tr("Integer overflow checking -Co"),m_codeWidget);
    m_O1Option = new QRadioButton(tr("Optimisation -O1"),m_codeWidget);
    m_O2Option = new QRadioButton(tr("Optimisation -O2"),m_codeWidget);
    m_00Option = new QRadioButton(tr("Aucune optimisation"),m_codeWidget);
    m_OGOption = new QRadioButton(tr("Privilégier la rapidité -OG"),m_codeWidget);
    m_OgOption = new QRadioButton(tr("Privilégier la taille -Og"),m_codeWidget);
    m_OG0Option = new QRadioButton(tr("Ne privilégier ni la rapidité, ni la taille"),m_codeWidget);
    QGroupBox *verificationGrBox = new QGroupBox(tr("Vérification"),m_codeWidget);
    QVBoxLayout *verificationLayout = new QVBoxLayout;
    verificationLayout->addWidget(m_CrOption);
    verificationLayout->addWidget(m_CtOption);
    verificationLayout->addWidget(m_CiOption);
    verificationLayout->addWidget(m_CoOption);
    verificationGrBox->setLayout(verificationLayout);
    globalLayout->addWidget(verificationGrBox);
    QGroupBox *optimisationGrBox = new QGroupBox(tr("Optimisation"),m_codeWidget);
    QVBoxLayout *optimisationLayout = new QVBoxLayout;
    optimisationLayout->addWidget(m_O1Option);
    optimisationLayout->addWidget(m_O2Option);
    optimisationLayout->addWidget(m_00Option);
    m_00Option->setChecked(true);
    optimisationGrBox->setLayout(optimisationLayout);
    globalLayout->addWidget(optimisationGrBox);
    QGroupBox *codeGrBox = new QGroupBox(tr("Code"),m_codeWidget);
    QVBoxLayout *codeLayout = new QVBoxLayout;
    codeLayout->addWidget(m_OGOption);
    codeLayout->addWidget(m_OgOption);
    codeLayout->addWidget(m_OG0Option);
    m_OG0Option->setChecked(true);
    codeGrBox->setLayout(codeLayout);
    globalLayout->addWidget(codeGrBox);
}

void SettingWindow::createTabWidget()
{
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(m_compilerWidget,tr("Options du compilateur"));
    m_tabWidget->addTab(m_pathWidget,tr("Chemins/répertoires"));
    m_tabWidget->addTab(m_advanceWidget,tr("Options avancées"));
    m_tabWidget->addTab(m_codeWidget,tr("Génération de code"));
}

void SettingWindow::createGlobalWidget()
{
    m_globalWidget = new QWidget(this);
    QVBoxLayout *globalLayout = new QVBoxLayout(m_globalWidget);
    globalLayout->addWidget(m_tabWidget);
    QWidget *bottom = new QWidget(m_globalWidget);
    m_defaultSettingButton = new QPushButton(tr("Rétablir les paramètres par défault"),bottom);
    m_okButton = new QPushButton(tr("OK"),bottom);
    m_cancelButton = new QPushButton(tr("Annuler"),bottom);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottom);
    bottomLayout->addWidget(m_defaultSettingButton);
    bottomLayout->addWidget(m_okButton);
    bottomLayout->addWidget(m_cancelButton);
    globalLayout->addWidget(bottom);
}
