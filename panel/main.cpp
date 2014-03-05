#include <QtGui/QApplication>
#include "onpanel.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    /*if(getenv("QT_GRAPHICSSYSTEM") == 0) {
        QApplication::setGraphicsSystem("raster");
    }*/
    QApplication::setStyle(new QWindowsStyle);
    QApplication::setDesktopSettingsAware(false);
    QApplication a(argc, argv);
    PanelStyle::instance();
    OnPanel w;
    w.show();

    return a.exec();
}
