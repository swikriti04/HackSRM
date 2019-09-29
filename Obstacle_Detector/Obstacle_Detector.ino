


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define TRIGGER_PIN  2
#define ECHO_PIN     0

WiFiClient client;
ESP8266WebServer server(80);

String MakerIFTTT_Key;
String MakerIFTTT_Event;
char *append_str(char *here, String s) { int i = 0; while (*here++ = s[i]) { i++; }; return here - 1; }
char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10)); }
char post_rqst[256]; 
char *p; 
char *content_length_here; 
char *json_start; int compi;

String makerKey = "bTybN0oWUtQWIi4RYu06fouxVNelwLwM7brXXi8X6q"; // Add your IFTTT key here
String makerEventName = "blind_guy"; // Add your event's name
bool isConnected = false;

char* ssid = "HackSRM-2019"; // Add wifi id
char* pwd = "TFest#2019"; // Add wifi password
String sms = "00919581570571"; // Enter the no. on which you want this software to send message
char* smsMessage = "Beware there is an obstacle ahead.";

const int wifiConnectTimeout = 40;
const int MaxReadings = 20;


void setup()
{
  Serial.begin (9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);    

	WiFi.disconnect();
	delay(3000);
	Serial.println(F("START"));

	ConnectToNetwork(ssid, pwd);

}


bool ConnectToNetwork(char* ssidName, char* Password) // This part will try to connect the wifi
{	
	isConnected = false;
	
	WiFi.begin(ssidName, Password);
	
	Serial.println("trying to connect");
	
	int connectTries = 0;

	while (!(WiFi.status() == WL_CONNECTED))
	{		
		Serial.print("#");
		delay(1000);
		
		connectTries += 1;

		if (connectTries == wifiConnectTimeout)
		{
			break;
		}
	}

	Serial.println("");

	Serial.println(WiFi.status() == 3 ? "Connected to WiFi network." : "Could not connect to WiFi.");
	
	if (WiFi.status() == 3)
	{
		isConnected = true;

		SendSMS("Connected to WIFI network");
	}
	else
	{
		isConnected = false;
	}

	Serial.println(WiFi.localIP());
	
	return isConnected;
}

void loop()
{
                                  // this whole part will calculate the distance and triggers message
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);

  if (distance<= 100)
   delay(20);
}

void SendSMS(char* Message)
{	
	if (client.connect("maker.ifttt.com", 80))
	{
		MakerIFTTT_Key = makerKey;
		MakerIFTTT_Event = makerEventName;
		p = post_rqst;
		p = append_str(p, "POST /trigger/");
		p = append_str(p, MakerIFTTT_Event);
		p = append_str(p, "/with/key/");
		p = append_str(p, MakerIFTTT_Key);
		p = append_str(p, " HTTP/1.1\r\n");
		p = append_str(p, "Host: maker.ifttt.com\r\n");
		p = append_str(p, "Content-Type: application/json\r\n");
		p = append_str(p, "Content-Length: ");
		content_length_here = p;
		p = append_str(p, "NN\r\n");
		p = append_str(p, "\r\n");
		json_start = p;
		p = append_str(p, "{\"value1\":\"");
		p = append_str(p, sms);
		p = append_str(p, "\",\"value2\":\"");
		p = append_str(p, "");
		p = append_str(p, "\",\"value3\":\"");
		p = append_str(p, Message);
		p = append_str(p, "\"}");

		compi = strlen(json_start);
		content_length_here[0] = '0' + (compi / 10);
		content_length_here[1] = '0' + (compi % 10);
		client.print(post_rqst);
		client.flush();
		Serial.println("Sent...");		
		p = "";
	}
}
