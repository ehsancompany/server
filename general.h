#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//#include "Fngerr308.h"
#include<SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

#define __debug__

//#define __SD_SUPORT__
//#define __USB_SUPORT__
//#define __SPEEKER_SUPORT__
//#define __SIM808__
//#define __R308_FINGERPRINT__

#define localUdpPort 6000

#define CS_PIN  D8
#define LED1	D3
#define LED2	D4
#define LED3	D0



//+++++++++++state machine++++++++++++++++++
#define STANDBY 1651
#define WORKING 1451

#define WORKING_RESP 1541
#define STANDBY_RESP 1561
//++++++++++needed codes to change++++++++++
#define VERIF_CODE_SERVER_1  1754 //ke server back mizand
#define VERIF_CODE_NODE_1 1574  //ke node1 ersal mikonad

#define VERIF_CODE_SERVER_2  1755
#define VERIF_CODE_NODE_2 1575

#define VERIF_CODE_SERVER_3  1756
#define VERIF_CODE_NODE_3 1576

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
void all_init();
void serial_init();
void station_wifi_init();
void try_to_connect_station();
void soft_ap_init();
void udp_config();
void transmit_protocol();
void init_RTC();
//------------------------------------------------------
void calculate_RTC();
//-------------------------------------------------------
uint8_t getFingerprintID();
int getFingerprintIDez();
void check_node_1_state();
//void send_tablet_timer();
void write_time_to_sd(unsigned char node_num,unsigned long state,
		unsigned long hr_dur,unsigned long min_dur,unsigned long sec_dur,
		unsigned long hr_from,unsigned long min_from,unsigned long sec_From,
		unsigned long hr_to,unsigned long min_to, unsigned long sec_to);
//++++++++++
void sending_udp_protocol(unsigned char a, unsigned char b, unsigned int c, IPAddress ipp);
void sending_udp_protocol_chk_online_node_1(unsigned char a, unsigned char b, unsigned int c);
void sending_udp_protocol_chk_online_node_2(unsigned char a, unsigned char b, unsigned int c);
void sending_udp_protocol_chk_online_node_3(unsigned char a, unsigned char b, unsigned int c);
void sending_udp_protocol_tablet(unsigned char a, unsigned char b, unsigned int c);

void send_to_tablet_timer();
void send_udp_to_all_recivers();
void reciveing_udp_and_send_verify();

unsigned char STRHEX_TO_VAL(unsigned char SH);
unsigned char VAL_TO_STRHEX(unsigned char SH);

//===================================
void sampling_period_node_1();
