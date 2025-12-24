#include "fileviewer.h"
#include "../../Includes/Formater.hpp"
#include "../Services/compress.cpp"
#include "../Services/decompress.cpp"
#include "../Services/JsonConverter.cpp"
#include "../Services/Prettifyer.cpp"
#include "../Services/visualize.cpp"
#include "../Services/SocialNetwork.cpp"
//#include "format.h"
//#include "convert.h"
#include "../Services/vaild.cpp"
#include "../Services/XML_Minifying.cpp"
#include "ui_fileviewer.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <bits/stdc++.h>
#include <QFileDialog>
#include <fstream>
#include <string>
//#include"vaild.h"
#include"../../Includes/Reader.hpp"
#include"../../Includes/User.hpp"
#include"../../Includes/Graphs.hpp"
#include"../../Includes/Posts.hpp"
#include"../../Includes/CLI_h.hpp"
#include"../../Includes/visualize.hpp"
#include"../../Includes\SocialNetwork.hpp"
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QString>

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
bool FileViewer::validateCurrentFile()
{
    if (m_filePath.isEmpty()) return false;

    // Read file
    std::ifstream file(m_filePath.toStdString());
    if (!file.is_open()) return false;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string xmlContent = buffer.str();

    // Check consistency
    std::vector<std::string> errorLog;
    std::vector<int> errorLines;
    checkConsistency(xmlContent, errorLog, errorLines);

    // Return true if no errors
    return errorLog.empty();
}
void FileViewer::resetBuilder() {
    builder = PostsBuilder(); // creates a fresh instance with empty IDs
}

void FileViewer::setFileContent(const QString &content)
{
    ui->textEdit->setPlainText(content);

}


// void FileViewer::on_pushButton_6_clicked()
// {

//     if (m_filePath.isEmpty()) {
//         QMessageBox::warning(this, "Error", "No input file path found!");
//         return;
//     }

//     // Set output file path to project folder: "file.xml"
//     QString outFileName = QDir::currentPath() + "/minified.xml";

//     // Call your existing minifying function
//     minifying(m_filePath.toStdString(), outFileName.toStdString());

//     QMessageBox::information(this, "Success", "Minified file created at: " + outFileName);

// }
void FileViewer::on_minify_clicked()
{

    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // Set output file path to project folder: "file.xml"
    // Correct path for Release / Standalone App
    QString outFileName = QDir::currentPath() + "/output/minified.xml";

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
 QString outFileName = QDir::currentPath() + "/output//compressed.comp";

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
 QString outFileName = QDir::currentPath() + "/output/decompressed.xml";


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
 QString outFileName = QDir::currentPath() + "/output/formated.xml";

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
QString outFileName = QDir::currentPath() + "/output/xmlToJson.json";

    convert(m_filePath.toStdString(),outFileName.toStdString());

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
            QString fixedFilePath =QDir::currentPath() + "/output/fixed.xml";
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




// }
void FileViewer::on_represent_clicked()
{
    // 1. Check if path is empty
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // 2. CALL VALIDATION BEFORE PROCEEDING
    if (!validateCurrentFile()) {
        QMessageBox::critical(this, "Invalid XML",
                              "The XML file is not valid. Please fix errors before representing.");
        return; // <--- This stops the function here
    }

    // 3. If we reach here, the file IS valid. Continue with formatting...
    QString formattedFile = QDir::currentPath() + "/formatted.xml";

    Format_XML_File(
        m_filePath.toStdString(),
        formattedFile.toStdString()
        );

    QString outputDir = QDir::currentPath() + "/output/representation";
    QDir dir(outputDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    visualize(
        formattedFile.toStdString(),
        outputDir.toStdString()
        );

    QMessageBox::information(this, "Success", "Representation complete!");
}


void FileViewer::on_mostactive_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // --- ADD VALIDATION CHECK ---
    if (!validateCurrentFile()) {
        QMessageBox::critical(this, "Invalid XML", "The XML is invalid. Please fix it before analysis.");
        return;
    }

    resetBuilder();
    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    SocialNetworkGraph network(formattedFile.toStdString(), builder);
    Users u = network.getMostActiveUser();

    QMessageBox::information(this, "Most Active User",
                             QString::fromStdString(u.getName()) + " (ID: " + QString::number(u.getId()) + ")");
}

void FileViewer::on_influencer_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // --- ADD VALIDATION CHECK ---
    if (!validateCurrentFile()) {
        QMessageBox::critical(this, "Invalid XML", "The XML is invalid. Please fix it before analysis.");
        return;
    }

    resetBuilder();
    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    SocialNetworkGraph network(formattedFile.toStdString(), builder);
    Users u = network.getInfluencer();

    QMessageBox::information(this, "Most Influencer User",
                             QString::fromStdString(u.getName()) + " (ID: " + QString::number(u.getId()) + ")");
}
#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QLabel>

void FileViewer::on_mutual_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // --- STEP 1: VALIDATE XML ---
    if (!validateCurrentFile()) {
        QMessageBox::critical(this, "Invalid XML", "The XML is invalid. Cannot find mutual followers.");
        return;
    }

    // --- STEP 2: PREPARE GRAPH ---
    resetBuilder();
    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());
    SocialNetworkGraph socialGraph(formattedFile.toStdString(), builder);

    // Get all users to populate the list
    const auto& users = socialGraph.network.getUsers();
    if (users.size() < 2) {
        QMessageBox::information(this, "Mutual Followers", "Not enough users in the file to compare.");
        return;
    }

    // --- STEP 3: CREATE SELECTION DIALOG ---
    QDialog selectDialog(this);
    selectDialog.setWindowTitle("Select Users");
    selectDialog.setMinimumWidth(300);
    selectDialog.setMinimumHeight(400);

    QVBoxLayout* layout = new QVBoxLayout(&selectDialog);
    QLabel* label = new QLabel("Select at least 2 users (Ctrl+Click to select multiple):", &selectDialog);
    layout->addWidget(label);

    // Create List Widget for Multi-Selection
    QListWidget* listWidget = new QListWidget(&selectDialog);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection); // Enable multi-select

    // Populate list with User Name and ID
    for (const auto& user : users) {
        QString userLabel = QString::fromStdString(user.getName()) + " (ID: " + QString::number(user.getId()) + ")";
        QListWidgetItem* item = new QListWidgetItem(userLabel);
        // Store the actual ID in the item's data for easy retrieval later
        item->setData(Qt::UserRole, user.getId());
        listWidget->addItem(item);
    }
    layout->addWidget(listWidget);

    // Add OK/Cancel buttons
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &selectDialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &selectDialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &selectDialog, &QDialog::reject);

    // --- STEP 4: HANDLE SELECTION ---
    if (selectDialog.exec() == QDialog::Accepted) {
        QList<QListWidgetItem*> selectedItems = listWidget->selectedItems();

        if (selectedItems.size() < 2) {
            QMessageBox::warning(this, "Selection Error", "Please select at least 2 users.");
            return;
        }

        // Extract IDs from selection
        std::vector<int> selectedIds;
        QString namesStr;
        for (auto* item : selectedItems) {
            selectedIds.push_back(item->data(Qt::UserRole).toInt());
            namesStr += item->text() + "\n";
        }

        // --- STEP 5: CALCULATE MUTUALS ---
        // Assuming your backend supports finding mutuals for N users (intersection of all N)
        // If your getMutualFollowers only takes 2 IDs, you might need to adapt logic.
        // Based on your CLI logic earlier, it seemed to handle a vector of IDs.
        std::vector<Users> mutuals = socialGraph.getMutualFollowers(selectedIds);

        // --- STEP 6: DISPLAY RESULTS ---
        QString msg = "Selected Users:\n" + namesStr + "\n";
        msg += "----------------------------------------\n";
        msg += "Mutual Followers (Common to ALL selected):\n\n";

        if (mutuals.empty()) {
            msg += "No mutual followers found.";
        } else {
            for (const Users& u : mutuals) {
                msg += QString(" - %1 (ID: %2)\n")
                .arg(QString::fromStdString(u.getName()))
                    .arg(u.getId());
            }
        }

        QMessageBox resultsBox;
        resultsBox.setWindowTitle("Mutual Followers Results");
        resultsBox.setText("Calculation Complete");
        resultsBox.setDetailedText(msg);
        resultsBox.exec();
    }
}
void FileViewer::on_suggested_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // --- STEP 1: VALIDATE XML ---
    if (!validateCurrentFile()) {
        QMessageBox::critical(this, "Invalid XML", "The XML is invalid. Cannot generate suggestions.");
        return;
    }

    resetBuilder(); // Important: reset unique ID counters
    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    QString outputDir = QDir::currentPath() + "/representation";
    QDir dir(outputDir);
    if (!dir.exists()) dir.mkpath(".");

    SocialNetworkGraph network(formattedFile.toStdString(), builder);
    auto suggestionsMap = network.getSuggestions();

    QString msg;
    for (auto& pair : suggestionsMap) {
        int userId = pair.first;
        const std::vector<Users>& suggested = pair.second;

        msg += "Suggestions for " +
               QString::fromStdString(network.getGraph().getUserFromID(userId).getName()) +
               ":\n";

        if (suggested.empty()) {
            msg += "  No suggestions\n\n";
            continue;
        }

        for (const Users& u : suggested) {
            msg += "  - " + QString::fromStdString(u.getName()) +
                   " (ID: " + QString::number(u.getId()) + ")\n";
        }
        msg += "\n";
    }

    QMessageBox::information(this, "Suggested Users", msg);
}
void FileViewer::on_word_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // --- STEP 1: VALIDATE XML ---
    if (!validateCurrentFile()) {
        QMessageBox::critical(this, "Invalid XML", "The XML is invalid. Search cannot be performed.");
        return;
    }

    resetBuilder(); // Clear previous IDs

    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    SocialNetworkGraph network(formattedFile.toStdString(), builder);

    bool ok;
    QString qword = QInputDialog::getText(this, "Search Posts",
                                          "Enter a word to search for:",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || qword.isEmpty()) return;

    std::vector<Posts> results = network.getPostsByWord(qword.toStdString());

    QString allPosts;
    if (results.empty()) {
        allPosts = "No posts found containing \"" + qword + "\"";
    } else {
        for (auto& p : results) {
            allPosts += QString::fromStdString(p.getcontent()) + "\n------\n";
        }
    }

    QMessageBox::information(this, "Results", allPosts);
}

void FileViewer::on_topic_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    if (!validateCurrentFile()) {
        QMessageBox::critical(this, "Invalid XML", "The XML is invalid.");
        return;
    }

    // 1. Ask for input FIRST before doing heavy processing
    bool ok;
    QString qtopic = QInputDialog::getText(this, "Search Posts",
                                           "Enter a topic to search for:",
                                           QLineEdit::Normal, "", &ok);
    if (!ok || qtopic.trimmed().isEmpty()) return;

    // 2. Now do the heavy lifting
    resetBuilder();
    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    // Local scope for network to ensure it's destroyed before the message box if needed
    SocialNetworkGraph network(formattedFile.toStdString(), builder);
    std::vector<Posts> results = network.getPostsByTopicFast(qtopic.trimmed().toStdString());

    QString allPosts;
    if (results.empty()) {
        allPosts = "No posts found containing \"" + qtopic + "\"";
    } else {
        // Use const reference to avoid triggering potentially buggy copy constructors
        for (const auto& p : results) {
            allPosts += QString::fromStdString(p.getcontent()) + "\n------\n";
        }
    }

    QMessageBox::information(this, "Results", allPosts);
}
