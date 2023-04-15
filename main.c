#include "main.h"

#define     LED     PORTD
#define     ON      1
#define     OFF     0
#define     SET_TIME     3
#define     WEEK    10
#define     DAY     11
#define     MONTH   12
#define     YEAR    20
#define     HOUR    21
#define     MINUTE  22
#define     SECOND  23
#define     ALM     24
#define     AHOUR   35
#define     AMINUTE 36
#define     ASECOND 37

unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
int h = 9,m = 0,s = 0, alm = 0;
int d = 1, y = 0;
int h1 = 9,m1 = 0,s1 = 0;
int k = 20;

void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);

//Bai tap chong rung nut nhan
char numberOfPushButton = 0;
unsigned char isButtonUp(int index);
unsigned char isButtonDown();
unsigned char status = OFF;
unsigned char status1 = OFF;
unsigned char setweek = 81;
unsigned char setmonth = 51;
unsigned char PRINT = 0;
void BaiTap_ChongRung();
void Kiem_tra();
void Kiem_tra2();
void SetTime();
void Print_Alm();
void Print_time();
void Bao_thuc();
void Nhacantoi();
void Baitap_Dongho();
void Test_KeyMatrix();

void main(void)
{
	init_system();
    LcdPrintString(0,0,"Xin chao cac ban");
    LcdPrintString(1,0,"Tui la dong ho!");
    delay_ms(1000);
	while (1)
	{
        while (!flag_timer3);
        flag_timer3 = 0;
        //scan_key_button();
        scan_key_matrix();
//        Test_KeyMatrix();


        SetTime();
        k--;
        if(!k)
        {
            k=20;
            Baitap_Dongho();
        }
        
        DisplayLcdScreen();
    }
}

void delay_ms(int value)
{
	int i,j;
	for(i = 0; i < value; i++)
		for(j = 0; j < 160; j++);
}

void init_system(void)
{
    TRISB = 0x00;		//setup PORTB is output
    TRISD = 0x00;
    init_lcd();
    LcdClearS();
    LED = 0x00;
    init_interrupt();
    delay_ms(1000);
    init_timer0(4695);  //dinh thoi 1ms
    init_timer1(9390);  //dinh thoi 2ms
    init_timer3(46950); //dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer1_ms(10);
    SetTimer3_ms(50);   //Chu ky thuc hien viec xu ly input,proccess,output
    init_key_matrix();
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED | arrayMapOfOutput[index];
	}
}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int i;
	for (i = 0; i <= 7 ; i++ )
	{
		OpenOutput(i);
		delay_ms(500);
		CloseOutput(i);
		delay_ms(500);
	}
}

unsigned char isButtonUp(int index)
{
    if (key_code[index] == 1 || (key_code[index] >= 20 && key_code[index]%2 == 1))
        return 1;
    else
        return 0;
}

unsigned char isButtonPress(int index)
{
    if (key_code[index] == 1 || (key_code[index] >= 60 && key_code[index]%2 == 1))
        return 1;
    else
        return 0;
}

unsigned char isButtonDown()
{
    if (key_code[5] == 1)
        return 1;
    else
        return 0;
}

void BaiTap_ChongRung()
{
    if (isButtonUp(8) == 1)
        numberOfPushButton ++;
    if (isButtonDown())
        numberOfPushButton --;
 
    LcdPrintStringS(0,0,"    ");
    LcdPrintNumS(0,0,numberOfPushButton);
}

void Kiem_tra()
{
    m = m + s/60;
    h = h + m/60;
    d = d + h/24;
    if(d == 32 && setmonth < 62)
    {
        d = 1;
        setmonth++;
    }
    if(d == 32 && setmonth == 62)
    {
        d = 1;
        setmonth = 51;
        y++;
    }
    if(y >= 100)
    {
        y = y%100;
    }
    if(s >=60)
    {
        s = s%60;
    }
    if(m >=60)
    {
        m = m%60;
    }
    if(h >=24)
    {
        h = h%24;
    }
}

void Kiem_tra2()
{
    if(d >= 29 && setmonth == 52 && (y %4 != 0 || y ==0))
    {
        d = 1;
        setmonth++;
    }
    if(d >= 30 && setmonth == 52 && y != 0 && y %4 == 0)
    {
        d = 1;
        setmonth++;
    }
    if(d >= 31 && (setmonth == 54 || setmonth == 56 || setmonth == 59 || setmonth == 61))
    {
        d = 1;
        setmonth++;
    }
}

void Print_week()
{
    switch(setweek)
    {
        case 81: //SUN
            LcdPrintStringS(0,0,"SUN");
            break;
        case 82: //MON
            LcdPrintStringS(0,0,"MON");
            break;
        case 83:
            LcdPrintStringS(0,0,"TUS");
            break;
        case 84:
            LcdPrintStringS(0,0,"WED");
            break;
        case 85:
            LcdPrintStringS(0,0,"THU");
            break;
        case 86:
            LcdPrintStringS(0,0,"FRI");
            break;
        case 87:
            LcdPrintStringS(0,0,"SAT");
            break;    
    }
}

void Print_month()
{
    switch(setmonth)
    {
        case 51: //SUN
            LcdPrintStringS(1,5,"JAN");
            break;
        case 52: //MON
            LcdPrintStringS(1,5,"FEB");
            break;
        case 53:
            LcdPrintStringS(1,5,"MAR");
            break;
        case 54:
            LcdPrintStringS(1,5,"APR");
            break;
        case 55:
            LcdPrintStringS(1,5,"MAY");
            break;
        case 56:
            LcdPrintStringS(1,5,"JUN");
            break;
        case 57:
            LcdPrintStringS(1,5,"JUL");
            break;  
        case 58:
            LcdPrintStringS(1,5,"AUG");
            break;
        case 59:
            LcdPrintStringS(1,5,"SEP");
            break;
        case 60:
            LcdPrintStringS(1,5,"OCT");
            break;
        case 61:
            LcdPrintStringS(1,5,"NOV");
            break;
        case 62:
            LcdPrintStringS(1,5,"DEC");
            break;
    }
}
    
void SetTime()
{
    switch(status)
    {
        case OFF:
            Kiem_tra2();
            if(isButtonUp(0))
            {
                status = WEEK;
            }
            break;
        case WEEK:
            if(k<10)
            {
                LcdPrintStringS(0,0,"   ");
            }
            if(isButtonUp(0))
            {
                status = HOUR;
            }
            if(isButtonUp(1))
            {
                if(setweek == 87)
                    setweek = 81;
                else
                    setweek++;
            }
            break;
        case HOUR:
            if(k<10)
            {
                LcdPrintStringS(0,4,"  ");
            }
            if(isButtonUp(0))
            {
                status = MINUTE;
            }
            if(isButtonUp(1))
            {
                h++;
                h = h%24;
            }
            break;
        case MINUTE:
            if(k<10)
            {
                LcdPrintStringS(0,7,"  ");
            }
            if(isButtonUp(0))
            {
                status = SECOND;
            }
            if(isButtonUp(1))
            {
                m++;
                m = m%60;
            }
            break;
        case SECOND:
            if(k<10)
            {
                LcdPrintStringS(0,10,"  ");
            }
            if(isButtonUp(0))
            {
                status = ALM;
            }
            if(isButtonUp(1))
            {
                s++;
                s = s%60;
            }
            break;
        case ALM:
            if(k<10)
            {
                LcdPrintStringS(0,13,"   ");
            }
            if(isButtonUp(1))
            {
                if(alm)
                {
                    alm = 0;
                }
                else
                    alm = 1;
            }
            if(isButtonPress(0))
            {
                status = AHOUR;
            }
            else if(isButtonUp(0))
            {
                status = DAY;
            }
            break;
        case AHOUR:
            PRINT = 1;
            if(k<10)
            {
                LcdPrintStringS(0,3," ");
            }
            if(isButtonUp(1))
            {
                h1++;
                h1 = h1%24;
            }
            if(isButtonPress(0))
            {
                status = DAY;
            }
            else if(isButtonUp(0))
            {
                status = AMINUTE;
            }
            break;
        case AMINUTE:
            if(k<10)
            {
                LcdPrintStringS(0,6," ");
            }
            if(isButtonPress(0))
            {
                status = DAY;
            }
            else if(isButtonUp(0))
            {
                status = ASECOND;
            }
            if(isButtonUp(1))
            {
                m1++;
                m1 = m1%24;
            }
            break;
        case ASECOND:
            if(k<10)
            {
                LcdPrintStringS(0,9," ");
            }
            if(isButtonUp(0))
            {
                status = DAY;
            }
            if(isButtonUp(1))
            {
                s1++;
                s1 = s1%24;
            }    
            break;
        case DAY:
            PRINT = 0;
            if(k<10)
            {
                LcdPrintStringS(1,2,"  ");
            }
            if(isButtonUp(0))
            {
                status = MONTH;
            }
            if(isButtonUp(1))
            {
                d++;
                d = d%32;
            }
            break;
        case MONTH:
            if(k<10)
            {
                LcdPrintStringS(1,5,"   ");
            }
            if(isButtonUp(0))
            {
                status = YEAR;
            }
            if(isButtonUp(1))
            {
                if(setmonth == 62)
                    setmonth = 51;
                else
                    setmonth++;
            }
            break;
        case YEAR:
            if(k<10)
            {
                LcdPrintStringS(1,9,"    ");
            }
            if(isButtonUp(0))
            {
                status = OFF;
            }
            if(isButtonUp(1))
            {
                y++; 
                y = y %100;
            }
            break;

        default:
            status = OFF;
            break;
    }
}

void Print_time()
{
    s = s++;
    Kiem_tra();
    Print_week();
    if(h<10)
    {
        LcdPrintStringS(0,4,"0");
        LcdPrintNumS(0,5,h);
    }
    else
        LcdPrintNumS(0,4,h);
    LcdPrintStringS(0,6,":");
    if(m<10)
    {
        LcdPrintStringS(0,7,"0");
        LcdPrintNumS(0,8,m);
    }
    else
        LcdPrintNumS(0,7,m);
    LcdPrintStringS(0,9,":");
    if(s<10)
    {
        LcdPrintStringS(0,10,"0");
        LcdPrintNumS(0,11,s);
    }
    else
        LcdPrintNumS(0,10,s); 
    if(alm)
        LcdPrintStringS(0,13,"ON");   
    else
        LcdPrintStringS(0,13,"OFF"); 
    //cai bao thuc
    //bat bao thuc->set gio ->set phut->setgiay -> oke 
    if(d<10)
    {
        LcdPrintStringS(1,2,"0");
        LcdPrintNumS(1,3,d);
    }
    else
        LcdPrintNumS(1,2,d);
    LcdPrintStringS(1,4," ");
    Print_month();
    LcdPrintStringS(1,9,"20");
    if(y<10)
    {
        LcdPrintStringS(1,11,"0");
        LcdPrintNumS(1,12,y);
    }
    else
        LcdPrintNumS(1,11,y); 
}

void Print_Alm()
{
    LcdPrintStringS(0,0,"A");
    if(h1<10)
    {
        LcdPrintStringS(0,2,"0");
        LcdPrintNumS(0,3,h1);
    }
    else
        LcdPrintNumS(0,2,h1);
    LcdPrintStringS(0,4,":");   
    if(m1<10)
    {
        LcdPrintStringS(0,5,"0");
        LcdPrintNumS(0,6,m1);
    }
    else
        LcdPrintNumS(0,5,m1);
    LcdPrintStringS(0,7,":");
    if(s1<10)
    {
        LcdPrintStringS(0,8,"0");
        LcdPrintNumS(0,9,s1);
    }
    else
        LcdPrintNumS(0,8,s1);
}

void Nhacantoi()
{
    if(s == 0 && m == 1 && h == 9)
    {
        LcdPrintStringS(0,0,"TOI GIO DI NGU ROI!");
    }
}

void Baitap_Dongho()
{
    if(PRINT == 0)
    {
        LcdClearS();
        Print_time();
    }
    if(PRINT == 1)
    {
        LcdClearS();
        Print_Alm();
    }
    if( h==h1 && m==m1 && s <=10 && alm == 1)
    {
        LcdClearS();
        LcdPrintStringS(0,2,"BAO THUC!"); 
    }
}
void Test_KeyMatrix()
{
    int i;
    for (i = 0; i < 16; i++)
    {
        if(key_code[i] != 0)
            LcdPrintStringS(1,i,"_");
        else
            LcdPrintStringS(1,i," ");
    }
}