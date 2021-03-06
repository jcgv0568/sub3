/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Raw.h"
#include "ui_Raw.h"

#include <QMessageBox>
#include <QTextCursor>
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/PassiveConfigDialog.h"


Raw::Raw(QWidget *parent, ProjectModel *project): AbstractEngine(parent, project),
    ui(new Ui::Raw),
    m_model(new QStandardItemModel),
    m_targetListModel(new QStringListModel),
    m_targetListModel_host(new QStringListModel),
    m_targetListModel_ip(new QStringListModel),
    m_targetListModel_asn(new QStringListModel),
    m_targetListModel_cidr(new QStringListModel),
    m_targetListModel_ssl(new QStringListModel),
    m_targetListModel_email(new QStringListModel),
    m_targetListModel_url(new QStringListModel),
    m_scanConfig(new ScanConfig),
    m_scanArgs(new ScanArgs)
{
    this->initUI();

    /* targets model */
    ui->targets->setListName(tr("Targets"));
    ui->targets->setListModel(m_targetListModel);

    /* results model */
    m_model->setHorizontalHeaderLabels({tr(" Properties"), tr(" Values")});
    proxyModel->setSourceModel(m_model);
    proxyModel->setFilterKeyColumn(0);
    ui->treeViewResults->setModel(proxyModel);
    ui->treeViewResults->setHeaderHidden(false);

    /* ... */
    m_scanArgs->config = m_scanConfig;
    this->initConfigValues();
}
Raw::~Raw(){
    delete m_scanConfig;
    delete m_scanArgs;
    delete m_targetListModel_url;
    delete m_targetListModel_host;
    delete m_targetListModel_email;
    delete m_targetListModel_cidr;
    delete m_targetListModel_ssl;
    delete m_targetListModel_asn;
    delete m_targetListModel_ip;
    delete m_targetListModel;
    delete m_model;
    delete ui;
}

void Raw::initUI(){
    ui->setupUi(this);

    /* disabling & hiding widgets */
    ui->buttonStop->setDisabled(true);
    ui->progressBar->hide();

    /* set properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);
    ui->labelResultsCountTree->setProperty("dark", true);

    /* placeholder texts */
    ui->lineEditTarget->setPlaceholderText(tr("Enter target..."));
    ui->lineEditFind->setPlaceholderText(tr("Find..."));
    ui->lineEditFind_json->setPlaceholderText("Find...");
    ui->lineEditTreeFilter->setPlaceholderText(tr("Filter..."));
    ui->plainTextEditJson->setPlaceholderText("Double-click on an item (first column) on Tree-tab to view it in json...");

    /* ... */
    ui->labelUrl->setTextFormat(Qt::RichText);
    ui->labelApiDoc->setTextFormat(Qt::RichText);
    ui->labelUrl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelApiDoc->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelUrl->setOpenExternalLinks(true);
    ui->labelApiDoc->setOpenExternalLinks(true);

    /* json syntax higlighting */
    m_resultsHighlighter = new JsonSyntaxHighlighter(ui->plainTextEditResults->document());
    m_jsonHighlighter = new JsonSyntaxHighlighter(ui->plainTextEditJson->document());
    if(s3s::is_dark_theme){
        m_resultsHighlighter->forDarkTheme();
        m_jsonHighlighter->forDarkTheme();
    }
    if(s3s::is_light_theme){
        m_resultsHighlighter->forLightTheme();
        m_jsonHighlighter->forLightTheme();
    }

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                           << static_cast<int>((this->width() * 0.50)));

    ui->targets->hide();
    ui->tabWidgetInput->adjustSize();
}

void Raw::initConfigValues(){
    CONFIG.beginGroup(CFG_RAW);
    m_scanConfig->timeout = CONFIG.value(CFG_VAL_TIMEOUT).toInt();
    m_scanConfig->autosaveToProject = CONFIG.value(CFG_VAL_AUTOSAVE).toBool();
    m_scanConfig->noDuplicates = CONFIG.value(CFG_VAL_DUPLICATES).toBool();
    m_scanArgs->config->setTimeout = CONFIG.value(CFG_VAL_SETTIMEOUT).toBool();
    CONFIG.endGroup();
}

void Raw::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void Raw::on_buttonStart_clicked(){
    if(ui->lineEditTarget->placeholderText() != PLACEHOLDERTEXT_NONE){
        if(ui->checkBoxMultipleTargets->isChecked() && ui->targets->getlistModel()->rowCount() == 0){
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter Target For Enumerations!"));
            return;
        }
        if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter Targets For Enumerations!"));
            return;
        }
    }

    /* getting the targets */
    m_scanArgs->targets.clear();
    if(ui->checkBoxMultipleTargets->isChecked()){
        foreach(const QString &target, ui->targets->getlistModel()->stringList())
            m_scanArgs->targets.enqueue(target);
    }else
        m_scanArgs->targets.enqueue(ui->lineEditTarget->text());

    /* starting the scan */
    this->startScan();

    /* after starting all choosen modules */
    if(status->activeScanThreads){
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
        log("------------------ start --------------");
        qInfo() << "[RAW] Scan Started";
    }
    else
        QMessageBox::warning(this, tr("Error!"), tr("Please Choose Engine For Enumeration!"));
}

void Raw::on_buttonStop_clicked(){
    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void Raw::on_comboBoxOptions_currentIndexChanged(const QString &arg1){
    if(arg1.length() == 0)
        return;

    ui->lineEditTarget->clear();
    QString placehodertxt(m_optionSet.value(arg1).at(0));

    ui->lineEditTarget->setPlaceholderText(placehodertxt);
    ui->textEditOptionSummary->setText(m_optionSet.value(arg1).at(1));

    if(placehodertxt == PLACEHOLDERTEXT_ASN || placehodertxt == PLACEHOLDERTEXT_AS){
        ui->targets->setListModel(m_targetListModel_asn);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_IP || placehodertxt == PLACEHOLDERTEXT_IP4 || placehodertxt == PLACEHOLDERTEXT_IP6){
        ui->targets->setListModel(m_targetListModel_ip);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_DOMAIN || placehodertxt == PLACEHOLDERTEXT_HOSTNAME || placehodertxt == PLACEHOLDERTEXT_NS || placehodertxt == PLACEHOLDERTEXT_MX){
        ui->targets->setListModel(m_targetListModel_host);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_CIDR){
        ui->targets->setListModel(m_targetListModel_cidr);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_SSLCERT){
        ui->targets->setListModel(m_targetListModel_ssl);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_URL){
        ui->targets->setListModel(m_targetListModel_url);
        return;
    }
    ui->targets->setListModel(m_targetListModel);
}

void Raw::on_buttoApiKeys_clicked(){
    ApiKeysDialog *apiKeys = new ApiKeysDialog(this);
    apiKeys->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeys->show();
}

void Raw::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->loadConfig_raw();
    scanConfig->show();
}

void Raw::log(const QString &log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Raw::on_lineEditTreeFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(proxyModel);
    ui->labelResultsCountTree->setNum(proxyModel->rowCount());
}

void Raw::on_treeViewResults_doubleClicked(const QModelIndex &index){
    if(index.column())
        return;

    QModelIndex model_index = proxyModel->mapToSource(index);

    if(model_index.parent() == m_model->invisibleRootItem()->index()){
        s3s_item::RAW *item = static_cast<s3s_item::RAW*>(m_model->itemFromIndex(model_index));
        ui->plainTextEditJson->setPlainText(item->json);
        return;
    }

    if(m_model->itemFromIndex(model_index)->hasChildren())
        ui->plainTextEditJson->setPlainText(item_to_json(m_model->itemFromIndex(model_index)));
}

void Raw::on_tabWidgetInput_currentChanged(int index){
    if(index)
        ui->targets->show();
    else
        ui->targets->hide();

    ui->tabWidgetInput->adjustSize();
}

///
/// find...
///
void Raw::find(const QString &searchTerm, QTextDocument::FindFlags flags){
    if(ui->plainTextEditResults->find(searchTerm, flags))
        ui->lineEditFind->setStyleSheet("color: white");
    else
        ui->lineEditFind->setStyleSheet("color: rgb(255, 86, 80);");
}

void Raw::find_json(const QString &searchTerm, QTextDocument::FindFlags flags){
    if(ui->plainTextEditJson->find(searchTerm, flags))
        ui->lineEditFind_json->setStyleSheet("color: white");
    else
        ui->lineEditFind_json->setStyleSheet("color: rgb(255, 86, 80);");
}

void Raw::on_buttonNext_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find(ui->lineEditFind->text(), flags);
}

void Raw::on_buttonPrev_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    flags |= QTextDocument::FindBackward;

    this->find(ui->lineEditFind->text(), flags);
}

void Raw::on_lineEditFind_textEdited(const QString &searchTerm){
    /* set cursor to the begining of the document... */
    QTextCursor currentCursorPosition;
    currentCursorPosition.setPosition(QTextCursor::Start);
    ui->plainTextEditResults->setTextCursor(currentCursorPosition);

    /* get option flags... */
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find(searchTerm, flags);
}

void Raw::on_buttonNext_json_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive_json->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find(ui->lineEditFind_json->text(), flags);
}

void Raw::on_buttonPrev_json_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive_json->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    flags |= QTextDocument::FindBackward;

    this->find(ui->lineEditFind_json->text(), flags);
}

void Raw::on_lineEditFind_json_textChanged(const QString &searchTerm){
    /* set cursor to the begining of the document... */
    QTextCursor currentCursorPosition;
    currentCursorPosition.setPosition(QTextCursor::Start);
    ui->plainTextEditJson->setTextCursor(currentCursorPosition);

    /* get option flags... */
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive_json->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find_json(searchTerm, flags);
}
