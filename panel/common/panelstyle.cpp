    /*
 * This file is part of unity-2d
 *
 * Copyright 2011 Canonical Ltd.
 *
 * Authors:
 * - Aurélien Gâteau <aurelien.gateau@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// Self
#include "panelstyle.h"

// libunity-2d
#include <common/cairoutils.h>
//#include <debug_p.h>
#include <common/gconnector.h>
#include <common/gscopedpointer.h>

// Qt
#include <QApplication>
#include <QPalette>
#include <QStyle>
#include <QFont>

#include <QDebug>

#undef signals

// GTK
#include <gtk/gtk.h>

class PanelStylePrivate
{
public:
    PanelStyle* q;
    GObjectScopedPointer<GtkStyleContext> m_styleContext;
    GConnector m_gConnector;

    QString m_themeName;

    static void onThemeChanged(GObject*, GParamSpec*, gpointer data)
    {
        qApp->setStyleSheet("");

        PanelStylePrivate* priv = reinterpret_cast<PanelStylePrivate*>(data);
        priv->updatePalette();

        qApp->setStyleSheet("QToolButton, QScrollArea { border: none;}");
    }

    void updatePalette()
    {
        gchar* themeName = 0;
        g_object_get(gtk_settings_get_default(), "gtk-theme-name", &themeName, NULL);
        m_themeName = QString::fromUtf8(themeName);
        g_free(themeName);

        GtkStyleContext* context = m_styleContext.data();
        gtk_style_context_invalidate(context);

        // Without this line, it seems the GtkStyleContext is not correctly
        // initialized and we get some uninitialized pixels in the background
        // brush.
        gtk_style_context_get(context, GTK_STATE_FLAG_NORMAL, NULL);

        QPalette pal;
        pal.setBrush(QPalette::Window, generateBackgroundBrush());


        GtkWidgetPath* widgetPath = gtk_widget_path_new ();
        gtk_widget_path_append_type(widgetPath, GTK_TYPE_WINDOW);
        gtk_widget_path_iter_set_name(widgetPath, -1 , "UnityPanelWidget");

        GtkStyleContext *mcontext = gtk_style_context_new();
        gtk_style_context_set_path(mcontext, widgetPath);
        gtk_style_context_add_class(mcontext, "gnome-panel-menu-bar");
        gtk_style_context_add_class(mcontext, "unity-panel");


        GdkRGBA color;
        // os_chrome_fg_color selected_fg_color selected_bg_color fg_color
        if(gtk_style_context_lookup_color(context, "os_chrome_fg_color", &color)) {
            QColor fontColor = QColor(color.red * 255, color.green * 255, color.blue * 255);
            pal.setColor(QPalette::ButtonText, fontColor);
        }
        //gdk_rgba_free(color);

        QApplication::setPalette(pal);
    }

    QBrush generateBackgroundBrush()
    {
        QImage image(100, 24, QImage::Format_ARGB32_Premultiplied); // FIXME: Hardcoded
        image.fill(Qt::transparent);
        CairoUtils::SurfacePointer surface(CairoUtils::createSurfaceForQImage(&image));
        CairoUtils::Pointer cr(cairo_create(surface.data()));
        gtk_render_background(m_styleContext.data(), cr.data(), 0, 0, image.width(), image.height());
        gtk_render_frame(m_styleContext.data(), cr.data(), 0, 0, image.width(), image.height());
        return QBrush(image);
    }

    static void onFontChanged(GObject*, GParamSpec*, gpointer data)
    {
        qApp->setStyleSheet("");

        PanelStylePrivate* priv = reinterpret_cast<PanelStylePrivate*>(data);
        priv->updateFont();

        qApp->setStyleSheet("QToolButton, QScrollArea { border: none;}");
    }

    void updateFont()
    {
        gchar* fontName = 0;
        g_object_get(gtk_settings_get_default(), "gtk-font-name", &fontName, NULL);
        GScopedPointer<PangoFontDescription, pango_font_description_free> fontDescription(
            pango_font_description_from_string(fontName)
            );
        g_free(fontName);

        int size = pango_font_description_get_size(fontDescription.data());

        QFont font = QFont(
            pango_font_description_get_family(fontDescription.data()),
            size / PANGO_SCALE
            );
        QApplication::setFont(font);
    }
};

PanelStyle::PanelStyle(QObject* parent)
: d(new PanelStylePrivate)
{
    d->q = this;
    d->m_styleContext.reset(gtk_style_context_new());

    GtkWidgetPath* widgetPath = gtk_widget_path_new ();
    gtk_widget_path_append_type(widgetPath, GTK_TYPE_WINDOW);
    gtk_widget_path_iter_set_name(widgetPath, -1 , "UnityPanelWidget");

    gtk_style_context_set_path(d->m_styleContext.data(), widgetPath);
    gtk_style_context_add_class(d->m_styleContext.data(), "gnome-panel-menu-bar");
    gtk_style_context_add_class(d->m_styleContext.data(), "unity-panel");

    gtk_widget_path_free (widgetPath);

    d->m_gConnector.connect(gtk_settings_get_default(), "notify::gtk-theme-name",
        G_CALLBACK(PanelStylePrivate::onThemeChanged), d);

    d->updatePalette();

    d->m_gConnector.connect(gtk_settings_get_default(), "notify::gtk-font-name",
        G_CALLBACK(PanelStylePrivate::onFontChanged), d);

    d->updateFont();
}

PanelStyle::~PanelStyle()
{
    delete d;
}

PanelStyle* PanelStyle::instance()
{
    static PanelStyle style;
    return &style;
}

GtkStyleContext* PanelStyle::styleContext() const
{
    return d->m_styleContext.data();
}
