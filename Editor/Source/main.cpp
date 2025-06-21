#include <QApplication>
#include "EditorWindow.h"
#include "Game.h"


int main(int argc, char *argv[])
{
	Game game("Limbic Engine Editor");

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Alek Mabry");
    QCoreApplication::setOrganizationDomain("alekstutorials.com");
    QCoreApplication::setApplicationName("Limbic Editor");
	
	EditorWindow w(&game);
	w.show();
    return a.exec();
}
