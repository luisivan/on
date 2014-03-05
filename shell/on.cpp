#include "on.h"

On::On(QWidget *parent)
    : QWebView(parent)
{
    setWindow();
    setWebSettings();
    setWallpaper();
    setNative();
    getRDConf();
    loadByLanguage();
    //setUrl(QUrl("/usr/share/on/shell/index.html"));
    connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(setNative()));
    //setWatchers();
    QFuture<void> future = QtConcurrent::run(this, &On::setWatchers);
}

On::~On()
{

}

QString On::username() { return QDir::homePath().remove("/home/"); }

QString On::userDir(int dir) { return QDesktopServices::storageLocation(QDesktopServices::StandardLocation(dir)); }

QString On::userDirName(int dir) { return QDesktopServices::displayName(QDesktopServices::StandardLocation(dir)); }

QString On::lang()
{
    QString lang = QLocale::system().name().split("_").first();
    if (lang == "C") {
        lang = "en";
    }
    return lang;
}

void On::getRDConf() {
    GSettings *rdSettings = g_settings_new("org.asturix.readdesktop.icon");
    defaultIcon = g_variant_get_string(g_settings_get_value(rdSettings, "default-icon"), NULL);
    iconSize = g_variant_get_int32(g_settings_get_value(rdSettings, "size"));
}

// Set the window that is going to be the whole desktop
void On::setWindow()
{
    /*if(getenv("QT_GRAPHICSSYSTEM") == 0) {
        QApplication::setGraphicsSystem("raster");
    }*/

    QDesktopWidget* desktop = QApplication::desktop();
    const QRect screen = desktop->screenGeometry(this);

    static Atom atom = XInternAtom(QX11Info::display(), "_NET_WM_DESKTOP", False);
    ulong data[1];
    data[0] = 0xFFFFFFFF;
    XChangeProperty(QX11Info::display(), winId(), atom, atom, 32, PropModeReplace, (unsigned char *) data, 1);

    //setWindowFlags(Qt::WindowStaysOnBottomHint);
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    //showMaximized();

    setGeometry(0, 24, desktop->width(), desktop->height());

    /*if (QX11Info::isCompositingManagerRunning()) {
        setAttribute(Qt::WA_TranslucentBackground);
    } else {
        setAutoFillBackground(true);
    }*/
    setContextMenuPolicy(Qt::NoContextMenu);
    setFocus();
}

// Set some things of WebKit
void On::setWebSettings()
{
    QSettings cfg(QSettings::IniFormat, QSettings::UserScope, "asturix", "on");
    QString cfgDir = QFileInfo(cfg.fileName()).absolutePath() + "/";
    settings()->setLocalStoragePath(cfgDir);
    settings()->setIconDatabasePath(cfgDir);
    settings()->setOfflineStoragePath(cfgDir);
    settings()->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);
    settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
    settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    settings()->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, true);
    settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
}

// Add this to the Javascript window object so this can be accessed from web code
void On::setNative() {
    page()->mainFrame()->addToJavaScriptWindowObject("native", this);
}

// Set the background
// NOTE: The background is controlled by Qt because setting it as a CSS background is too slow
void On::setWallpaper()
{
    GSettings *backgroundSettings = g_settings_new("org.gnome.desktop.background");
    QString bg = QUrl::fromEncoded(g_variant_get_string(g_settings_get_value(backgroundSettings, "picture-uri"), 0)).toString();
    QPalette p = palette();
    QRect rect = this->rect();
    if (!bg.isEmpty()) {
        bg = bg.remove("file://");
        QPixmap background(QFileInfo(bg).canonicalFilePath());
        if (background.isNull()) {
            background = QPixmap("/usr/share/themes/Asturix/backgrounds/asturix4.jpg");
        }
        QSize size(rect.width(), rect.height());
        QPixmap pixmap(background.scaledToWidth(size.width(), Qt::SmoothTransformation));
        //QPixmap pixmap(background.scaled(size, Qt::KeepAspectRatio));
        //QPixmap pixmap(background.scaled(size));
        p.setBrush(QPalette::Background, pixmap);
        // TODO: Modes. picture-options: zoom, fill, span, scale, center, tile
    } else {
        QColor primColor(g_variant_get_string(g_settings_get_value(backgroundSettings, "primary-color"), 0));
        qDebug(g_variant_get_string(g_settings_get_value(backgroundSettings, "primary-color"), 0));
        QColor secColor(g_variant_get_string(g_settings_get_value(backgroundSettings, "secondary-color"), 0));
        QString mode = QString(g_variant_get_string(g_settings_get_value(backgroundSettings, "color-shading-type"), 0));
        QBrush brush;
        if (mode == "solid") {
            brush = QBrush(primColor);
        } else if (mode == "vertical") {
            QLinearGradient grad(QPointF(0, 0), QPointF(0, rect.height()));
            grad.setColorAt(0, primColor);
            grad.setColorAt(1, secColor);
            brush = QBrush(grad);
        } else {
            QLinearGradient grad(QPointF(0, 0), QPointF(rect.width(), 0));
            grad.setColorAt(0, primColor);
            grad.setColorAt(1, secColor);
            brush = QBrush(grad);
        }
        p.setBrush(QPalette::Background, brush);
    }
    setPalette(p);
    p.setBrush(QPalette::Base, Qt::transparent);
    page()->setPalette(p);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    g_object_unref(backgroundSettings);
}

void On::loadByLanguage() {
    QFile file("/usr/share/on/shell/index.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString contents;
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            contents.append(line);
        }
        setHtml(contents.replace("LANGUAGE", lang()), QUrl("file:///usr/share/on/shell/index.html"));
    }
}

QString On::appsJSON() {
    QFile file("/var/local/asturix/launcherjsonview.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString contents;
        while (!file.atEnd()) {
            QString line = QString::fromUtf8(file.readLine());
            contents.append(line);
        }
        return contents;
    }
    return QString();
}

// Get the base64 of the theme icon
QString On::getThemeIcon(QString icon) {

    if (icon.contains("/") && !QFile::exists(icon)) {
        icon = defaultIcon;
        /*GtkIconInfo *iconInfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(), "application-default-icon", 48, (GtkIconLookupFlags)0);
        icon = gtk_icon_info_get_filename(iconInfo);
        gtk_icon_info_free(iconInfo);*/
    } else if (!icon.contains("/")) {
        GtkIconInfo *iconInfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(), icon.toAscii(), iconSize, (GtkIconLookupFlags)0);
        if (iconInfo == NULL) {
            iconInfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(), "application-default-icon", iconSize, (GtkIconLookupFlags)0);
        }
        icon = gtk_icon_info_get_filename(iconInfo);
        gtk_icon_info_free(iconInfo);
    }
    QPixmap pixmap = QIcon(icon).pixmap(iconSize, iconSize);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pixmap.save(&buffer, "PNG");
    QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
    return iconBase64;
}

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

// Set some watchers so we can monitorize dirs, files etc.
void On::setWatchers() {

    // Set JSON watcher
    Inotify notify;
    InotifyWatch watch("/var/local/asturix/launcherjsonview.json", IN_CLOSE_WRITE);
    notify.Add(watch);
    for (;;) {
        notify.WaitForEvents();
        size_t count = notify.GetEventCount();
        while (count > 0) {
            InotifyEvent event;
            bool got_event = notify.GetEvent(&event);
            if (got_event) {
                QMetaObject::invokeMethod(this,"appsChanged",Qt::QueuedConnection);
            }
            count--;
        }
    }

    DConfClient *dConfWallpaper = dconf_client_new(NULL, iconThemeChanged, NULL, NULL);
    dconf_client_watch(dConfWallpaper, "/org/gnome/desktop/interface/icon-theme", NULL, NULL);
}

void On::iconThemeChanged(DConfClient *client, const gchar *path, const gchar * const *items, gint n_items, const gchar *tag, gpointer user_data) {
    qDebug("Icon theme has been changed");
    QProcess::startDetached("/usr/bin/rdprocessincron IconThemeChanged");
}

// Execute code when /var/local/asturix/launcherjsonview.json file is modified
void On::appsChanged() {
    page()->mainFrame()->evaluateJavaScript("refreshAppsList();");
}

// Execute a command
// NOTE: The buffer of the process is being read and shown
void On::exec(QString exec) {
    /*QProcess proc;
    proc.start("/bin/sh -c \""+exec+"\"");*/
    QProcess::startDetached("/bin/sh -c \""+exec+"\"");
}

/*void On::showNotification(QString title, QString content, QString themeIcon, int time) {
    //QProcess::startDetached("notify-send "+title+" "+content+" -i "+themeIcon);

    NotifyNotification *notif = notify_notification_new(title.toUtf8(), content.toUtf8(), themeIcon.toUtf8());
    notify_notification_set_timeout(notif, time);
    notify_notification_show(notif, NULL);
    g_object_unref(notif);
}*/

QString On::gdkPixbufToBase64(GdkPixbuf *pixbuf) {
    QImage image;
    if (pixbuf == NULL) {
        image = QIcon(defaultIcon).pixmap(iconSize, iconSize).toImage();
        /*GtkIconInfo *iconInfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(), "application-default-icon", iconSize, (GtkIconLookupFlags)0);
        pixbuf = gtk_icon_info_load_icon(iconInfo, NULL);
        gtk_icon_info_free(iconInfo);*/
    } else {
        image = QImage(gdk_pixbuf_get_pixels(pixbuf),
            gdk_pixbuf_get_width(pixbuf),
            gdk_pixbuf_get_height(pixbuf),
            gdk_pixbuf_get_rowstride(pixbuf),
            QImage::Format_ARGB32);
    }
    QImage swappedImage = image.rgbSwapped();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    swappedImage.save(&buffer, "PNG");
    QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
    g_object_unref(pixbuf);
    return iconBase64;
}

#define GFOREACH(item, list) for(GList *__glist = list; __glist && (item = __glist->data, true); __glist = __glist->next)

QStringList On::placesBookmarks() {

    QFile file(QDir::homePath()+"/.gtk-bookmarks");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList bookmarks;
        QString line;
	in.setCodec("UTF-8");
        while (!in.atEnd()) {
            QString line = in.readLine();
            GFile *file = g_file_new_for_uri(line.toLocal8Bit());
            GFileInfo *fileInfo = g_file_query_info(file, "standard::*", 0, NULL, NULL);
            QString icon = g_icon_to_string(g_file_info_get_icon(fileInfo));
            if (icon != NULL) {
                if (icon.contains(" "))
                    icon = icon.split(" ")[2];
                bookmarks.append(line);
                bookmarks.append(QUrl::fromPercentEncoding(QFileInfo(line).fileName().toLocal8Bit()));
                bookmarks.append(getThemeIcon(icon));
            }
        }
        file.close();
        return bookmarks;
    }
    return QStringList();
}

QStringList On::recentFiles() {

    QStringList recent;

    GtkRecentManager *manager = gtk_recent_manager_get_default();
    GList *list = gtk_recent_manager_get_items(manager);
    GtkRecentInfo *item;

    int i = 0;

    GFOREACH(item, list) {
        recent.append(QUrl::fromEncoded(gtk_recent_info_get_uri(item)).toString());
        recent.append(QUrl::fromEncoded(gtk_recent_info_get_display_name(item)).toString());
        recent.append(gdkPixbufToBase64(gtk_recent_info_get_icon(item, iconSize)));
        if (i == 50) {
            gtk_recent_info_unref(item);
            g_list_free(list);
            return recent;
        }
        i++;
    }
    gtk_recent_info_unref(item);
    g_list_free(list);
    //g_object_unref(manager);
    return recent;
}
