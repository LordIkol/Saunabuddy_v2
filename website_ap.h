const char WIFI_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Enter your Wifi credentials</h2>
  <form action="\WifiLogin" method="post">
  <p>
  <label>SSID:&nbsp;</label>
  <input maxlength="30" name="ssid"><br>
  <label>Key:&nbsp;&nbsp;&nbsp;&nbsp;</label><input maxlength="30" name="password"><br>
  <input type="submit" value="Save">
  </p>  
  </form> 
</body>
</html>)rawliteral";
