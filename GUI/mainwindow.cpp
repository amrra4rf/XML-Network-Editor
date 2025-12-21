#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "fileviewer.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"),       // Dialog title
        QDir::homePath(),      // Default directory
        tr("All Files (*.*);;Text Files (*.txt)") // Filter
        );

    if (!fileName.isEmpty()) {
        ui->lineEdit->setText(fileName);
    }
}



void MainWindow::on_open_clicked()
{
    QString path = ui->lineEdit->text();
        if (path.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please select a file first!");
            return;
        }

        QFileInfo checkFile(path);
        if (!checkFile.exists() || !checkFile.isFile()) {
            QMessageBox::warning(this, "Warning", "File does not exist!");
            return;
        }

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Cannot open file: " + path);
            return;
        }

        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        // Open the FileViewer window
        FileViewer *viewer = new FileViewer();
        viewer->setFileContent(content);
        viewer->setFileContent(content, path);
        viewer->show();
}
//////////////




void MainWindow::on_save_clicked()
{

    QString textContent = ui->textEdit->toPlainText();

    if (textContent.isEmpty()) {
        return;
    }

    // 2. Open Save Dialog
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save XML"),
                                                    QDir::homePath() + "/output.xml",
                                                    tr("XML Files (*.xml)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            // 3. DO NOT use toHtmlEscaped() here.
            // Write the text directly so the < > stay as tags.
            out << textContent;

            file.close();
        }
    }
}

