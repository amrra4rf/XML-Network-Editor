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
#include"SocialNetworkGraphTemp.h"
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
void FileViewer::resetBuilder() {
    builder = PostsBuilder(); // creates a fresh instance with empty IDs
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

    // 1️⃣ Create formatted XML path
    QString formattedFile =
        QDir::currentPath() + "/formatted.xml";

    // 2️⃣ First: format the XML
    Format_XML_File(
        m_filePath.toStdString(),
        formattedFile.toStdString()
        );

    // 3️⃣ Create output directory for representation
    QString outputDir =
        QDir::currentPath() + "/representation";

    QDir dir(outputDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // 4️⃣ Use FORMATTED file as input to visualize
    visualize(
        formattedFile.toStdString(),
        outputDir.toStdString()
        );

    QMessageBox::information(
        this,
        "Success",
        "Formatted XML created at:\n" + formattedFile +
            "\n\nGraph representation created at:\n" + outputDir
        );
}


void FileViewer::on_mostactive_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    resetBuilder();  // <-- important: clear previous IDs

    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    QString outputDir = QDir::currentPath() + "/representation";
    QDir dir(outputDir);
    if (!dir.exists()) dir.mkpath(".");

    // Pass the builder to avoid duplicate ID issues
    SocialNetworkGraph network(formattedFile.toStdString(), builder);

    Users u = network.getMostActiveUser();

    QMessageBox::information(
        this,
        "Most Active User",
        QString::fromStdString(u.getName()) +
            " (ID: " + QString::number(u.getId()) + ")"
        );
}




void FileViewer::on_influencer_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    resetBuilder(); // <-- reset before reading XML

    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    // Pass builder to SocialNetworkGraph
    SocialNetworkGraph network(formattedFile.toStdString(), builder);

    Users u = network.getInfluencer();

    QMessageBox::information(
        this,
        "Most Influencer User",
        QString::fromStdString(u.getName()) + " (ID: " +
            QString::number(u.getId()) + ")"
        );
}

void FileViewer::on_mutual_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    // 1. Prepare formatting
    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    // 2. We need a PostsBuilder to initialize SocialNetworkGraph
    PostsBuilder builder;
    SocialNetworkGraph socialGraph(formattedFile.toStdString(), builder);

    // 3. Access the users list through the Graph object
    // Using network (lowercase) to match your class definition
    const auto& users = socialGraph.network.getUsers();

    if (users.size() < 2) {
        QMessageBox::information(this, "Mutual Followers", "Not enough users to compare.");
        return;
    }

    QString msg = "Mutual Followers for each user pair:\n\n";

    // 4. Calculate intersections for pairs
    for (size_t i = 0; i < users.size(); ++i) {
        for (size_t j = i + 1; j < users.size(); ++j) {
            vector<int> ids = { users[i].getId(), users[j].getId() };

            // Calling the new mutual followers function
            vector<Users> mutuals = socialGraph.getMutualFollowers(ids);

            msg += QString("Users %1 & %2:\n")
                       .arg(QString::fromStdString(users[i].getName()))
                       .arg(QString::fromStdString(users[j].getName()));

            if (mutuals.empty()) {
                msg += "  No mutual followers\n\n";
            } else {
                for (const Users& u : mutuals) {
                    msg += QString("  - %1 (ID: %2)\n")
                    .arg(QString::fromStdString(u.getName()))
                        .arg(u.getId());
                }
                msg += "\n";
            }
        }
    }

    // 5. Display the result in a scrollable message box if it's too long
    QMessageBox msgBox;
    msgBox.setWindowTitle("Mutual Followers");
    msgBox.setText("Results of pair-wise mutual follower search:");
    msgBox.setDetailedText(msg); // Good for long text outputs
    msgBox.exec();
}

void FileViewer::on_suggested_clicked()
{
    if (m_filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file path found!");
        return;
    }

    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    QString outputDir = QDir::currentPath() + "/representation";
    QDir dir(outputDir);
    if (!dir.exists()) dir.mkpath(".");

    SocialNetworkGraph network(formattedFile.toStdString(),builder);
    auto suggestionsMap = network.getSuggestions();

    QString msg;
    for (auto& pair : suggestionsMap) {
        int userId = pair.first;
        const vector<Users>& suggested = pair.second;

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

    resetBuilder(); // <-- important!

    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    SocialNetworkGraph network(formattedFile.toStdString(), builder); // pass builder

    bool ok;
    QString qword = QInputDialog::getText(this, "Search Posts",
                                          "Enter a word to search for:",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || qword.isEmpty()) return;

    vector<Posts> results = network.getPostsByWord(qword.toStdString());

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

    resetBuilder(); // <-- Important: clear IDs before reading XML

    QString formattedFile = QDir::currentPath() + "/formatted.xml";
    Format_XML_File(m_filePath.toStdString(), formattedFile.toStdString());

    QString outputDir = QDir::currentPath() + "/representation";
    QDir dir(outputDir);
    if (!dir.exists()) dir.mkpath(".");

    bool ok;
    QString qtopic = QInputDialog::getText(this, "Search Posts",
                                           "Enter a topic to search for:",
                                           QLineEdit::Normal, "", &ok);
    if (!ok || qtopic.trimmed().isEmpty()) return;

    // Pass the builder to avoid duplicate IDs
    SocialNetworkGraph network(formattedFile.toStdString(), builder);

    vector<Posts> results = network.getPostsByTopicFast(qtopic.trimmed().toStdString());

    QString allPosts;
    if (results.empty()) {
        allPosts = "No posts found containing \"" + qtopic + "\"";
    } else {
        for (auto& p : results) {
            allPosts += QString::fromStdString(p.getcontent()) + "\n------\n";
        }
    }

    QMessageBox::information(this, "Results", allPosts);
}