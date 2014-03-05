#include "wallpaper.h"

On* Wallpaper::on = NULL;

void Wallpaper::start() {

    //GConf
    /*gConfWallpaper = gconf_client_get_default();
    gconf_client_add_dir(gConfWallpaper, "/desktop/gnome/background", GCONF_CLIENT_PRELOAD_NONE, NULL);
    gconf_client_notify_add(gConfWallpaper, "/desktop/gnome/background/picture_filename", valueChanged, NULL, NULL, NULL);
    gconf_client_notify_add(gConfWallpaper, "/desktop/gnome/background/primary_color", valueChanged, NULL, NULL, NULL);
    gconf_client_notify_add(gConfWallpaper, "/desktop/gnome/background/secondary_color", valueChanged, NULL, NULL, NULL);
    gconf_client_notify_add(gConfWallpaper, "/desktop/gnome/background/color_shading_type", valueChanged, NULL, NULL, NULL);*/


    dConfWallpaper = dconf_client_new(NULL, dConfValueChanged, NULL, NULL);
    dconf_client_watch(dConfWallpaper, "/org/gnome/desktop/background/", NULL, NULL);

}

/*void Wallpaper::valueChanged(GConfClient *client, guint cnxn_id, GConfEntry *entry, gpointer user_data) {
    Wallpaper::on->setWallpaper();

    GConfValue *val = gconf_entry_get_value(entry);
    QTextStream qcout(stdout);
    qcout << (char*) gconf_value_get_string(val);
}*/

void Wallpaper::dConfValueChanged(DConfClient *client, const gchar *path, const gchar * const *items, gint n_items, const gchar *tag, gpointer user_data) {
    Wallpaper::on->setWallpaper();
}

void Wallpaper::setOn(On *on) {
    Wallpaper::on = on;
}
