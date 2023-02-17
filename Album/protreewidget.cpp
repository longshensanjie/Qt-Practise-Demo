#include "protreewidget.h"
#include "protreeitem.h"
#include "removeprodialog.h"
#include "const.h"

#include <QTreeWidget>
#include <QTreeView>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QGuiApplication>
#include <QFileDialog>
#include <QSharedPointer>
#include <QProgressDialog>
#include <QSoundEffect>


ProTreeWidget::ProTreeWidget(QWidget *parent)
    :QTreeWidget(parent), _rightBtnItem(nullptr), _selectdItem(nullptr), _activeItem(nullptr)
{
    qRegisterMetaType<QVector<int>>("QVector<int>");
    this->setHeaderHidden(true);

    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::slotItemPressed);
    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::slotItemDoubleClicked);

    _importAction = new QAction(QIcon(":/icon/import.png"), tr("导入文件"), this);
    connect(_importAction, &QAction::triggered, this, &ProTreeWidget::slotImport);

    _setStartAction = new QAction(QIcon(":/icon/core.png"), tr("设置活动项目"), this);
    connect(_setStartAction, &QAction::triggered, this, &ProTreeWidget::slotSetActive);

    _closeProAction = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"), this);
    connect(_closeProAction, &QAction::triggered, this, &ProTreeWidget::slotClosePro);

    _slideShowAction = new QAction(QIcon(":/icon/closeshow.png"), tr("轮播图播放"), this);
    connect(_slideShowAction, &QAction::triggered, this, &ProTreeWidget::slotSlideShow);

    _sound = new QSoundEffect(this);
    _sound->setVolume(20);
}

void ProTreeWidget::addProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString filePath = dir.absoluteFilePath(name);
    if(_pathSet.find(filePath) != _pathSet.end())
    {
        return;
    }

    QDir proDir(filePath);
    if(!proDir.exists())
    {
        bool enable = proDir.mkpath(filePath);
        if(!enable)
            return;
    }

    _pathSet.insert(filePath);

    ProTreeItem *item = new ProTreeItem(this, name, filePath, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, filePath);
    this->addTopLevelItem(item);
}

void ProTreeWidget::slotItemPressed(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if(QGuiApplication::mouseButtons() == Qt::RightButton)
    {
        QMenu menu(this);
        int itemType = item->type();
        if(itemType == TreeItemPro)
        {
            _rightBtnItem = item;
            menu.addAction(_importAction);
            menu.addAction(_setStartAction);
            menu.addAction(_closeProAction);
            menu.addAction(_slideShowAction);
            menu.exec(QCursor::pos());
        }
    }
}

void ProTreeWidget::slotItemDoubleClicked(QTreeWidgetItem *item, int column)
{
     Q_UNUSED(column);
    if(QGuiApplication::mouseButtons() == Qt::LeftButton)
    {
        auto *treeDoubleItem = dynamic_cast<ProTreeItem *>(item);
        if(!treeDoubleItem)
            return;

        int itemType = treeDoubleItem->type();
        if(itemType == TreeItemPic)
        {
            emit sigUpdateSelected(treeDoubleItem->getPath());
            _selectdItem = treeDoubleItem;
        }
    }
}

void ProTreeWidget::slotImport()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("选择导入的文件夹");
    QString path = "";
    if(!_rightBtnItem)
    {
        path = QDir::currentPath();
        return;
    }

    path = dynamic_cast<ProTreeItem *>(_rightBtnItem)->getPath();
    fileDialog.setDirectory(path);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(fileDialog.exec())
    {
        fileNames = fileDialog.selectedFiles();
    }

    if(fileNames.size() <= 0)
        return;

    QString importPath = fileNames.at(0);

    int fileCount = 0;

    _progressDialog = new QProgressDialog(this);

    _createProThread = std::make_shared<ProTreeThread>(importPath, path,
                                                             _rightBtnItem,
                                                             fileCount, this, _rightBtnItem, nullptr);

    connect(_createProThread.get(), &ProTreeThread::sigUpdateProgress,
            this, ProTreeWidget::slotUpdateProgress);

    connect(_createProThread.get(), &ProTreeThread::sigFinishProgress,
            this, ProTreeWidget::slotFinishProgress);

    connect(_progressDialog, &QProgressDialog::canceled, this, ProTreeWidget::slotCancelProgress);
    connect(this, &ProTreeWidget::sigCancelProgress, _createProThread.get(), &ProTreeThread::slotCancelProgress);
    _createProThread->start();

    _progressDialog->setWindowTitle("please wait...");
    _progressDialog->setFixedWidth(PROGRESS_WIDTH);
    _progressDialog->setRange(0, PROGRESS_MAX);
    _progressDialog->exec();
}

void ProTreeWidget::slotSetActive()
{
    if(!_rightBtnItem)
    {
        return;
    }

    QFont nullFont;
    nullFont.setBold(false);
    if(_activeItem)
    {
        _activeItem->setFont(0, nullFont);
    }

    _activeItem = _rightBtnItem;
    nullFont.setBold(true);
    _activeItem->setFont(0, nullFont);
}

void ProTreeWidget::slotClosePro()
{
    RemoveProDialog  removeProDialog;
    removeProDialog.exec();
    bool isRemove = removeProDialog.isRemoved();
    int rightBtnIndex = this->indexOfTopLevelItem(_rightBtnItem);
    ProTreeItem * proTreeItem = dynamic_cast<ProTreeItem*>(_rightBtnItem);
    ProTreeItem * selectedItem = dynamic_cast<ProTreeItem*>(_selectdItem);

    QString deletePath = proTreeItem->getPath();
    qDebug() << "remove project from path: " << deletePath;

    _pathSet.remove(deletePath);
    if(isRemove){
        QDir deleteDir(deletePath);
        deleteDir.removeRecursively();
    }

    if(proTreeItem == _activeItem){
        _activeItem = nullptr;
    }

    if(selectedItem && proTreeItem == selectedItem->getRoot()){
        selectedItem = nullptr;
        emit sigClearSelected();
    }

    delete this->takeTopLevelItem(rightBtnIndex);
    _rightBtnItem = nullptr;
}

void ProTreeWidget::slotSlideShow()
{
    if(!_rightBtnItem)
        return;

    auto *rightProItem = dynamic_cast<ProTreeItem *>(_rightBtnItem);

    auto *lastChildItem = rightProItem->getLastPicChild();
    if(!lastChildItem)
        return;

    auto *firstChildItem = rightProItem->getFirstPicChild();
    if(!firstChildItem)
        return;

    _slideShowDlg = std::make_shared<SlideShowDlg>(this, firstChildItem, lastChildItem);
    _slideShowDlg->setModal(true);
    _slideShowDlg->showMaximized();
}

void ProTreeWidget::slotUpdateProgress(int count)
{
    if(!_progressDialog)
        return;

    if(count >= PROGRESS_MAX)
    {
        _progressDialog->setValue(count % PROGRESS_MAX);
    }
    else
    {
        _progressDialog->setValue(count % PROGRESS_MAX);
    }

}

void ProTreeWidget::slotFinishProgress()
{
    _progressDialog->setValue(PROGRESS_MAX);
    _progressDialog->deleteLater();
}


void ProTreeWidget::slotCancelProgress()
{
    emit sigCancelProgress();
    delete _progressDialog;
    _progressDialog = nullptr;
}

void ProTreeWidget::slotUpOpenPgogress(int count)
{
    if(!_openProProgressDlg){
        return;
    }
    //qDebug()<<"SlotUpOpenProgress count is " << count;
    if(count >= PROGRESS_MAX){
        _openProProgressDlg->setValue(count%PROGRESS_MAX);
    }else{
        _openProProgressDlg->setValue(count%PROGRESS_MAX);
    }
}

void ProTreeWidget::slotFinishOpenPgogress()
{
    if(!_openProProgressDlg){
        return;
    }
    _openProProgressDlg->setValue(PROGRESS_MAX);
    delete  _openProProgressDlg;
    _openProProgressDlg = nullptr;
}



void ProTreeWidget::slotPreShow()
{
    if(!_selectdItem)
        return;

    auto *curItem  = dynamic_cast<ProTreeItem *>(_selectdItem)->getPreItem();
    if(!curItem)
    {
        return;
    }

    emit sigUpdatePic(curItem->getPath());
    _selectdItem = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::slotNextShow()
{
    if(!_selectdItem)
        return;

    auto *curItem  = dynamic_cast<ProTreeItem *>(_selectdItem)->getNextItem();
    if(!curItem)
    {
        return;
    }

    emit sigUpdatePic(curItem->getPath());
    _selectdItem = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::slotOpenPro(const QString &path)
{
    if(_pathSet.find(path) != _pathSet.end())
    {
        return;
    }

    _pathSet.insert(path);
    int fileCount = 0;
    QDir proDir(path);

    _openProThread = std::make_shared<OpenTreeThread>(path, fileCount, this, nullptr);
    _openProThread->start();

    _openProProgressDlg  = new QProgressDialog(this);

    connect(_openProThread.get(), &OpenTreeThread::sigUpdateProgress,
            this,&ProTreeWidget::slotUpOpenPgogress);
    connect(_openProThread.get(), &OpenTreeThread::sigFinishProgress,
            this,&ProTreeWidget::slotFinishOpenPgogress);

    _openProProgressDlg->setWindowTitle("Please wait...");
    _openProProgressDlg->setFixedWidth(PROGRESS_WIDTH);
    _openProProgressDlg->setRange(0, PROGRESS_MAX);
    _openProProgressDlg->exec();

}

void ProTreeWidget::slotSetMusic(bool)
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setWindowTitle("选择导入的文件夹");

    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setNameFilter("(*.wav)");

    QStringList fileNames;
    if (fileDialog.exec()){
         fileNames = fileDialog.selectedFiles();
    }

    if(fileNames.length() <= 0){
          return;
    }

    auto filename = fileNames.at(0);
    _sound->setSource(QUrl::fromLocalFile(filename));
}

void ProTreeWidget::slotStartMusic()
{
    _sound->play();
}

void ProTreeWidget::slotStopMusic()
{
    _sound->stop();
}
