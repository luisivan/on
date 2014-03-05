install: 
	mkdir -p $(DESTDIR)/usr/share/xsessions
	cp -f on.desktop $(DESTDIR)/usr/share/xsessions
	mkdir -p $(DESTDIR)/usr/share/applications
	cp -f session/onshell.desktop $(DESTDIR)/usr/share/applications
	cp -f session/onpanel.desktop $(DESTDIR)/usr/share/applications
	mkdir -p $(DESTDIR)/usr/share/gnome-session/sessions
	cp -f session/on.session $(DESTDIR)/usr/share/gnome-session/sessions
	cp -f session/on-2d.session $(DESTDIR)/usr/share/gnome-session/sessions
	mkdir -p $(DESTDIR)/usr/share/apport/package-hooks
	cp -f apport/source_on.py $(DESTDIR)/usr/share/apport/package-hooks
	mkdir -p $(DESTDIR)/etc/apport/crashdb.conf.d
	cp -f apport/on-crashdb.conf $(DESTDIR)/etc/apport/crashdb.conf.d
