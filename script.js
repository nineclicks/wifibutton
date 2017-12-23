var ssidList = null;

window.onload = function() {
  scanWifi(fillWifi);
}

var fillWifi = function(ssids) {
  var elem = document.querySelector('#ssidTable');
  elem.innerHTML = '<thead><tr><td>SSID</td><td>Signal</td><td>Secured</td></tr></thead>';
  for (var i in ssids) {
    var ssid = ssids[i];
    var row = '<tr><td>' + ssid.ssid + '</td><td>' + ssid.signal + '%</td>';
    row += '<td>' + ((ssid.secured == 1)? '*' : '') + '</td>';
    row += '</tr>';
    elem.innerHTML += row;
  }
}

var scanWifi = function(callback) {
  var r = new XMLHttpRequest();
  r.open('GET', '/scan', true);
  r.onload = function() {
    if (r.status >= 200 && r.status < 400) {
      ssidList = JSON.parse(r.responseText);
      callback(ssidList);
    }
  };
  r.send();
}
