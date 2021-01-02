function beforeSort() {

  fetch("./sale_records.json").then(
    response => {
      response.json().then(
        data => {
          // console.log(data);
          if (data.length > 0) {
            let temp = `
            <tr>
              <th>ID</th>
              <th>Region</th>
              <th>Country</th>
              <th>Item Type</th>
              <th>Sales Channel</th>
              <th>Order Priority</th>
              <th>Order Date</th>
              <th>Order Id</th>
              <th>Ship Date</th>
              <th>Units Sold</th>
              <th>Unit Price</th>
              <th>Unit Cost</th>
              <th>Total Revenue</th>
              <th>Total Cost</th>
              <th>Total Profit</th>
            </tr>`;

            // data.forEach((u )=>{
            for (let i = 0; i < 1000; i++) {
              let u = data[i];
              temp += "<tr>";
              temp += "<td>" + u.Id + "</td>";
              temp += "<td>" + u.Region + "</td>";
              temp += "<td>" + u.Country + "</td>";
              temp += "<td>" + u.ItemType + "</td>";
              temp += "<td>" + u.SalesChannel + "</td>";
              temp += "<td>" + u.OrderPriority + "</td>";
              temp += "<td>" + u.OrderDate + "</td>";
              temp += "<td>" + u.OrderID + "</td>";
              temp += "<td>" + u.ShipDate + "</td>";
              temp += "<td>" + u.UnitsSold + "</td>";
              temp += "<td>" + u.UnitPrice + "</td>";
              temp += "<td>" + u.UnitCost + "</td>";
              temp += "<td>" + u.TotalRevenue + "</td>";
              temp += "<td>" + u.TotalCost + "</td>";
              temp += "<td>" + u.TotalProfit + "</td>";
              // temp += "<td>" + u. + "</td>";
              temp += "</tr>";
            }

            document.getElementById("data").innerHTML = temp;
          }
        }
      )
    }
  )
}
function afterSort() {

  fetch("./sale_records.json").then(
    response => {
      response.json().then(
        data => {
          // console.log(data);
          if (data.length > 0) {
            let temp = `
            <tr>
              <th>ID</th>
              <th>Region</th>
              <th>Country</th>
              <th>Item Type</th>
              <th>Sales Channel</th>
              <th>Order Priority</th>
              <th>Order Date</th>
              <th>Order Id</th>
              <th>Ship Date</th>
              <th>Units Sold</th>
              <th>Unit Price</th>
              <th>Unit Cost</th>
              <th>Total Revenue</th>
              <th>Total Cost</th>
              <th>Total Profit</th>
            </tr>`;

            // data.forEach((u )=>{
            for (let i = 0; i < 1000; i++) {
              let u = data[order[i]-1];
              temp += "<tr>";
              temp += "<td>" + u.Id + "</td>";
              temp += "<td>" + u.Region + "</td>";
              temp += "<td>" + u.Country + "</td>";
              temp += "<td>" + u.ItemType + "</td>";
              temp += "<td>" + u.SalesChannel + "</td>";
              temp += "<td>" + u.OrderPriority + "</td>";
              temp += "<td>" + u.OrderDate + "</td>";
              temp += "<td>" + u.OrderID + "</td>";
              temp += "<td>" + u.ShipDate + "</td>";
              temp += "<td>" + u.UnitsSold + "</td>";
              temp += "<td>" + u.UnitPrice + "</td>";
              temp += "<td>" + u.UnitCost + "</td>";
              temp += "<td>" + u.TotalRevenue + "</td>";
              temp += "<td>" + u.TotalCost + "</td>";
              temp += "<td>" + u.TotalProfit + "</td>";
              // temp += "<td>" + u. + "</td>";
              temp += "</tr>";
            }

            document.getElementById("data").innerHTML = temp;
          }
        }
      )
    }
  )
}

var order = new Array();

const input = document.querySelector('input[type="file"]');
input.addEventListener(
  "change",
  function (e) {
    const reader = new FileReader();
    reader.onload = function () {
      let lines = reader.result.split("\n");
      for (let line = 0; line < lines.length; line++){
        order[line] = parseInt(lines[line]);
        console.log(lines[line]);
      }
    }
    reader.readAsText(input.files[0]);
  },
  false
);