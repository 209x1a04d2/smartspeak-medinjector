
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define USE_ARDUINO_INTERRUPTS true
// Include necessary libraries
#include <PulseSensorPlayground.h>
 
// Constants
const int PULSE_SENSOR_PIN = 0;  // Analog PIN where the PulseSensor is connected
const int LED_PIN = 13;          // On-board LED PIN
const int THRESHOLD = 550;       // Threshold for detecting a heartbeat
 
// Create PulseSensorPlayground object
PulseSensorPlayground pulseSensor;
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000

// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

// Callback routine is executed when a pulse is detected
void onBeatDetected() {
    Serial.println("Beat!");
}

int Signal;                // Store incoming ADC data. Value can range from 0-1024
int Threshold = 550;       // Determine which Signal to "count as a beat" and which to ignore.
#include<DHT.h>
DHT dht(2,DHT11);
LiquidCrystal_I2C lcd(0x27,16,2);
#define vibration   8
#define sw   9
#define motor   12
#define buz   3

unsigned int count = 0;

char gsm[70], irq_gsm = 0;
char msg[13], mobilenum1[11];
char mobilenum2[11] = "8099495301";

void gsm_init(void);
char gsm_clear_msg(char );
void gsm_send_num_mesg1(char *, char *, char *, char *);
void read_msg();

void gps_read();
unsigned char uart1_getch();

unsigned char valid = 0;
char s4[11], s5[11];

void buz_on()
{
  pinMode(buz, OUTPUT);
  digitalWrite(buz, HIGH);
}
void buz_off()
{
  pinMode(buz, OUTPUT);
  digitalWrite(buz, LOW);
}

void motor_on()
{
  pinMode(motor, OUTPUT);
  digitalWrite(motor, HIGH);
}
void motor_off()
{
  pinMode(motor, OUTPUT);
  digitalWrite(motor, LOW);
}

void clear_gsm()
{
  unsigned char uc;
  for (uc = 0; uc < 64; uc++)
    gsm[uc] = '\0';
}


void read_gsm(void)
{
  while (Serial.available())
  {
    gsm[irq_gsm] = Serial.read();
    irq_gsm++;
    if (irq_gsm > 62)
      irq_gsm = 64;
  }
}

/*--------------------------------------------------------------------*/
/* integer to string convertion */
/* ------------------------------------------------------------------*/
void reverse(char* str, int length)
{
  int i = 0, j = length - 1;
  char tmp;
  while (i < j) {
    tmp = str[i];
    str[i] = str[j];
    str[j] = tmp;
    i++; j--;
  }
}

int INT_TO_STRING(long int n, char* out)
{
  // if negative, need 1 char for the sign
  int sign = n < 0 ? 1 : 0;
  int i = 0;
  if (n == 0) {
    out[i++] = '0';
  } else if (n < 0) {
    out[i++] = '-';
    n = -n;
  }
  while (n > 0) {
    out[i++] = '0' + n % 10;
    n /= 10;
  }
  out[i] = '\0';
  reverse(out + sign, i - sign);
  return 0;
}



/*---------------------------------------------------------------*/
/* float to string convertion   */
/*---------------------------------------------------------------*/
void FLOAT_TO_STRING (float F, char *RESULT, char PRECISION)
{
  long int INTEGER_PART = 0, FLOAT_PART = 0, PRECISION_PART = 1;
  char count = 0, length = 0, BUFFER[10], k = 0;
  for (count = 0; count < PRECISION; count++)
  {
    PRECISION_PART = PRECISION_PART * 10;
  }
  INTEGER_PART = F;
  FLOAT_PART = ((F - INTEGER_PART) * PRECISION_PART );
  INT_TO_STRING(INTEGER_PART, BUFFER);
  for (count = 0; BUFFER[k] != '\0'; count++)
  {
    RESULT[count] = BUFFER[k];
    k++;
  }
  RESULT[count] = '.';
  INT_TO_STRING(FLOAT_PART, BUFFER);
  k = 0; length = count + 1;
  for (count = length; BUFFER[k] != '\0'; count++)
  {
    RESULT[count] = BUFFER[k];
    k++;
  }
  RESULT[count++] = '\0';
}


void setup()
{
   Serial.print("Initializing pulse oximeter..");
//pox.begin();
    // Initialize sensor

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);  
  dht.begin();
  pinMode(sw, INPUT_PULLUP);
  pinMode(vibration, INPUT_PULLUP);
  motor_off();
  buz_on();
  Serial.begin(9600);
//  mySerial.begin(9600);
  lcd.init();lcd.backlight();
  lcd.clear();
  lcd.print(F("    WELCOME   ")); delay(1500);
  buz_off();
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(F(" MEDICINE INJECTOR "));
  lcd.setCursor(0, 1); lcd.print(F("USING ")); delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(F(" VOICE"));
  lcd.setCursor(0, 1); lcd.print(F("   ANNOUNCEMENT       ")); delay(1500);
     if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

  gsm_init();
  gsm_clear_msg(1);
  gsm_clear_msg(2);

  clear_gsm(); irq_gsm = 0;

  lcd.clear(); lcd.print(F("please send msg."));
  lcd.setCursor(0, 1);  lcd.print(F("To store MBL no.")); delay(1500);
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(THRESHOLD);

  // Check if PulseSensor is initialized
  if (pulseSensor.begin())
  {
    //Serial.println("PulseSensor object created successfully!");
  }

  while (1)
  {
    if (Serial.available())
    {
      delay(300);
      Serial.print("AT+CMGR=1"); irq_gsm = 0;  Serial.println(); delay(500);

      read_gsm();
      //lcd.clear();
      //lcd.write(gsm[2]);  lcd.write(gsm[3]);   lcd.write(gsm[4]);     lcd.write(gsm[5]);     lcd.write(gsm[6]);

      if (gsm[2] == '+' && gsm[3] == 'C' && gsm[4] == 'M' && gsm[5] == 'T' && gsm[6] == 'I' && gsm[7] == ':')
      {
        lcd.clear(); lcd.setCursor(2, 0); lcd.print(F("I GOT MSG"));   delay(1000);
        read_msg();

        lcd.clear();
        lcd.setCursor(0, 0); lcd.print((String)mobilenum1);
        lcd.setCursor(0, 1); lcd.print((String)msg); delay(3000);

        irq_gsm = 0;
        gsm_send_num_mesg1(mobilenum1, "Mobile number1 stored successfully", "", "");
        lcd.clear();
        break;
      }
      else
      {
        read_gsm(); delay(500); clear_gsm(); irq_gsm = 0;
        lcd.clear();
      }
    }
  }
//  
}



void loop()
{
  int l1 = 0, l2 = 0, state = 0;

  motor_off();
start1:
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(F("MEICINE "));
  lcd.setCursor(0, 1); lcd.print(F("INJECTOR ")); delay(1500);

  while (1)
  {
  int currentBPM = pulseSensor.getBeatsPerMinute();
 
  // Check if a heartbeat is detected
  if (pulseSensor.sawStartOfBeat())
  {
//    Serial.println("♥ A HeartBeat Happened!");
//    Serial.print("BPM: ");
//    Serial.println(currentBPM);
//    delay(20);
  lcd.setCursor(0,0);
  lcd.print("Pulse: ");
  lcd.print(currentBPM);
  lcd.print("bpm");
  delay(1000);
  lcd.clear();
  if(currentBPM<70){
     buz_on();
      motor_on();
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(F("LOW PULSE DETECT "));
      lcd.setCursor(0, 1); lcd.print(F("dispense 10ml"));

      delay(2000);motor_off();
      buz_off();
      gsm_send_num_mesg1(mobilenum1, "LOW PULSE DETECTED", "", ""); delay(100);
  }
  }
 
  // Add a small delay to reduce CPU usage
 
 
 float temp = dht.readTemperature();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");
  delay(500);
  lcd.clear();
if(temp>40){
   buz_on();motor_on();
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(F("HIGH TEMP DTCT"));
      lcd.setCursor(0, 1); lcd.print(F("dispensing 10ml"));
      motor_off();
      delay(2000);
      buz_off();
      gsm_send_num_mesg1(mobilenum1, "high temp DETECTED", "", ""); delay(100);
}
 pox.update();
float sp;
    // Grab the updated heart rate and SpO2 levels
//Serial.print("Heart rate:");
//        Serial.print(pox.getHeartRate());
//        Serial.print("bpm / SpO2:");

//        Serial.print(sp);
//        Serial.println("%");
        lcd.setCursor(0,0);
        lcd.print("sp0@:");
        lcd.print(sp);
        lcd.print("%");
       
        delay(1000);
        lcd.clear();
       
        if(sp<91 && sp>10  ){
          buz_on();
      motor_on();
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(F("LOW SP02 DTCT"));
      lcd.setCursor(0, 1); lcd.print(F("dispensing 10ml"));
      delay(2000);
      motor_off();
      buz_off();
      gsm_send_num_mesg1(mobilenum1, "LOW sp02 DETECTED", "", ""); delay(100);
         
        }

       

 

    if (Serial.available())
    {
      delay(300);
      Serial.print("AT+CMGR=1"); irq_gsm = 0;  Serial.println(); delay(500);

      read_gsm();
      //lcd.clear();
      //lcd.write(gsm[2]);  lcd.write(gsm[3]);   lcd.write(gsm[4]);     lcd.write(gsm[5]);     lcd.write(gsm[6]);

      if (gsm[2] == '+' && gsm[3] == 'C' && gsm[4] == 'M' && gsm[5] == 'T' && gsm[6] == 'I' && gsm[7] == ':')
      {
        lcd.clear(); lcd.setCursor(2, 0); lcd.print(F("I GOT MSG"));   delay(1000);
        read_msg();

        lcd.clear();
        lcd.setCursor(0, 0); lcd.print((String)mobilenum1);
        lcd.setCursor(0, 1); lcd.print((String)msg); delay(3000);

        irq_gsm = 0;
        if (!(strcmp(msg, "inject")))
        {
          buz_on();
          valid = 0;motor_on();
          //gps_read(); delay(200);
          buz_off();
           lcd.clear();
        lcd.setCursor(0, 0); lcd.print("injecting ");
        lcd.setCursor(0, 1); lcd.print("10ml"); delay(3000); motor_off();
          gsm_send_num_mesg1(mobilenum1, "injected 10ml  into body", "", ""); delay(100);
         ;goto start1;
        }
        if (!(strcmp(msg, "vials")))
        {
          buz_on();
          motor_on();
          valid = 0;
          //gps_read(); delay(200);
          buz_off();
         
          gsm_send_num_mesg1(mobilenum1, (char *)"Vehicle at http://google.co.in/maps/place/", "", ""); delay(100);
          goto start1;
        }
        if (!(strcmp(msg, "ENGINE OFF")))
        {
          buz_on();
          motor_off();
          valid = 0;
          //gps_read(); delay(200);
          buz_off();
          gsm_send_num_mesg1(mobilenum1, (char *)"Vehicle at http://google.co.in/maps/place/", "", ""); delay(100);
          goto start1;
        }
      }
      else
      {
        read_gsm(); delay(500); clear_gsm(); irq_gsm = 0;
      }

    }
  }
}
void gsm_init(void)
{
  unsigned int ui_t1;
  char E_count = 0;

gsminit:
  clear_gsm();
  lcd.clear(); lcd.print("GSM INIT...");
  Serial.print("ATE0"); irq_gsm = 0; Serial.println(); delay(500);

  read_gsm(); clear_gsm(); E_count = 0;
AT:
  delay(100);
  Serial.print("AT"); irq_gsm = 0; Serial.println(); delay(500);
  read_gsm();

  if (gsm[2] == 'O' && gsm[3] == 'K')
  {
    lcd.clear(); lcd.print("AT :  OK");    delay(500);
  }
  else
  {
    E_count++;
    lcd.clear(); lcd.print(" GSM Not");
    lcd.setCursor(0, 1); lcd.print("     Conected");    delay(500);
    if (E_count > 9)
    {
      E_count = 0;
      goto gsminit;
    }
    goto AT;
  }


  clear_gsm(); E_count = 0;
  Serial.print("ATE0"); irq_gsm = 0; Serial.println(); delay(500);

  read_gsm(); clear_gsm(); E_count = 0;
ATE0:
  Serial.print("ATE0"); irq_gsm = 0; Serial.println(); delay(500);
  read_gsm();
  if (gsm[2] == 'O' && gsm[3] == 'K')
  {
    lcd.clear(); lcd.print("ATE0 :  OK");  delay(500);
  }
  else
  {
    E_count++;
    lcd.clear(); lcd.print(" GSM Not");
    lcd.setCursor(0, 1); lcd.print("   Connected");     delay(500);
    if (E_count > 9)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATE0;
  }

  clear_gsm(); E_count = 0;
ATCPIN:
  Serial.print("AT+CPIN?");  Serial.println();  irq_gsm = 0;  delay(500);
  read_gsm();

  if (gsm[2] == '+' && gsm[3] == 'C' && gsm[4] == 'P' && gsm[5] == 'I' && gsm[6] == 'N' && gsm[7] == ':' && gsm[8] == ' ' && gsm[9] == 'R')
  {
    lcd.clear(); lcd.print(" SIM ");
    lcd.setCursor(0, 1); lcd.print("    CONNECTED"); delay(800);
  }
  else
  {
    E_count++;
    lcd.clear();  lcd.print("SIM CHECKING ");
    lcd.setCursor(0, 1);  lcd.print("     PLEASE WAIT");  delay(800);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCPIN;
  }



  clear_gsm(); E_count = 0;
ATCREG:
  Serial.print("AT+CREG?"); Serial.println();   irq_gsm = 0; delay(500);
  read_gsm();

  if (gsm[11] == '0')
  {
    E_count++;
    lcd.clear(); lcd.print(F(" Not Registered"));
    lcd.setCursor(0, 1); lcd.print(F("MT NOT searching"));   delay(800);
    if (E_count > 9)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCREG;
  }
  else if (gsm[11] == '1')
  {
    lcd.clear(); lcd.print(F("     SIM "));
    lcd.setCursor(0, 1); lcd.print(F("Registered H N/W"));   delay(800);
  }
  else if (gsm[11] == '2')
  {
    E_count++;
    lcd.clear(); lcd.print(F(" Not Registered"));
    lcd.setCursor(0, 1); lcd.print(F("MT searching NW"));  delay(800);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCREG;
  }
  else if (gsm[11] == '3')
  {
    E_count++;
    lcd.clear(); lcd.print(F(" Registration"));
    lcd.setCursor(0, 1); lcd.print(F("   Denied "));   delay(800);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCREG;
  }
  else if (gsm[11] == '4')
  {
    E_count++;
    lcd.clear(); lcd.print(F(" UNKNOWN "));    delay(800);
    if (E_count > 9)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCREG;
  }
  else if (gsm[11] == '5')
  {
    lcd.clear(); lcd.print(F(" Registered"));
    lcd.setCursor(0, 1); lcd.print(F("       Roaming"));   (1000);
  }
  else
  {
    E_count++;
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCREG;
  }


  /*    clear_gsm(); E_count = 0;
    ATCSQ:
      Serial.print("AT+CSQ"); irq_gsm = 0; Serial.println();  delay(500);
      read_gsm();

      if (gsm[2] == '+' && gsm[3] == 'C' && gsm[4] == 'S' && gsm[5] == 'Q' && gsm[6] == ':')
      {
        lcd.clear(); lcd.print("Signal: OK"); delay(500);

        if (gsm[9] != ',')
          ui_t1 = ((gsm[8] - 48) * 10) + (gsm[9] - 48);
        else
          ui_t1 = ((gsm[8] - 48));

        lcd.clear();  lcd.print("Signal Strength");  lcd.setCursor(5, 1);  lcd.print(ui_t1);  delay(1000);

        if (ui_t1 > 3 && ui_t1 <= 31)
        {
          lcd.clear();  lcd.print("Signal ");   lcd.setCursor(0, 1);  lcd.print(" strength OK");   delay(1000);
        }
        else
        {
          lcd.clear(); lcd.print("Signal strength"); lcd.setCursor(0, 1); lcd.print("   NOT OK ");  delay(800);
          goto ATCSQ;
        }
      }
      else
      {
        E_count++;
        lcd.clear(); lcd.print("SIGNAL ");
        lcd.setCursor(0, 1); lcd.print("   NOT  OK   ");    delay(800);

        if (E_count > 50)
        {
          E_count = 0;
          goto gsminit;
        }

        goto ATCSQ;
      }

      /*
        clear_gsm(); E_count = 0;
        ATCOPS:
        Serial.print("AT+COPS?"); irq_gsm = 0; Serial.println(); delay(500);
        read_gsm();

        if ( gsm[0] == 13 && gsm[1] == 10 && gsm[2] == '+' && gsm[3] == 'C' && gsm[4] == 'O' && gsm[5] == 'P' && gsm[6] == 'S' && gsm[7] == ':' && gsm[8] == ' ' && gsm[9] == '0' && gsm[10] == ',' && gsm[11] == '0' && gsm[12] == ',' && gsm[13] == '"')
        {
          lcd.clear(); lcd.print("MS: OK");
        }
        else
        {
          lcd.clear(); lcd.print("MS :  NOT OK ");   delay(500);
          goto ATCOPS;
        }
  */

  clear_gsm(); E_count = 0;
ATCMGF:
  Serial.print("AT+CMGF=1"); irq_gsm = 0;  Serial.println(); delay(500);

  /*read_gsm();

    if (gsm[2] == 'O' && gsm[3] == 'K')
    {
    lcd.clear(); lcd.print("TEXT MODE OK");  delay(800);
    }
    else
    {
    E_count++;
    lcd.clear(); lcd.print("TEXT MODE NOT");
    lcd.setCursor(0, 1); lcd.print("     OK   ");   delay(800);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCMGF;
    }

    /*
    clear_gsm(); E_count = 0;
    ATCLTS:
    Serial.print("AT+CLTS=1"); irq_gsm = 0; Serial.println(); delay(500);
    read_gsm();

    if (gsm[2] == 'O' && gsm[3] == 'K')
    {
      lcd.clear(); lcd.print("Time: OK");  delay(100);
    }
    else
    {
      E_count++;
      lcd.clear(); lcd.print("Time: N OK");  delay(300);
      if (E_count > 50)
      {
        E_count = 0;
        goto gsminit;
      }
      goto ATCLTS;
    }

  */

  clear_gsm(); E_count = 0;
ATCNMI:
  Serial.print("AT+CNMI=2,1"); irq_gsm = 0; Serial.println(); delay(500);
  read_gsm();

  if (gsm[2] == 'O' && gsm[3] == 'K')
  {
    lcd.clear(); lcd.print(F("SMS Settings OK"));  delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear(); lcd.print(F("SMS Settings "));
    lcd.setCursor(0, 1); lcd.print("  NOT  OK   ");    delay(1000);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCNMI;
  }




  clear_gsm(); E_count = 0;
ATW:
  Serial.print("AT&W"); irq_gsm = 0; Serial.println();  delay(500);
  /*  read_gsm();

    if (gsm[2] == 'O' && gsm[3] == 'K')
    {
      lcd.clear(); lcd.print("AT&W :  OK");  delay(1000);
    }
    else
    {
      E_count++;
      lcd.clear(); lcd.print(" SAVE  Not OK");    delay(1000);
      if (E_count > 9)
      {
        E_count = 0;
        goto gsminit;
      }
      goto ATW;
    }*/
}




char gsm_clear_msg(char num)
{
  char E_count = 0;

  clear_gsm(); E_count = 0;
ATCMGD:
  Serial.print("AT+CMGD="); Serial.print(num, DEC);     irq_gsm = 0;  Serial.println(); delay(500);
  read_gsm();

  if (gsm[2] == 'O' && gsm[3] == 'K')
  {
    lcd.clear(); lcd.print(F(" MSG DELETED"));  delay(800);
  }
  else
  {
    E_count++;
    lcd.clear(); lcd.print(F("MSG DELETE ERROR"));  delay(800);
    if (E_count > 50)
    {
      E_count = 0;
      return (0);
    }
    goto ATCMGD;
  }
  return (1);
}

void gsm_send_num_mesg1(char *num, char *msg1, char *msg2, char *msg3)
{
  lcd.clear();  lcd.print(F("MSG SENDING..."));
  lcd.setCursor(0, 1); lcd.print((String )num); delay(500);
  Serial.print("AT+CMGS="); Serial.print('"');  Serial.print((String)num);  Serial.print('"');  Serial.println("");  delay(1000);
  Serial.print((String)msg1);
  Serial.print((String)msg2);
  Serial.print((String)msg3);
  Serial.print(s4); Serial.print(','); Serial.print(s5);
  Serial.print("  ");   delay(500);
  Serial.write(0x1A); delay(500);
  read_gsm();
  while (!Serial.available());
  delay(3000);
  irq_gsm = 0; clear_gsm();
  read_gsm();

  lcd.clear();
  //lcd.print(gsm); delay(3000);
  if (gsm[2] == '+' && gsm[3] == 'C')
  {
    lcd.print(F(" MSG SENT "));
    lcd.setCursor(0, 1); lcd.print(F("    Successfully"));
  }
  else
  {
    lcd.print(F(" MSG Sending "));
    lcd.setCursor(0, 1); lcd.print(F("Error"));
  }
  Serial.println(); delay(2000);

}


void read_msg()
{
  unsigned char a, b, i, count, numcnt, j;

  while (Serial.available())
  {
    j = !Serial.available();
    delay(100);
  }

  Serial.print("AT+CMGR=1");
  Serial.println();

  count = 0;
  i = 0;
  a = 0;
  numcnt = 0;

  while (count != 3)
  {
    while (!Serial.available());
    b = Serial.read();;
    //Serial.println(b);

    if ((b == ',') || (a == 1))
    {
      if (numcnt < 15)
      {
        if (numcnt > 4)
        {
          mobilenum1[numcnt - 5] = b;
        }
        a = 1;
        numcnt++;
      }
      else
        a = 0;
    }
    if (count == 2)
    {
      msg[i] = b;
      i++;
    }
    j = 0;
    if (b == 10)
      count += 1;
  }
  msg[--i] = '\0';
  msg[--i] = '\0';
  mobilenum1[10] = '\0';
  Serial.print("AT+CMGD=1");
  Serial.println(); delay(500);
}
