/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "EmailEnum.h"
#include "ui_EmailEnum.h"


void EmailEnum::onResult(s3s_struct::Email results){
    if(m_resultsSet.contains(results.email)){
        s3s_item::Email *item = m_resultsSet.value(results.email);
        item->setValues(results);
    }
    else{
        s3s_item::Email *item = new s3s_item::Email;
        item->setValues(results);
        m_model->appendRow(item);
        m_resultsSet.insert(results.email, item);
        ui->labelResultsCount->setNum(proxyModel->rowCount());
    }

    if(m_scanConfig->autosaveToProject)
        project->addEnumEmail(results);
}

void EmailEnum::onScanLog(ScanLog log){
    if(log.error){
        QString message("<font color=\"red\">"+log.message+"</font>");
        QString module("<font color=\"red\">"+log.moduleName+"</font>");
        QString target("<font color=\"red\">"+log.target+"</font>");
        QString status("<font color=\"red\">"+QString::number(log.statusCode)+"</font>");
        ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
        ui->plainTextEditLogs->appendHtml("[Target]        :"+target);
        ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
        ui->plainTextEditLogs->appendHtml("[Error message] :"+message);
        m_failedScans.insert(log.target, log.message);
    }
    else{
        QString module("<font color=\"green\">"+log.moduleName+"</font>");
        QString target("<font color=\"green\">"+log.target+"</font>");
        QString status("<font color=\"green\">"+QString::number(log.statusCode)+"</font>");
        ui->plainTextEditLogs->appendHtml("[Module]        :"+module);
        ui->plainTextEditLogs->appendHtml("[Target]        :"+target);
        ui->plainTextEditLogs->appendHtml("[Status Code]   :"+status);
    }

    ui->plainTextEditLogs->appendPlainText("");
}
