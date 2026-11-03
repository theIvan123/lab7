#include "lab7.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    lab7 window;
    window.show();
    return app.exec();
}
