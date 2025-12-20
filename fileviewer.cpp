#include "fileviewer.h"
#include "compress.h"
#include "decompress.h"
#include "format.h"
#include "convert.h"
#include"XML_Minifying.h"
#include "ui_fileviewer.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <bits/stdc++.h>
#include <QFileDialog>
#include <fstream>
#include <string>
#include"vaild.h"
#include"vis.h"
//#include"SocialNetworkGraphTemp.h"


using namespace std;


FileViewer::FileViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileViewer)
{
    ui->setupUi(this);
}

FileViewer::~FileViewer()
{
    delete ui;
}
////////////////////
void FileViewer::setFileContent(const QString &content, const QString &filePath)
{
    ui->textEdit->setPlainText(content);
    m_filePath = filePath; // store original file path
}

void FileViewer::setFileContent(const QString &content)
{
    ui->textEdit->setPlainText(content);

}


void FileViewer::on_pushButton_6_clicked()
{

    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // Set output file path to project folder: "file.xml"
    QString outFileName = QDir::currentPath() + "/minified.xml";

    // Call your existing minifying function
    minifying(m_filePath.toStdString(), outFileName.toStdString());

    QMessageBox::information(this, "Success", "Minified file created at: " + outFileName);

}
void FileViewer::on_minify_clicked()
{

    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // Set output file path to project folder: "file.xml"
    QString outFileName = QDir::currentPath() + "/minified.xml";

    // Call your existing minifying function
    minifying(m_filePath.toStdString(), outFileName.toStdString());

    QMessageBox::information(this, "Success", "Minified file created at: " + outFileName);

}



void FileViewer::on_compress_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // Set output file path to project folder: "file.xml"
    QString outFileName = QDir::currentPath() + "/compressed.comp";

    // Call your existing minifying function
    compress(m_filePath.toStdString(), outFileName.toStdString());

    QMessageBox::information(this, "Success", "Compressed file created at: " + outFileName);

}


void FileViewer::on_decompress_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open File"),       // Dialog title
    QDir::homePath(),      // Default directory
    tr("All Files (*.*);;Text Files (*.txt)") // Filter
    );
    QString outFileName = QDir::currentPath() + "/decompressed.xml";


    if (!fileName.isEmpty()) {
        decompress(fileName.toStdString(),outFileName.toStdString());
        QMessageBox::information(this, "Success", "Decompressed file created at: " + outFileName);
    }
    else{
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }
}


void FileViewer::on_format_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // Set output file path to project folder: "file.xml"
    QString outFileName = QDir::currentPath() + "/formated.xml";

    // Call your existing minifying function
    Format_XML_File(m_filePath.toStdString(), outFileName.toStdString());

    QMessageBox::information(this, "Success", "formated file created at: " + outFileName);
}


void FileViewer::on_convert_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // Set output file path to project folder: "file.xml"
    QString outFileName = QDir::currentPath() + "/converted.json";

    // Call your existing minifying function
    convert(m_filePath.toStdString());

    QMessageBox::information(this, "Success", "converted file created at: " + outFileName);
}


void FileViewer::on_vaild_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected");
        return;
    }

    // 1. Read file content
    std::ifstream file(m_filePath.toStdString());
    if (!file.is_open()) {
        QMessageBox::warning(this, "Error", "Could not read file");
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string xmlContent = buffer.str();

    // 2. Validate XML
    std::vector<std::string> errorLog;
    std::vector<int> errorLines;

    checkConsistency(xmlContent, errorLog, errorLines);

    // 3. Show result
    if (errorLog.empty()) {
        QMessageBox::information(this, "Valid XML", "XML file is valid ✔");
    } else {
        QString errors;
        for (size_t i = 0; i < errorLog.size(); ++i) {
            errors += QString::fromStdString("Line " + std::to_string(errorLines[i]) + ": " + errorLog[i]) + "\n";
        }

        // Ask user if they want to try to fix it
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Invalid XML",
                                      errors + "\nDo you want to attempt fixing the XML?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Call your fixXml function
            std::string fixedXml = fixXml(xmlContent, errorLines);

            // Save as a new file "fixed.xml" in the current project directory
            QString fixedFilePath = QDir::currentPath() + "/fixed.xml";
            std::ofstream outFile(fixedFilePath.toStdString());
            if (outFile.is_open()) {
                outFile << fixedXml;
                outFile.close();
                QMessageBox::information(this, "XML Fixed",
                                         "Fixed XML file has been saved as:\n" + fixedFilePath);
            } else {
                QMessageBox::warning(this, "Fix Failed", "Could not save the fixed XML file.");
            }
        }
    }
}



void FileViewer::on_represent_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // Create output directory inside project folder
    QString outputDir = QDir::currentPath() + "/representation";

    QDir dir(outputDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // Call visualize with DIRECTORY, not file
    visualize(
        m_filePath.toStdString(),
        outputDir.toStdString()
        );

    QMessageBox::information(
        this,
        "Success",
        "Graph representation created at:\n" + outputDir
        );
}


void FileViewer::on_mostactive_clicked()
{
  /* if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    SocialNetworkGraph x(m_filePath.toStdString());
    Users u = x.getMostActiveUser();

    QMessageBox::information(
        this,
        "Most Active User",
        QString::fromStdString(u.getName())
        );*/
}






void FileViewer::on_influencer_clicked()
{

}


void FileViewer::on_mutual_clicked()
{

}


void FileViewer::on_suggested_clicked()
{

}


void FileViewer::on_word_clicked()
{

}


void FileViewer::on_topic_clicked()
{

}

