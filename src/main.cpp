#include <Arduino.h>
#include <Adafruit_GFX.h>      // include Adafruit graphics library
#include <Adafruit_ST7735.h>   // include Adafruit ST7735 TFT library
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
//***********[ ST7735 TFT module connections ]***********
#define TFT_RST   D4     // TFT RST pin is connected to NodeMCU pin D4 (GPIO2)
#define TFT_CS    D3     // TFT CS  pin is connected to NodeMCU pin D4 (GPIO0)
#define TFT_DC    D2     // TFT DC  pin is connected to NodeMCU pin D4 (GPIO4)
// initialize ST7735 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)

//***********[ Predefined Values ]***********
const char* ssid = "InterZet610_2.4";
const char* password = "0987654321000";
String page = "";

//***********[ Object Inits ]***********
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST); //tft screen object
ESP8266WebServer server(80);

//***********[ Function Prototypes ]***********
String prepareHtmlPage();
void refresh(Adafruit_ST7735*);

//***********[ Inits and One-Time Run ]***********
void setup(void) 
{
  page += String("<h1>ESP8266 Web Server</h1>\n");
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);
  delay(500);
  tft.printf("Connecting to\n%s...\n", ssid);
  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.begin(ssid, password);
  delay(1000);
  /*while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    tft.print(".");
  }*/
  tft.println("Connected.");
  server.begin();
  tft.print("Web server started,\nlocal ip is: ");
  delay(5500);
  
  tft.println(WiFi.localIP().toString().c_str());
  /* tft.drawRect(0,0,128,25,ST7735_WHITE);
  tft.drawRect(0,25,128,80,ST7735_WHITE);
  tft.drawRect(0,105,128,55,ST7735_WHITE);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(1);
  tft.println("12:00");
  tft.setTextSize(0);
  tft.setCursor(5, 15);
  tft.println("01.10.19");
  tft.setCursor(105,5);
  tft.print("90%");
  tft.setTextSize(5);
  tft.setCursor(20,50);
  tft.print("19");
  tft.setTextSize(1);
  tft.println(" km/h");
  tft.setCursor(5, 110);
  tft.setTextSize(2);
  tft.print("o 12'000");
  tft.setTextSize(1);
  tft.println(" rpm");
  tft.setTextSize(2);
  tft.setCursor(5, 140);
  tft.print("H 90");
  tft.setTextSize(1);
  tft.println(" b/min"); */
  server.on("/", []()
  {
    server.send(200, "text/html", String(page)+"Waiting for params.\n");
  });
  server.on("/param1", []()
  {
    server.send(200, "text/html", String(page)+"Random number is: "+rand()%100+1+"\n");
  });
}

//***********[ Inf Loop ]***********
void loop(void) 
{
  server.handleClient();
  /*WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    tft.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        tft.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
      }
    }
    delay(1000); // give the web browser time to receive the data
    // close the connection:
    client.stop();
    tft.println("[Client disonnected]");
  }
  refresh(&tft);*/
}

//***********[ Function Definitions ]***********
String prepareHtmlPage()
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: keep-alive\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "Ret Value:  HELLO" +
            "</html>" +
            "\r\n";
  return htmlPage;
}

void refresh(Adafruit_ST7735* screen)
{
  screen->print("[REFRESH IN 7s]");
  delay(7000);
  screen->fillScreen(ST7735_BLACK);
  screen->setCursor(0,0);
}

//EXAMPLE FUNCS BEGIN
/*
void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}
void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST7735_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST7735_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST7735_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST7735_WHITE);
  tft.print(" seconds.");
}
*/
//FUNCS END