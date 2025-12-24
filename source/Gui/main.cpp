#include "mainwindow.h"

#include <QApplication>
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // --- 2. ADD THIS BLOCK HERE (Before creating the window) ---
    QDir dir("output");
    if (!dir.exists()) {
        dir.mkpath("."); // Creates the "output" folder if it's missing
    }
    // -----------------------------------------------------------
    MainWindow w;
    w.show();
    return a.exec();
}
