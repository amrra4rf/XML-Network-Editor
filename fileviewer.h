#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QWidget>
#include <QString>
namespace Ui {
class FileViewer;
}

class FileViewer : public QWidget
{
    Q_OBJECT

public:
    explicit FileViewer(QWidget *parent = nullptr);
    ~FileViewer();
    void setFileContent(const QString &content); // Function to set file content
     void setFileContent(const QString &content, const QString &filePath);
private slots:
    void on_pushButton_6_clicked();

    void on_compress_clicked();

    void on_minify_clicked();

    void on_decompress_clicked();

    void on_format_clicked();

    void on_convert_clicked();

    void on_vaild_clicked();

    void on_represent_clicked();

    void on_mostactive_clicked();

    void on_influencer_clicked();

    void on_mutual_clicked();

    void on_suggested_clicked();

    void on_word_clicked();

    void on_topic_clicked();

private:
    Ui::FileViewer *ui;
    QString m_filePath;
};

#endif // FILEVIEWER_H
////////////////////////////





