var desktop = new Ext.Panel({
  layout: 'hbox',
  fullscreen: true,
  scroll: 'horizontal',
  dockedItems: [{
    dock: 'top',
    xtype: 'toolbar',
    title: 'On',
    items: [{
      xtype: 'button',
      iconMask: true,
      iconCls: 'add',
      text: Locale.addPanel,
      handler: addNewPanel
    },{
      xtype: 'button',
      iconMask: true,
      iconCls: 'screens',
      text: Locale.addWidgets,
      /*handler: function() {
				var panel = Ext.getCmp(desktop.items.items[desktop.items.length-1].id);
				addNewWidget('postit');
			}*/
      handler: showWidgetsScreen
    },
    {xtype: 'spacer'},
    {
      xtype: 'button',
      iconMask: true,
      iconCls: 'settings',
      text: Locale.settings,
      handler: showSettingsScreen
    }]
  }]
});
