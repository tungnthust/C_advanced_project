var order = new Array(10);
order = [7661, 123, 45, 11, 256, 1541, 485, 789, 1211, 13];

fetch("./sale_records.json").then((response) => {
  response.json().then((data) => {
    // console.log(data);
    if (data.length > 0) {
      let temp = "";
      for (let j = 0; j < 10; j++) {
        let u = data[order[j] - 1];

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
  });
});
