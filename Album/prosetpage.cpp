#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QDir>
#include <QFileDialog>


ProSetPage::ProSetPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proName*", ui->lineEdit);
    registerField("proPath", ui->lineEdit_2);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, ProSetPage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, ProSetPage::completeChanged);

    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());
    ui->lineEdit_2->setClearButtonEnabled(true);
    ui->lineEdit->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::getProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}


 bool ProSetPage::isComplete() const
 {
    if(ui->lineEdit->text() == "" || ui->lineEdit_2->text() == "")
        return false;

    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists())
    {
        ui->tips->setText("Project path is not exists");
        return false;
    }

    QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
    QDir distDir(absFilePath);
    if(distDir.exists())
    {
        ui->tips->setText("project has exists, change the name or path");
        return false;
    }

    ui->tips->setText("");
    return QWizardPage::isComplete();
 }

void ProSetPage::on_pushButton_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("选择导入的文件夹");
    auto path = QDir::currentPath();
    fileDialog.setDirectory(path);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(fileDialog.exec())
    {
        fileNames = fileDialog.selectedFiles();
    }

    if(fileNames.size() <= 0)
        return;

    QString importPpath = fileNames.at(0);
    ui->lineEdit_2->setText(importPpath);
}

