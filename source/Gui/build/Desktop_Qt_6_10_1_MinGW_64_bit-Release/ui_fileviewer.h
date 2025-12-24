/********************************************************************************
** Form generated from reading UI file 'fileviewer.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEVIEWER_H
#define UI_FILEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileViewer
{
public:
    QTextEdit *textEdit;
    QPushButton *vaild;
    QPushButton *format;
    QPushButton *compress;
    QPushButton *decompress;
    QPushButton *convert;
    QPushButton *minify;
    QPushButton *represent;
    QPushButton *influencer;
    QPushButton *mutual;
    QPushButton *mostactive;
    QPushButton *word;
    QPushButton *topic;
    QPushButton *suggested;

    void setupUi(QWidget *FileViewer)
    {
        if (FileViewer->objectName().isEmpty())
            FileViewer->setObjectName("FileViewer");
        FileViewer->resize(1676, 984);
        textEdit = new QTextEdit(FileViewer);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(0, 0, 1681, 681));
        textEdit->setReadOnly(true);
        vaild = new QPushButton(FileViewer);
        vaild->setObjectName("vaild");
        vaild->setGeometry(QRect(0, 700, 90, 29));
        format = new QPushButton(FileViewer);
        format->setObjectName("format");
        format->setGeometry(QRect(115, 700, 90, 29));
        compress = new QPushButton(FileViewer);
        compress->setObjectName("compress");
        compress->setGeometry(QRect(230, 700, 90, 29));
        decompress = new QPushButton(FileViewer);
        decompress->setObjectName("decompress");
        decompress->setGeometry(QRect(345, 700, 90, 29));
        convert = new QPushButton(FileViewer);
        convert->setObjectName("convert");
        convert->setGeometry(QRect(460, 700, 90, 29));
        minify = new QPushButton(FileViewer);
        minify->setObjectName("minify");
        minify->setGeometry(QRect(575, 700, 90, 29));
        represent = new QPushButton(FileViewer);
        represent->setObjectName("represent");
        represent->setGeometry(QRect(690, 700, 90, 29));
        influencer = new QPushButton(FileViewer);
        influencer->setObjectName("influencer");
        influencer->setGeometry(QRect(920, 700, 90, 29));
        mutual = new QPushButton(FileViewer);
        mutual->setObjectName("mutual");
        mutual->setGeometry(QRect(1035, 700, 90, 29));
        mostactive = new QPushButton(FileViewer);
        mostactive->setObjectName("mostactive");
        mostactive->setGeometry(QRect(805, 700, 90, 29));
        word = new QPushButton(FileViewer);
        word->setObjectName("word");
        word->setGeometry(QRect(1290, 700, 111, 29));
        topic = new QPushButton(FileViewer);
        topic->setObjectName("topic");
        topic->setGeometry(QRect(1420, 700, 111, 29));
        suggested = new QPushButton(FileViewer);
        suggested->setObjectName("suggested");
        suggested->setGeometry(QRect(1150, 700, 120, 29));

        retranslateUi(FileViewer);

        QMetaObject::connectSlotsByName(FileViewer);
    } // setupUi

    void retranslateUi(QWidget *FileViewer)
    {
        FileViewer->setWindowTitle(QCoreApplication::translate("FileViewer", "Form", nullptr));
        vaild->setText(QCoreApplication::translate("FileViewer", "Vaild", nullptr));
        format->setText(QCoreApplication::translate("FileViewer", "Format", nullptr));
        compress->setText(QCoreApplication::translate("FileViewer", "Comperss", nullptr));
        decompress->setText(QCoreApplication::translate("FileViewer", "Decomperss", nullptr));
        convert->setText(QCoreApplication::translate("FileViewer", "Convert", nullptr));
        minify->setText(QCoreApplication::translate("FileViewer", "Minify", nullptr));
        represent->setText(QCoreApplication::translate("FileViewer", "Represent", nullptr));
        influencer->setText(QCoreApplication::translate("FileViewer", "Influencer", nullptr));
        mutual->setText(QCoreApplication::translate("FileViewer", "Mutual", nullptr));
        mostactive->setText(QCoreApplication::translate("FileViewer", "Most Active", nullptr));
        word->setText(QCoreApplication::translate("FileViewer", "Search by word", nullptr));
        topic->setText(QCoreApplication::translate("FileViewer", "Search by topic", nullptr));
        suggested->setText(QCoreApplication::translate("FileViewer", "suggested users", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileViewer: public Ui_FileViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEVIEWER_H
