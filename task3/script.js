var distanceMatrix_text;
var map;
var geocoder;
var latitude;
var longitude;
var coords;
var distanceMatrix;
var i, j;
var countRoute = 4;
var node_data_text = "";
var weight = new Array(40);
var markers = [];
// Initialize and add the map
function initMap() {
  var hanoi;
  hanoi = { lat: 21.0278, lng: 105.8342 };
  map = new google.maps.Map(document.getElementById("map"), {
    zoom: 13,
    center: hanoi,
  });
  geocoder = new google.maps.Geocoder();

  toggleloader(1);
  toggleloader(2);
}

var address = new Array(40);
address[0] = "DEPOT";

const geocodeLatLng = (geocoder, coord, i) => {
  return sleep(3000).then((v) => {
    geocoder.geocode({ location: coord }, (results, status) => {
      if (status === "OK") {
        if (results[0]) {
          address[i] = results[0].formatted_address;
        } else {
          window.alert("No results found");
        }
      } else {
        window.alert("Geocoder failed due to: " + status);
      }
    });
  });
};

async function getPlacesAndDisplay() {
  toggleloader(1);
  latitude = new Array(39);
  longitude = new Array(39);

  //Random coordinates
  for (i = 0; i < 39; i++) {
    latitude[i] = parseFloat((Math.random() * 0.068 + 20.975).toFixed(4));
    longitude[i] = parseFloat((Math.random() * 0.094 + 105.77).toFixed(4));
  }

  coords = new Array(40);
  //Coordinate and weight of depot
  coords[0] = { lat: 21.002666, lng: 105.833105 };
  weight[0] = 0;

  for (i = 1; i <= 39; i++) {
    coords[i] = { lat: latitude[i - 1], lng: longitude[i - 1] };
    weight[i] = parseFloat((Math.random() * 1.5 + 0.5).toFixed(2));
  }
  addMarkerDepot(coords[0], weight[0]);
  // Display all places
  for (i = 1; i < 40; i++) {
    addMarker(coords[i], weight[i]);
    // address[i] = "abcd";
    const s = await geocodeLatLng(geocoder, coords[i], i);
  }
  toggleloader(1);

  function addMarker(coords, weight) {
    var contentString = "<h1>" + weight + " kg</h1>";

    var infowindow = new google.maps.InfoWindow({
      content: contentString,
    });

    var marker = new google.maps.Marker({
      position: coords,
      map: map,
      // title: weight.toString() + " kg",
    });
    markers.push(marker);

    marker.addListener("click", () => {
      infowindow.open(map, marker);
    });
  }

  function addMarkerDepot(coords, weight) {
    var contentString = "<h1>" + weight + " kg</h1>";

    var infowindow = new google.maps.InfoWindow({
      content: contentString,
    });

    var marker = new google.maps.Marker({
      position: coords,
      map: map,
      icon: "http://maps.google.com/mapfiles/kml/shapes/ranger_station.png",
    });
    markers.push(markers);

    marker.addListener("click", () => {
      infowindow.open(map, marker);
    });
  }
}

var route = new Array();
// route[0] = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
// route[1] = [0, 10, 11, 12, 13, 14, 14, 15, 16, 17, 18, 19, 0];
// route[2] = [0, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 0];
// route[3] = [0, 31, 32, 33, 34, 35, 36, 37, 38, 39, 0];

// Color for each route
var color = ["#b5183a", "#114b78", "#109445", "#c98402", "#d95b9c"];

var routeColor = ["red", "blue", "green", "yellow"];
var finalmarkers = [];
function addFinalMarker(coords, weight, order, color) {
  var contentString = "<h1>" + weight + " kg</h1>";

  var infowindow = new google.maps.InfoWindow({
    content: contentString,
  });

  var marker = new google.maps.Marker({
    position: coords,
    map: map,
    icon: "./marker/" + routeColor[color] + "/" + order + ".png",
  });
  finalmarkers.push(marker);

  marker.addListener("click", () => {
    infowindow.open(map, marker);
  });
}
var directionRenders = [];
const getDirection = (i, j) => {
  return sleep(500).then((v) => {
    let directionsService = new google.maps.DirectionsService();
    let directionsRenderer = new google.maps.DirectionsRenderer({
      suppressMarkers: true,
    });
    directionsRenderer.setMap(map);
    directionsRenderer.setOptions({
      polylineOptions: {
        strokeColor: color[i],
      },
    });
    directionsService.route(
      {
        origin: coords[route[i][j]],
        destination: coords[route[i][j + 1]],
        travelMode: google.maps.TravelMode.DRIVING,
      },
      (response, status) => {
        if (status === "OK") {
          directionsRenderer.setDirections(response);
          directionRenders.push(directionsRenderer);
        } else {
          console.log("Directions request failed due to " + status);
        }
      }
    );
  });
};
var c = 0,
  d = 0;
function displayRoute() {
  getDirection(c, d);
  if (d < route[c].length - 2) {
    markers[route[c][d + 1]].setMap(null);
    addFinalMarker(coords[route[c][d + 1]], weight[route[c][d + 1]], d + 1, c);
  }
  d = d + 1;
  if (d == route[c].length - 1) {
    c = c + 1;
    d = 0;
  }
  // if (d == 0 && c == countRoute) {
  //   for (c = 0; c < countRoute; c++) {
  //     for (d = 0; d < route[c].length - 1; d++) {
  //       if (d < route[c].length - 2) {

  //       }
  //     }
  //   }
  // }
}

async function displayRoutes() {
  for (c = 0; c < countRoute; c++) {
    for (d = 0; d < route[c].length - 1; d++) {
      console.log(d);

      const x = await getDirection(c, d);
    }
  }
}

// 2D array for Distance Matrix
distanceMatrix = new Array(40);
// Convert matrix to string for download as file later
distanceMatrix_text = "";
var iter1, iter2;

for (i = 0; i < 40; i++) {
  distanceMatrix[i] = new Array(40);
}

// Get distance matrix
const sleep = (ms) => {
  return new Promise((resolve) => setTimeout(resolve, ms));
};

const getDist = (iter1, iter2) => {
  return sleep(15000).then((v) => {
    var service = new google.maps.DistanceMatrixService();
    service.getDistanceMatrix(
      {
        origins: coords.slice(5 * iter1, 5 * (iter1 + 1)),
        destinations: coords.slice(20 * iter2, 20 * (iter2 + 1)),
        travelMode: "DRIVING",
      },
      function (response, status) {
        do {
          if (status == "OK") {
            console.log("Iter1: " + iter1 + " Iter2: " + iter2 + " OK");
            var origins = response.originAddresses;
            for (var i = 0; i < origins.length; i++) {
              var results = response.rows[i].elements;
              for (var j = 0; j < results.length; j++) {
                var element = results[j];
                var distance = parseFloat(
                  element.distance.text.split(" ")[0].replace(",", ".")
                );
                if (element.distance.text.split(" ")[1] == "m") {
                  distance = 0;
                }
                distanceMatrix[5 * iter1 + i][20 * iter2 + j] = distance;
              }
            }
          } else {
            console.log("Iter1: " + iter1 + " Iter2: " + iter2 + " " + status);
          }
        } while (status !== "OK");
      }
    );
  });
};

async function getDistanceMatrix() {
  toggleloader(2);
  try {
    let count;
    for (iter1 = 0; iter1 < 8; iter1++) {
      for (iter2 = 0; iter2 < 2; iter2++) {
        const x = await getDist(iter1, iter2);
      }
    }
    toggleloader(2);
  } catch (error) {
    console.log("Error");
  }
}

function toggleloader(i) {
  var loader = document.getElementById("loader" + i);
  if (loader.style.display === "none") {
    loader.style.display = "inline-block";
  } else {
    loader.style.display = "none";
  }
}

function download(filename, text) {
  var element = document.createElement("a");
  element.setAttribute(
    "href",
    "data:text/plain;charset=utf-8," + encodeURIComponent(text)
  );
  element.setAttribute("download", filename);

  element.style.display = "none";
  document.body.appendChild(element);

  element.click();

  document.body.removeChild(element);
}

// Start file download.
document.getElementById("dwn-btn").addEventListener(
  "click",
  function (e) {
    e.preventDefault();
    var a;
    var b;
    for (a = 0; a < 40; a++) {
      for (b = 0; b < 40; b++) {
        distanceMatrix_text = distanceMatrix_text + distanceMatrix[a][b] + " ";
      }
      distanceMatrix_text = distanceMatrix_text + "\r\n";
    }
    var filename = "distanceMatrix.txt";

    download(filename, distanceMatrix_text);
  },
  false
);

document.getElementById("dwn-dd").addEventListener(
  "click",
  function (e) {
    e.preventDefault();
    for (let i = 0; i < 40; i++) {
      node_data_text = node_data_text + i + " " + weight[i] + "\r\n";
    }
    var filename2 = "node_data.txt";
    download(filename2, node_data_text);
  },
  false
);

// Read output file from C
const input = document.querySelector('input[type="file"]');
input.addEventListener(
  "change",
  function (e) {
    const reader = new FileReader();
    reader.onload = function () {
      let lines = reader.result.split("\n");
      countRoute = lines.length - 1;
      for (let line = 0; line < lines.length; line++) {
        let nodes = lines[line].split(/(\s+)/).filter(function (e) {
          return e.trim().length > 0;
        });
        route[line] = new Array();
        for (let node = 0; node < nodes.length; node++) {
          route[line][node] = parseInt(nodes[node]);
        }
      }
    };
    reader.readAsText(input.files[0]);
  },
  false
);

var veh;
var load = 0;
var distance = 0;
var veh_container;
var buttonArr;
var memory = [];
function getResult() {
  for (let k = 0; k < 39; k++) {
    memory[k] = k;
  }
  var checkBox = document.getElementById("checkbox_container");
  if (checkBox.style.display === "none") {
    checkBox.style.display = "block";
  } else {
    checkBox.style.display = "block";
  }
  buttonArr = new Array(countRoute);
  var vehicles = document.getElementById("routes");

  for (let i = 0; i < countRoute; i++) {
    let headers = ["Order", "Address", "Demand"];

    veh_container = document.createElement("div");
    veh_container.className = "veh_container";
    veh = document.createElement("div");
    veh.id = "vehicle_" + i;
    veh.style.display = "none";
    let table = document.createElement("table");
    let headerRow = document.createElement("tr");

    headers.forEach((headerText) => {
      let header = document.createElement("th");
      let textNode = document.createTextNode(headerText);
      header.appendChild(textNode);
      headerRow.appendChild(header);
    });
    table.appendChild(headerRow);
    for (let j = 0; j < route[i].length; j++) {
      let row = document.createElement("tr");

      let cell1 = document.createElement("td");
      let textNode1 = document.createTextNode(j);
      cell1.appendChild(textNode1);
      row.appendChild(cell1);
      let cell2 = document.createElement("td");
      let textNode2 = document.createTextNode(address[route[i][j]]);
      cell2.appendChild(textNode2);
      row.appendChild(cell2);
      let cell3 = document.createElement("td");
      let textNode3 = document.createTextNode(weight[route[i][j]]);
      cell3.appendChild(textNode3);
      row.appendChild(cell3);
      table.appendChild(row);
      load += weight[route[i][j]];
      if (j < route[i].length - 1) {
        distance += distanceMatrix[route[i][j]][route[i][j + 1]];
      }
    }
    load = load.toFixed(2);
    distance = distance.toFixed(2);
    veh.appendChild(table);
    let totalload = document.createElement("P");
    totalload.innerHTML = "Load: " + load + " kg.";
    let totalDistance = document.createElement("P");
    totalDistance.innerHTML = "Distance: " + distance + " km.";
    veh.appendChild(totalload);
    veh.appendChild(totalDistance);
    veh_container.append(veh);
    vehicles.appendChild(veh_container);
    load = 0;
    distance = 0;
  }
  var vehiclesButton = document.getElementById("routesButton");
  vehiclesButton.className = "button_container";
  for (let i = 0; i < countRoute; i++) {
    var button_container = document.createElement("span");
    var veh_button = document.createElement("button");
    veh_button.innerHTML = "Vehicle " + i;
    veh_button.className = "box_button";
    veh_button.id = i;
    buttonArr[i] = document.getElementById("vehicle_" + i);
    veh_button.onclick = function () {
      document.getElementById("hide_markers").checked = false;
      document.getElementById("all_routes").checked = false;
      memory = [];
      const [startRoute, startMarker] = computeIndex(i);
      for (let t = 0; t < directionRenders.length; t++) {
        directionRenders[t].setMap(null);
      }
      for (let t = startRoute; t < startRoute + route[i].length - 1; t++) {
        directionRenders[t].setMap(map);
      }

      for (let t = 0; t < finalmarkers.length; t++) {
        finalmarkers[t].setMap(null);
      }

      for (let t = startMarker; t < startMarker + route[i].length - 2; t++) {
        finalmarkers[t].setMap(map);
        memory.push(t);
      }

      var id = event.target.id;
      for (let j = 0; j < countRoute; j++) {
        if (j != i) {
          if (buttonArr[j].style.display === "block") {
            buttonArr[j].style.display = "none";
          } else {
            buttonArr[j].style.display = "none";
          }
        }
      }
      if (buttonArr[i].style.display === "none") {
        buttonArr[i].style.display = "block";
      } else {
        buttonArr[i].style.display = "block";
      }
    };
    button_container.appendChild(veh_button);
    vehiclesButton.appendChild(button_container);
  }
}

function computeIndex(i) {
  var startRoute = 0;
  var startMarker = 0;
  for (let j = 0; j < i; j++) {
    startRoute += route[j].length - 1;
    startMarker += route[j].length - 2;
  }
  return [startRoute, startMarker];
}

function showAllRoutes() {
  var checkBox = document.getElementById("all_routes");

  if (checkBox.checked == true) {
    for (let i = 0; i < directionRenders.length; i++) {
      directionRenders[i].setMap(map);
    }
    for (let i = 0; i < finalmarkers.length; i++) {
      finalmarkers[i].setMap(map);
    }
    for (let k = 0; k < 39; k++) {
      memory[k] = k;
    }
    document.getElementById("hide_markers").checked = false;
  } else {
    for (let i = 0; i < directionRenders.length; i++) {
      directionRenders[i].setMap(null);
    }
    for (let i = 0; i < finalmarkers.length; i++) {
      finalmarkers[i].setMap(null);
    }
    document.getElementById("hide_markers").checked = false;
  }
}

function hideAllMarkers() {
  var checkBox = document.getElementById("hide_markers");

  if (checkBox.checked == true) {
    for (let i = 0; i < finalmarkers.length; i++) {
      finalmarkers[i].setMap(null);
    }
  } else {
    for (let i = 0; i < memory.length; i++) {
      finalmarkers[memory[i]].setMap(map);
    }
  }
}
