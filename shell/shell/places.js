// Set a variable with the places list
var placesMenu = new Array();

var places = new Ext.Panel({
  layout: {
    type: 'vbox',
    align: 'left'
  },
  id: 'places',
  scroll: 'vertical',
  dockedItems: [{
    dock: 'top',
    xtype: 'toolbar',
    title: Locale.places,
    items: [
      new Ext.form.Search({
          listeners: {
            keyup: function(thisField, e) {
              var buttons = places.el.select('div.onButton').elements;
              for (i=0; i < buttons.length; i++) {
                var text = buttons[i].innerText.toLowerCase();
                if (text.indexOf(thisField.getValue().toLowerCase()) == -1) {
                  buttons[i].style.display = 'none';
                } else {
                  buttons[i].style.display = 'inline-block';
                }
              }
            }
          }
    }),
    {xtype: 'spacer'},
    {
      xtype: 'button',
      iconMask: true,
      iconCls: 'screens',
      text: Locale.apps,
      handler: showApps
    },{
      xtype: 'button',
      iconMask: true,
      iconCls: 'settings',
      handler: showSettingsScreen
    }]
  }],
  afterRender: function() {
    /*placesMenu = JSON.parse(localStorage.getItem('placesMenu'));
    if (placesMenu != null) {
      //places.setLoading(true);
      renderPlaces();
      console.log('Ywah');
      //places.setLoading(false);
    } else {*/
      refreshPlacesList();
    //}
  }
});

function refreshPlacesList() {
  
  var bookmarks = native.placesBookmarks();
  for(var i=0; i<bookmarks.length; i = i+3) {
    var file = {
      Name: bookmarks[i+1],
      Icon: 'data:image/png;base64,'+bookmarks[i+2],
      Exec: 'xdg-open '+bookmarks[i]
    }
    placesMenu.push(file);
  }
  
  var recentFiles = native.recentFiles();
  for(var i=0; i<recentFiles.length; i = i+3) {
    var file = {
      Name: recentFiles[i+1],
      Icon: 'data:image/png;base64,'+recentFiles[i+2],
      Exec: 'xdg-open '+recentFiles[i]
    }
    placesMenu.push(file);
  }
  renderPlaces();
}

function renderPlaces() {
  
  var placesContainer = places.getEl().dom.children[0].children[0];
  // If contains a toolbar all the DOM it's loaded
  if (placesContainer.className.indexOf('toolbar') != -1) {
    placesContainer = places.getEl().dom.children[1].children[0];
    while (placesContainer.childNodes.length >= 1 )
    {
      placesContainer.removeChild(placesContainer.firstChild);       
    } 
  };
  
  for(var i=0; i < placesMenu.length; i++) {
    var button = document.createElement('div');
    button.setAttribute('class','onButton');
    button.setAttribute('onclick','launch("'+placesMenu[i].Exec+'")');
    button.setAttribute('title', placesMenu[i].Name);
    button.innerHTML = '<img src="'+placesMenu[i].Icon+'"/><div>'+placesMenu[i].Name+'</div>';
    placesContainer.appendChild(button);
  }
  places.doLayout();
}
