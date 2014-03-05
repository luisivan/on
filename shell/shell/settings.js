Ext.regModel('Theme', {
  fields: ['name', 'file']
});

var themesPath = 'resources/themes/'

var themesData = [
  {name: 'Sencha', file: themesPath+'sencha-touch.css'},
  {name: 'Cupertino', file: themesPath+'apple.css'},
  {name: 'Mountain View', file: themesPath+'android.css'},
  {name: 'Toronto', file: themesPath+'bb6.css'},
  {name: 'Onyx', file: themesPath+'onyx.css'},
  {name: 'Roookies', file: themesPath+'roookies.css'},
  {name: '3D', file: themesPath+'3d.css'}
]

var themeStore = new Ext.data.Store({
  model: 'Theme',
  data: themesData
});

var themeSettings = new Ext.Panel({
  layout: {
    type: 'vbox',
    align: 'center',
    pack: 'center'
  },
  items: [{
    width: 300,
    xtype: 'list',
    store: themeStore,
    itemTpl: '<div class="theme_choice">{name}</div>',
    onItemTap: function(item, index) {
      recordData = this.store.getAt(index).data;
      var oldTheme = localStorage.getItem('theme');
      var oldCss = document.getElementById(oldTheme);
      oldCss.parentNode.removeChild(oldCss);
      loadCss(recordData.file);
      localStorage.setItem('theme', recordData.file);
    }
  }]
});

/*function openMail(mail) {
  launch('xdg-open mailto:'+mail);
}

var credits = new Ext.Panel({
  id: 'credits',
  floating: true,
  modal: true,
  centered: true,
  width: 600,
  height: 200,
  styleHtmlContent: true,
  scroll: 'vertical',
  html: '<ul>Developers<li><a onclick="openMail(\'luisivan@asturix.com\')">Luis Iván Cuende</a></li> \
             <li><a onclick="openMail(\'joaquim@asturix.com\')">Quim Mocanut</a></li> \
             <li><a onclick="openMail(\'carlos@asturix.com\')">Carlos Hellín</a></li></ul> \
         <ul>Marketing<li><a onclick="openMail(\'xandru@asturix.com\')">Xandru Cancelas</a></li></ul> \
         <ul>Communication<li><a onclick="openMail(\'ricardo@asturix.com\')">Ricardo López</a></li></ul> \
         <ul>Graphics<li><a onclick="openMail(\'marcos@asturix.com\')">Marcos Menéndez</a></li></ul> \
         <ul>Testing<li><a onclick="openMail(\'abiola89@gmail.com\')">Ibrahim Abiola</a></li></ul> \
        ',
  dockedItems: [{
    dock: 'top',
    xtype: 'toolbar',
    title: Locale.credits
  }]
});*/

var aboutSettings = new Ext.Panel({
  layout: {
    type: 'vbox',
    align: 'center',
    pack: 'center'
  },
  id: 'about',
  items: [{
    html: '<img id="logo" src="resources/img/on.png" />'
  }],
  dockedItems: [{
    dock: 'bottom',
    html: '<h1 id="creditsButton" onclick="launch(\'asturix-home credits\')">'+Locale.credits+'</h1><h1><a onclick="launch(\'xdg-open http://asturix.com/os\')">'+Locale.proudOfAsturix+'</a></h1>'
  }]
});

var settingsStore = new Ext.data.Store({
  model: Ext.regModel('', {
    fields: [
      {name:'id', type:'int'},
      {name:'title', type:'string'}
    ]
  }),
  data: [
    {id: 0, title: Locale.themes, icon: 'window'},
    {id: 2, title: Locale.reportBug, icon: 'bug'},
    {id: 1, title: Locale.about, icon: 'info'}
  ]
});

var menuList = new Ext.List({
  cls: 'x-toolbar',
  store: settingsStore,
  iconMask: true,
  iconCls: 'window',
  itemTpl: '<div class="x-button x-button-normal listItemWithIcon"><img src="data:image/gif;base64,R0lGODlhAQABAID/AMDAwAAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==" class="{icon} x-icon-mask"><span>{title}</span></div>',
  onItemTap: function(item, index) {
    recordData = this.store.getAt(index).data;
    if (recordData.id == 2) {
      //launch('xdg-open http://asturix.com/')
      launch('apport-bug on');
      return;
    }
    settingsScreen.setActiveItem(recordData.id, 'fade');
  }
});

var settingsScreen = new Ext.Panel({
  layout: 'card',
  fullscreen: true,
  dockedItems: [{
    dock: 'top',
    xtype: 'toolbar',
    title: Locale.settings,
    items: [{
      xtype: 'button',
      ui: 'back',
      text: '←',
      handler: showApps
    },
    {xtype: 'spacer'}]
  },
  {
    dock: 'left',
    xtype: 'panel',
    layout: 'fit',
    items: [menuList],
    width: 200
  }],
  items:[themeSettings, aboutSettings]
});
