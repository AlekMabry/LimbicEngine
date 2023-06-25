#include <QApplication>
#include "EditorWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Alek Mabry");
    QCoreApplication::setOrganizationDomain("alekstutorials.com");
    QCoreApplication::setApplicationName("Limbic Editor");
    EditorWindow w;
    w.show();
    return a.exec();
}
