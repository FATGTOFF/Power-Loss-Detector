#ifndef _WEBPAGE_H
#define _WEBPAGE_H

constexpr char* webPage = R""""(
<!-- HTTP/1.1 200 OK
Content-type:text/html -->

<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Controller</title>
  <style>
    body { text-align: center; font-family: Arial, sans-serif; }    
    button { background-color: slateblue; color: white; border: none; width: 70px; height: 50px; }
    button:hover { background-color: darkslateblue; }    
    button:active { background-color: mediumslateblue; }    
  </style>  
</head>
<body>
  <h1>ESP32 Controller</h1>
  <button onclick="getData()">Get Data</button>
  <p id="TempData">--</p>
  <p id="VoltageData">--</p>
 <script>
 function getData() {
    fetch('/TempData')
        .then(response => response.text())
        .then(TempData => {
            document.getElementById("TempData").innerHTML = TempData;
        });

    fetch('/VoltageData')
        .then(response => response.text())
        .then(VoltageData => {
            document.getElementById("VoltageData").innerHTML = VoltageData;
        });
}

</script>
 
</body>
</html>
)"""";

#endif