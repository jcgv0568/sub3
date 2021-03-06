#include "ProjectConfigDialog.h"
#include "ui_ProjectConfigDialog.h"

#include "src/utils/Config.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>


ProjectConfigDialog::ProjectConfigDialog(ProjectModel *projectModel, QWidget *parent) :QDialog(parent),
    ui(new Ui::ProjectConfigDialog),
    m_projectModel(projectModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    /* setting the data */
    ui->lineEditName->setText(m_projectModel->info.name);

    if(m_projectModel->info.path.isEmpty())
        ui->lineEditFile->setText(CONFIG.value("projects_path").toString());
    else{
        QString path = m_projectModel->info.path.remove(m_projectModel->info.name+".s3s");
        ui->lineEditFile->setText(path);
    }
}
ProjectConfigDialog::~ProjectConfigDialog(){
    delete ui;
}

void ProjectConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void ProjectConfigDialog::on_buttonOk_clicked(){
    /* get data */
    QString path = ui->lineEditFile->text();
    QString name = ui->lineEditName->text();

    /* checks */
    if(path.isEmpty() || name.isEmpty()){
        QMessageBox::warning(this, "Error", "Please fill-in the details!");
        return;
    }
    if(!QDir::isAbsolutePath(path)){
        QMessageBox::warning(this, "Error", "Wrong Path For Project File!");
        return;
    }

    m_projectModel->info.name = name;
    m_projectModel->info.path = path+"/"+name+".s3s";
    m_projectModel->info.isConfigured = true;

    /* checks */
    m_projectModel->explorer->project->setText(m_projectModel->info.name);
    accept();
}

void ProjectConfigDialog::on_buttonChoosePath_clicked(){
    QString path = QFileDialog::getExistingDirectory(this, "Path", ui->lineEditFile->text());
    if(path.isEmpty()){
        return;
    }
    ui->lineEditFile->setText(path);
}
