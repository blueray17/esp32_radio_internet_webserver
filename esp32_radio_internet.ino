#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include <WebServer.h>

#define I2S_DOUT  32
#define I2S_BCLK  33
#define I2S_LRC   25

//WiFiServer server(80);
WebServer server(80);
int a = 0;
Audio audio;

String ssid =    "ssid";
String password = "password";

String url[][2] = {
  {"Bass 93.2 FM"," http://live.bassfm.id/;"}
  ,{"Bens Radio 106.2 FM Jakarta"," http://stream-157.zeno.fm/047dgsp3v68uv?zs=SVB0N5mHSHC8YLwZR1QcDg"}
  ,{"Classy NetRadio"," http://streaming.classynetradio.com/radio/8000/classySD"}
  ,{"Gajahmada"," http://server.radioimeldafm.co.id:8040/gajahmadafm"}
  ,{"Heartline Lampung"," http://svara-stream.radioddns.net:8443/lampung_heartline_mp3"}
  ,{"IRadio Jakarta "," http://n12.radiojar.com/4ywdgup3bnzuv"}
  ,{"KISI 93.4 FM BOGOR"," http://live.serverstreaming.net:9340/kisifm"}
  ,{"KISS 105 FM"," http://live.kissfm-medan.com:8080/kissfm.mp3"}
  ,{"Menara FM Bali"," http://c1.siar.us:10280/menarafmbali"}
  ,{"Mettaswara 90's Tangerang"," http://mettaswara.com:8700/d4d"}
  ,{"Motion"," http://cast1.my-control-panel.com/proxy/radioso2/stream"}
  ,{"Nagaswara"," http://live.nagaswarafm.com:8088/stream"}
  ,{"Ninetysix Radio"," http://c4.siar.us:8950/stream"}
  ,{"Prambors FM 102.2 Jakarta"," http://23683.live.streamtheworld.com/PRAMBORS_FM_SC"}
  ,{"Radio Kasih"," http://hello.citrus3.com:2020/stream/radiokasih"}
  ,{"Radio online Tulungagung"," http://stream-159.zeno.fm/954yxsx00ehvv?zs=CRyBDUT1SXu6XSa_zSwoiA"}
  ,{"Radio Rodja Bogor"," http://radioislamindonesia.com/rodja.mp3"}
  ,{"Rama FM Bandung"," http://i.klikhost.com:8330/;stream.nsv"}
  ,{"Retjobuntung"," http://45.64.97.82:9940/stream"}
  ,{"Sanjaya FM Magetan"," http://sanjayafm.u7.myradio.web.id:8888/sanjayafm"}
  ,{"Suara Muslim Surabaya"," http://pu.klikhost.com:8052/stream"}
  ,{"Suara Surabaya"," http://c5.siar.us:8000/stream"}
  ,{"TOPFM 95.1"," http://streaming.topfm951.net:9510/stream"}
  ,{"Trax 101.4 FM Jakarta"," http://n07.radiojar.com/rrqf78p3bnzuv.mp3?rj-tok=AAABi_8o08gA5jtQz3StVoSgZA&rj-ttl=5"}
  ,{"V Radio 106 6 FM Jakarta  "," http://202.147.199.100:8000/;stream.nsv"}

};
int i = 1, indxst = 1, jml = sizeof(url)/sizeof(url[0]), volume = 10;
int sss = sizeof(url[0]);
String SendHTML() {
  

  String mpage = "<!DOCTYPE html>\n";
  mpage += "<html>\n";
  mpage += "<body style=font-size:20px;>\n";
  mpage += "<form class='form-horizontal' action='/action_page' method='post'>\n";
  mpage += "<fieldset>\n";

  mpage += "<legend style=font-size:60px;>Ipin FM</legend>\n";
  mpage += "<span class='font-weight-bold'>Radio Name : "+ url[i][0] +"</span><br>\n";
  mpage += "<span class='font-weight-bold'>Link : "+ url[i][1] +"</span><br>\n";
  mpage += "<span class='font-weight-bold'>Volume : "+ String((volume)/5) +" (0-20)</span><br><p></p>\n";
  
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
  for(int ii = 0; ii<jml;ii++){
      mpage += "    <option value='"+String(ii)+"'>"+url[ii][0]+"</option>\n";
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
    server.sendHeader("Location", "/", true);
    server.send(302, "text/html", SendHTML());
  }
  if (vp == "volplus") {
    volume += 5;
    if (volume >= 100) volume = 100;
    audio.setVolume(volume);
    server.sendHeader("Location", "/", true);
    server.send(302, "text/html", SendHTML());
  }
  if (sb == "submit") {
    i = sl;
    audio.connecttohost(url[i][1].c_str());
    server.sendHeader("Location", "/", true);
    server.send(302, "text/html", SendHTML());
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.print("IP ; "); Serial.println(WiFi.localIP());

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume); // 0...21
  audio.connecttohost(url[i][1].c_str());
  audio.setTone(6, 3, 0); //-40 - 6
  delay(100);
  Serial.print("ESP32 IP on the WiFi network: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here
  server.onNotFound(handleRoot);
  server.begin();
  delay(100); 


}

void loop(){
  server.handleClient();
  audio.loop();

}

// optional
void audio_info(const char *info) {
  Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info) { //id3 metadata
  Serial.print("id3data     "); Serial.println(info);
}
void audio_eof_mp3(const char *info) { //end of file
  Serial.print("eof_mp3     "); Serial.println(info);
}
void audio_showstation(const char *info) {
  Serial.print("station     "); Serial.println(info);
}
void audio_showstreaminfo(const char *info) {
  Serial.print("streaminfo  "); Serial.println(info);
}
void audio_showstreamtitle(const char *info) {
  Serial.print("streamtitle "); Serial.println(info);
}
void audio_bitrate(const char *info) {
  Serial.print("bitrate     "); Serial.println(info);
}
void audio_commercial(const char *info) { //duration in sec
  Serial.print("commercial  "); Serial.println(info);
}
void audio_icyurl(const char *info) { //homepage
  Serial.print("icyurl      "); Serial.println(info);
}
void audio_lasthost(const char *info) { //stream URL played
  Serial.print("lasthost    "); Serial.println(info);
}
void audio_eof_speech(const char *info) {
  Serial.print("eof_speech  "); Serial.println(info);
}
