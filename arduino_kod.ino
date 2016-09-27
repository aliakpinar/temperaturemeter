#include <DHT.h>
#include <Ethernet.h>
#include <SPI.h>  
byte mac[] = { 0x08, 0x52, 0x76, 0x29, 0xAE, 0xCA }; 
IPAddress server1( 192,168,1,42); // bilgisayarımın yerel IP ‘ si
EthernetServer server(80);
EthernetClient client;
#define DHTPIN 2 // SENSOR PIN
#define DHTTYPE DHT11 // Daha fazla DHT modeli desteklemesi için
DHT dht(DHTPIN, DHTTYPE);
long previousMillis = 0;
unsigned long currentMillis = 0;
long interval = 250000; // OKUMA ARALIĞI
int t = 0;	// SICAKLIK DEĞERİ
int h = 0;	// NEM DEĞERİ
String data;
void setup() {   
	Serial.begin(115200);
	if (Ethernet.begin(mac) == 0) {
		Serial.println("DHCP kullanarak Ethernet yapılandırılamadı"); 
	}  
Serial.print(Ethernet.localIP());


	dht.begin(); 
	delay(300); 

	h = (int) dht.readHumidity(); 
	t = (int) dht.readTemperature(); 

	data = "";
}

void loop(){

	currentMillis = millis();
	if(currentMillis - previousMillis > interval) { // PERİYOT BAŞINA  BİR KEZ OKUMA
		previousMillis = currentMillis;
		h = (int) dht.readHumidity();
		t = (int) dht.readTemperature();
	}
Serial.println(t);
	data = "temp1=";

data.concat(t);

data.concat("&hum1=");

data.concat(h);
Serial.println(data);

	if (client.connect(server1,80)) {  //POST İÇİN BAĞLANTI AYARLARI
	Serial.print("connect");
	if (client.connected()) {    
	client.println("POST /sensor/add.php HTTP/1.1"); 
		client.println("HOST: 192.168.1.42"); // 
		client.println("Content-Type: application/x-www-form-urlencoded"); 
		client.print("Content-Length: "); 
		client.println(data.length()); 
		client.println(); 
		client.print(data); 
		client.stop();	//bağlantıyı durdur
Serial.println("stop");
	}
}
	delay(300); // yeniden veri göndermek için bekle
}

