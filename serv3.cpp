
#include "general.h"
#define CS_PIN  D8

int buttonState = 0;
int last_button_state=5;
char  tx_buffer11[10];
extern WiFiUDP Udp;
extern char rx_buffer1[255];  // buffer for incoming packets

extern bool delayRunning;
extern unsigned long delayStart;

extern char data_to_send_to_tablet[10];
bool sd_write_permission=false;
File myFile;
//RTC_DS1307 rtc;
//RTC_Millis rtc;
RTC_DS1307 rtc;
DateTime now;




void setup() {


	all_init();
	//SD.begin(CS_PIN);
//		if (!SD.begin(CS_PIN)) {
//			Serial.println("Card failed, or not present");
//			sd_write_permission=false;
//			while (1);
//		}
//		else{
//			sd_write_permission=true;
//		}
	pinMode(2,OUTPUT);
	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);
	pinMode(LED3,OUTPUT);



	delayRunning=true;
	delayStart=millis();

	//-----in broadcast mikone ta vagti server power on shod vaziyat node haro bgire----
	sending_udp_protocol_chk_online_node_1(2,10,1850);//send each X ms to get node 1 states
	//reciveing_udp_and_send_verify();
	//		sending_udp_protocol_chk_online_node_2(2,10,1850);//send each X ms to get node 2 states
	//		reciveing_udp_and_send_verify();
	//		sending_udp_protocol_chk_online_node_3(2,10,1850);//send each X ms to get node 3 states
	//		reciveing_udp_and_send_verify();

	//==============================

}

void loop() {
	//now = rtc.now();
	check_node_1_state();
	send_to_tablet_timer();
	reciveing_udp_and_send_verify();
	//calculate_RTC();
	sampling_period_node_1();
	// so you have to close this one before opening another.

}
