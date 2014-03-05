self.onmessage = function(event) {
  var appsJSON = JSON.parse(event.data).Applications;
  var appsMenu = new Array();
  for(var i=0; i < appsJSON.length; i++) {
    if (appsJSON[i].NoDisplay == true) {
      continue;
    } else if (appsJSON[i].Type != 'Application') {
      continue;
    }/* else if (appsJSON[i].Categories != undefined) {
      if (appsJSON[i].Categories.indexOf('Settings') != -1) {
        continue;
      }
    }*/
    appsMenu.push(appsJSON[i]);
  }
  self.postMessage(appsMenu);
};
