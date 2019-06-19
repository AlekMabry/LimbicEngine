#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QtCore/QFile>
#include <QMainWindow>


#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication qtApp(argc, argv);

    /*
    QMainWindow *widget = new QMainWindow;
    Ui::MainWindow ui;
    ui.setupUi(widget);
     */

    MainWindow *mainWindow = new MainWindow;


    mainWindow->show();
    return qtApp.exec();
}