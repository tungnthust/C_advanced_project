fetch("./sale_records.json").then(
  response => {
    response.json().then(
      data => {
        // console.log(data);
        if(data.length > 0){
          let temp = "";
          
          // data.forEach((u )=>{
          for (let i = 0; i < 1000; i++){
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