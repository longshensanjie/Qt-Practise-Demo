#include "widget.h"
#include "ui_widget.h"
#include "qdebug.h"
#include "qfiledialog.h"
#include "qfileinfo.h"
#include "qdrag.h"
#include "QDragEnterEvent"
#include "QDropEvent"
#include "QMimeData"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    on_pushButtonClear_clicked();
    initForm();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initForm()
{
    resize(1000, 800);

    QStringList headNext;
    headNext << "文件名" << "类型" << "大小" << "总行数" << "代码行数" << "注释行数" << "空白行数" << "路径";

    QList<int> ColumnWidth;
    ColumnWidth << 130 << 50 << 70 << 80 << 70 << 70 << 70 << 150;
    int columnCount = headNext.size();

    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setHorizontalHeaderLabels(headNext);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidget->verticalHeader()->setHighlightSections(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);

    for(auto i = 0; i != columnCount; ++i)
    {
        ui->tableWidget->setColumnWidth(i, ColumnWidth.at(i));
    }

    ui->lineEditFileCount->setStyleSheet("color:#17A086;");
    ui->lineEditFileSize->setStyleSheet("color:#17A086;");
    ui->lineEditFileRow->setStyleSheet("color:#17A086;");
    ui->lineEditCodeRow->setStyleSheet("color:#17A086;");
    ui->lineEditNoteRow->setStyleSheet("color:#17A086;");
    ui->lineEditBlankRow->setStyleSheet("color:#17A086;");

    QFont font;
    font.setBold(true);
    font.setPixelSize(12);

    ui->lineEditFileCount->setFont(font);
    ui->lineEditFileSize->setFont(font);
    ui->lineEditFileRow->setFont(font);
    ui->lineEditCodeRow->setFont(font);
    ui->lineEditNoteRow->setFont(font);
    ui->lineEditBlankRow->setFont(font);

    ui->lineEditFiliter->setPlaceholderText("例如: *.h *.c *.cpp");

}

bool Widget::checkFile(const QString& fileName)
{
    QFileInfo file(fileName);
    QString suffix = "*." + file.suffix();
    QString filter = ui->lineEditFiliter->text().trimmed();
    QStringList filters = filter.split(" ");
    return filters.contains(suffix);
}

void Widget::countPathFileCount(const QString& path)
{
    QDir dir(path);
    QFileInfoList fileInfos = dir.entryInfoList();

    foreach (QFileInfo info, fileInfos) {
        QString fileName = info.fileName();
        if(info.isFile())
        {
            if(checkFile(fileName))
                listFiles << info.filePath();
        }
        else
        {
            if(fileName == "." || fileName == "..")
            {
                continue;
            }
            countPathFileCount(info.absoluteFilePath());
        }
    }
}
void Widget::countFileRows(const QStringList& files)
{
    int count = files.size();
    on_pushButtonClear_clicked();
    ui->tableWidget->setRowCount(count);
    quint32 totalLines = 0;
    quint32 totalBytes = 0;
    quint32 totalCodes = 0;
    quint32 totalNotes = 0;
    quint32 totalBlank = 0;
    for(auto i = 0; i != count; ++i)
    {
        QFileInfo info(files.at(i));
        int lineCode;
        int lineNote;
        int lineBlank;
        countCodeRows(info.filePath(), lineCode, lineNote, lineBlank);
        int lineAll = lineCode + lineNote + lineBlank;

        QTableWidgetItem *itemName = new QTableWidgetItem;
        itemName->setText(info.fileName());

        QTableWidgetItem *itemSuffix = new QTableWidgetItem;
        itemSuffix->setText(info.suffix());

        QTableWidgetItem *itemSize = new QTableWidgetItem;
        itemSize->setText(QString::number(info.size()));

        QTableWidgetItem *itemLine = new QTableWidgetItem;
        itemLine->setText(QString::number(lineAll));

        QTableWidgetItem *itemCode = new QTableWidgetItem;
        itemCode->setText(QString::number(lineCode));

        QTableWidgetItem *itemNode = new QTableWidgetItem;
        itemNode->setText(QString::number(lineNote));

        QTableWidgetItem *itemBlank = new QTableWidgetItem;
        itemBlank->setText(QString::number(lineBlank));

        QTableWidgetItem *itemPath = new QTableWidgetItem;
        itemPath->setText(info.filePath());

        itemName->setTextAlignment(Qt::AlignCenter);
        itemSuffix->setTextAlignment(Qt::AlignCenter);
        itemSize->setTextAlignment(Qt::AlignCenter);
        itemLine->setTextAlignment(Qt::AlignCenter);
        itemCode->setTextAlignment(Qt::AlignCenter);
        itemNode->setTextAlignment(Qt::AlignCenter);
        itemBlank->setTextAlignment(Qt::AlignCenter);
        itemPath->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(i, 0, itemName);
        ui->tableWidget->setItem(i, 1, itemSuffix);
        ui->tableWidget->setItem(i, 2, itemSize);
        ui->tableWidget->setItem(i, 3, itemLine);
        ui->tableWidget->setItem(i, 4, itemCode);
        ui->tableWidget->setItem(i, 5, itemNode);
        ui->tableWidget->setItem(i, 6, itemBlank);
        ui->tableWidget->setItem(i, 7, itemPath);

        totalBytes += info.size();
        totalLines += lineAll;
        totalCodes += lineCode;
        totalNotes += lineNote;
        totalBlank += lineBlank;

        if(i % 100 == 0)
        {
            qApp->processEvents();
        }
    }

    listFiles.clear();
    ui->lineEditFileCount->setText(QString::number(count));

    QString suf = "bytes";
    if(totalBytes > 1024)
    {
        totalBytes /= 1024;
        suf = "KB";
    }
    if(totalBytes > 1024)
    {
        totalBytes /= 1024;
        suf = "MB";
    }
    if(totalBytes > 1024)
    {
        totalBytes /= 1024;
        suf = "GB";
    }
    ui->lineEditFileSize->setText(QString::number(totalBytes) + suf);

    ui->lineEditFileRow->setText(QString::number(totalLines));
    ui->lineEditCodeRow->setText(QString::number(totalCodes));
    ui->lineEditNoteRow->setText(QString::number(totalNotes));
    ui->lineEditBlankRow->setText(QString::number(totalBlank));

    double percent = 0.0;
    percent = (static_cast<double>(totalCodes)/totalLines) * 100;
    ui->labelPercentCodeRow->setText(QString("%1%").arg(percent, 5, 'f', 2,QChar(' ')));
    percent = (static_cast<double>(totalNotes)/totalLines) * 100;
    ui->labelPercentNoteRow->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
    percent = (static_cast<double>(totalBlank)/totalLines) * 100;
    ui->labelPercentBlankRow->setText(QString("%1%").arg(percent, 5, 'f', 2,  QChar(' ')));

}
void Widget::countCodeRows(const QString& fileName, int& lineCode, int& lineNote, int& lineBlank)
{
    lineCode = lineNote = lineBlank = 0;
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        QString line;
        bool isNode = false;
        while (!in.atEnd()) {
            line = in.readLine();
            if(line.startsWith(" "))
                line.remove(" ");
            if(line.startsWith("/*"))
            {
                isNode = true;
            }

            if(isNode)
            {
                lineNote++;
            }
            else
            {
                if(line.startsWith("//"))
                {
                    lineNote++;
                }
                else if(line.isEmpty())
                {
                    lineBlank++;
                }
                else
                {
                    lineCode++;
                }

            }

            if(line.endsWith("*/"))
            {
                isNode = false;
            }
        }
    }
}
void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}
void Widget::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    listFiles.clear();
    if(mimeData->hasUrls())
    {
        QList<QUrl> listUrl = mimeData->urls();
        QFileInfo info;
        for(int i = 0; i != listUrl.size(); ++i)
        {
            QString filePath = listUrl.at(i).toLocalFile();
            info.setFile(filePath);
            if(info.isDir())
            {
                countPathFileCount(filePath);
            }
            else
            {
                if(checkFile(filePath))
                    listFiles << filePath;
            }
        }
    }
    countFileRows(listFiles);
}
void Widget::on_pushButtonOpenFile_clicked()
{
    QString filer = QString("代码后缀(%1").arg(ui->lineEditFiliter->text().trimmed());
    QStringList files = QFileDialog::getOpenFileNames(this, "选择文件", "./", filer);
    if(files.size() > 0)
    {
        ui->lineEditFile->setText(files.join("|"));
        countFileRows(files);
    }
}
void Widget::on_pushButtonOpenDir_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "选择目录",  "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!path.isEmpty())
    {
        ui->lineEditDir->setText(path);
        countPathFileCount(path);
        countFileRows(listFiles);
    }
}
void Widget::on_pushButtonClear_clicked()
{
    ui->lineEditFileCount->setText("0");
    ui->lineEditFileSize->setText("0");
    ui->lineEditFileRow->setText("0");
    ui->lineEditCodeRow->setText("0");
    ui->lineEditNoteRow->setText("0");
    ui->lineEditBlankRow->setText("0");
//    ui->lineEditFile->setText("");
//    ui->lineEditDir->setText("");

    ui->tableWidget->setRowCount(0);

    ui->labelPercentCodeRow->setText("00.00%");
    ui->labelPercentNoteRow->setText("00.00%");
    ui->labelPercentBlankRow->setText("00.00%");
}

