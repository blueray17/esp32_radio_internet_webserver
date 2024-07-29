#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include <WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//i2s
#define I2S_DOUT  32
#define I2S_BCLK  33
#define I2S_LRC   25

//oled
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//WiFiServer server(80);
WebServer server(80);
int a = 0;
Audio audio;
const int up = 23;
const int down = 22;
const int vol_up = 19;
const int vol_down = 21;

String ssid =    "gundul123";
String password = "jagung123";
String IP;

String url[][2] = {
  {"Bass 93.2 FM", " http://live.bassfm.id/;"}
  , {"Bens Radio 106.2 FM Jakarta", " http://stream-157.zeno.fm/047dgsp3v68uv?zs=SVB0N5mHSHC8YLwZR1QcDg"}
  , {"CampursariFM 89.2", "https://a8.siar.us/listen/campursari/stream"}
  , {"Classy NetRadio", " http://streaming.classynetradio.com/radio/8000/classySD"}
  , {"Gajahmada", " http://server.radioimeldafm.co.id:8040/gajahmadafm"}
  , {"GeFM Madiun", "http://gefm.onlivestreaming.net:9520/live?1701156080944"}
  , {"Gita FM Jombang", "http://svara-stream.radioddns.net:8000/jombang_gitafm_?1701156041862"}
  , {"Heartline Lampung", " http://svara-stream.radioddns.net:8443/lampung_heartline_mp3"}
  , {"IRadio Jakarta ", " http://n12.radiojar.com/4ywdgup3bnzuv"}
  , {"KISI 93.4 FM BOGOR", " http://live.serverstreaming.net:9340/kisifm"}
  , {"KISS 105 FM", " http://live.kissfm-medan.com:8080/kissfm.mp3"}
  , {"Mahardika FM Blitar", "http://streaming.mahardhikafm.com:9590/radio?1701155604129"}
  , {"Menara FM Bali", " http://c1.siar.us:10280/menarafmbali"}
  , {"Mettaswara 2010 an", " http://mettaswara.com:8700/m2010"}
  , {"Mettaswara Campursari", " http://mettaswara.com:8700/cps"}
  , {"Mettaswara Dangdut", " http://mettaswara.com:8700/d4d"}
  , {"Mettaswara Indo 2000an", " http://mettaswara.com:8700/indo00"}
  , {"Mettaswara Indonesia", " http://mettaswara.com:8700/indo"}
  , {"Mettaswara Jawa", " http://mettaswara.com:8700/java"}
  , {"Mettaswara Love", " http://mettaswara.com:8700/love"}
  , {"Mettaswara Mix", " http://mettaswara.com:8700/mix"}
  , {"Mettaswara Koplo", " http://mettaswara.com:8700/koplo"}
  , {"Mettaswara Slow", " http://mettaswara.com:8700/slow"}
  , {"Mettaswara Slow Rock", " http://mettaswara.com:8700/slowrock"}
  , {"Motion", " http://cast1.my-control-panel.com/proxy/radioso2/stream"}
  , {"Nagaswara", " http://live.nagaswarafm.com:8088/stream"}
  , {"Ninetysix Radio", " http://c4.siar.us:8950/stream"}
  , {"PAS FM Pati", "http://45.64.97.82:8886/;?1701155704090"}
  , {"Prambors FM 102.2 Jakarta", " http://23683.live.streamtheworld.com/PRAMBORS_FM_SC"}
  , {"Radio Kasih", " http://hello.citrus3.com:2020/stream/radiokasih"}
  , {"Radio online Tulungagung", " http://stream-159.zeno.fm/954yxsx00ehvv?zs=CRyBDUT1SXu6XSa_zSwoiA"}
  , {"Radio Rodja Bogor", " http://radioislamindonesia.com/rodja.mp3"}
  , {"Rama FM Bandung", " http://i.klikhost.com:8330/;stream.nsv"}
  , {"Retjobuntung", " http://45.64.97.82:9940/stream"}
  , {"RSFM Pati", "http://live4.rs3fm.com:8000/rs3?1701156161974"}
  , {"Sanjaya FM Magetan", " http://sanjayafm.u7.myradio.web.id:8888/sanjayafm"}
  , {"Suara Muslim Surabaya", " http://pu.klikhost.com:8052/stream"}
  , {"Suara Surabaya", " http://c5.siar.us:8000/stream"}
  , {"TOPFM 95.1", " http://streaming.topfm951.net:9510/stream"}
  , {"Trax 101.4 FM Jakarta", " http://n07.radiojar.com/rrqf78p3bnzuv.mp3?rj-tok=AAABi_8o08gA5jtQz3StVoSgZA&rj-ttl=5"}
  , {"Urfan Radio", "https://stream-149.zeno.fm/1tozd0cybi3tv?zs=-ihA3XtmQ-S-szGHaguhaQ"}
  , {"V Radio 106 6 FM Jakarta  ", " http://202.147.199.100:8000/;stream.nsv"}

};
int i = 17, indxst = 1, jml = sizeof(url) / sizeof(url[0]), volume = 10;
int sss = sizeof(url[0]);
String SendHTML() {


  String mpage = "<!DOCTYPE html>\n";
  mpage += "<html>\n";
  mpage += "<body style=font-size:20px;>\n";
  mpage += "<form class='form-horizontal' action='/action_page' method='post'>\n";
  mpage += "<fieldset>\n";

  mpage += "<legend style=font-size:60px;>Ipin FM</legend>\n";
  mpage += "<span class='font-weight-bold'>Radio Name : " + url[i][0] + "</span><br>\n";
  mpage += "<span class='font-weight-bold'>Link : " + url[i][1] + "</span><br>\n";
  mpage += "<span class='font-weight-bold'>Volume : " + String((volume) / 5) + " (0-20)</span><br><p></p>\n";

  mpage += "<div class='form-group'>\n";
  mpage += "  <div class='col-md-8'>\n";
  mpage += "    <button type='submit' value='volmin' id='button1id' name='button1id' class='btn btn-success' style=font-size:40px;>Volume - </button>\n";
  mpage += "    <button type='submit' value='volplus' id='button2id' name='button2id' class='btn btn-danger' style=font-size:40px;>Volume + </button>\n";
  mpage += "  </div>\n";
  mpage += "</div>\n";
  mpage += "<br>\n";

  mpage += "<div class='form-group'>\n";
  mpage += "  <div class='col-md-8'>\n";
  mpage += "      <select id='select' name='select' class='form-control' style='width: 470px !important;height: 40px !important;'>\n";
  mpage += "        <option value=''></option>\n";
  for (int ii = 0; ii < jml; ii++) {
    mpage += "    <option value='" + String(ii) + "'>" + url[ii][0] + "</option>\n";
  }
  mpage += "      </select>\n";
  mpage += "  </div>\n";
  mpage += "</div>\n";
  mpage += "<br>\n";

  mpage += "<div class='form-group'>\n";
  mpage += "  <div class='col-md-8'>\n";
  mpage += "  </div>\n";
  mpage += "  <div class='col-md-8'>\n";
  mpage += "    <button type='submit' value='submit' id='submit' name='submit' class='btn btn-success' style=font-size:40px;>Pilih Radio</button>\n";
  mpage += "  </div>\n";
  mpage += "</div>\n";
  mpage += "<br>\n";


  mpage += "</fieldset>\n";
  mpage += "</form>\n";
  mpage += "</body>\n";
  mpage += "</html>";
  return mpage;
}

void PrintText(int a) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  if (a == 1) {
    display.drawLine(0, 0, display.width() - 1, 0, SSD1306_WHITE);
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setCursor(0, 7);            // Start at top-left corner
    display.println("CONNECTING");
    display.drawLine(0, display.height() - 1, display.width() - 1, display.height() - 1 , SSD1306_WHITE);
  } else if (a == 2) {
    display.drawLine(0, 0, display.width() - 1, 0, SSD1306_WHITE);
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setCursor(0, 7);            // Start at top-left corner
    display.println(" CONNECTED");
    display.drawLine(0, display.height() - 1, display.width() - 1, display.height() - 1 , SSD1306_WHITE);
  } else if (a == 3) {
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setCursor(0, 0);            // Start at top-left corner
    display.print("IP  : ");
    display.println(IP);
    display.print("Vol : ");
    display.print(volume / 5);
    display.print("| Channel : ");
    display.println(i);

    
    //display.setCursor(0, 7);            // Start at top-left corner
    display.print("Name: ");
    display.println(url[i][0].c_str());
  }


  display.display();
}


void handleRoot() {
  server.send(200, "text/html", SendHTML()); //Send web page
}

void handleForm() {
  String vm = server.arg("button1id");
  String vp = server.arg("button2id");
  String sb = server.arg("submit");
  int sl = server.arg("select").toInt();
  if (vm == "volmin") {
    volume -= 5;
    if (volume < 0) volume = 0;
    audio.setVolume(volume);
    PrintText(3);
    server.sendHeader("Location", "/", true);
    server.send(302, "text/html", SendHTML());
  }
  if (vp == "volplus") {
    volume += 5;
    if (volume >= 100) volume = 100;
    audio.setVolume(volume);
    PrintText(3);
    server.sendHeader("Location", "/", true);
    server.send(302, "text/html", SendHTML());
  }
  if (sb == "submit") {
    i = sl;
    audio.connecttohost(url[i][1].c_str());
    PrintText(3);
    server.sendHeader("Location", "/", true);
    server.send(302, "text/html", SendHTML());
  }
}


void setup() {
  Wire.begin(5, 18);
  Serial.begin(115200);
  WiFi.begin(ssid.c_str(), password.c_str());
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  Serial.println("Mulaii");
  PrintText(1);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.print("IP ; "); Serial.println(WiFi.localIP());
  delay(200);
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume); // 0...21
  audio.connecttohost(url[i][1].c_str());
  audio.setTone(6, 3, 0); //-40 - 6
  delay(100);
  Serial.print("ESP32 IP on the WiFi network: ");
  Serial.println(WiFi.localIP());
  IP = WiFi.localIP().toString();

  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(vol_up, INPUT);
  pinMode(vol_down, INPUT);

  PrintText(2);
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here
  server.onNotFound(handleRoot);
  server.begin();
  delay(5000);
  PrintText(3);
}

void loop() {
  server.handleClient();
  audio.loop();

  if (digitalRead(up) == HIGH) {
    Serial.println("up");
    i=i+1;
    if(i==jml)i=0;
    audio.connecttohost(url[i][1].c_str());
    PrintText(3);
    delay(200);
  }
  if (digitalRead(down) == HIGH) {
    Serial.println("down");
    i=i-1;
    if(i<0)i=jml-1;
    audio.connecttohost(url[i][1].c_str());
    PrintText(3);
    delay(200);
  }
  if (digitalRead(vol_up) == HIGH) {
    Serial.println("vol_up");
    volume += 5;
    if (volume >= 100) volume = 100;
    audio.setVolume(volume);
    PrintText(3);
    delay(200);
  }
  if (digitalRead(vol_down) == HIGH) {
    Serial.println("vol up");
    volume -= 5;
    if (volume < 0) volume = 0;
    audio.setVolume(volume);
    PrintText(3);
    delay(200);
  }

}
