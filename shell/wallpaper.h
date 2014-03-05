#ifndef WWALLPAPER_H
#define WWALLPAPER_H

#include <QtCore/QCoreApplication>

#undef signals

#include <dconf/dconf-client.h>
//#include <gconf/gconf-client.h>

#include "on.h"

class Wallpaper : public QObject
{
    Q_OBJECT
public:
    //static void valueChanged(GConfClient *client, guint cnxn_id, GConfEntry *entry, gpointer user_data);
    static void dConfValueChanged(DConfClient *client, const gchar *path, const gchar * const *items, gint n_items, const gchar *tag, gpointer user_data);
    static On *on;
    void setOn(On *on);
    void start();

private:
    //GConfClient *gConfWallpaper;
    DConfClient *dConfWallpaper;
};

#endif // WALLPAPER_H
