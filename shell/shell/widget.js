function Widget(id)
{
  this.id=id;
}

addWidget = function (panel, index, id) {
  panel.add({
    xtype: 'panel',
    style: 'background:#000;',
    cls: 'widget',
    html: '<iframe seamless scrolling="no" allowtransparency="true" frameborder="0" sandbox="allow-forms allow-same-origin allow-scripts" src="widgets/'+id+'/index.html"></iframe>',
    dockedItems: [{
      dock: 'top',
      xtype: 'toolbar',
      title: id,
      cls: 'widgetToolbar',
      items: [
      {
        xtype: 'button',
        iconMask: true,
        iconCls: 'arrow_left',
        handler: function() {
          moveWidgetLeft(this.ownerCt.ownerCt);
        }
      },
      {
        xtype: 'button',
        iconMask: true,
        iconCls: 'arrow_right',
        handler: function() {
          moveWidgetRight(this.ownerCt.ownerCt);
        }
      },
      {xtype: 'spacer'},
      {
        xtype: 'button',
        iconMask: true,
        iconCls: 'delete',
        handler: function() {
          removeWidget(this.ownerCt.ownerCt);
        }
      }]
    }]
  });
  panel.doLayout();
  var panelIndex = getPanelIndex(panel.el.dom);
  saveWidgetStorage(panelIndex, index, id);
}

addNewWidget = function (id) {
  var panels = JSON.parse(localStorage.getItem('panels'));
  var index = panels[panels.length-1].widgets.length;
  var panel = Ext.getCmp(desktop.items.items[desktop.items.length-1].id);
  addWidget(panel, index, id);
}

removeWidget = function (widget) {
  Ext.Msg.confirm(Locale.confirm, '', function(e) {
    if(e == 'yes') {
      widget.hide('pop', 300);
      setTimeout(function() {
        widget.destroy();
      }, 300);
      var panel = widget.ownerCt;
      var panelIndex = getPanelIndex(panel.el.dom);
      var index = getWidgetIndex(widget.el.dom);
      removeWidgetStorage(panelIndex, index);
    }
  });
}

saveWidgetStorage = function (panelIndex, widgetIndex, id) {
  var panels = JSON.parse(localStorage.getItem('panels'));
  var panel = panels[panelIndex];
  var widget = new Widget(id);
  panel.widgets[widgetIndex] = widget;
  if (widget == null) {
    Ext.Msg.alert(Locale.error, '');
    return;
  }
  localStorage.setItem('panels', JSON.stringify(panels));
}

removeWidgetStorage = function (panelIndex, widgetIndex) {
  var panels = JSON.parse(localStorage.getItem('panels'));
  var panel = panels[panelIndex];
  panel.widgets.splice(widgetIndex, 1);
  localStorage.setItem('panels', JSON.stringify(panels));
}

getWidgetIndex = function (widget) {
  var k = 0;
  while(widget.previousSibling) {
    k++;
    widget = widget.previousSibling;
  }
  return k;
}

moveWidgetLeft = function (widget) {
  var panelIndex = getPanelIndex(widget.ownerCt.el.dom);
  console.log(JSON.parse(localStorage.getItem('panels'))[panelIndex-1]);
  if (JSON.parse(localStorage.getItem('panels'))[panelIndex-1] == undefined) {
    Ext.Msg.alert(Locale.youCannot, '');
    return;
  }
  var index = getWidgetIndex(widget.el.dom);
  widget.hide('slide', 300);
  setTimeout(function() {
    widget.destroy();
  }, 300);
  var len = JSON.parse(localStorage.getItem('panels'))[panelIndex-1].widgets.length;
  var id = JSON.parse(localStorage.getItem('panels'))[panelIndex].widgets[index].id;
  removeWidgetStorage(panelIndex, index);
  var panel2Index = getPanelIndex(widget.ownerCt.el.dom);
  var panel = Ext.getCmp(desktop.items.items[panelIndex-1].id);
  addWidget(panel, len, id);
}

moveWidgetRight = function (widget) {
  var panelIndex = getPanelIndex(widget.ownerCt.el.dom);
  if (JSON.parse(localStorage.getItem('panels'))[panelIndex+1] == undefined) {
    Ext.Msg.alert(Locale.youCannot, '');
    return;
  }
  var index = getWidgetIndex(widget.el.dom);
  widget.hide('slide', 300);
  setTimeout(function() {
    widget.destroy();
  }, 300);
  var len = JSON.parse(localStorage.getItem('panels'))[panelIndex+1].widgets.length;
  var id = JSON.parse(localStorage.getItem('panels'))[panelIndex].widgets[index].id;
  removeWidgetStorage(panelIndex, index);
  var panel2Index = getPanelIndex(widget.ownerCt.el.dom);
  var panel = Ext.getCmp(desktop.items.items[panelIndex+1].id);
  addWidget(panel, len, id);
}
