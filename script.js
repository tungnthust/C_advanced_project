var distanceMatrix_text;
var map;
var latitude;
var longitude;
var coords;
var distanceMatrix;
var i, j;

// Initialize and add the map
function initMap() {
  var hanoi;
  hanoi = { lat: 21.0278, lng: 105.8342 };
  map = new google.maps.Map(document.getElementById("map"), {
    zoom: 13,
    center: hanoi,
  });
}

function getPlacesAndDisplay() {
  latitude = new Array(39);
  longitude = new Array(39);

  //Random coordinates
  for (i = 0; i < 39; i++) {
    latitude[i] = parseFloat((Math.random() * 0.068 + 20.975).toFixed(4));
    longitude[i] = parseFloat((Math.random() * 0.094 + 105.77).toFixed(4));
  }

  coords = new Array(40);
  //Coordinate of depot
  coords[0] = { lat: 21.0278, lng: 105.8342 };

  for (i = 1; i <= 39; i++) {
    coords[i] = { lat: latitude[i - 1], lng: longitude[i - 1] };
  }
  addMarkerDepot(coords[0]);
  // Display all places
  for (i = 1; i < 40; i++) {
    addMarker(coords[i]);
  }

  function addMarker(coords) {
    var marker = new google.maps.Marker({
      position: coords,
      map: map,
    });
  }

  function addMarkerDepot(coords) {
    var marker = new google.maps.Marker({
      position: coords,
      map: map,
      icon: "http://maps.google.com/mapfiles/kml/shapes/ranger_station.png",
    });
  }
}

// Route of each vehicle
var route = new Array(4);
route[0] = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
route[1] = [0, 10, 11, 12, 13, 14, 14, 15, 16, 17, 18, 19, 0];
route[2] = [0, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 0];
route[3] = [0, 31, 32, 33, 34, 35, 36, 37, 38, 39, 0];
// Color for each route
var color = ["blue", "red", "green", "yellow"];

const getDirection = (i, j) => {
  return sleep(5000).then((v) => {
    let directionsService = new google.maps.DirectionsService();
    let directionsRenderer = new google.maps.DirectionsRenderer();
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
        } else {
          console.log("Directions request failed due to " + status);
        }
      }
    );
  });
};

async function displayRoutes() {
  for (i = 0; i < 4; t++) {
    for (j = 0; j < route[i].length - 1; j++) {
      const x = await getDirection(i, j);
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

const getDist = (iter1, iter2, count) => {
  return sleep(15000).then((v) => {
    var service = new google.maps.DistanceMatrixService();
    service.getDistanceMatrix(
      {
        origins: coords.slice(5 * iter1, 5 * (iter1 + 1)),
        destinations: coords.slice(20 * iter2, 20 * (iter2 + 1)),
        travelMode: "DRIVING",
      },
      function (response, status) {
        if (status == "OK") {
          console.log("Iter1: " + iter1 + " Iter2: " + iter2 + " OK");
          var origins = response.originAddresses;
          var destinations = response.destinationAddresses;
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
      }
    );
  });
};

async function getDistanceMatrix() {
  try {
    for (iter1 = 0; iter1 < 8; iter1++) {
      for (iter2 = 0; iter2 < 2; iter2++) {
        const x = await getDist(iter1, iter2, count);
      }
    }
  } catch (error) {
    console.log("Error");
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
  function () {
    var a;
    var b;
    for (a = 0; a < 40; a++) {
      for (b = 0; b < 40; b++) {
        distanceMatrix_text = distanceMatrix_text + distanceMatrix[a][b] + " ";
      }
      distanceMatrix_text = distanceMatrix_text + "\r\n";
    }
    // Generate download of hello.txt file with some content
    var filename = "distanceMatrix.txt";

    download(filename, distanceMatrix_text);
  },
  false
);
