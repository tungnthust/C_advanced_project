var c = document.getElementById("myCanvas");
var i = 0;
var x = 60;
var j = 0;
for (i = 0; i < 10; i++) {
  for (j = 0; j < 4; j++) {
    ctx = c.getContext("2d");
    ctx.fillStyle = "#d6a94d";
    ctx.fillRect(x, 20 + 156 * j, 30, 130);
    ctx = c.getContext("2d");
    ctx.fillStyle = "#baa697";
    ctx.fillRect(x + 30, 20 + 156 * j, 30, 130);
  }
  x = x + 120;
}

var res = [
  0,
  62,
  19,
  42,
  153,
  174,
  119,
  85,
  96,
  140,
  204,
  189,
  199,
  78,
  100,
  123,
  112,
  69,
  58,
  35,
  0,
];
var points = new Array(20);
for (i = 0; i < 20; i++) {
  if (res[i] % 11 == 10) points[i] = (res[i] - Math.floor(res[i] / 11)) * 2 - 1;
  else if (res[i] % 11 == 0) points[i] = (res[i] - Math.floor(res[i] / 11)) * 2;
  else {
    let random = parseInt(Math.random() * 2);
    points[i] = (res[i] - Math.floor(res[i] / 11)) * 2 - random;
  }
}

for (i = 1; i < points.length; i++) {
  const [coordX, coordY] = getCoord(points[i]);
  ctx.beginPath();
  ctx.arc(coordX, coordY, 5, 0, 2 * Math.PI, false);
  ctx.fillStyle = "black";
  ctx.fill();
  ctx.closePath();
}
var i1 = 0,
  i2 = 1;
var t = 1;
function displayPath() {
  drawPath(res[i1], res[i2]);
  const [coordX1, coordY1] = getCoord(points[t]);
  ctx.beginPath();
  ctx.arc(coordX1, coordY1, 5, 0, 2 * Math.PI, false);
  ctx.fillStyle = "red";
  ctx.fill();
  ctx.closePath();
  t = t + 1;
  i1 = i1 + 1;
  i2 = i2 + 1;
}
function drawPath(x, y) {
  const [x1, y1] = getCoord2(x);
  const [x2, y2] = getCoord2(y);
  if (x1 == x2) {
    ctx.beginPath();
    ctx.moveTo(x1, y1);
    ctx.lineTo(x2, y2);
    ctx.lineWidth = 5;
    ctx.strokeStyle = "rgba(255,0,0,0.5)";
    ctx.stroke();
    ctx.closePath();
  } else {
    let a = Math.floor(x / 55);
    let b = Math.floor(y / 55);
    if (a == b) {
      if (
        Math.abs((x % 11) - (y % 11)) * 120 +
          (Math.floor(x / 11) - a * 5 + Math.floor(y / 11) - b * 5 + 2) * 26 <
        Math.abs((x % 11) - (y % 11)) * 120 +
          (2 * 5 - Math.floor(x / 11) + a * 5 - Math.floor(y / 11) + b * 5) * 26
      ) {
        ctx.beginPath();
        ctx.moveTo(x1, y1);
        ctx.lineTo(x1, y1 - (Math.floor(x / 11) - a * 5 + 1) * 26);
        ctx.lineTo(x2, y1 - (Math.floor(x / 11) - a * 5 + 1) * 26);
        ctx.lineTo(x2, y2);
        ctx.lineWidth = 5;
        ctx.strokeStyle = "rgba(255,0,0,0.5)";
        ctx.stroke();
        ctx.closePath();
      } else {
        ctx.beginPath();
        ctx.moveTo(x1, y1);
        ctx.lineTo(x1, y1 + (5 - Math.floor(x / 11) + a * 5) * 26);
        ctx.lineTo(x2, y1 + (5 - Math.floor(x / 11) + a * 5) * 26);
        ctx.lineTo(x2, y2);
        ctx.lineWidth = 5;
        ctx.strokeStyle = "rgba(255,0,0,0.5)";
        ctx.stroke();
        ctx.closePath();
      }
    } else if (a < b) {
      ctx.beginPath();
      ctx.moveTo(x1, y1);
      ctx.lineTo(x1, y1 + (5 - Math.floor(x / 11) + a * 5) * 26);
      ctx.lineTo(x2, y1 + (5 - Math.floor(x / 11) + a * 5) * 26);
      ctx.lineTo(x2, y2);
      ctx.lineWidth = 5;
      ctx.strokeStyle = "rgba(255,0,0,0.5)";
      ctx.stroke();
      ctx.closePath();
    } else {
      ctx.beginPath();
      ctx.moveTo(x1, y1);
      ctx.lineTo(x1, y1 - (Math.floor(x / 11) - a * 5 + 1) * 26);
      ctx.lineTo(x2, y1 - (Math.floor(x / 11) - a * 5 + 1) * 26);
      ctx.lineTo(x2, y2);
      ctx.lineWidth = 5;
      ctx.strokeStyle = "rgba(255,0,0,0.5)";
      ctx.stroke();
      ctx.closePath();
    }
  }
}

function getCoord(point) {
  var X, Y;
  if (point % 2 == 0) {
    X = ((point % 20) + 1) * 60 + 15;
  } else {
    X = ((point % 20) + 1) * 60 - 15;
  }
  Y = 33 + Math.floor(point / 20) * 26 + Math.floor(point / 100) * 26;
  return [X, Y];
}

function getCoord2(point) {
  var X, Y;
  X = (point % 11) * 120 + 30;

  Y = 33 + Math.floor(point / 11) * 26 + Math.floor(point / 55) * 26;
  return [X, Y];
}
