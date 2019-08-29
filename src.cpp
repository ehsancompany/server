/*
 * src.cpp
 *
 *  Created on: May 2, 2019
 *      Author: ehsancompany
 */

#include "general.h"
#include <Wire.h>

//extern Fnger_r308 finger;
extern WiFiUDP Udp;

extern char rx_buffer1[255];  // buffer for incoming packets
char  tx_buffer1[30];
char  tx_buffer_tb[30];



unsigned char   esp_num;
unsigned char  parametr_num;
unsigned int parametr_data;
bool state_online_node_1=true;
bool state_online_node_2=true;
bool state_online_node_3=true;

bool state_pin_2_trig=true;

bool pin_1_node_[100];
//bool pin_1_node_2=false;

IPAddress ip_tab1(192,168,0,101);
unsigned int port_tab1=0;

IPAddress ip_node1(192,168,0,111);
IPAddress ip_node2(192,168,0,112);
IPAddress ip_node3(192,168,0,113);
IPAddress broadcast(192,168,0,255);



unsigned int error_node_1_counter;
unsigned int error_node_2_counter;
unsigned int error_node_3_counter;

bool delayRunning_sampling;
bool delayRunning_tab=true;
unsigned long delayStart_tab;
unsigned long delayStart_sampling;


char data_to_send_to_tablet[10];

unsigned char counter_online_node_1;
unsigned char counter_offline_node_1;
unsigned char counter_online_node_2;
unsigned char counter_offline_node_2;
unsigned char counter_online_node_3;
unsigned char counter_offline_node_3;
//+++++++++++save time parameters++++++++++++++++++++
extern RTC_DS1307 rtc;
extern DateTime now;
unsigned char last_state_date;
bool init_node1=true; bool work=false; bool stdby=false;

unsigned long sec_begin_work[50];unsigned long sec_end_work[50];
unsigned long min_begin_work[50];unsigned long min_end_work[50];
unsigned long hr_begin_work[50];unsigned long hr_end_work[50];

unsigned long sec_begin_stdby[50];unsigned long sec_end_stdby[50];
unsigned long min_begin_stdby[50];unsigned long min_end_stdby[50];
unsigned long hr_begin_stdby[50];unsigned long hr_end_stdby[50];



unsigned long tot_work[50]; unsigned long convert_hr_work[50];unsigned long convert_min_work[50];unsigned long convert_sec_work[50];
unsigned long tot_stdby[50];unsigned long convert_hr_stdby[50];unsigned long convert_min_stdby[50];unsigned long convert_sec_stdby[50];


//+++++++++++++sd process+++++++++++++++++++++
extern File myFile;
extern bool sd_write_permission;




void reciveing_udp_and_send_verify()
{
	int packetSize = Udp.parsePacket();
	if (packetSize)
	{
		delayStart_tab=millis();
		delayRunning_tab=true;
		//		Serial.print("Remote: ");
		//
		//Serial.println(Udp.remoteIP());


		// receive incoming UDP packets

		//Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());

		int len = Udp.read(rx_buffer1, 255);

		//	Serial.println(rx_buffer1);


		if (len > 0)
		{
			rx_buffer1[len] = 0;
		}

		if(rx_buffer1[0]=='$'&&rx_buffer1[1]=='X' && rx_buffer1[10]=='&' )
		{

			int8 cksum= rx_buffer1[2]^rx_buffer1[3]^rx_buffer1[4]^rx_buffer1[5]^rx_buffer1[6]^rx_buffer1[7]^rx_buffer1[8]
																													   ^rx_buffer1[9];

			if((cksum+'0') == rx_buffer1[11])
			{
				esp_num=STRHEX_TO_VAL(rx_buffer1[2])*0x10+STRHEX_TO_VAL(rx_buffer1[3]);
				parametr_num=STRHEX_TO_VAL(rx_buffer1[4])*0x10+STRHEX_TO_VAL(rx_buffer1[5]);

				parametr_data=STRHEX_TO_VAL(rx_buffer1[6])*0x1000+STRHEX_TO_VAL(rx_buffer1[7])*0x100+
						STRHEX_TO_VAL(rx_buffer1[8])*0x10+STRHEX_TO_VAL(rx_buffer1[9]);
				//
				//				Serial.print("esp_num;");
				//
				//				Serial.println(esp_num);
				//				Serial.print("parametr_num;");
				//				Serial.println(parametr_num);
				//				Serial.print("IP: ");
				//				Serial.println(Udp.remoteIP());
				//				Serial.print("parametr_data:");
				//				Serial.println(parametr_data);

				if(Udp.remoteIP()==ip_node1){
					//						Serial.print("Error_node_1: ");
					//						Serial.println(error_node_1_counter);
					switch (parametr_data) {

					case VERIF_CODE_NODE_1:
						pin_1_node_[1]=parametr_num;
						state_online_node_1=true;
						break;

					case WORKING:
						state_online_node_1=true;
						sending_udp_protocol(2,10,1541,ip_node1);
						//Serial.println("node1_bk_pressed");
						digitalWrite(LED2,0);
						pin_1_node_[1]=1;
						data_to_send_to_tablet[0]=14;
						//========capture to sd=========
						if(init_node1){

							//++to peerform init func. once++
							init_node1=false;

							//++to update running start time++
							now = rtc.now();
							hr_begin_work[1]=now.hour();
							min_begin_work[1]=now.minute();
							sec_begin_work[1]=now.second();

							//++level permission, other states must put true ***
							work=false;stdby=true;

							//++to give ID to state++
							last_state_date=2;
							Serial.println("injaaaaaaaaaaaaaaaaaaaaaaaaaaa_init2");

						}
//						if(work){
//							if(last_state_date==1){
//
//								Serial.println("badesh injaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//
//								//+++register the last time sdby, to calculate duration+++
//								now = rtc.now();
//								hr_end_stdby[1]=now.hour();
//								min_end_stdby[1]=now.minute();
//								sec_end_stdby[1]=now.second();
//
//								//+++calculate_stdby_duration, do conversion++++
//								tot_stdby[1]=(3600*hr_end_stdby[1]+60*min_end_stdby[1]+sec_end_stdby[1])-(3600*hr_begin_stdby[1]+60*min_begin_stdby[1]+sec_begin_stdby[1]);
//								convert_hr_stdby[1]=tot_stdby[1]/3600;convert_min_stdby[1]=tot_stdby[1]%3600;
//								convert_sec_stdby[1]=convert_min_stdby[1]%60;convert_min_stdby[1]=convert_min_stdby[1]/60;
//
//								Serial.print("tot_stdby[1]");
//								Serial.println(tot_stdby[1]);
//								Serial.print("convert_hr_stdby[1]: ");
//								Serial.println(convert_hr_stdby[1]);
//								Serial.print("convert_min_stdby[1]: ");
//								Serial.println(convert_min_stdby[1]);
//								Serial.print("convert_sec_stdby[1]: ");
//								Serial.println(convert_sec_stdby[1]);
//								Serial.print("FROM: ");
//								Serial.print(hr_begin_stdby[1]);
//								Serial.print(":");
//								Serial.print(min_begin_stdby[1]);
//								Serial.print(":");
//								Serial.println(sec_begin_stdby[1]);
//								Serial.print("TO: ");
//								Serial.print(hr_end_stdby[1]);
//								Serial.print(":");
//								Serial.print(min_end_stdby[1]);
//								Serial.print(":");
//								Serial.println(sec_end_stdby[1]);
//
//								//								if(sd_write_permission){
//								//									write_time_to_sd(1,0,
//								//											convert_hr_stdby[1],convert_min_stdby[1],convert_sec_stdby[1],
//								//											hr_begin_stdby[1],min_begin_stdby[1],sec_begin_stdby[1],
//								//											hr_end_stdby[1],min_end_stdby[1],sec_end_stdby[1]);
//								//								}
//								hr_begin_work[1]=now.hour();
//								min_begin_work[1]=now.minute();
//								sec_begin_work[1]=now.second();
//
//								work=false;stdby=true;
//								last_state_date=2;
//							}
//						}
						//============end capture===========
						break;

					case STANDBY://work
						state_online_node_1=true;
						sending_udp_protocol(2,10,1561,ip_node1);
						//	Serial.println("node1_bk_release");
						digitalWrite(LED2,1);
						pin_1_node_[1]=0;
						data_to_send_to_tablet[0]=16;
						//========capture to sd=========
//						if(init_node1){
//							init_node1=false;
//							now = rtc.now();
//							hr_begin_stdby[1]=now.hour();
//							min_begin_stdby[1]=now.minute();
//							sec_begin_stdby[1]=now.second();
//							work=true;stdby=false;
//							last_state_date=1;
//							Serial.println("injaaaaaaaaaaaaaaaaaaaaaaaaaaa_init1");
//						}
//						if(stdby){
//							if(last_state_date==2){
//								now = rtc.now();
//								hr_end_work[1]=now.hour();
//								min_end_work[1]=now.minute();
//								sec_end_work[1]=now.second();
//
//								tot_work[1]=(3600*hr_end_work[1]+60*min_end_work[1]+sec_end_work[1])-(3600*hr_begin_work[1]+60*min_begin_work[1]+sec_begin_work[1]);
//								convert_hr_work[1]=tot_work[1]/3600;convert_min_work[1]=tot_work[1]%3600;
//								convert_sec_work[1]=convert_min_work[1]%60;convert_min_work[1]=convert_min_work[1]/60;
//								Serial.print("tot_work[1]");
//								Serial.println(tot_work[1]);
//								Serial.print("convert_hr_work[1]: ");
//								Serial.println(convert_hr_work[1]);
//								Serial.print("convert_min_work[1]: ");
//								Serial.println(convert_min_work[1]);
//								Serial.print("convert_sec_work[1]: ");
//								Serial.println(convert_sec_work[1]);
//								Serial.print("FROM: ");
//								Serial.print(hr_begin_work[1]);
//								Serial.print(":");
//								Serial.print(min_begin_work[1]);
//								Serial.print(":");
//								Serial.println(sec_begin_work[1]);
//								Serial.print("TO: ");
//								Serial.print(hr_end_work[1]);
//								Serial.print(":");
//								Serial.print(min_end_work[1]);
//								Serial.print(":");
//								Serial.println(sec_end_work[1]);
//
//
//
//								Serial.print("tot_work[1]");
//								Serial.println(tot_work[1]);
//								//
//								//	if(sd_write_permission){
//								//									write_time_to_sd(1,1,
//								//											convert_hr_work[1],convert_min_work[1],convert_sec_work[1],
//								//											hr_begin_work[1],min_begin_work[1],sec_begin_work[1],
//								//											hr_end_work[1],min_end_work[1],sec_end_work[1]);
//								//	}
//								Serial.println("injaaaaaaaaaaaaaaaaaaaaaaaaaaa_stdby");
//
//								//+++++++re-initilize value++++++++
//								hr_begin_stdby[1]=now.hour();
//								min_begin_stdby[1]=now.minute();
//								sec_begin_stdby[1]=now.second();
//
//								//+++++++flag-updates++++++++++++++
//								work=true;stdby=false;
//
//								//+++give id to each machine's state+++
//								last_state_date=1;
//							}
//						}
						//============end capture===========
						break;

					default:
						//	Serial.println("NOT DEFINED_node1!");
						//Serial.println(parametr_data);
						error_node_1_counter++;
						state_online_node_1=true;
						break;
					}
				}
				if(Udp.remoteIP()==ip_node2){
					//	Serial.print("Error_node_2: ");
					//	Serial.println(error_node_2_counter);
					switch (parametr_data) {
					case VERIF_CODE_NODE_2:
						pin_1_node_[2]=parametr_num;
						state_online_node_2=true;
						break;
					case WORKING:
						state_online_node_2=true;
						sending_udp_protocol(2,10,1541,ip_node2);
						//Serial.println("node2_bk_pressed");
						pin_1_node_[2]=1;
						data_to_send_to_tablet[2]=14;

						digitalWrite(LED1,0);
						break;
					case STANDBY:
						state_online_node_2=true;
						sending_udp_protocol(2,10,1561,ip_node2);
						//Serial.println("node2_bk_release");
						pin_1_node_[2]=0;
						data_to_send_to_tablet[2]=16;

						digitalWrite(LED1,1);
						break;
					default:
						//Serial.println("NOT DEFINED_node2!");
						//Serial.println(parametr_data);
						error_node_2_counter++;
						state_online_node_2=true;
						break;
					}
				}
				if(Udp.remoteIP()==ip_node3){
					//	Serial.print("Error_node_3: ");
					//	Serial.println(error_node_3_counter);
					switch (parametr_data) {

					case VERIF_CODE_NODE_3:
						pin_1_node_[3]=parametr_num;
						state_online_node_3=true;
						break;

					case WORKING:
						state_online_node_3=true;
						sending_udp_protocol(2,10,1541,ip_node3);
						//Serial.println("node3_bk_pressed");
						digitalWrite(LED3,0);
						pin_1_node_[3]=1;
						data_to_send_to_tablet[4]=14;
						break;

					case STANDBY:
						state_online_node_3=true;
						sending_udp_protocol(2,10,1561,ip_node3);
						//Serial.println("node3_bk_release");
						digitalWrite(LED3,1);
						pin_1_node_[3]=0;
						data_to_send_to_tablet[4]=16;
						//						data_to_send_to_tablet[2]=16;
						//						data_to_send_to_tablet[4]=16;
						//						data_to_send_to_tablet[6]=16;
						//						data_to_send_to_tablet[8]=16;
						break;

					default:
						//	Serial.println("NOT DEFINED_node3!");
						//Serial.println(parametr_data);
						error_node_3_counter++;
						state_online_node_3=true;
						break;
					}
				}
				rx_buffer1[255] = 0;//not sure about that, to reset the buffer
				parametr_data=0;
				//delay(1);
			}
		}
		if(rx_buffer1[0]=='t'&&rx_buffer1[1]=='a'){
			//Serial.println("tab was here");
			ip_tab1=Udp.remoteIP();
			//	Serial.println(ip_tab1);
			port_tab1=Udp.remotePort();
			//	Serial.println(port_tab1);
			if(ip_tab1!=ip_node1  || ip_tab1!=ip_node2 || ip_tab1!=ip_node3){
				//				delayStart_tab=millis();
				//				delayRunning_tab=true;
				Udp.beginPacket(ip_tab1,port_tab1);
				//Udp.write("IAMOK");
				Udp.write(data_to_send_to_tablet);
				Udp.endPacket();
			}
		}
	}
}
void sending_udp_protocol_tablet(unsigned char a, unsigned char b, unsigned int c)
{

	String cksumm;
	char ataa ;

	esp_num=a;
	parametr_num=b;
	parametr_data=c;


	//=============================================
	tx_buffer_tb[0]='#';
	tx_buffer_tb[1]='Y';

	tx_buffer_tb[2]=VAL_TO_STRHEX((esp_num & 0xf0)>>4);
	tx_buffer_tb[3]=VAL_TO_STRHEX((esp_num & 0xf));

	tx_buffer_tb[4]=VAL_TO_STRHEX((parametr_num & 0xf0)>>4);
	tx_buffer_tb[5]=VAL_TO_STRHEX((parametr_num & 0xf));

	tx_buffer_tb[6]=VAL_TO_STRHEX((parametr_data & 0xf000)>>12);
	tx_buffer_tb[7]=VAL_TO_STRHEX((parametr_data & 0xf00)>>8);
	tx_buffer_tb[8]=VAL_TO_STRHEX((parametr_data & 0xf0)>>4);
	tx_buffer_tb[9]=VAL_TO_STRHEX((parametr_data & 0xf));

	tx_buffer_tb[10]='%';


	//==========check-sum value========================
	ataa =   VAL_TO_STRHEX((parametr_data & 0xf000)>>12);
	ataa ^=  VAL_TO_STRHEX((parametr_data & 0xf00)>>8) ;
	ataa ^=  VAL_TO_STRHEX((parametr_data & 0xf0)>>4) ;
	ataa ^=  VAL_TO_STRHEX((parametr_data & 0xf));

	ataa ^=  VAL_TO_STRHEX((parametr_num & 0xf0)>>4) ;
	ataa ^=  VAL_TO_STRHEX((parametr_num & 0xf));

	ataa ^=  VAL_TO_STRHEX((esp_num & 0xf0)>>4) ;
	ataa ^=  VAL_TO_STRHEX((esp_num & 0xf));
	ataa+=48;
	tx_buffer_tb[11]=ataa;
	tx_buffer_tb[12]=13;
	tx_buffer_tb[13]=0;

	Udp.beginPacket(ip_tab1,port_tab1);
	Udp.write(tx_buffer_tb);
	Udp.endPacket();
}
void sending_udp_protocol_chk_online_node_1(unsigned char a, unsigned char b, unsigned int c){
	esp_num=a;
	parametr_num=b;
	parametr_data=c;
	transmit_protocol();
	Udp.beginPacket(ip_node1,6000);
	Udp.write(tx_buffer1);
	Udp.endPacket();
}
void sending_udp_protocol_chk_online_node_2(unsigned char a, unsigned char b, unsigned int c){
	esp_num=a;
	parametr_num=b;
	parametr_data=c;

	transmit_protocol();
	Udp.beginPacket(ip_node2,6000);
	Udp.write(tx_buffer1);
	Udp.endPacket();
}
void sending_udp_protocol_chk_online_node_3(unsigned char a, unsigned char b, unsigned int c){
	esp_num=a;
	parametr_num=b;
	parametr_data=c;

	transmit_protocol();
	Udp.beginPacket(ip_node3,6000);
	Udp.write(tx_buffer1);
	Udp.endPacket();
}
void transmit_protocol(){
	String cksum;
	char ata ;

	//=============================================
	tx_buffer1[0]='$';
	tx_buffer1[1]='X';

	tx_buffer1[2]=VAL_TO_STRHEX((esp_num & 0xf0)>>4);
	tx_buffer1[3]=VAL_TO_STRHEX((esp_num & 0xf));

	tx_buffer1[4]=VAL_TO_STRHEX((parametr_num & 0xf0)>>4);
	tx_buffer1[5]=VAL_TO_STRHEX((parametr_num & 0xf));

	tx_buffer1[6]=VAL_TO_STRHEX((parametr_data & 0xf000)>>12);
	tx_buffer1[7]=VAL_TO_STRHEX((parametr_data & 0xf00)>>8);
	tx_buffer1[8]=VAL_TO_STRHEX((parametr_data & 0xf0)>>4);
	tx_buffer1[9]=VAL_TO_STRHEX((parametr_data & 0xf));

	tx_buffer1[10]='&';



	ata =   VAL_TO_STRHEX((parametr_data & 0xf000)>>12);
	ata ^=  VAL_TO_STRHEX((parametr_data & 0xf00)>>8) ;
	ata ^=  VAL_TO_STRHEX((parametr_data & 0xf0)>>4) ;
	ata ^=  VAL_TO_STRHEX((parametr_data & 0xf));

	ata ^=  VAL_TO_STRHEX((parametr_num & 0xf0)>>4) ;
	ata ^=  VAL_TO_STRHEX((parametr_num & 0xf));

	ata ^=  VAL_TO_STRHEX((esp_num & 0xf0)>>4) ;
	ata ^=  VAL_TO_STRHEX((esp_num & 0xf));
	ata+=48;
	tx_buffer1[11]=ata;
	tx_buffer1[12]=13;
	tx_buffer1[13]=0;
}

void sending_udp_protocol(unsigned char a, unsigned char b, unsigned int c, IPAddress ipp)
{
	esp_num=a;
	parametr_num=b;
	parametr_data=c;

	transmit_protocol();
	Udp.beginPacket(ipp,Udp.remotePort());
	Udp.write(tx_buffer1);
	Udp.endPacket();

}
void write_time_to_sd(unsigned char node_num,unsigned long state,
		unsigned long hr_dur,unsigned long min_dur,unsigned long sec_dur,
		unsigned long hr_from,unsigned long min_from,unsigned long sec_from,
		unsigned long hr_to,unsigned long min_to, unsigned long sec_to){

	myFile = SD.open("thur3.txt", FILE_WRITE);

	// if the file opened okay, write to it:
	if (myFile) {
		myFile.print(node_num);
		myFile.print('-');
		myFile.print(state);
		myFile.print('-');
		myFile.print(hr_dur);myFile.print(':');myFile.print(min_dur);myFile.print(':');myFile.print(sec_dur);
		myFile.print('-');
		myFile.print(hr_from);myFile.print(':');myFile.print(min_from);myFile.print(':');myFile.print(sec_from);
		myFile.print('-');
		myFile.print(hr_to);myFile.print(':');myFile.print(min_to);myFile.print(':');myFile.print(sec_to);
		myFile.println();





		// close the file:
		myFile.close();
		Serial.println("done writing");
	} else {
		// if the file didn't open, print an error:
		Serial.println("error opening test.txt");
	}
}

void send_udp_to_all_recivers()
{
	//    Udp.beginPacket("192.168.4.255",4450);
	//    Udp.write(replyPacket);
	//    Udp.endPacket();
}



