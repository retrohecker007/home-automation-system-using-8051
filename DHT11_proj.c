#include<reg52.h>
#include<intrins.h>


void delay_ms(unsigned char x);


sbit DHT11=P1^1;

sbit rs = P3^5;
sbit rw = P3^4;
sbit en = P3^3;

int threshold=30;

sbit relay1=P1^7;

int lower=24;

sbit relay2=P1^6;

void welcome();


void cmd(unsigned char);
void dis(unsigned char);
void delay(unsigned int);

int int_humid,int_temp,int_humiddec,int_tempdec,chksum;

void delay_40us(){
    _nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();_nop_();
}


void  request(){

    
		DHT11=0;
		delay_ms(18);
    DHT11=1;
    
}


void response() {
		
		while(DHT11==1);
		while(DHT11==0);
		while(DHT11==1);
	
}

int recieve(){
    int i,data_DHT=0;
    for(i=0;i<8;i++){
        while(DHT11==0);
        delay_40us();
        if(DHT11==1)
            data_DHT=(data_DHT<<1) | 0x01;
        else
            data_DHT=(data_DHT<<1);
        while(DHT11==1);
    }
    return data_DHT;
	}


void main(){
    
		welcome();
	
		relay1=0;
		relay2=0;
	
    P2 = 0x00;
    while(1){
			
      request();
      response();
      int_humid=recieve(); 
     int_humiddec=recieve();
       int_temp=recieve();
      int_tempdec=recieve();
       chksum=recieve();
       
        if (((int_humid + int_humiddec + int_temp + int_tempdec) & 0xff) != chksum) {
            // checksum error
            continue;
        }
        
        cmd(0x38);
        delay_ms(1);
        cmd(0x0F);
        delay_ms(1);
        cmd(0x01);
        delay_ms(1);
        cmd(0x10);
        delay_ms(1);
        cmd(0x0C);
        delay_ms(1);
        cmd(0x80);
        delay_ms(1);

        // display temperature
				dis('T');
				dis('e');
				dis('m');
				dis('p');
				dis(' ');
				dis(':');
				dis(' ');

				dis(int_temp/10 + '0');
				dis(int_temp%10 + '0');
				dis('.');
				dis(int_tempdec%10 + '0');
				
				dis('C');
				

				// set DDRAM address to beginning of second line
				cmd(0xC0);

				// display humidity
				dis('H');
				dis('u');
				dis('m');
				dis('i');
				dis('d');
				dis(':');
				dis(' ');
				dis(int_humid/10 + '0');
				dis(int_humid%10 + '0');
				dis('.');
				dis(int_humiddec%10 + '0');
				
				dis('%');
				
				dis(' ');
				
				if(int_temp>threshold){
					
					relay1=1;
					
				}
				else if(int_temp<lower){
				
					relay2=1;
				
				}
				
				else if(relay1==1 && int_temp<28){
				
				relay1=0;
				
				}
				else if(relay2==1 && int_temp>26){
				
					relay2=0;
				
				}
    
			  delay_ms(500); // wait for 500ms before updating values
}

}

void cmd(unsigned char value){
P2=value;
rs=0;
rw=0;
en=1;
delay(1);
en=0;
}

void dis(unsigned char value) {
  P2=value;
	rs=1;
	rw=0;
	en=1;
	delay(1);
	en=0;
}



void delay(unsigned int x){
unsigned int i,j;
for(i=0;i<x;i++){
for(j=0;j<1275;j++);
}
}

void delay_ms(unsigned int x){
    unsigned int i,j;
    for(i=0;i<x;i++){
        for(j=0;j<12000;j++){
            
					
        }
    }
}

 void welcome(){
 
	 cmd(0x0F);
	 delay_ms(1);
	 cmd(0X01);
	 delay_ms(1);
	 cmd(0x0E);
	 delay_ms(1);
	 dis('w');
	 dis('e');
	 dis('l');
	 dis('c');
	 dis('o');
	 dis('m');
	 dis('e');
	 
	 delay_ms(1);
	 	 
	 delay(10);
	 
 }   



