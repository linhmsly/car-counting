#include<reg51.h>
#include<stdio.h>
#define lcdport P1

sbit rs = P1 ^ 0;
sbit en = P1 ^ 1;

sbit SDA = P2 ^ 1;
sbit SCL = P2 ^ 0;

sbit next = P2 ^ 2;                     //increment digit
sbit inc = P2 ^ 3;                     //increment value
sbit set = P2 ^ 4;                     //set time

char ack;
unsigned char day1 = 1;
unsigned char k, x;
unsigned int date = 1, mon = 1, hour = 0, min = 0, sec = 0;
int year = 0;

void delay(int itime)
{
    int i, j;
    for (i = 0; i < itime; i++)
        for (j = 0; j < 1275; j++);
}

void daten()
{
    rs = 1;
    en = 1;
    delay(1);
    en = 0;
}

void lcddata(unsigned char ch)
{
    lcdport = ch & 0xf0;
    daten();
    lcdport = (ch << 4) & 0xf0;
    daten();
}

void cmden(void)
{
    rs = 0;
    en = 1;
    delay(1);
    en = 0;
}

void lcdcmd(unsigned char ch)
{
    lcdport = ch & 0xf0;
    cmden();
    lcdport = (ch << 4) & 0xf0;
    cmden();
}

void lcdprint(char *str)
{
    while (*str)
    {
        lcddata(*str);
        str++;
    }
}

void lcd_init(void)
{
    lcdcmd(0x02);
    lcdcmd(0x28);
    lcdcmd(0x0c);
    lcdcmd(0x01);
}

void I2CStart() { SDA = 1; SCL = 1, SDA = 0, SCL = 0; }             //"start" function for communicate with ds1307 RTC
void I2CStop() { SDA = 0, SCL = 1, SDA = 1; }                     //"stop" function for communicate wit ds1307 RTC

unsigned char I2CSend(unsigned char Data)             //send data to ds1307
{
    char i;
    char ack_bit;
    for (i = 0; i < 8; i++)
    {
        if (Data & 0x80) SDA = 1;
        else SDA = 0;
        SCL = 1;
        Data <<= 1;
        SCL = 0;
    }
    SDA = 1, SCL = 1;
    ack_bit = SDA;
    SCL = 0;
    return ack_bit;
}

unsigned char I2CRead(char ack)                      //receive data from ds1307
{
    unsigned char i, Data = 0;
    SDA = 1;
    for (i = 0; i < 8; i++)
    {
        Data <<= 1;
        do { SCL = 1; } while (SCL == 0);
        if (SDA) Data |= 1;
        SCL = 0;
    }
    if (ack)
    {
        SDA = 0;
    }
    else
    {
        SDA = 1;
    }
    SCL = 1;
    SCL = 0;
    SDA = 1;
    return Data;
}

void day(char d)                                // Function for display day on LCD
{
    switch (d)
    {
    case 0:
        lcdprint("DAY");
        break;

    case 1:
        lcdprint("SUN");
        break;

    case 2:
        lcdprint("MON");
        break;

    case 3:
        lcdprint("TUE");
        break;

    case 4:
        lcdprint("WED");
        break;

    case 5:
        lcdprint("THU");
        break;

    case 6:
        lcdprint("FRI");
        break;

    case 7:
        lcdprint("SAT");
        break;
    }
}

int BCDToDecimal(char bcdByte)
{
    char a, b, dec;
    a = (((bcdByte & 0xF0) >> 4) * 10);
    b = (bcdByte & 0x0F);
    dec = a + b;
    return dec;
}

char DecimalToBCD(int decimalByte)
{
    char a, b, bcd;
    a = ((decimalByte / 10) << 4);
    b = (decimalByte % 10);
    bcd = a | b;
    return bcd;
}



void show_time()                            //function to display time/date/day on LCD
{
    char var[5];
    lcdcmd(0x80);
    lcdprint("Date:");
    sprintf(var, "%d", date);
    lcdprint(var);
    sprintf(var, "/%d", mon);
    lcdprint(var);
    sprintf(var, "/%d", year + 2000);
    lcdprint(var);
    lcdprint("   ");
    lcdcmd(0xc0);
    lcdprint("Time:");
    sprintf(var, "%d", hour);
    lcdprint(var);
    sprintf(var, ":%d", min);
    lcdprint(var);
    sprintf(var, ":%d", sec);
    lcdprint(var);
    lcdprint(" ");
    // day(day1);
    lcdprint("   ");
}

void set_time()                                            //time set function
{
    lcdcmd(0x0e);
    while (k < 7)
    {
        while (k == 3)                                            //set date
        {
            x = year % 4;
            if (inc == 0)
            {
                date++; while (inc == 0);
                if (x == 1 && mon == 2 && date == 28)
                {
                    date = 1;                 //check for 28 day febuary
                }
                if (x == 0 && mon == 2 && date == 29)
                {
                    date = 1;                 //check for 29 day febuary
                }
                if ((date == 31) && (mon == 4) || (mon == 6) || (mon == 9) || (mon == 17))
                {
                    date = 1;                 // check for 30 day month
                }
                if (date == 32)
                {
                    date = 1;                 // check for 31 day month
                }
                show_time();
            }
            if (next == 0)
            {
                k = 5;
                while (next == 0);
            }                                                //check for next digit
            lcdcmd(0x85);
        }

        while (k == 2)                                            //set month
        {
            if (inc == 0)
            {
                mon++; while (inc == 0);
                if (mon == 13) { mon = 1; }                                  //check for end of year
                show_time();
            }
            if (next == 0) {
                k = 3;
                while (next == 0);

            }
            lcdcmd(0x88);
        }

        while (k == 1)                                         //set year
        {
            if (inc == 0)
            {
                year++; while (inc == 0);
                if (year == 30) { year = 0; }
                show_time();
            }
            if (next == 0) {
                k = 2;

                while (next == 0);
            }
            lcdcmd(0x8d);
        }

        while (k == 5)                                      //set hour
        {
            if (inc == 0)
            {
                hour++; while (inc == 0);
                if (hour == 24) { hour = 0; }
                show_time();
            }
            if (next == 0) {
                k = 6;
                while (next == 0);
            }
            lcdcmd(0xc5);
        }

        while (k == 6)                                       //set min
        {
            if (inc == 0)
            {
                min++; while (inc == 0);
                if (min == 60) { min = 0; }
                show_time();
            }
            if (next == 0) {
                k = 10;
                while (next == 0);
            }
            lcdcmd(0xc8);
        }
    }
}

void main()
{
    lcd_init();
    lcdprint("Digital Clock");
    lcdcmd(0xc0);
    lcdprint(" Using 8051  ");
    delay(400);
    lcdcmd(1);
    lcdprint("Circuit Digest");
    lcdcmd(192);
    lcdprint("Saddam Khan");
    delay(400);
    while (1)
    {
        if (set == 0)                                     // check time set button press
        {
            I2CStart();
            I2CSend(0xD0);
            I2CSend(0x00);
            I2CSend(0x00);
            I2CSend(0x00);
            I2CSend(0x00);
            I2CSend(0x01);
            I2CSend(0x01);
            I2CSend(0x01);
            I2CSend(0x00);
            I2CSend(0x80);
            I2CStop();
            k = 1;
            set_time();                     // call time set function
            min = DecimalToBCD(min);
            sec = DecimalToBCD(0);
            hour = DecimalToBCD(hour);
            year = DecimalToBCD(year);
            mon = DecimalToBCD(mon);
            date = DecimalToBCD(date);
            I2CStart();
            I2CSend(0xD0);
            I2CSend(0x00);
            I2CSend(0x00);
            I2CSend(min);
            I2CSend(hour);
            I2CSend(day1);
            I2CSend(date);
            I2CSend(mon);
            I2CSend(year);
            I2CSend(0x80);
            I2CStop();
            lcdcmd(1);
            lcdcmd(0x0c);
        }

        I2CStart();
        I2CSend(0xD0);
        I2CSend(0x00);
        I2CStart();
        I2CSend(0xD1);
        sec = BCDToDecimal(I2CRead(1));
        min = BCDToDecimal(I2CRead(1));
        hour = BCDToDecimal(I2CRead(1));
        day1 = BCDToDecimal(I2CRead(1));
        date = BCDToDecimal(I2CRead(1));
        mon = BCDToDecimal(I2CRead(1));
        year = BCDToDecimal(I2CRead(1));
        I2CStop();
        show_time();                                           //display time/date/day
        delay(1);
    }
}














































void set_time()
{
    while (k < 7)
    {
        while (k == 3)                                            //set date
        {
            x = year % 4;
            if (inc == 0)
            {
                date++;
                while (inc == 0);
                if (x == 1 && mon == 2 && date == 28)
                {
                    date = 1;                 //check for 28 day febuary
                }
                if (x == 0 && mon == 2 && date == 29)
                {
                    date = 1;                 //check for 29 day febuary
                }
                if ((date == 31) && (mon == 4) || (mon == 6) || (mon == 9) || (mon == 17))
                {
                    date = 1;                 // check for 30 day month
                }
                if (date == 32)
                {
                    date = 1;                 // check for 31 day month
                }
                show_time();
            }
            if (next == 0)
            {
                k = 5;
                while (next == 0);
            }                                                //check for next digit
            lcdcmd(0x85);
        }

        while (k == 2)                                       //set month
        {
            if (inc == 0)
            {
                mon++;
                while (inc == 0);
                if (mon == 13)
                {
                    mon = 1;                                //check for end of year
                }
                show_time();
            }
            if (next == 0) {
                k = 3;
                while (next == 0);
            }
            lcdcmd(0x88);
        }

        while (k == 1)                                         //set year
        {
            if (inc == 0)
            {
                year++;
                while (inc == 0);
                if (year == 30) { year = 0; }
                show_time();
            }
            if (next == 0) {
                k = 2;

                while (next == 0);
            }
            lcdcmd(0x8d);
        }

        while (k == 5)                                      //set hour
        {
            if (inc == 0)
            {
                hour++;
                while (inc == 0);
                if (hour == 24) { hour = 0; }
                show_time();
            }
            if (next == 0) {
                k = 6;
                while (next == 0);
            }
            lcdcmd(0xc5);
        }

        while (k == 6)                                       //set min
        {
            if (inc == 0)
            {
                min++;
                while(inc == 0);
                if (min == 60)
                {
                    min = 0;
                }
                show_time();
            }
            if (next == 0)
            {
                k = 10;
                while (next == 0);
            }
            lcdcmd(0xc8);
        }
    }
}
