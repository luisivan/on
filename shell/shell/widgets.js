var widgetsScreen = new Ext.Panel({
  layout:'fit',
  fullscreen: true,
  scroll: 'vertical',
  dockedItems: [{
    dock: 'top',
    xtype: 'toolbar',
    title: Locale.addWidgets,
    items: [{
      xtype: 'button',
      ui: 'back',
      text: Locale.back,
      handler: showApps
    },
    {xtype: 'spacer'},
    {
      xtype: 'searchfield',
      placeholder: 'Search'
    }]
  }],
  items: [{
    xtype: 'tabpanel',
    tabBar: {
      dock: 'bottom',
      layout: {
        pack: 'center'
      }
    },
    cardSwitchAnimation: {
      type: 'slide',
      direction: 'bottom'
    },
    defaults: {
      scroll: 'vertical'
    },
    items: [{
      id: 'homeWidgetsList',
      title: Locale.getWidgets,
      iconMask: true,
      iconCls: 'home',
      xtype: 'panel',
      layout: {
        type: 'vbox',
        align: 'center',
        pack: 'center'
      },
      html: 'On Widgets is under heavy development. Stay tuned!'
    },
    {
      id: 'installedWidgetsList',
      title: Locale.installed,
      iconMask: true,
      iconCls: 'download',
      badgeText: '2',
      xtype: 'panel',
      html: 'ff'
    }]
  }],
  listeners: {
    afterRender: function () {
      getInstalledWidgets(function(widgets) {
        widgets.forEach(function(widget) {
          Ext.getCmp('installedWidgetsList').add({
            xtype: 'button',
            cls: 'widgetButton',
            html: widget.id,
            handler: function () {
              addNewWidget(widget.id);
              showDesktop();
            }
          });
        });
      });
    }
  }
});

getInstalledWidgets = function(callback) {
  var widget1 = new Widget('postit');
  var widget2 = new Widget('twitter');
  var widgets = new Array(widget1, widget2);
  callback(widgets);
}

getWidgets = function(amount, callback) {
  
  var widget2 = new Widget('twitter');
  var widgets = new Array(widget1, widget2);
  callback(widgets);
}
