/*
 * isr.cpp
 *
 *  Created on: May 2, 2019
 *      Author: ehsancompany
 */


#include "general.h"

bool delayRunning=false;
unsigned long delayStart=0;
extern bool state_online_node_1;
extern bool state_online_node_2;
extern bool state_online_node_3;

extern bool pin_1_node_[100];
//1extern bool pin_1_node_2;

bool last_pin_1_server;

extern char rx_buffer1[255];  // buffer for incoming packets

extern bool delayRunning_sampling;
extern bool delayRunning_tab;
extern unsigned long delayStart_tab;
extern unsigned long delayStart_sampling;

extern IPAddress ip_tab1;
extern unsigned int port_tab1;

extern char data_to_send_to_tablet[10];

extern unsigned char counter_online_node_1;
extern unsigned char counter_offline_node_1;
extern unsigned char counter_online_node_2;
extern unsigned char counter_offline_node_2;
extern unsigned char counter_online_node_3;
extern unsigned char counter_offline_node_3;

extern WiFiUDP Udp;
extern RTC_DS1307 rtc;
extern DateTime now;


void sampling_period_node_1()
{
	if (((millis() - delayStart_sampling) >= 3000)) {
		delayStart_sampling=millis();
		now = rtc.now();
//				Serial.print(now.hour(), DEC);
//				Serial.print(':');
//				Serial.print(now.minute(), DEC);
//				Serial.print(':');
//				Serial.print(now.second(), DEC);
//				Serial.println();
		if ( rtc.isrunning()) {
//			Serial.println("Runningggg!");
//			Serial.println("++++++++work++++++++++");
//			Serial.print(hr_begin_stdby);
//			Serial.print(":");
//			Serial.print(min_begin_stdby);
//			Serial.print(":");
//			Serial.println(sec_begin_stdby);
//			Serial.println("==========");
//			Serial.print(hr_end_stdby);
//			Serial.print(":");
//			Serial.print(min_end_stdby);
//			Serial.print(":");
//			Serial.println(sec_end_stdby);
//			Serial.println("++++++++stdby++++++++++");
//			Serial.print(hr_begin_work);
//			Serial.print(":");
//			Serial.print(min_begin_work);
//			Serial.print(":");
//			Serial.println(sec_begin_work);
//			Serial.println("==========");
//			Serial.print(hr_end_work);
//			Serial.print(":");
//			Serial.print(min_end_work);
//			Serial.print(":");
//			Serial.println(sec_end_work);
//			Serial.print("WORK_TIME: ");
//			Serial.print(dif_hr_stdby);
//			Serial.print(":");
//			Serial.print(dif_min_stdby);
//			Serial.print(":");
//			Serial.println(dif_sec_stdby,DEC);
			//+++++++++++++++++++++++++++++++
//			Serial.print("STDBY_TIME: ");
//			Serial.print(dif_hr_work);
//			Serial.print(":");
//			Serial.print(dif_min_work);
//			Serial.print(":");
//			Serial.println(dif_sec_work,DEC);
		}
		else{
			init_RTC();
		}
	}
}

void send_to_tablet_timer()
{
	if (delayRunning_tab && ((millis() - delayStart_tab) >= 200)) {
		delayStart_tab=millis();
		//Serial.println("timer_tab_activated");
		//sending_udp_protocol_tablet(1,10,132);
		Udp.beginPacket(ip_tab1,port_tab1);
		Udp.write(data_to_send_to_tablet);
		Udp.endPacket();
	}
}

void check_node_1_state()
{
	if (delayRunning && ((millis() - delayStart) >= 500)) {
		delayStart=millis();

		//Serial.printf("\nConnection status: %d\n", WiFi.status());



		if(state_online_node_1){
			digitalWrite(LED2,!pin_1_node_[1]);
			counter_offline_node_1=0;
			counter_online_node_1++;
			//Serial.print("counter_NODE1_ONLINE: ");
			//Serial.println(counter_online_node_1);
			if(counter_online_node_1==1){
				counter_online_node_1=0;
				//Serial.println("NODE1_ONLINE");
				data_to_send_to_tablet[1]=1;
			}
		}
		if(!state_online_node_1){
			digitalWrite(LED2,!pin_1_node_[1]);
			counter_online_node_1=0;
			counter_offline_node_1++;
//			Serial.print("counter_NODE1_OFFLINE: ");
//			Serial.println(counter_offline_node_1);
			if(counter_offline_node_1==20){
				counter_offline_node_1=0;
				Serial.println("NODE1_OFFLINE");
				data_to_send_to_tablet[1]=2;
			}
		}

		if(state_online_node_2){
			digitalWrite(LED1,!pin_1_node_[2]);
			counter_offline_node_2=0;
			counter_online_node_2++;
			//Serial.print("counter_NODE2_ONLINE: ");
			//Serial.println(counter_online_node_2);
			if(counter_online_node_2==1){
				counter_online_node_2=0;
				//Serial.println("NODE2_ONLINE");
				data_to_send_to_tablet[3]=1;
			}
		}
		if(!state_online_node_2){
			digitalWrite(LED1,!pin_1_node_[2]);
			counter_online_node_2=0;
			counter_offline_node_2++;
			//Serial.print("counter_NODE2_OFFLINE: ");
			//Serial.println(counter_offline_node_2);
			if(counter_offline_node_2==20){
				counter_offline_node_2=0;
				Serial.println("NODE2_OFFLINE");
				data_to_send_to_tablet[3]=2;
			}
		}
		//==============================================
		if(state_online_node_3){
			digitalWrite(LED3,!pin_1_node_[3]);
			counter_offline_node_3=0;
			counter_online_node_3++;
			//Serial.print("counter_NODE3_ONLINE: ");
			//Serial.println(counter_online_node_3);
			if(counter_online_node_3==1){
				counter_online_node_3=0;
				//Serial.println("NODE3_ONLINE");
				data_to_send_to_tablet[5]=1;
			}
		}
		if(!state_online_node_3){
			digitalWrite(LED3,!pin_1_node_[3]);
			counter_online_node_3=0;
			counter_offline_node_3++;
			//	Serial.print("counter_NODE3_OFFLINE: ");
			//	Serial.println(counter_offline_node_3);
			if(counter_offline_node_3==20){
				counter_offline_node_3=0;
				Serial.println("NODE3_OFFLINE");
				data_to_send_to_tablet[5]=2;
			}
		}

		//Serial.println("===============================");
		//===================send to get nodes statues==============================
		sending_udp_protocol_chk_online_node_1(2,10,1754);//send each X ms to get node 1 states
		//delay();
		sending_udp_protocol_chk_online_node_2(2,10,1755);//send each X ms to get node 2 states
		//delay(1);
		sending_udp_protocol_chk_online_node_3(2,10,1756);//send each X ms to get node 2 states
		//delay(10);

		state_online_node_1=false;
		state_online_node_2=false;
		state_online_node_3=false;
	}
}




