#ifndef ON_H
#define ON_H

#undef signals

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

//#include <libnotify/notify.h>

#include <QtGui/QWidget>
#include <QWebView>
#include <QWebFrame>
#include <QDir>
#include <QIcon>
#include <QSettings>
#include <QProcess>
#include <QFileSystemWatcher>
#include <QPalette>
#include <QLinearGradient>
#include <QDesktopWidget>
#include <QApplication>
#include <QDesktopServices>
#include <QLocale>
#include <QX11Info>
#include <QtConcurrentRun>

#include <QBuffer>

#include <QCleanlooksStyle>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <inotify/inotify-cxx.h>
#include <dconf/dconf-client.h>

class On : public QWebView
{
    Q_OBJECT

public:
    On(QWidget *parent = 0);
    ~On();
    Q_INVOKABLE QString username();
    Q_INVOKABLE QString userDir(int dir);
    Q_INVOKABLE QString userDirName(int dir);
    Q_INVOKABLE QString lang();
    Q_INVOKABLE QString appsJSON();
    //Q_INVOKABLE void showNotification(QString title, QString content, QString themedIcon, int time);
    Q_INVOKABLE QString getThemeIcon(QString icon);
    QString gdkPixbufToBase64(GdkPixbuf *pixbuf);
    Q_INVOKABLE QStringList placesBookmarks();
    Q_INVOKABLE QStringList recentFiles();
    QString defaultIcon;
    int iconSize;

public slots:
    void getRDConf();
    void setWindow();
    void setWebSettings();
    void setNative();
    void setWallpaper();
    void loadByLanguage();
    void setWatchers();
    static void iconThemeChanged(DConfClient *client, const gchar *path, const gchar * const *items, gint n_items, const gchar *tag, gpointer user_data);
    void appsChanged();
    void exec(QString exec);

};

#endif // ON_H
