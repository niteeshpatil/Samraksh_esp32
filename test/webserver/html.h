/*
  ESP32 HTML WebServer Page Code
  http:://www.electronicwings.com
*/

const char html_page[] PROGMEM = R"RawString(
<!DOCTYPE html>
<html>
  <style>
    body {font-family: sans-serif;}
    h1 {text-align: center; font-size: 30px;}
    p {text-align: center; color: #4CAF50; font-size: 40px;}
  </style>

<body>
  <h1>ADXL345 Interface with ESP32 </h1><br>
  <p>X : <span id="_X">0</span> m/s&#178;</p>
  <p>Y : <span id="_Y">0</span> m/s&#178;</p>
  <p>Z : <span id="_Z">0</span> m/s&#178;</p>

<script>
  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        const text = this.responseText;
        const myArr = JSON.parse(text);
        document.getElementById("_X").innerHTML = myArr[0];
        document.getElementById("_Y").innerHTML = myArr[1];
        document.getElementById("_Z").innerHTML = myArr[2];
      }
    };
    xhttp.open("GET", "readADXL345", true);
    xhttp.send();
  },100);
</script>
</body>
</html>
)RawString";
