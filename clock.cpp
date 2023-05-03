#include <Wire.h>
#include <RTClib.h>
#include <string.h>
RTC_DS1307 rtc; 

#define leds_number 47
#define power_points 5
#define points_number 2
#define first_2_number 2
#define sector_number 13
#define zero_size 12
#define one_size 5
#define two_size 11
#define three_size 11
#define four_size 9
#define five_size 11
#define six_size 12
#define seven_size 7
#define eight_size 13
#define nine_size 12
#define fourd_5_size 5
#define clock_pins_size 2

#pragma region variables
int leds[leds_number] = {7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 ,31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53};

int power[power_points] = {2, 3, 4, 5, 6};
int points[points_number] = {A9, A8};
int first_2[first_2_number] = {A14, A15};
int fourd_5[fourd_5_size] = {A1, A0, A2, A3, A4};
int clock_pins[clock_pins_size] = {20, 21};

int first_sector[sector_number] = {43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, first_2[0], first_2[1]};
int second_sector[sector_number] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
int third_sector[sector_number] = {15, 16, 17, 18, 19, 22, 23, 24, 25, 26, 27, 28, 29};
int fourd_sector[sector_number] = {fourd_5[0], fourd_5[1], fourd_5[2], fourd_5[3], fourd_5[4], 7, 8, 9, 10, 11, 12, 13, 14};

int zero[zero_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int one[one_size] = {6, 5, 4, 3, 2};
int two[two_size] = {8, 7, 6, 5, 4, 12, 10, 11, 0, 1, 2};
int three[three_size] = {8, 7, 6, 5, 4, 12, 10, 3, 2, 1, 0};
int four[four_size] = {8, 9, 10, 12, 4, 5, 6, 3, 2};
int five[five_size] = {6, 7, 8, 9, 10, 12, 4, 3, 2, 1, 0};
int six[six_size] = {6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 12};
int seven[seven_size] = {8, 7, 6, 5, 4, 3, 2};
int eight[eight_size] = {12, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 11};
int nine[nine_size] = {12, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

int first_hour = 0;
int old_first_hour = 0;
int second_hour = 0;
int old_second_hour = 0;
int first_minute = 0;
int old_first_minute = 0;
int second_minute = 0;
int old_second_minute = 0;
int old_hour_int = 0;
int old_minutes_int = 0;

int stop1 = 0;
int intensity = 0;
int stop2 = 100;
#pragma endregion variables

bool move_back = false;
bool move_forward = false;
bool sunday = false;

void setup()
{
  if (! rtc.begin()) 
  {
    while (1);
  }

  DateTime now = rtc.now();

  for(int k(0); k < leds_number; k++)pinMode(leds[k], OUTPUT);
  for(int k(0); k < points_number; k++)pinMode(points[k], OUTPUT);
  for(int k(0); k < first_2_number; k++)pinMode(first_2[k], OUTPUT);
  for(int k(0); k < power_points; k++)pinMode(power[k], OUTPUT);
  for(int k(0); k < fourd_5_size; k++)pinMode(fourd_5[k], OUTPUT);

  for(int k(0); k < leds_number; k++)digitalWrite(leds[k], 1);
  for(int k(0); k < first_2_number; k++)digitalWrite(first_2[k], 1);
  for(int k(0); k < fourd_5_size; k++)digitalWrite(fourd_5[k], 1);
  
  analogWrite(2, 1);
  analogWrite(3, 1);
  analogWrite(4, 1);
  analogWrite(5, 1);
  analogWrite(6, 1);

  char buf1[] = "hh";
  char buf2[] = "mm";

  String hour_str = now.toString(buf1);
  int old_hour_int = hour_str.toInt();

  if(old_hour_int == 0)old_hour_int = 24;
  old_hour_int--;
  
  String minutes_str = now.toString(buf2);
  int old_minutes_int = minutes_str.toInt();

  int all_minutes = old_hour_int *60;
  all_minutes += old_minutes_int;
  all_minutes -= 10;
  
  old_hour_int = all_minutes / 60;
  old_minutes_int = all_minutes % 60;

  while(old_hour_int >= 10){old_hour_int -= 10; ++first_hour;}old_second_hour = old_hour_int;
  while(old_minutes_int >= 10){old_minutes_int -= 10; ++first_minute;}old_second_minute = old_minutes_int;
   
  first_f(first_hour, stop1);
  second_f(old_second_hour, stop1);
  third_f(first_minute, stop1);
  fourd_f(old_second_minute, stop1); 
}

void loop() {
   
  //Serial.begin(9600);
  DateTime now = rtc.now();
  
  char buf1[] = "hh";
  char buf2[] = "mm";

  String hour_str = now.toString(buf1);
  String minutes_str = now.toString(buf2);

  char buf4[] = "MM";
  char buf5[] = "DDD";
  char buf6[] = "DD";
  String month_str = now.toString(buf4);
  String day_str = now.toString(buf5);
  String day_str_int = now.toString(buf6);

  int month_int = month_str.toInt();
  int day_int = day_str_int.toInt();

  if(buf5[0] == 'S' && buf5[1] == 'u' && buf5[2] == 'n')sunday = true;

  if(month_int == 10 || month_int == 3)
  {
    if(day_int >= 24 && month_int == 10 && sunday){move_back = true; move_forward = false;}
    if(day_int >= 24 && month_int == 3 && sunday){move_forward = true; move_back = false;}
  }
    
  int hour_int = hour_str.toInt();
  int minutes_int = minutes_str.toInt();

  if(hour_int == 0)hour_int = 24;
  hour_int--;

  int all_minutes = hour_int *60;
  all_minutes += minutes_int;
  all_minutes -= 10;
  
  hour_int = all_minutes / 60;
  minutes_int = all_minutes % 60;

  if(move_forward == true)hour_int++;
  if(move_back == true)hour_int--;
   
  //mornning and afternoon
  if(hour_int < 18 && hour_int > 8)
  {
    intensity = 5;
    analogWrite(2, intensity);
    analogWrite(3, intensity);
    analogWrite(4, intensity);
    analogWrite(5, intensity);
    analogWrite(6, intensity);
  }
  
  else
  {
    //night
    intensity = 2;
    analogWrite(2, intensity);
    analogWrite(3, intensity);
    analogWrite(4, intensity);
    analogWrite(5, intensity);
    analogWrite(6, intensity);
  }
  
  second_hour = 0;
  second_minute = 0;

  if(hour_int != old_hour_int)
  {
    old_hour_int = hour_int;
    bool passed = false;

    switch(hour_int)
    {
      case 0:
      twenty_four_f(first_sector, second_sector, stop2);
      passed = true;
      break;
      case 10:
      ten_f(first_sector, second_sector, stop2);
      passed = true;
      break;
      case 20:
      twenty_f(first_sector, second_sector, stop2);
      passed = true;
      break;
    }

    if(!passed)
    {
      while(hour_int >= 10){hour_int-=10;}second_hour = hour_int;

      if(old_second_hour != second_hour)second_f(second_hour, stop2);old_second_hour = second_hour;
    
    }

  }


  if(minutes_int != old_minutes_int)
  {
    old_minutes_int = minutes_int;
    bool passed = false;

      switch(minutes_int)
      {
        
        case 10:
        ten_f(third_sector, fourd_sector, stop2);
        passed = true;
        break;
        case 20:
        twenty_f(third_sector, fourd_sector, stop2);
        passed = true;
        break;
        case 30:
        thirty_f(third_sector, fourd_sector, stop2);
        passed = true;
        break;
        case 40:
        forty_f(third_sector, fourd_sector, stop2);
        passed = true;
        break;
        case 50:
        fifty_f(third_sector, fourd_sector, stop2);
        passed = true;
        break;
        case 0:
        sixty_f(third_sector, fourd_sector, stop2);
        passed = true;
        break;
      }
      
    if(!passed)
    {
    while(minutes_int >= 10){minutes_int-=10;}second_minute = minutes_int;
    if(old_second_minute != second_minute)fourd_f(second_minute, stop2);old_second_minute = second_minute;
    }
  }

digitalWrite(A8, 0);
digitalWrite(A9, 0);
delay(1100);
digitalWrite(A8, 1);
digitalWrite(A9, 1);
delay(1100);

}


#pragma region fonctions

void zero_f(int sector[], int stop_time)
{
  for(int k(0); k < zero_size; k++){digitalWrite(sector[zero[k]], 0); delay(stop_time);}
}

void one_f(int sector[], int stop_time)
{
  for(int k(0); k < zero_size; k++){digitalWrite(sector[zero[k]], 1); delay(stop_time);}
  for(int k(0); k < one_size; k++){digitalWrite(sector[one[k]], 0); delay(stop_time);}
}

void two_f(int sector[], int stop_time)
{
  for(int k(0); k < one_size; k++){digitalWrite(sector[one[k]], 1); delay(stop_time);}
  for(int k(0); k < two_size; k++){digitalWrite(sector[two[k]], 0); delay(stop_time);}
}

void three_f(int sector[], int stop_time)
{
  for(int k(0); k < two_size; k++){digitalWrite(sector[two[k]], 1); delay(stop_time);}
  for(int k(0); k < three_size; k++){digitalWrite(sector[three[k]], 0); delay(stop_time);}
}

void four_f(int sector[], int stop_time)
{
  for(int k(0); k < three_size; k++){digitalWrite(sector[three[k]], 1); delay(stop_time);}
  for(int k(0); k < four_size; k++){digitalWrite(sector[four[k]], 0); delay(stop_time);}
}

void five_f(int sector[], int stop_time)
{
  for(int k(0); k < four_size; k++){digitalWrite(sector[four[k]], 1); delay(stop_time);}
  for(int k(0); k < five_size; k++){digitalWrite(sector[five[k]], 0); delay(stop_time);}
}

void six_f(int sector[], int stop_time)
{
  for(int k(0); k < five_size; k++){digitalWrite(sector[five[k]], 1); delay(stop_time);}
  for(int k(0); k < six_size; k++){digitalWrite(sector[six[k]], 0); delay(stop_time);}
}

void seven_f(int sector[], int stop_time)
{
  for(int k(0); k < six_size; k++){digitalWrite(sector[six[k]], 1); delay(stop_time);}
  for(int k(0); k < seven_size; k++){digitalWrite(sector[seven[k]], 0); delay(stop_time);}
}

void eight_f(int sector[], int stop_time)
{
  for(int k(0); k < seven_size; k++){digitalWrite(sector[seven[k]], 1); delay(stop_time);}
  for(int k(0); k < eight_size; k++){digitalWrite(sector[eight[k]], 0); delay(stop_time);}
}

void nine_f(int sector[], int stop_time)
{
  for(int k(0); k < eight_size; k++){digitalWrite(sector[eight[k]], 1); delay(stop_time);}
  for(int k(0); k < nine_size; k++){digitalWrite(sector[nine[k]], 0); delay(stop_time);}
}

void ten_f(int sector[], int sector2[], int stop_time)
{
    for(int k(0); k < sector_number; k++)
    {
        if(k < zero_size)digitalWrite(sector[zero[k]], 1);
        if(k < one_size) digitalWrite(sector2[nine[k]], 1);
        delay(stop_time);
    }

    for(int k(0); k < sector_number; k++)
    {
        if(k < one_size)digitalWrite(sector[one[k]], 0);
        if(k < zero_size)digitalWrite(sector2[zero[k]], 0);
        delay(stop_time);
    }
  
}

void twenty_f(int sector[], int sector2[], int stop_time)
{
    for(int k(0); k < sector_number; k++)
    {
        if(k < one_size)digitalWrite(sector[one[k]], 1);
        if(k < nine_size)digitalWrite(sector2[nine[k]], 1);
        delay(stop_time);
    } 

    for(int k(0); k < sector_number; k++)
    {
        if(k < two_size)digitalWrite(sector[two[k]], 0);
        if(k < zero_size)digitalWrite(sector2[zero[k]], 0);
        delay(stop_time);
    } 
}

void thirty_f(int sector[], int sector2[], int stop_time)
{
    for(int k(0); k < sector_number; k++)
    {
        if(k < two_size)digitalWrite(sector[two[k]], 1);
        if(k < nine_size)digitalWrite(sector2[nine[k]], 1);
        delay(stop_time);
    }  

    for(int k(0); k < sector_number; k++)
    {
        if(k < three_size)digitalWrite(sector[three[k]], 0);
        if(k < zero_size)digitalWrite(sector2[zero[k]], 0);
        delay(stop_time);
    }  
}

void forty_f(int sector[], int sector2[], int stop_time)
{
    for(int k(0); k < sector_number; k++)
    {
        if(k < three_size)digitalWrite(sector[three[k]], 1);
        if(k < nine_size)digitalWrite(sector2[nine[k]], 1);
        delay(stop_time);
    }

    for(int k(0); k < sector_number; k++)
    {
        if(k < four_size)digitalWrite(sector[four[k]], 0);
        if(k < zero_size)digitalWrite(sector2[zero[k]], 0);
        delay(stop_time);
    } 
}

void fifty_f(int sector[], int sector2[], int stop_time)
{
    for(int k(0); k < sector_number; k++)
    {
        if(k < four_size)digitalWrite(sector[four[k]], 1);
        if(k < nine_size)digitalWrite(sector2[nine[k]], 1);
        delay(stop_time);
    }

    for(int k(0); k < sector_number; k++)
    {
        if(k < five_size)digitalWrite(sector[five[k]], 0);
        if(k < zero_size)digitalWrite(sector2[zero[k]], 0);
        delay(stop_time);
    }
}

void sixty_f(int sector[], int sector2[], int stop_time)
{
    for(int k(0); k < sector_number; k++)
    {
        if(k < five_size)digitalWrite(sector[five[k]], 1);
        if(k < nine_size)digitalWrite(sector2[nine[k]], 1);
        delay(stop_time);
    }  

    for(int k(0); k < zero_size; k++)
    {
        digitalWrite(sector[zero[k]], 0);
        digitalWrite(sector2[zero[k]], 0);
        delay(stop_time);
    }  
}

void twenty_four_f(int sector[], int sector2[], int stop_time)
{
    for(int k(0); k < sector_number; k++)
    {
        if(k < two_size)digitalWrite(sector[two[k]], 1);
        if(k < four_size)digitalWrite(sector2[four[k]], 1);
        delay(stop_time);
    }

    for(int k(0); k < zero_size; k++)
    {
        digitalWrite(sector[zero[k]], 0);
        digitalWrite(sector2[zero[k]], 0);
        delay(stop_time);
    } 
}

void first_f(int first_hour, int stop_time)
{
  switch(first_hour)
    {
      case 0:
      zero_f(first_sector, stop_time);
      break;
      case 1:
      one_f(first_sector, stop_time);
      break;
      case 2:
      two_f(first_sector, stop_time);
      break;
    }
}

void second_f(int second_hour, int stop_time)
{
  switch(second_hour)
  {
    case 0:
    zero_f(second_sector, stop_time);
    break;
    case 1:
    one_f(second_sector, stop_time);
    break;
    case 2:
    two_f(second_sector, stop_time);
    break;
    case 3:
    three_f(second_sector, stop_time);
    break;
    case 4:
    four_f(second_sector, stop_time);
    break;
    case 5:
    five_f(second_sector, stop_time);
    break;
    case 6:
    six_f(second_sector, stop_time);
    break;
    case 7:
    seven_f(second_sector, stop_time);
    break;
    case 8:
    eight_f(second_sector, stop_time);
    break;
    case 9:
    nine_f(second_sector, stop_time);
    break;
  }
}

void third_f(int first_minute, int stop_time)
{
  switch(first_minute)
  {
    case 0:
    zero_f(third_sector, stop_time);
    break;
    case 1:
    one_f(third_sector, stop_time);
    break;
    case 2:
    two_f(third_sector, stop_time);
    break;
    case 3:
    three_f(third_sector, stop_time);
    break;
    case 4:
    four_f(third_sector, stop_time);
    break;
    case 5:
    five_f(third_sector, stop_time);
    break;
   
  }
}

void fourd_f(int second_minute, int stop_time)
{
  switch(second_minute)
  {
    case 0:
    zero_f(fourd_sector, stop_time);
    break;
    case 1:
    one_f(fourd_sector, stop_time);
    break;
    case 2:
    two_f(fourd_sector, stop_time);
    break;
    case 3:
    three_f(fourd_sector, stop_time);
    break;
    case 4:
    four_f(fourd_sector, stop_time);
    break;
    case 5:
    five_f(fourd_sector, stop_time);
    break;
    case 6:
    six_f(fourd_sector, stop_time);
    break;
    case 7:
    seven_f(fourd_sector, stop_time);
    break;
    case 8:
    eight_f(fourd_sector, stop_time);
    break;
    case 9:
    nine_f(fourd_sector, stop_time);
    break;
  }
}


#pragma endregion fonctions
