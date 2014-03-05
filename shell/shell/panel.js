function Panel(title, widgets)
{
  this.title=title;
  this.widgets=widgets;
}

addPanel = function (id, title, widgets) {
  desktop.add({
    scroll: 'vertical',
    title: title,
    xtype: 'panel',
    width: 300,
    height: '100%',
    cls: 'panel',
    dockedItems: [{
      dock: 'top',
      xtype: 'toolbar',
      title: title,
      cls: 'panelToolbar',
      items: [
      {xtype: 'spacer'},
      {
        xtype: 'button',
        iconMask: true,
        iconCls: 'compose',
        handler: function() {
          renamePanel(this.ownerCt.ownerCt);
        }
      },
      {
        xtype: 'button',
        iconMask: true,
        iconCls: 'delete',
        handler: function() {
          removePanel(this.ownerCt.ownerCt);
        }
      }]
    }]
  });
  desktop.doLayout();
  var panel = Ext.getCmp(desktop.items.items[desktop.items.length-1].id);
  new Ext.util.Sortable(panel.id, {
    itemSelector: 'div.x-panel',
    direction: 'vertical',
    scroll: true,
    constrain: true,
    handleSelector: '.widgetToolbar',
    listeners: {
      sortstart: function (sortable, event) {
        this.panelY = event.pageY;
      },
      sortchange: function (sortable, element, index1) {
        var index2;
        if (event.pageY<this.panelY) {
          index2 = index1+1;
        } else {
          index2 = index1-1;
        }
        var panelIndex = getPanelIndex(panel.el.dom);
        var panels = JSON.parse(localStorage.getItem('panels'));
        var widgets = panels[panelIndex].widgets;
        var widget1Storage = widgets[index2];
        var widget2Storage = widgets[index1];
        widgets[index1] = widget1Storage;
        widgets[index2] = widget2Storage;
        panels[panelIndex].widgets = widgets;
        localStorage.setItem('panels', JSON.stringify(panels));
      }
    }
  });
  savePanelStorage(id, title, widgets);
  for (var i=0; i<widgets.length; i++) {
    addWidget(panel, i, widgets[i].id);
  }
}

restorePanels = function () {
  var panels = JSON.parse(localStorage.getItem('panels'));
  for (var i=0; i<panels.length; i++) {
    var widgets = panels[i].widgets;
    addPanel(i, panels[i].title, widgets);
  }
} 

addNewPanel = function (id) {
  Ext.Msg.prompt(Locale.newPanel, '',  function(btn, text) {
    if (btn == 'ok') {
      if (text == '')
        return;
      var id = JSON.parse(localStorage.getItem('panels')).length;
      addPanel(id, text, new Array());
    }
  });
}

renamePanel = function (panel) {
  Ext.Msg.prompt(Locale.renamePanel, '',  function(btn, text) {
    if (btn == 'ok') {
      if (text == '')
        return;
      var panelTitle = panel.dockedItems.items[0].titleEl.dom;
      panelTitle.innerHTML = text;
      var panels = JSON.parse(localStorage.getItem('panels'));
      var index = getPanelIndex(panel.el.dom);
      panels[index].title = text;
      localStorage.setItem('panels', JSON.stringify(panels));
    }
  });
}

removePanel = function (panel) {
  Ext.Msg.confirm(Locale.confirm, '', function(e) {
    if(e == 'yes') {
      panel.hide('pop', 300);
      setTimeout(function() {
        panel.destroy();
      }, 300);
      var index = getPanelIndex(panel.el.dom);
      removePanelStorage(index);
    }
  });
}

savePanelStorage = function (id, title, widgets) {
  var panels = JSON.parse(localStorage.getItem('panels'));
  var panel = new Panel(title, widgets);
  panels[id] = panel;
  if (panel == null) {
    Ext.Msg.alert(Locale.error, '');
    return;
  }
  localStorage.setItem('panels', JSON.stringify(panels));
}

removePanelStorage = function (id) {
  var panels = JSON.parse(localStorage.getItem('panels'));
  panels.splice(id, 1);
  localStorage.setItem('panels', JSON.stringify(panels));
}

getPanelIndex = function (panel) {
  var k = 0;
  while(panel.previousSibling) {
    k++;
    panel = panel.previousSibling;
  }
  return k;
}

Ext.setup({onReady: function () {
  new Ext.util.Sortable(desktop.id, {
    itemSelector: '.panel',
    direction: 'horizontal',
    scroll: true,
    constrain: true,
    handleSelector: '.panelToolbar',
    listeners: {
      sortstart: function (sortable, event) {
        this.panelX = event.pageX;
      },
      sortchange: function (sortable, element, index1) {
        var index2;
        if (event.pageX<this.panelX) {
          index2 = index1+1;
        } else {
          index2 = index1-1;
        }
        var panels = JSON.parse(localStorage.getItem('panels'));
        var panel1Storage = panels[index2];
        var panel2Storage = panels[index1];
        panels[index1] = panel1Storage;
        panels[index2] = panel2Storage;
        localStorage.setItem('panels', JSON.stringify(panels));
      }
    }
  });
  }
});
