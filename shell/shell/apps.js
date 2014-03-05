// Set a variable with the apps list so we haven't parse the JSON string each time
var appsMenu;

var apps = new Ext.Panel({
  layout: {
    type: 'vbox',
    align: 'left'
  },
  id: 'apps',
  scroll: 'vertical',
  dockedItems: [{
    dock: 'top',
    xtype: 'toolbar',
    title: Locale.apps,
    items: [
      new Ext.form.Search({
          listeners: {
            keyup: function(thisField, e) {
              var buttons = apps.el.select('div.onButton').elements;
              for (i=0; i < buttons.length; i++) {
                var text = buttons[i].innerText.toLowerCase();
                if (text.indexOf(thisField.getValue().toLowerCase()) == -1) {
                  buttons[i].style.display = 'none';
                } else {
                  buttons[i].style.display = 'inline-block';
                }
              }
              apps.doLayout();
            }
          }
    }),
    {xtype: 'spacer'},
    {
      xtype: 'button',
      iconMask: true,
      iconCls: 'folder',
      text: Locale.places,
      handler: showPlaces
    },{
      xtype: 'button',
      iconMask: true,
      iconCls: 'settings',
      handler: showSettingsScreen
    }]
  }],
  afterRender: function() {
    
    appsMenu = JSON.parse(localStorage.getItem('appsMenu'));
    if (appsMenu != null) {
      //apps.setLoading(true);
      renderApps();
      //apps.setLoading(false);
    } else {
      refreshAppsList();
    }
    renderFavs();
  }
});

function refreshAppsList()
{
  showApps();
  apps.setLoading(true);
  var worker = new Worker("appsWorker.js");
  worker.postMessage(native.appsJSON());
  worker.onmessage = function(event) {
    appsMenu = event.data;
    localStorage.setItem('appsMenu', JSON.stringify(event.data));
    worker.terminate();
    renderApps();
    apps.setLoading(false);
  }

}

function renderApps()
{
  var appsContainer = apps.getEl().dom.children[0].children[0];
  // If contains a toolbar all the DOM it's loaded
  if (appsContainer.className.indexOf('toolbar') != -1) {
    appsContainer = apps.getEl().dom.children[1].children[0];
    while (appsContainer.childNodes.length >= 1 )
    {
      appsContainer.removeChild(appsContainer.firstChild);       
    } 
  }
  
  for(var i=0; i < appsMenu.length; i++) {
    var button = document.createElement('div');
    button.setAttribute('id', appsMenu[i].Id);
    button.setAttribute('class', 'onButton');
    button.innerHTML = '<img src="data:image/png;base64,'+native.getThemeIcon(appsMenu[i].Icon)+'"/><div>'+appsMenu[i].Name+'</div>';
    button.setAttribute('exec', appsMenu[i].Exec);
    button.setAttribute('title', appsMenu[i].Name);
    button.onmousedown = function (e) {
      if (e.which != 3) {
        launch(this.getAttribute('exec'));
      } else {
        addFav(this);
      }
    }
    appsContainer.appendChild(button);
  }
  apps.doLayout();
}

var favsDiv;

function renderFavs()
{
  var appsContainer = apps.getEl().dom.children[0].children[0];
  favsDiv = document.createElement('div');
  favsDiv.setAttribute('id', 'favsDiv');
  document.body.appendChild(favsDiv);
  var favs = JSON.parse(localStorage.getItem('favs'));
  if (favs == null) {
    favs = new Array();
  }
  for(var i=0; i<favs.length; i++) {
    var fav = favs[i];
    var favButton = document.createElement('div');
    favButton.setAttribute('class', 'onButton');
    favButton.innerHTML = '<img src="'+fav.Icon+'"/>';
    favButton.setAttribute('exec', fav.Exec);
    favButton.onmousedown = function (e) {
      if (e.which != 3) {
        launch(this.getAttribute('exec'));
      } else {
        for (var k=0,e=this; e = e.previousSibling; ++k);
        favsDiv.removeChild(this);
        var favs = JSON.parse(localStorage.getItem('favs'));
        favs.splice(k, 1);
        localStorage.setItem('favs', JSON.stringify(favs));
      }
    }
    favsDiv.appendChild(favButton);
  }
}

function addFav(item)
{
  var entry = item.cloneNode(true);
  entry.removeChild(entry.children[1]);
  entry.onmousedown = function(e) {
    if (e.which != 3) {
      launch(this.getAttribute('exec'));
    } else {
      for (var k=0,e=this; e = e.previousSibling; ++k);
      favsDiv.removeChild(this);
      var favs = JSON.parse(localStorage.getItem('favs'));
      favs.splice(k, 1);
      localStorage.setItem('favs', JSON.stringify(favs));
    }
  }
  favsDiv.appendChild(entry);
  var favs = JSON.parse(localStorage.getItem('favs'));
  if (favs == null) {
    favs = new Array();
  }
  favs.push({
    Icon: item.children[0].src,
    Exec: item.getAttribute('exec')
  });
  localStorage.setItem('favs', JSON.stringify(favs));
}
