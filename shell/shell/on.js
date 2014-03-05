var container;

showApps = function () {
  container.setActiveItem(0, {type: 'slide', direction: 'left'});
}
showPlaces = function () {
  container.setActiveItem(1, {type: 'slide', direction: 'right'});
}
showSettingsScreen = function () {
  container.setActiveItem(2, 'pop');
}

loadCss = function (file) {
  var css = document.createElement('link');
  css.setAttribute('rel', 'stylesheet');
  css.setAttribute('type', 'text/css');
  css.setAttribute('href', file);
  css.setAttribute('id', file);
  document.getElementsByTagName('head')[0].appendChild(css);
}

loadJs = function (file) {
  var js = document.createElement('script');
  js.setAttribute('type', 'text/javascript');
  js.setAttribute('src', file);
  js.setAttribute('id', file);
  document.getElementsByTagName('head')[0].appendChild(js);
}

launch = function(exec) {
  exec = exec.replace(/%u/i, '');
  exec = exec.replace(/%f/i, '');
  native.exec(exec);
}

Ext.setup({
  onReady: function() {
    
    if (!localStorage.getItem('theme')) {
      localStorage.setItem('theme', 'sencha/resources/css/sencha-touch.css');
    }
    
    loadCss(localStorage.getItem('theme'));
    
    container = new Ext.Panel({
      layout:'card',
      fullscreen: true,
      items: [
        apps,
        places,
        settingsScreen
      ]
    });
    
    container.body.on('mousewheel', function(event, el) {
      var offset, scroller, _results;
      offset = Ext.util.Offset();
      _results = [];
      while (el !== document.body) {
        if (el.className.indexOf("x-scroller-parent") > 0) {
          scroller = Ext.ScrollManager.get(el.firstChild.id);
          if (scroller) {
            scroller.fireEvent('scrollstart', scroller, event);
            offset.y = event.browserEvent.wheelDelta;
            scroller.scrollBy(offset, true);
            scroller.snapToBoundary(true);
            scroller.fireEvent('scrollend', scroller, offset);
            break;
          }
        }
        _results.push(el = el.parentNode);
      }
      return _results;
    });
    
  }
});
