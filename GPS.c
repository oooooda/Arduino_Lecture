
#include <mega128.h>
#include <stdio.h>
#include <string.h>
#include <delay.h> 
#include <stdlib.h>


#define    null         0                                                // C 형 NULL 값 (0)  

//비교 NMEA_code 
unsigned char NAME[10]="$GPGGA";                             //NMEA _ GPGGA             
 
//UART GPS_DATA 저장 및 피신 
unsigned char GPS_DATA[100];                                        // UART로 받은 데이터 저장 
unsigned char GPS_DATA1[100];                                      // GPGGA DATA  피신 저장

// Count 및 flag 변수
int GPScnt;                                                                 // UART로 받은 데이터 저장 COUNT
int GPScnt1;                                                                // UART COUNT 값 피신 저장
int GPSflag;         
int i;                                                                         

// ( , ) 구간 별로 저장하기 위한 포인터
char *GPS_cut=null;                                                        // STRTOK  함수 사용을 위한 변수
char *GPS_Dcut[15]={null};                                               // GPGGA 값을 ( , )  STRTOK 로  나누어 저장

unsigned char test0[20];                                                //위도
unsigned char test1[20];                                                //경도



void TX1_CH(char ch){ while(!(UCSR1A&0x20)); UDR1=ch; }
void TX1_STR(char *str)                                                 // 문자열 전송
{
 while(*str)TX1_CH(*str++);
}


interrupt [USART0_RXC] void usart0_rx_isr(void)                     // USART0 interrupt
{
unsigned char RX;                                                         // UDR0  char data 저장 변수
RX= UDR0
;                                                                        // UDR0  char 값을 RX 변수에 저장
   
        if(RX == '$'){                                                        // GPS NMEA_code  [STX]
             GPS_DATA[0]='$';                                              // GPS_DATA[0] 에 '$' 저장
             GPScnt=1;                                                       // GPScnt  = 1 저장
             GPSflag=0;                                                       // GPSflag = 0 저장
             }  
         else {                                                                 
               GPS_DATA[GPScnt]=RX;                                          // char 형 DATA RX 를 GPS_DATA 배열에 저장
               if(GPS_DATA[GPScnt] == 0x0a) {                                // 위에서 ETX = 0x0A 라고 했으므로  조건을 걸어버린다.
                        GPScnt1=GPScnt;                                      // 조건이 성립되면 COUNT 된 값을 GPScnt1으로 피신시킨다.  
                        if(strncmp(NAME,GPS_DATA,6)==0)  {                   //  이 부분은 NAME = "$GPGGA" 비교한 것이다.  
                                        memmove(GPS_DATA1,GPS_DATA,GPScnt1);     // 메모리 복사                        
                                        GPS_cut= strtok(GPS_DATA1,",");                   // STRTOK 구간 나누는 함수
                                        for(i=0;GPS_cut!=null;i++){                           // 나누는 조건이 0일때까지 
                                                        GPS_Dcut[i]=GPS_cut;                 // 나눈값 포인터 배열에 저장    
                                                        GPS_cut=strtok(null,",");              // 구간 ( , ) 나누기
                                        } 
                                        GPSflag=1;                                              // flag =1    
                        }  

               }                   
               GPScnt++;  
        }

}

void GPS_Latitude()                                                                   // 위 도                   
{
unsigned char DO,BOON;                                                                // 도, 분  저장 변수 
double CHO;                                                                         // 초 저장 변수
double Latitude;                                                                     // GPS_LATITUDE 값 저장 변수
 
//atol() => LONG 변환 , atoi() => INT 변환, atof() => DOUBLE 변환
//순수 숫자나 부호로 이루어진 문자열을 해당 타입으로 변환    
// $GPGGA,035155.00,3723.19464,N,12656.72748,E,1,06,1.69,47.0,M,18.5,M,,*6F 출력 샘플
// 위도 는 처음 도분 형태로 들어온다. 우리는 구글 맵상과 비교를 위하여 도분을 도분초로 변환 한다.

        Latitude=atof(GPS_Dcut[2]);                                                // 문자열을 DOUBLE 형 으로 변환후 저장 => " 3723.19464 "
        DO=Latitude/100;                                                            // 도 위치인 37 저장
        BOON=(int)Latitude%100;                                                  // 분 위치인 23 저장
        CHO=(Latitude-(int)Latitude)*60;                                          // 초 를 구하기 위하여 (3723.19464 - 3723) * 60
                                                                                          // 소숫점만 빼내어 60을 곱하면 초로 변환 된다.  
        sprintf(test0,"LATITUDE  : %2d'%2d`%0.02lf %s \n\r",DO,BOON,CHO,GPS_Dcut[3]); // 도,분,초,N    
        TX1_STR(test0);
        delay_ms(10);
}

void GPS_Longitude()                                                                // 경 도                               
{                                                                                          
unsigned char DO,BOON;                                                           // 도, 분  저장 변수 
double CHO;                                                                           // 초 저장 변수   
double Longitude;                                                                    // GPS_LATITUDE 값 저장 변수  
 
//atol() => LONG 변환 , atoi() => INT 변환, atof() => DOUBLE 변환
//순수 숫자나 부호로 이루어진 문자열을 해당 타입으로 변환    
// $GPGGA,035155.00,3723.19464,N,12656.72748,E,1,06,1.69,47.0,M,18.5,M,,*6F 출력 샘플
// 경도 는 처음 도분 형태로 들어온다. 우리는 구글 맵상과 비교를 위하여 도분을 도분초로 변환 한다. 
        
        Longitude=atof(GPS_Dcut[4]);                                               // 문자열을 DOUBLE 형 으로 변환후 저장 =>" 12656.72748 "   
        DO=(int)Longitude/100;                                                      // 도 위치인 126 저장
        BOON=((int)Longitude%100);                                                // 분 위치인 56  저장
        CHO=(Longitude-(int)Longitude)*60;                                       // 초 를 구하기 위하여  (12656.72748 - 12656) * 60
        sprintf(test1,"LONGITUDE :%3d'%02d`%0.02lf %s \n\r",DO,BOON,CHO,GPS_Dcut[5]); //도,분,초, E        
        TX1_STR(test1);
        delay_ms(10);
}

void Init_USART1(void)
{
    UCSR1B = 0x98; 
    UCSR1A = 0x00; //상태 레지스터 A 초기화
    UCSR1C = 0x06; //USART 전송 문자비트를 8비트를 쓴다
    UBRR1H = 0x00;  
    UBRR1L = 0x33; // 19200bps 아트메가 16MHz 비동기 usart 통신 데이터 오차율이 제일 작은게 0.2퍼  p404
}

void Init_USART0(void)
{
    UCSR0B = 0x98; 
    UCSR0A = 0x00; //상태 레지스터 A 초기화
    UCSR0C = 0x06; //USART 전송 문자비트를 8비트를 쓴다
    UBRR0H = 0x00;  
    UBRR0L = 0x67; // 9600bps 아트메가 16MHz 비동기 usart 통신 데이터 오차율이 제일 작은게 0.2퍼  p404
}


void main(void)
{
Init_USART1();
Init_USART0();
SREG |= 0x80;  //전체 인터럽트 허가

while(1){ 
// $GPGGA,035155.00,3723.19464,N,12656.72748,E,{1},06,1.69,47.0,M,18.5,M,,*6F 출력 샘플       
        if(GPSflag == 1 ){                                                                // flag == 1이면 
                        GPS_Latitude();                                                   //  위도
                        GPS_Longitude();                                                 // 경도
                        GPSflag=0;                                                         // flag = 0
        }        
};
}
