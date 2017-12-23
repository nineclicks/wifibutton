window.onload = function() {
  scanWifi(fillWifi);
}

var fillWifi = function(ssids) {
  var elem = document.querySelector('#ssidTable')
  elem.innerHTML = '';
  for (var i in ssids) {
    var ssid = ssids[i];
    elem.innerHTML += '<tr><td>' + ssid.ssid + '</td><td>' + ssid.signal + '%</td></tr>';
  }
}

var scanWifi = function(callback) {
  var r = new XMLHttpRequest();
  r.open('GET', '/scan', true);
  r.onload = function() {
    if (r.status >= 200 && r.status < 400) {
      var ssids = JSON.parse(r.responseText);
      callback(ssids);
    }
  };
  r.send();
}
