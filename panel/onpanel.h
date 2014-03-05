#ifndef ONPANEL_H
#define ONPANEL_H

#include <QtGui/QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QX11Info>
//#include <QX11EmbedContainer>
#include <QDesktopWidget>
#include <QToolButton>
#include <QCleanlooksStyle>
#include <QFileInfo>
#include <QDir>
#include <QFrame>
#include <QScrollArea>
#include <QFontMetrics>

#include <indicator/indicatorapplet.h>
#include <legacytray/legacytrayapplet.h>
#include <common/panelstyle.h>

#include <bamf-application.h>
#include <bamf-matcher.h>
#include <bamf-window.h>
//#include <bamf-list.h>
#include <bamf-factory.h>

#undef signals

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

class OnPanel : public QWidget
{
    Q_OBJECT

public:
    OnPanel(QWidget *parent = 0);
    ~OnPanel();
    QToolButton *windowButton;
    QHBoxLayout *hlayout;
    QScrollArea *windowList;
    QIcon getIcon(QString icon);
    //QPixmap getWindowScreen(WId xid);

public slots:
    void setPanelSpace();
    void setWindowButton();
    void updateWindowButton(QString text, QIcon icon);
    void windowUpdate();
    void setWindowList();
    void addWindow(BamfView *view);
    void removeWindow(BamfView *view);
    void windowTriggered();
    void showDesktop();
    //void embedWindow(WId xid);

protected:
    bool eventFilter(QObject *, QEvent *);

};

#endif // ONPANEL_H
