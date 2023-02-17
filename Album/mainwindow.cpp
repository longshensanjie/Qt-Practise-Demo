#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wizard.h"
#include "protree.h"
#include "picshow.h"
#include "protreewidget.h"

#include <QMenu>
#include <QAction>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), _proTree(nullptr), _picShow(nullptr)
{
    ui->setupUi(this);
    QMenu *menu_file = menuBar()->addMenu(tr("文件(&F)"));

    QAction *createProAction = new QAction(QIcon(":/icon/createpro.png"), tr("创建项目"), this);
    createProAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menu_file->addAction(createProAction);

    QAction *openProAction= new QAction(QIcon(""), tr("打开项目"), this);
    openProAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menu_file->addAction(openProAction);

    QMenu *setMenu = menuBar()->addMenu(tr("设置(&S)"));

    QAction *setMusicAction = new QAction(QIcon(":/icon/music.png"), tr("背景音乐"), this);
    setMusicAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    setMenu->addAction(setMusicAction);


    _proTree = new ProTree();
    ui->proLayout->addWidget(_proTree, 0);

    _picShow = new PicShow();
    ui->piclLayout->addWidget(_picShow);

    PicShow *picShowPro = dynamic_cast<PicShow *>(_picShow);
    QTreeWidget *treeWidget = dynamic_cast<ProTree *>(_proTree)->GetTreeWidget();
    ProTreeWidget *proTreeWidget = dynamic_cast<ProTreeWidget *>(treeWidget);

    connect(createProAction, &QAction::triggered, this, &MainWindow::slotCreatePro);
    connect(openProAction, &QAction::triggered, this, &MainWindow::slotOpenPro);

    connect(this, &MainWindow::sigOpenPro, proTreeWidget, &ProTreeWidget::slotOpenPro);
    connect(setMusicAction, &QAction::triggered, proTreeWidget, &ProTreeWidget::slotSetMusic);

    connect(proTreeWidget, &ProTreeWidget::sigUpdateSelected,
            picShowPro, &PicShow::slotSelectItem);
    connect(proTreeWidget, &ProTreeWidget::sigClearSelected,
            picShowPro, &PicShow::slotDeleteItem );

    connect(picShowPro, &PicShow::sigPreBtnClicked,
            proTreeWidget, &ProTreeWidget::slotPreShow);
    connect(picShowPro, &PicShow::sigNextBtnClicked,
            proTreeWidget, &ProTreeWidget::slotNextShow);
    connect(proTreeWidget, &ProTreeWidget::sigUpdatePic,
            picShowPro, &PicShow::slotUpdatePic);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotCreatePro(bool)
{
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    QWizardPage *page = wizard.page(0);
    page->setWindowTitle(tr("设置项目配置"));
    connect(&wizard, &Wizard::sigProSettings,
            dynamic_cast<ProTree *>(_proTree), ProTree::addProToTree);

    wizard.show();
    wizard.exec();

    disconnect();
}

void MainWindow::slotOpenPro(bool)
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("选择导入的文件夹");

    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(fileDialog.exec())
    {
        fileNames = fileDialog.selectedFiles();
    }

    if(fileNames.size() <= 0)
        return;

    QString importPath = fileNames.at(0);
    emit sigOpenPro(importPath);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    PicShow *picShowPro = dynamic_cast<PicShow *>(_picShow);
    picShowPro->reloadPic();
    QMainWindow::resizeEvent(event);
}

