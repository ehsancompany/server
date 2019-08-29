#include "general.h"


const char* ssid_station = "dlink";
const char* password_station = "";

//const char* ssid_station = "FTR_PLN_1";
//const char* password_station = "12345678";

const char* ssid_esp = "_DS_CNC_1";
const char* password_esp = "12345678";


char rx_buffer1[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back

unsigned char ctr_time_20second;

WiFiUDP Udp;
//SoftwareSerial espSerial(D2, D1);
extern RTC_DS1307 rtc;
extern DateTime now;

//Fnger_r308 finger = Fnger_r308(&espSerial);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++     all_init()      ++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void all_init()
{
	serial_init();
	station_wifi_init();
	try_to_connect_station();
	//soft_ap_init();
	udp_config();
	init_RTC();
}
//+++++++++++++++++++++++++++++++++++++++++++++++
void init_RTC()
{
#ifndef ESP8266
	while (!Serial); // for Leonardo/Micro/Zero
#endif
	//Serial.begin(115200);
	if (! rtc.begin()) {
		Serial.println("Couldn't find RTC/ 1wire problem!");
		while (1);
	}
	else{
		Serial.println("1wire connected");
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

	}

	if (! rtc.isrunning()) {
		Serial.println("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
	//	rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++
void calculate_RTC(){
	now = rtc.now();
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println();
	//delay(1000);
}
//++++++++++++++++++++++++++++++++++++++++++++++++
void serial_init()
{
	Serial.begin(115200);
	Serial1.begin(115200);

#ifdef __debug__
	Serial.println("\n\r\n\r serial init");
#endif
}
//++++++++++++++++++++++++++++++++++++++++++++++++
void station_wifi_init()
{

	IPAddress ip(192,168,0,200);
	IPAddress gateway(192,168,0,1);
	IPAddress subnet(255,255,255,0);
	WiFi.config(ip, gateway, subnet);
	WiFi.begin(ssid_station, password_station);



#ifdef __debug__

	Serial.println("station config done");
#endif
}
//++++++++++++++++++++++++++++++++++++++++++++++++
void try_to_connect_station()
{
#ifdef __debug__
	Serial.printf("Connecting to %s", ssid_station);
#endif
	while (++ctr_time_20second < 200)
	{
		delay(100);
#ifdef __debug__
		Serial.print(".");
#endif

		if (WiFi.status() == WL_CONNECTED)
		{
			break;
		}
	}
	if (ctr_time_20second == 200)
	{
#ifdef __debug__
		Serial.println("ssid_station NNOOTT CONNECTED!!!1");
#endif
	}
	else
	{
#ifdef __debug__
		Serial.println("ssid_station connectd");
#endif
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++
void soft_ap_init()
{
	//WiFi.softAP(ssid_esp,password_esp);
	// softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
	//	WiFi.softAP(ssid_esp,password_esp,1,0,4);
	//WiFi.softAP(ssid_esp,password_esp);


#ifdef __debug__
	Serial.println("software_AP_init_done");
#endif
}
//++++++++++++++++++++++++++++++++++++++++++++++++

void udp_config()
{
	Udp.begin(localUdpPort);
#ifdef __debug__
	Serial.printf("Now listening  UDP port %d\n", localUdpPort);
#endif

}

//+++++++++++++++++++++------------------------------------------ STRHEX_TO_VAL ---------------------------------++++++++++++++++++
unsigned char STRHEX_TO_VAL(unsigned char SH)
{
	if(SH>'9')
	{
		return (SH-55);
	}

	else if(SH>='0')
	{
		return (SH-'0');
	}
	else
	{
		return (0);
	}
}

//+++++++++++++++++++++------------------------------------------ VAL_TO_STRHEX ---------------------------------++++++++++++++++++
unsigned char VAL_TO_STRHEX(unsigned char SH)
{
	if(SH>9)
	{
		return (SH+55);
	}

	else if(SH>=0)
	{
		return (SH+'0');
	}
	else
	{
		return (0);
	}
}

