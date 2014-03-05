#include "onpanel.h"

OnPanel::OnPanel(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("On");

    setPanelSpace();

    hlayout = new QHBoxLayout();

    setLayout(hlayout);
    layout()->setMargin(0);
    layout()->setSpacing(0);

    show();

    setWindowButton();
    setWindowList();

    connect(&BamfMatcher::get_default(), SIGNAL(ActiveApplicationChanged(BamfApplication*,BamfApplication*)), SLOT(windowUpdate()));
    /* Work around a bug in BAMF: the ActiveWindowChanged signal is not emitted
       for some windows that open maximized. This is for example the case of the
       LibreOffice startcenter.*/
    connect(&BamfMatcher::get_default(), SIGNAL(ViewOpened(BamfView*)), SLOT(windowUpdate()));
    /* Work around a BAMF bug: it does not emit ActiveWindowChanged when the
       last window is closed. Should be removed when this bug is fixed. */
    connect(&BamfMatcher::get_default(), SIGNAL(ViewClosed(BamfView*)), SLOT(windowUpdate()));

    connect(&BamfMatcher::get_default(), SIGNAL(ViewOpened(BamfView*)), SLOT(addWindow(BamfView*)));
    connect(&BamfMatcher::get_default(), SIGNAL(ViewClosed(BamfView*)), SLOT(removeWindow(BamfView*)));

    layout()->addWidget(new LegacyTrayApplet);
    layout()->addWidget(new IndicatorApplet);
}

OnPanel::~OnPanel()
{

}

void OnPanel::setPanelSpace()
{
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    setAttribute(Qt::WA_X11DoNotAcceptFocus);
    //setWindowFlags(Qt::WindowStaysOnBottomHint);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    QDesktopWidget* desktop = QApplication::desktop();
    const QRect screen = desktop->screenGeometry(this);
    int width = desktop->width();

    QRect rect = QRect(0, 0, width, 24);
    setGeometry(rect);
    setFixedHeight(24);

    ulong struts[12] = {};
    struts[2] = 24;
    struts[8] = screen.left();
    struts[9] = screen.x() + screen.width();

    static Atom strutAtom = XInternAtom(QX11Info::display(), "_NET_WM_STRUT_PARTIAL", False);
    XChangeProperty(QX11Info::display(), winId(), strutAtom, XA_CARDINAL, 32, PropModeAppend, (unsigned char *) struts, 12);

    static Atom desktopAtom = XInternAtom(QX11Info::display(), "_NET_WM_DESKTOP", False);
    ulong data[1];
    data[0] = 0xFFFFFFFF;
    XChangeProperty(QX11Info::display(), winId(), desktopAtom, desktopAtom, 32, PropModeReplace, (unsigned char *) data, 1);

    wnck_set_client_type(WNCK_CLIENT_TYPE_PAGER);

    setStyleSheet("QToolButton { border: none;}");
    setMouseTracking(true);
    qApp->installEventFilter(this);
    setAttribute(Qt::WA_NoMousePropagation, true);
}

void OnPanel::setWindowButton()
{
    // Set up the button
    windowButton = new QToolButton();
    windowButton->setIconSize(QSize(48, 24));
    windowButton->setAutoRaise(true);
    windowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    windowButton->setArrowType(Qt::NoArrow);
    windowButton->setPopupMode(QToolButton::InstantPopup);
    windowButton->setText("On");
    windowButton->setFixedSize(150, 24);

    QFont font;
    font.setBold(true);
    windowButton->setFont(font);

    layout()->addWidget(windowButton);
    connect(windowButton, SIGNAL(clicked()), SLOT(showDesktop()));

    // Separator
    QFrame *separator = new QFrame();
    separator->setFrameStyle(QFrame::VLine | QFrame::Sunken);
    layout()->addWidget(separator);

    windowUpdate();
}

void OnPanel::windowUpdate()
{
    BamfApplication* app = BamfMatcher::get_default().active_application();
    if (app != NULL && app->user_visible()) {
        updateWindowButton(app->name(), getIcon(app->icon()));
    } else {
        updateWindowButton("On", getIcon("/usr/share/on/shell/resources/img/icon.png"));
    }
}

void OnPanel::updateWindowButton(QString text, QIcon icon) {
    windowButton->setIcon(icon);
    QFontMetrics fm(windowButton->font());
    if(windowButton->width()-58 < fm.width(text)) {
        windowButton->setText(windowButton->fontMetrics().elidedText(text, Qt::ElideRight, windowButton->width()-58, Qt::TextWrapAnywhere));
    } else {
        windowButton->setText(text);
    }
}

QIcon OnPanel::getIcon(QString icon)
{
    if (icon.contains("/") && !QFile::exists(icon)) {
        GtkIconInfo *iconInfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(), "application-default-icon", 48, (GtkIconLookupFlags)0);
        icon = gtk_icon_info_get_filename(iconInfo);
        gtk_icon_info_free(iconInfo);
    } else if (icon.contains("/") == false) {
        GtkIconInfo *iconInfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(), icon.toAscii(), 48, (GtkIconLookupFlags)0);
        if (iconInfo == NULL) {
            iconInfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(), "application-default-icon", 48, (GtkIconLookupFlags)0);
        }
        icon = gtk_icon_info_get_filename(iconInfo);
        gtk_icon_info_free(iconInfo);
    }
    QPixmap pixmap = QIcon(icon).pixmap(48, 48).copy(0, 12, 48, 24);
    return QIcon(pixmap);
}

void OnPanel::setWindowList()
{
    // Set up the widget
    //windowList = new QWidget();
    windowList = new QScrollArea();
    windowList->setFrameShape(QFrame::NoFrame);
    windowList->setContentsMargins(0,0,0,0);
    windowList->setLayout(new QHBoxLayout());
    windowList->layout()->setAlignment(Qt::AlignLeft);
    windowList->layout()->setMargin(0);
    windowList->layout()->setContentsMargins(10, 0, 10, 0);
    windowList->layout()->setSpacing(0);
    layout()->addWidget(windowList);

    // Get running apps
    BamfMatcher& matcher = BamfMatcher::get_default();
    QScopedPointer<BamfApplicationList> applications(matcher.running_applications());
    BamfApplication* application;
    for(int i=0; i < applications->size(); i++) {
        // Get windows of this app
        application = applications->at(i);
        QScopedPointer<BamfWindowList> windows(application->windows());
        for (int j=0; j < windows->size(); j++) {
            BamfWindow *window = windows->at(j);
            //BamfView* window = qobject_cast<BamfView*>(windows->at(j));
            addWindow(window);
        }
    }
}

void OnPanel::addWindow(BamfView *view)
{
    BamfWindow* window = qobject_cast<BamfWindow*>(view);
    if (window == NULL || window->transient()) {
        return;
    }
    BamfApplication* app = BamfMatcher::get_default().application_for_xid(window->xid());
    if (app != NULL) {
        QToolButton *button = new QToolButton();
        button->setIconSize(QSize(48, 24));
        button->setAutoRaise(true);
        button->setToolButtonStyle(Qt::ToolButtonIconOnly);
        button->setArrowType(Qt::NoArrow);
        button->setPopupMode(QToolButton::DelayedPopup);
        button->setText(app->name());
        button->setToolTip(window->name());
        button->setIcon(getIcon(app->icon()));
        windowList->layout()->addWidget(button);    
        /*QMenu *buttonMenu = new QMenu();
        button->setMenu(buttonMenu);*/
        connect(button, SIGNAL(clicked()), SLOT(windowTriggered()));
        button->setObjectName(window->path());
    }
}

void OnPanel::removeWindow(BamfView *view)
{
    BamfWindow *window = qobject_cast<BamfWindow *>(view);
    if (window != NULL) {
        QString path = view->path();
        QToolButton *button = windowList->findChild<QToolButton*>(view->path());
        button->deleteLater();
    }
}

void OnPanel::windowTriggered()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    BamfView *view = BamfFactory::get_default().view_for_path(button->objectName());
    if (view->user_visible()) {
        BamfWindow *bamfWindow = qobject_cast<BamfWindow *>(view);
        WnckWindow *window = wnck_window_get(bamfWindow->xid());
        if (window == NULL) {
            wnck_screen_force_update(wnck_screen_get_default());
            if (window == NULL) {
                return;
            }
        }
        if (!bamfWindow->active()) {
            wnck_window_activate(window, CurrentTime);
        } else {

            if (!wnck_window_is_minimized(window)) {
                wnck_window_minimize(window);
            } else {
                wnck_window_unminimize(window, CurrentTime);
            }
        }
    }
}

void OnPanel::showDesktop() {
    if (!wnck_screen_get_showing_desktop(wnck_screen_get_default())) {
        wnck_screen_toggle_showing_desktop(wnck_screen_get_default(), true);
    } else {
        wnck_screen_toggle_showing_desktop(wnck_screen_get_default(), false);
    }
}

/*QPixmap OnPanel::getWindowScreen(WId xid)
{
    QPixmap screenshot = QPixmap::grabWindow(xid);
    return screenshot;
}

void OnPanel::embedWindow(WId xid)
{
    QX11EmbedContainer *container = new QX11EmbedContainer(0);
    sleep(3);
    container->embedClient(xid);
    container->show();
    container->setWindowTitle("Container");
}*/

bool OnPanel::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Enter)
    {
        if (QToolButton *button = qobject_cast<QToolButton *>(obj)) {
            if (button->width() != 150) {
                updateWindowButton(button->toolTip(), button->icon());
            }
        }
    } else if (event->type() == QEvent::Leave) {
        windowUpdate();
    }
    return false;
}
