
#include <mega128.h>
#include <stdio.h>
#include <string.h>
#include <delay.h> 
#include <stdlib.h>


#define    null         0                                                // C �� NULL �� (0)  

//�� NMEA_code 
unsigned char NAME[10]="$GPGGA";                             //NMEA _ GPGGA             
 
//UART GPS_DATA ���� �� �ǽ� 
unsigned char GPS_DATA[100];                                        // UART�� ���� ������ ���� 
unsigned char GPS_DATA1[100];                                      // GPGGA DATA  �ǽ� ����

// Count �� flag ����
int GPScnt;                                                                 // UART�� ���� ������ ���� COUNT
int GPScnt1;                                                                // UART COUNT �� �ǽ� ����
int GPSflag;         
int i;                                                                         

// ( , ) ���� ���� �����ϱ� ���� ������
char *GPS_cut=null;                                                        // STRTOK  �Լ� ����� ���� ����
char *GPS_Dcut[15]={null};                                               // GPGGA ���� ( , )  STRTOK ��  ������ ����

unsigned char test0[20];                                                //����
unsigned char test1[20];                                                //�浵



void TX1_CH(char ch){ while(!(UCSR1A&0x20)); UDR1=ch; }
void TX1_STR(char *str)                                                 // ���ڿ� ����
{
 while(*str)TX1_CH(*str++);
}


interrupt [USART0_RXC] void usart0_rx_isr(void)                     // USART0 interrupt
{
unsigned char RX;                                                         // UDR0  char data ���� ����
RX= UDR0
;                                                                        // UDR0  char ���� RX ������ ����
   
        if(RX == '$'){                                                        // GPS NMEA_code  [STX]
             GPS_DATA[0]='$';                                              // GPS_DATA[0] �� '$' ����
             GPScnt=1;                                                       // GPScnt  = 1 ����
             GPSflag=0;                                                       // GPSflag = 0 ����
             }  
         else {                                                                 
               GPS_DATA[GPScnt]=RX;                                          // char �� DATA RX �� GPS_DATA �迭�� ����
               if(GPS_DATA[GPScnt] == 0x0a) {                                // ������ ETX = 0x0A ��� �����Ƿ�  ������ �ɾ������.
                        GPScnt1=GPScnt;                                      // ������ �����Ǹ� COUNT �� ���� GPScnt1���� �ǽŽ�Ų��.  
                        if(strncmp(NAME,GPS_DATA,6)==0)  {                   //  �� �κ��� NAME = "$GPGGA" ���� ���̴�.  
                                        memmove(GPS_DATA1,GPS_DATA,GPScnt1);     // �޸� ����                        
                                        GPS_cut= strtok(GPS_DATA1,",");                   // STRTOK ���� ������ �Լ�
                                        for(i=0;GPS_cut!=null;i++){                           // ������ ������ 0�϶����� 
                                                        GPS_Dcut[i]=GPS_cut;                 // ������ ������ �迭�� ����    
                                                        GPS_cut=strtok(null,",");              // ���� ( , ) ������
                                        } 
                                        GPSflag=1;                                              // flag =1    
                        }  

               }                   
               GPScnt++;  
        }

}

void GPS_Latitude()                                                                   // �� ��                   
{
unsigned char DO,BOON;                                                                // ��, ��  ���� ���� 
double CHO;                                                                         // �� ���� ����
double Latitude;                                                                     // GPS_LATITUDE �� ���� ����
 
//atol() => LONG ��ȯ , atoi() => INT ��ȯ, atof() => DOUBLE ��ȯ
//���� ���ڳ� ��ȣ�� �̷���� ���ڿ��� �ش� Ÿ������ ��ȯ    
// $GPGGA,035155.00,3723.19464,N,12656.72748,E,1,06,1.69,47.0,M,18.5,M,,*6F ��� ����
// ���� �� ó�� ���� ���·� ���´�. �츮�� ���� �ʻ�� �񱳸� ���Ͽ� ������ �����ʷ� ��ȯ �Ѵ�.

        Latitude=atof(GPS_Dcut[2]);                                                // ���ڿ��� DOUBLE �� ���� ��ȯ�� ���� => " 3723.19464 "
        DO=Latitude/100;                                                            // �� ��ġ�� 37 ����
        BOON=(int)Latitude%100;                                                  // �� ��ġ�� 23 ����
        CHO=(Latitude-(int)Latitude)*60;                                          // �� �� ���ϱ� ���Ͽ� (3723.19464 - 3723) * 60
                                                                                          // �Ҽ����� ������ 60�� ���ϸ� �ʷ� ��ȯ �ȴ�.  
        sprintf(test0,"LATITUDE  : %2d'%2d`%0.02lf %s \n\r",DO,BOON,CHO,GPS_Dcut[3]); // ��,��,��,N    
        TX1_STR(test0);
        delay_ms(10);
}

void GPS_Longitude()                                                                // �� ��                               
{                                                                                          
unsigned char DO,BOON;                                                           // ��, ��  ���� ���� 
double CHO;                                                                           // �� ���� ����   
double Longitude;                                                                    // GPS_LATITUDE �� ���� ����  
 
//atol() => LONG ��ȯ , atoi() => INT ��ȯ, atof() => DOUBLE ��ȯ
//���� ���ڳ� ��ȣ�� �̷���� ���ڿ��� �ش� Ÿ������ ��ȯ    
// $GPGGA,035155.00,3723.19464,N,12656.72748,E,1,06,1.69,47.0,M,18.5,M,,*6F ��� ����
// �浵 �� ó�� ���� ���·� ���´�. �츮�� ���� �ʻ�� �񱳸� ���Ͽ� ������ �����ʷ� ��ȯ �Ѵ�. 
        
        Longitude=atof(GPS_Dcut[4]);                                               // ���ڿ��� DOUBLE �� ���� ��ȯ�� ���� =>" 12656.72748 "   
        DO=(int)Longitude/100;                                                      // �� ��ġ�� 126 ����
        BOON=((int)Longitude%100);                                                // �� ��ġ�� 56  ����
        CHO=(Longitude-(int)Longitude)*60;                                       // �� �� ���ϱ� ���Ͽ�  (12656.72748 - 12656) * 60
        sprintf(test1,"LONGITUDE :%3d'%02d`%0.02lf %s \n\r",DO,BOON,CHO,GPS_Dcut[5]); //��,��,��, E        
        TX1_STR(test1);
        delay_ms(10);
}

void Init_USART1(void)
{
    UCSR1B = 0x98; 
    UCSR1A = 0x00; //���� �������� A �ʱ�ȭ
    UCSR1C = 0x06; //USART ���� ���ں�Ʈ�� 8��Ʈ�� ����
    UBRR1H = 0x00;  
    UBRR1L = 0x33; // 19200bps ��Ʈ�ް� 16MHz �񵿱� usart ��� ������ �������� ���� ������ 0.2��  p404
}

void Init_USART0(void)
{
    UCSR0B = 0x98; 
    UCSR0A = 0x00; //���� �������� A �ʱ�ȭ
    UCSR0C = 0x06; //USART ���� ���ں�Ʈ�� 8��Ʈ�� ����
    UBRR0H = 0x00;  
    UBRR0L = 0x67; // 9600bps ��Ʈ�ް� 16MHz �񵿱� usart ��� ������ �������� ���� ������ 0.2��  p404
}


void main(void)
{
Init_USART1();
Init_USART0();
SREG |= 0x80;  //��ü ���ͷ�Ʈ �㰡

while(1){ 
// $GPGGA,035155.00,3723.19464,N,12656.72748,E,{1},06,1.69,47.0,M,18.5,M,,*6F ��� ����       
        if(GPSflag == 1 ){                                                                // flag == 1�̸� 
                        GPS_Latitude();                                                   //  ����
                        GPS_Longitude();                                                 // �浵
                        GPSflag=0;                                                         // flag = 0
        }        
};
}
