#include <QtGui/QApplication>
#include "on.h"
#include "wallpaper.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    QApplication::setStyle(new QCleanlooksStyle());
    QApplication a(argc, argv);
    On w;

    Wallpaper wp;
    wp.setOn(&w);
    wp.start();

    w.show();
    //notify_init("Asturix On");

    return a.exec();
}
