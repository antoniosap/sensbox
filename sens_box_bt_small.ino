
/******************************************************************
 * soft.vers:             SENS_BOX_BT_SMALL
 * arduino soft.version:  1.0.5
 * arduino board version: Duemilanove, Arduino UNO
 * bootloader:            Duemilanove --> se diverso da error sync
 * interface:             9600/38400/115200,8,N,1
 ******************************************************************/

int dummy = 0;

typedef union {
  uint32_t dword;
  struct {
     uint8_t byte0;
     uint8_t byte1;
     uint8_t byte2;
     uint8_t byte3;
  } byte;
} dwordType;

typedef union {
  float tfloat;
  struct {
     uint8_t byte0;
     uint8_t byte1;
     uint8_t byte2;
     uint8_t byte3;
  } byte;
} floatType;

typedef union {
  uint16_t word;
  struct {
     uint8_t lo;
     uint8_t hi;
  } byte;
} wordType;

/*** CONFIGURATION MAIN *******************************************/
#define SENS_BOX_BT_SMALL
//#define DEV_SENSIRION_SHT1x
//#define DEV_LM35DZ
//#define DEV_HCZ-5D-A
#define DEV_DHT11
//#define HC_05_PROGRAM
/*** CONFIGURATION ************************************************/
#ifdef SENS_BOX_BT_SMALL
const char *cVersion = "SENS_BOX_BT_SMALL 20130909";
#else
const char *cVersion = "SENS_BOX_SERIAL 20130122";
#endif
const char *cSerialCode = "902390";
// ANDROID
#define ANDROID_ENABLE
#define BLUETOOTH_ENABLE
// 
#define LOG_ENABLE
#define LOG_DEBUG
#define DEBUG_MSG(a)     /* Serial << "->" << a << endl; */
/** RELE DRIVER****************/
// definire solo 1 dei 2
#ifdef SENS_BOX_BT_SMALL
#define RELE_DRIVER_DIRECT
#else
#define RELE_DRIVER_74HC595
#endif
/********************************/
//const unsigned long TRHSTEP   = 60000UL; // Sensor query period: standard = 60 sec
const unsigned long TRHSTEP   = 5000UL;    // Sensor query period: fast = 5 sec

/** SENSIRON ********************/
#ifdef DEV_SENSIRION_SHT1x
const byte dataPin =  9;                 // SHTxx serial data
const byte sclkPin =  8;                 // SHTxx serial clock
/// in arduino 1.0.5 da errore ---> #include <Sensirion.h>
Sensirion sht = Sensirion(dataPin, sclkPin);
unsigned int rawData;
byte measActive = false;
byte measType = TEMP;
#endif

float temperature;
float humidity;
float dewpoint;

byte ledState = 0;

unsigned long trhMillis = 0;             // Time interval tracking
unsigned long blinkMillis = 0;
/******************************/

/** SERIAL ********************/
//
/******************************/

/** TEMPERATURE OPT 1 *********/
#ifdef DEV_LM35DZ
#define LM35DZ_PIN          A2
#endif
/******************************/

/** HUMIDITY OPT 1 ************/
#ifdef DEV_HCZ-5D-A
#define HCZ_5D_A_DRV1_PIN   9    // DRV1->R->SENS->GND
#define HCZ_5D_A_DRV2_PIN   8    // DRV2->SENS->R->GND
#define HCZ_5D_A_IN_PIN     A3
#define HCZ_RDIV_OHM        (27000)
#endif
/******************************/

/** DHT11 HUMIDITY/TEMP OPT 2 */
#ifdef DEV_DHT11
#include <DHT11.h>
#define DHT11_PIN   (9)
DHT11 dht11(DHT11_PIN);  
#endif
/******************************/

/** BLUETOOTH *****************/
#define HC_05_BLUETOOTH

#ifdef BLUETOOTH_ENABLE
#ifdef HC_05_BLUETOOTH
#define cBAUDRATE 9600
#else
#define cBAUDRATE  115200
#endif
#include <SoftwareSerial.h>
#else
#define cBAUDRATE  38400
#endif
//#define HC_05_BAUDRATE_FIND
/******************************/

/** WATCHDOG ******************/
#include <avr/wdt.h>
/******************************/

#ifdef ANDROID_ENABLE
/** ANDROID ********************/
//#include <MeetAndroid.h>
// Bluetooth MAC: 00:06:66:08:E7:60
//MeetAndroid meetAndroid;
/******************************/
#endif

/** DISPLAY *******************/
//const byte ledPin  = 13;               // Arduino built-in LED, NO, ce il 74HC595 clock
const unsigned long BLINKSTEP =  250UL;  // LED blink period
/******************************/

/** KEYBOARD ******************/
#include <Bounce.h>
#define cLEDLPin            5
#define cLEDRPin            4
#define cBTNLPin            3
#define cBTNRPin            2
byte LEDLblink = 0;
byte LEDRblink = 0;
byte BTNLvalue;
byte BTNRvalue;
Bounce BTNLbouncer = Bounce(cBTNLPin, 5); 
Bounce BTNRbouncer = Bounce(cBTNRPin, 5); 
/******************************/

/** BATTERY *******************/
#define VBAT_PIN            A0
#define VBATT_DIVIDER       (5)       // divisore front-end analogico
#define VBAT_LOWBATT        (1130)    // = 11.30V di batteria 12V @5v vref
// volt batteria misurato: nel formato 99.99V - è il valore * 100
word vBAT;
/******************************/

/** LIGHT SENSOR **************/
#define VLIGHT_PIN          A1
word vLIGHT;
/******************************/

/** EEPROM ********************/
#include <EEPROM.h>
/******************************/

/** RTC / CLOCK ***************/
// Wire a 2 buffer TX e RX separati, di default 32 bytes
// quindi per trasmettere sul bus I2C max 32 bytes alla volta
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
/******************************/

/** EEPROM EXTERNAL I2C *******/
// Wire.h a 2 buffer TX e RX separati, di default 32 bytes
// quindi per trasmettere sul bus I2C max 32 bytes alla volta
// ATT ! il byte ad address 0 non lo scrive !!!

// EEPROM address = 0101/0-B0-A1-A0=52, A1=1, A0=0
#define EE24AA1025_ADDRESS_L  (0x50+0x02)
#define EE24AA1025_ADDRESS_H  (0x50+0x02+0x04)
#define EE24AA1025_PAGESIZE   (128)
#define EE24AA1025_VALID_ADDR (0x00000001) 
#define EE24AA1025_START_ADDR (0x00000002)
#define EE24AA1025_END_ADDR   (0x0001FFFF)
#define EE24AA1025_VALID_VAL  57    /* magik number */
/******************************/
// LOG STORAGE - SRAM RTC BACKED UP (56 bytes)
#define DS1307_ADDRESS       0x68
#define LOG_SRAM_START_ADDR  0x08
#define LOG_SRAM_END_ADDR    0x3F
//
/* byte:sensor failure flag */
#define SENS_FAULT_ADDR      (LOG_SRAM_START_ADDR + 0)
#define SENS_OK              '*'
#define SENS_FAULT           'x'
/* byte:verifica la validita dei valori seguenti */
#define LOG_SRAM_VALID_ADDR  (LOG_SRAM_START_ADDR + 1)  
#define LOG_SRAM_VALID_VAL   58    /* magik number */
/* word: se = 0 il log è disattivo, > 0, imposta il numero di minuti */
                                   /* tra le singole letture */                      
#define LOG_SRAM_DELAY_ADDR  (LOG_SRAM_VALID_ADDR + 1)
/* unsigned long: puntatore al primo byte in EEPROM del log */
#define LOG_SRAM_EEPTRS_ADDR (LOG_SRAM_DELAY_ADDR + 2)
/* unsigned long: puntatore all'ultimo byte in EEPROM del log */
#define LOG_SRAM_EEPTRE_ADDR (LOG_SRAM_EEPTRS_ADDR + 4)
/* unsigned long: ultimo timestamp del log */
#define LOG_SRAM_TIME_ADDR   (LOG_SRAM_EEPTRE_ADDR + 4)
// byte: log status, opzioni in 'or'
#define LOG_FILE_SRAM_STAT   (LOG_SRAM_TIME_ADDR + 4)
// unsigned long: log status, opzioni in 'or'
#define LOG_FILE_NR_READINGS (LOG_FILE_SRAM_STAT + 1)
/* primo indirizzo disponibile in SRAM */
#define LOG_FILE_SRAM_ADDR   (LOG_FILE_NR_READINGS + 4)
// log records a misura fissa:
// type record: 1 byte fisso
// 'T' = time stamps, uint32_t, 32-bit times as seconds since 1/1/1970 (len=4)
// 'I' = interval time, seconds uint16_t (len=2)
// 'S' = sample record, (len=10)
//     TL temperature low byte: 00..99
//     TH temperature high byte: -127..+127
//     HL humidity low byte: 00..99
//     HH humidity high byte: -127..+127
//     DL dew point low byte: 00..99
//     DH dew point high byte: -127..+127
//     RL rele status: 00004321
//     ST status: 00 = OK, 01 = loss sensor, 02 = low battery, 04 = leftbtn, 08 = rightbtn
//     XX check sum record
enum {
  logSTAT_OK   = 0b00000000,
  logSTAT_LS   = 0b00000001,
  logSTAT_LOW  = 0b00000010,
  logSTAT_BTNL = 0b00000100,
  logSTAT_BTNR = 0b00001000,
  logSTAT_POW  = 0b00010000, // POWER ON / system started
  logSTAT_CRC  = 0b00100000, // CRC error
  logSTAT_SRAM = 0b01000000, // SRAM no go
  logSTAT_EEPR = 0b10000000  // external EEPROM no go
} logStatusCodeDef;
byte logStatus = 0;
//
#define LOG_SLOT_LEN  18
byte logRecord[LOG_SLOT_LEN];
#define LOG_SLOT_MAX  ((EE24AA1025_END_ADDR - EE24AA1025_START_ADDR) / LOG_SLOT_LEN - 1)
// LOG STORAGE - EEPROM 24AA1025 (128k x 8 bytes)
// totale 131072 bytes in due blocchi separati di 65536 bytes
// B0 = block select bit = 0 log number 1
// B0 = block select bit = 1 log number 2
//
// indirizzi di controllo i2c
#define aSRAM  DS1307_ADDRESS
#define aEE25  EE24AA1025_ADDRESS
// log dump
uint32_t iLogAddress = 0;
uint16_t iLogIndex;
enum logDumpCodeDef {
  logDUMP_OFF   = 0,
  logDUMP_RUN_1 = 1,
  logDUMP_RUN_2 = 2,
  logDUMP_END   = 3
};
logDumpCodeDef  bLogDump = logDUMP_OFF;
/******************************/

/** BUZZER ********************/
#define cBUZZPin        6
/******************************/

/** SERIAL NR *****************/
word serialID;
/******************************/

/** RELE MEMORY****************/
// EEPROM sezione 1
#define cBaseMagikKey   0
#define cMagikKey1      26
#define cMagikKey2      95
// EEPROM sezione 2
#define cBaseEEPROM     2
#define cReleMemory     8
// RAM reles memory
volatile byte      memRreleNr[cReleMemory];
volatile byte      memRreleSt[cReleMemory];
volatile byte      memRtypeop[cReleMemory];
volatile floatType memRfvalue[cReleMemory];

enum {
  krT_GT_EQ  = 0,
  krT_LT_EQ  = 1,
  krH_GT_EQ  = 2,
  krH_LT_EQ  = 3,
  krD_GT_EQ  = 4,
  krD_LT_EQ  = 5,
  krVL_GT_EQ = 6,
  krVL_LT_EQ = 7,
  krV_GT_EQ  = 8,
  krV_LT_EQ  = 9,
  krP_GT_EQ  = 10,
  krP_LT_EQ  = 11,
  krNOOP     = 12
};

const char *codeRelOp[] = {
  "T>=", "T<=", "H>=", "H<=", "D>=", "D<=", "L>=", "L<=", "V>=", "V<=", "P>=", "P<="
};

const char *moonPhaseStr[30] = {
                                  "NM",   "NM+1", "NM+2", "NM+3", 
  "FQ-4", "FQ-3", "FQ-2", "FQ-1", "FQ",   "FQ+1", "FQ+2", "FQ+3",
          "FM-3", "FM-2", "FM-1", "FM",   "FM+1", "FM+2", "FM+3",
          "LQ-3", "LQ-2", "LQ-1", "LQ",   "LQ+1", "LQ+2", "LQ+3", "LQ+4",
  "NM-3", "NM-2", "NM-1"
};
/******************************/

/** RELE DRIVER****************/
#ifdef RELE_DRIVER_DIRECT
  // versione a 4 rele ad 1 bobina
  const byte cREL1Pin = 10;
  const byte cREL2Pin = 11;
  const byte cREL3Pin = 12;
  const byte cREL4Pin = 13;
#endif
#ifdef RELE_DRIVER_74HC595
  // versione a 4 rele con 74HC595
  // con 2 bobine per ogni rele 
  //Pin connected to ST_CP of 74HC595
  const int latch595Pin = 10;
  //Pin connected to SH_CP of 74HC595
  const int clock595Pin = 13;
  //Pin connected to DS of 74HC595
  const int data595Pin = 11;
  // stato delle bobine dei relais
  // bit 0 = rele 1 OFF 
  // bit 1 = rele 1 ON 
  // bit 2 = rele 2 OFF 
  // bit 3 = rele 2 ON 
  // bit 4 = rele 3 OFF
  // bit 5 = rele 3 ON
  // bit 6 = rele 4 OFF 
  // bit 7 = rele 4 ON 
  byte hc595 = 0;
#endif
/** RELE **********************/

/** CONSOLE *******************/
#include <CmdMessenger.h>
#include <Base64.h>
#include <Streaming.h>
// Mustnt conflict / collide with our message payload data. 
// Fine if we use base64 library ^^ above
const char field_separator   = ' ';
const char command_separator = '\n';
byte errorStatus = 0;
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

/** MOON PHASE *******************/
int  moonYY;
byte moonMM;
byte moonDD;
byte moonPhase; // the phase day (0 to 29, where 0=new moon, 15=full etc.)

// ------------------ C M D  L I S T I N G ( T X / R X ) ---------------------

// We can define up to a default of 50 cmds total, including both directions (send + recieve)
// and including also the first 4 default command codes for the generic error handling.
// If you run out of message slots, then just increase the value of MAXCALLBACKS in CmdMessenger.h

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 'Q', // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 'A', // Arduino acknowledges cmd was received
  kARDUINO_READY = 'J', // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 'X', // Arduino reports badly formatted cmd, or cmd not recognised

  // Now we can define many more 'send' commands, coming from the arduino -> the PC, eg
  kSENS_READ       = 'R',
  kVOLT_BATT       = 'V',
  kRELE_SET        = 'C',
  kRELE_STATUS     = 'S',
  kRELE_LIMIT      = 'T',
  kRELE_MEMORY     = 'M',
  kRELE_MEMORY_DEL = 'Z',
  kRELE_SAVE       = 'W',
  kRELE_RESTORE    = 'D',
  kLED_SET         = 'L',
  kBUZZER          = 'B',
  // For the above commands, we just call cmdMessenger.sendCmd() anywhere we want in our Arduino program.

  kSEND_CMDS_END, // Mustnt delete this line
};
/******************************/

/*** END CONFIGURATION *********************************************/
/*** END CONFIGURATION *********************************************/
/*** END CONFIGURATION *********************************************/

void readVoltBatt() {
   vBAT = (analogRead(VBAT_PIN) * 5.0 / 1024.0) * VBATT_DIVIDER * 100;
   // DEBUG 
   //Serial << "V-> " << analogRead(VBAT_PIN) << " V-> " << vBAT << endl;
}

void readVoltLight() {
   vLIGHT = analogRead(VLIGHT_PIN);
   // DEBUG 
   //Serial << "V-> " << analogRead(VLIGHT_PIN) << " V-> " << vBAT << endl;
}

// ------------------ C A L L B A C K  M E T H O D S -------------------------

void exeSENS_READ() {
  Serial << "T[C]:" << temperature << endl;
  Serial << "H[%]:" << humidity << endl;
  Serial << "D[C]:" << dewpoint << endl;
  arduino_ready();  
}

void readVOLT_BATT() {
  readVoltBatt();
  Serial << "V:" << (((float)vBAT) / 100.0) << endl;
}

void readVOLT_LIGHT() {
  readVoltLight();
  Serial << "VL:" << int(vLIGHT) << endl;
}

void exeVOLT_BATT() {
  // div 4 voltage divider 
  readVOLT_BATT();
  arduino_ready();
}

void exeVOLT_LIGHT() {
  readVOLT_LIGHT();
  arduino_ready();
}

byte decodeRelPin(char symbol) {
  if (symbol == 'Y') {
    return 1;
  } 
  else {
    return 0;
  }
}

#ifdef RELE_DRIVER_DIRECT
// versione a 4 rele 
byte exeRELE_SETProcedure(byte releNr, byte releSt) {
  if (releNr == 1) {
    pinMode(cREL1Pin, OUTPUT);
    digitalWrite(cREL1Pin, !decodeRelPin(releSt));
  } 
  else if (releNr == 2) {
    pinMode(cREL2Pin, OUTPUT);
    digitalWrite(cREL2Pin, !decodeRelPin(releSt));
  } 
  else if (releNr == 3) {
    pinMode(cREL3Pin, OUTPUT);
    digitalWrite(cREL3Pin, !decodeRelPin(releSt));
  } 
  else if (releNr == 4) {
    pinMode(cREL4Pin, OUTPUT);
    digitalWrite(cREL4Pin, !decodeRelPin(releSt));
  } else {
    return 0;
  }
  return 1;
}

// versione a 4 rele 
void exeRELE_SET() {
  byte releNr = cmdMessenger.readInt();
  byte releSt = cmdMessenger.readChar();
  
  if (exeRELE_SETProcedure(releNr, releSt)) {
    Serial << "C " << int(releNr) << " " << char(releSt) << endl;
    arduino_ready();
  } else {
    unknownCmd();
  }
}

#ifdef RELE_DRIVER_DIRECT
void exeRELE_STATUS() {
  Serial << "C 1 " << codeRelPin(!digitalRead(cREL1Pin)) << endl; 
  Serial << "C 2 " << codeRelPin(!digitalRead(cREL2Pin)) << endl;
  Serial << "C 3 " << codeRelPin(!digitalRead(cREL3Pin)) << endl;
  Serial << "C 4 " << codeRelPin(!digitalRead(cREL4Pin)) << endl;
  
  arduino_ready(); 
}
#endif

void exeRELE_RESET_ALL() { 
  pinMode(cREL1Pin, OUTPUT);
  digitalWrite(cREL1Pin, HIGH);
  pinMode(cREL2Pin, OUTPUT);
  digitalWrite(cREL2Pin, HIGH);
  pinMode(cREL3Pin, OUTPUT);
  digitalWrite(cREL3Pin, HIGH);
  pinMode(cREL4Pin, OUTPUT);
  digitalWrite(cREL4Pin, HIGH);
}
#endif

#ifdef RELE_DRIVER_74HC595
// versione a 4 rele con 74HC595 
byte exeRELE_SETProcedure(byte releNr, byte releSt) {
  // stato delle bobine dei relais
  // cioè dei bit nel 74HC595
  // se = 1 attiva la bobina corrispondente
  // bit 0 = rele 1 OFF 
  // bit 1 = rele 1 ON 
  // bit 2 = rele 2 OFF 
  // bit 3 = rele 2 ON 
  // bit 4 = rele 3 OFF 
  // bit 5 = rele 3 ON
  // bit 6 = rele 4 OFF 
  // bit 7 = rele 4 ON
  
  switch (releNr) {
    case 1:
       if (decodeRelPin(releSt)) {
         bitSet(hc595,1);
         bitClear(hc595,0);
       } else {
         bitSet(hc595,0);
         bitClear(hc595,1);
       }
       break;
    case 2:
       if (decodeRelPin(releSt)) {
         bitSet(hc595,3);
         bitClear(hc595,2);
       } else {
         bitSet(hc595,2);
         bitClear(hc595,3);
       }
       break;
    case 3:
       if (decodeRelPin(releSt)) {
         bitSet(hc595,5);
         bitClear(hc595,4);
       } else {
         bitSet(hc595,4);
         bitClear(hc595,5);
       }
       break;
    case 4:
    if (decodeRelPin(releSt)) {
         bitSet(hc595,7);
         bitClear(hc595,6);
       } else {
         bitSet(hc595,6);
         bitClear(hc595,7);
       }
       break;
    default:
       return 0;
  }
  return 1; 
}

// versione a 4 rele con 74HC595 
void exeRELE_SET() {
  byte releNr = cmdMessenger.readInt();
  byte releSt = cmdMessenger.readChar();

  if (exeRELE_SETProcedure(releNr, releSt)) {
    writeHC595(hc595);

    Serial << "C " << int(releNr) << " " << releSt << endl;
    arduino_ready();
  } else {
    unknownCmd();
  } 
}

void exeRELE_RESET_ALL() { 
  for (int releNr = 1; releNr <= 4; releNr++) {
    if (exeRELE_SETProcedure(releNr, 'N')) {
      writeHC595(hc595);
      delay(250);
    }
  } 
}

void writeHC595(byte value) {
  pinMode(latch595Pin, OUTPUT);
  pinMode(clock595Pin, OUTPUT);
  pinMode(data595Pin,  OUTPUT);
  //
  digitalWrite(latch595Pin, HIGH);
  digitalWrite(clock595Pin, HIGH);
  digitalWrite(data595Pin, HIGH);
  // take the latchPin low so 
  // the output don't change while you're sending in bits:
  digitalWrite(latch595Pin, LOW);
  shiftOut(data595Pin, clock595Pin, MSBFIRST, value);
  digitalWrite(latch595Pin, HIGH);
  // attiva le bobine
  delay(200);
  // rilascia tutte le bobine
  digitalWrite(latch595Pin, LOW);
  shiftOut(data595Pin, clock595Pin, MSBFIRST, 0);
  digitalWrite(latch595Pin, HIGH);  
}

void exeRELE_STATUS() {
  // stato delle bobine dei relais
  // cioè dei bit nel 74HC595
  // se = 1 è stata attivata la bobina corrispondente
  // bit 0 = rele 1 OFF 
  // bit 1 = rele 1 ON 
  // bit 2 = rele 2 OFF 
  // bit 3 = rele 2 ON 
  // bit 4 = rele 3 OFF 
  // bit 5 = rele 3 ON
  // bit 6 = rele 4 OFF 
  // bit 7 = rele 4 ON
  
  Serial << "C 1 " << codeRelPin(bitRead(hc595,1)) << endl; 
  Serial << "C 2 " << codeRelPin(bitRead(hc595,3)) << endl;
  Serial << "C 3 " << codeRelPin(bitRead(hc595,5)) << endl;
  Serial << "C 4 " << codeRelPin(bitRead(hc595,7)) << endl;

  arduino_ready(); 
}
#endif

#ifdef RELE_DRIVER_DIRECT
byte packRelStatus() {
  return ((!digitalRead(cREL4Pin)) << 3) |
         ((!digitalRead(cREL3Pin)) << 2) |
         ((!digitalRead(cREL2Pin)) << 1) |
         ((!digitalRead(cREL1Pin)));
}
#endif

#ifdef RELE_DRIVER_74HC595
byte packRelStatus() {
  return (bitRead(hc595,1) << 3) |
         (bitRead(hc595,3) << 2) |
         (bitRead(hc595,5) << 1) |
         (bitRead(hc595,7));
}
#endif

char codeRelPin(byte pin) {
  if (pin) {
    return 'Y';
  } 
  else {
    return 'N';
  }
}

void readRELE_MEMORY(int i) {
  Serial << "M" << i;
  if (memRtypeop[i] != krNOOP) {
      Serial << " " << int(memRreleNr[i]) << " " << char(memRreleSt[i]) << 
                " " << codeRelOp[memRtypeop[i]] << " " << memRfvalue[i].tfloat;
  } else {
      Serial << " FREE";
  }
  Serial << endl;
}

void exeRELE_LIMIT() {
  int  memNr  = cmdMessenger.readInt();
  byte releNr = cmdMessenger.readInt();
  byte releSt = cmdMessenger.readChar();
  char typeop[4+1];
  char valuec[10+1];
  float value;
  
  cmdMessenger.copyString(typeop,4);
  cmdMessenger.copyString(valuec,10);
  value = atof(valuec);
  
  // debug
  //Serial << int(memNr) << endl;
  //Serial << int(releNr) << endl;
  //Serial << releSt << endl;
  //Serial << typeop << endl; 
  //Serial << valuec << endl; 
  //Serial << value << endl; 
  
  for (int i = 0; i < krNOOP; i++) { 
    //Serial << codeRelOp[i] << "*";
    if (strcmp(typeop,codeRelOp[i]) == 0) {
      //Serial << "typeopCode " << codeRelOp[i] << endl; 
      
      if ((memNr >= 0) && (memNr < cReleMemory)) {
        memRreleNr[memNr] = releNr;
        memRreleSt[memNr] = releSt;
        memRtypeop[memNr] = i;
        memRfvalue[memNr].tfloat = value;
      } else {
        unknownCmd();
        return;
      }
      
      readRELE_MEMORY(memNr);
      Serial << "OK";
      arduino_ready();
      return;
  } // end if
 } // end for
}

void exeRELE_MEMORY() {
  Serial << endl;
  for (int i = 0; i < cReleMemory; i++) {
    readRELE_MEMORY(i);
  }
  Serial << "OK";
  arduino_ready(); 
}

void exeRELE_MEMORY_DEL() {
  int memNr  = cmdMessenger.readInt();

  if (memNr == 9872) {
    for (int i = 0; i < cReleMemory; i++) {
      memRtypeop[i] = krNOOP;
    }
  } else if ((memNr >= 0) && (memNr < cReleMemory)) {
    memRtypeop[memNr] = krNOOP;
  } else {
    unknownCmd();
    return;
  }

  Serial << "OK";
  arduino_ready(); 
}

void exeRELE_SAVE() {
  int k1 = cmdMessenger.readInt();
  int k2 = cmdMessenger.readInt();
  int pt = cBaseEEPROM;
  
  if ((k1 == 8543) && (k2 == 1295)) {
    for (int i = 0; i < cReleMemory; i++) {
      EEPROM.write(pt++, memRreleNr[i]);
      EEPROM.write(pt++, memRreleSt[i]);
      EEPROM.write(pt++, memRtypeop[i]);
      EEPROM.write(pt++, memRfvalue[i].byte.byte0);
      EEPROM.write(pt++, memRfvalue[i].byte.byte1);
      EEPROM.write(pt++, memRfvalue[i].byte.byte2);
      EEPROM.write(pt++, memRfvalue[i].byte.byte3);
    }
  } else {
    unknownCmd();
    return;
  }

  Serial << "SAVED" << endl << "OK";
  arduino_ready();  
}

void exeRELE_RESTORE() {
  int pt = cBaseEEPROM;
  
  for (int i = 0; i < cReleMemory; i++) {
    memRreleNr[i] = EEPROM.read(pt++);
    memRreleSt[i] = EEPROM.read(pt++);
    memRtypeop[i] = EEPROM.read(pt++);
    memRfvalue[i].byte.byte0 = EEPROM.read(pt++);
    memRfvalue[i].byte.byte1 = EEPROM.read(pt++);
    memRfvalue[i].byte.byte2 = EEPROM.read(pt++);
    memRfvalue[i].byte.byte3 = EEPROM.read(pt++);
  }

  Serial << endl << "RESTORED" << endl;
  arduino_ready();  
}

// auto inizializza la eeprom
// dopo la programmazione
// ma solo alla prima accensione in assoluto
void MagikEEPROM() { 
  int pt = cBaseEEPROM;
   
  if ((EEPROM.read(cBaseMagikKey+0) == cMagikKey1) && 
      (EEPROM.read(cBaseMagikKey+1) == cMagikKey2)) {
    exeRELE_RESTORE();
  } else {
    for (int i = 0; i < cReleMemory; i++) {
      memRtypeop[i] = krNOOP;
    }
    for (int i = 0; i < cReleMemory; i++) {
      EEPROM.write(pt++, 0);
      EEPROM.write(pt++, 0);
      EEPROM.write(pt++, krNOOP);
      EEPROM.write(pt++, 0);
      EEPROM.write(pt++, 0);
      EEPROM.write(pt++, 0);
      EEPROM.write(pt++, 0);
    }
    EEPROM.write(cBaseMagikKey+0, cMagikKey1);
    EEPROM.write(cBaseMagikKey+1, cMagikKey2);
    
    Serial << "INIT" << endl << "OK";
    arduino_ready();     
  }
}

void ComputeLimits() {
   byte bTrigger = 0;
   
   for (int i = 0; i < cReleMemory; i++) {
      float memValueStore = memRfvalue[i].tfloat;
      
      switch (memRtypeop[i]) {
        case krT_GT_EQ: // T>=
          if (temperature >= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krT_LT_EQ: // T<=
          if (temperature <= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krH_GT_EQ: // H>=
          if (humidity >= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krH_LT_EQ: // H<=
          if (humidity <= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krD_GT_EQ: // D>=
          if (dewpoint >= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krD_LT_EQ: // D<=
          if (dewpoint <= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krVL_LT_EQ: // L light<=
          if (vLIGHT <= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krVL_GT_EQ: // L light>=
          if (vLIGHT >= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krV_LT_EQ:  // V batt<=
          if ((((float)vBAT) / 100.0) <= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krV_GT_EQ:  // V batt>=
          if ((((float)vBAT) / 100.0) >= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krP_LT_EQ:  // moonPhase>=
          if (moonPhase <= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        case krP_GT_EQ:  // moonPhase<=
          if (moonPhase >= memValueStore) {
            bTrigger = exeRELE_SETProcedure(memRreleNr[i], memRreleSt[i]);
          }
          break;
          
        default: break;
      } // end switch
      
//    if (bTrigger) {
//      bTrigger = 0;
//#ifdef RELE_DRIVER_74HC595
//      writeHC595(hc595);
//#endif
//      Serial << "C " << int(memRreleNr[i]) << " " << char(memRreleSt[i]) << endl;
//      arduino_ready();
//    }
    } // end for
}

byte decodeLedPin(char symbol) {
  if (symbol == 'Y') {
    return 1;
  } 
  else {
    return 0;
  }
}

void exeLED_SET() {
  byte ledNr = cmdMessenger.readInt();
  byte ledSt = cmdMessenger.readChar();

  if (ledNr == 1) {
    digitalWrite(cLEDLPin, decodeLedPin(ledSt));
    if (ledSt == 'B') {
      LEDLblink = 1;
    } 
    else {
      LEDLblink = 0;
    }
  } 
  else if (ledNr == 2) {
    digitalWrite(cLEDRPin, decodeLedPin(ledSt));
    if (ledSt == 'B') {
      LEDRblink = 1;
    } 
    else {
      LEDRblink = 0;
    }
  } 
  else {
    unknownCmd();
    return;
  }

  Serial << "L " << int(ledNr) << " " << char(ledSt) << endl;
  arduino_ready();   
}

void exeBUTTON() {
  byte btnNr = cmdMessenger.readInt();

  Serial << "Q " << int(btnNr) << endl;
  
  if (btnNr == 1) {
    digitalWrite(cLEDLPin, 1);
    Serial.println("BTNLP");
    Serial.println("BTNLR");
    digitalWrite(cLEDLPin, 0);    
  } else if (btnNr == 2) {
    digitalWrite(cLEDRPin, 1);
    Serial.println("BTNRP");
    Serial.println("BTNRR");
    digitalWrite(cLEDRPin, 0);
  }
}

void exeBUZZER() {
  tone(cBUZZPin, 440, 500);
}

void exeSERIAL_CODE() {
  Serial << "SN:" << cSerialCode << "-" << _HEX(serialID) << endl;
}

void exeSET_RTC() {
  //          0123456789 01234567
  // formato: AAAA-MM-GG HH:MM:SS KKKK in tre parametri
  char dt[11];
  char tm[9];
  
  cmdMessenger.copyString(dt,11);
  cmdMessenger.copyString(tm,9);
  int k1 = cmdMessenger.readInt();
  
  if (k1 == 8592) {
     RTC.adjust(DateTime(dt, tm));
     ComputeMoonPhase();
     exeREAD_RTC();
  } else {
     unknownCmd();
  }
}

void exeREAD_RTC() {
  if (! RTC.isrunning()) {
      Serial << "RTC:STOPPED" << endl;
  } else {
      showRTC();
      Serial << endl;
  }
  arduino_ready();
}

void exeSTARTUP() {
  // BUILT-IN TEST *****************************************************
  Serial << cVersion << " FR:" << freeRam() << endl;
  // SERIAL NUMBER *****************************************************
  exeSERIAL_CODE();
  // SENSIRON **********************************************************
  exeSENS();
  // MONITOR BATTERY VOLTAGE *******************************************
  readVOLT_BATT();
  // LOG DUMP OFF ******************************************************
  bLogDump = logDUMP_OFF;
  // RTC / CLOCK *******************************************************
  ComputeMoonPhase();
  exeREAD_RTC(); 
}

#ifdef DEV_SENSIRION_SHT1x
void exeSENS() {
  byte stat;
  
  if (errorStatus = sht.readSR(&stat))         // Read sensor status register
    logError(errorStatus);
  Serial << "SENS:";
  Serial.println(stat, HEX);  
}
#endif

#ifdef DEV_LM35DZ
#ifdef DEV_HCZ-5D-A
void exeSENS() {
  errorStatus = 1;
  Serial << "SENS:";
  Serial.println(errorStatus, HEX);  
}
#endif
#endif

#ifdef DEV_DHT11
void exeSENS() {
  Serial << "SENS:";

  if (i2c_read_byte(aSRAM, SENS_FAULT_ADDR) == SENS_FAULT) {
    Serial << "DHT11_FAULT" << endl;
  } else {
    Serial << "0" << endl;
  }
}
#endif

// ------------------ D E F A U L T  C A L L B A C K S -----------------------

void arduino_ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  // POCO PRATICO
  // cmdMessenger.sendCmd(kACK,">");
  // USO QUESTO PROMPT
  Serial << ">";
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  // POCO PRATICO
  // cmdMessenger.sendCmd(kERR,"KO>");
  // USO QUESTO PROMPT
  Serial << "KO>";
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------

// ------------------ S E T U P ----------------------------------------------

void setup() {
  byte stat;
  
  exeRELE_RESET_ALL();

  // WATCHDOG 8 secs ***
  wdt_enable(WDTO_8S);
  
  // SENSORS ***********************************************************
  #ifdef DEV_LM35DZ
    initLM35DZ();
  #endif
  #ifdef DEV_HCZ-5D-A
    initHCZ5DA();
  #endif
  #ifdef DEV_DHT11
    // no init functions
  #endif

  // CONSOLE SETUP *****************************************************
  Serial.begin(cBAUDRATE);
  //pinMode(ledPin, OUTPUT); ora ce il 74HC595
  delay(15);                             // Wait >= 11 ms before first cmd

  // RTC / CLOCK *******************************************************
  Wire.begin();
  RTC.begin();
  DateTime now = RTC.now(); // dummy read
  
  // SERIAL NUMBER *****************************************************
  randomSeed(analogRead(0));
  serialID = random(63535);
  
  // STARTUP initial message *******************************************
  exeSTARTUP();
  
  // HC-05 bluetooth  **************************************************
#ifdef HC_05_PROGRAM 
  programHC_05();
#endif

#ifdef HC_05_BAUDRATE_FIND
  while (1) {
    printBaudRateHC_05();
    wdt_reset(); // max 8 secs cycle
  }
#endif
  
  // KEYOARD TEST ******************************************************
  pinMode(cLEDLPin, OUTPUT);
  pinMode(cLEDRPin, OUTPUT);
  pinMode(cBTNLPin, INPUT);
  pinMode(cBTNRPin, INPUT);

  for (int i=0; i <= 3; i++) {
    digitalWrite(cLEDLPin, HIGH);   // sets the LED on
    delay(500);                     // 
    digitalWrite(cLEDLPin, LOW);    // sets the LED off
    delay(500);
    wdt_reset(); 
  }

  for (int i=0; i <= 3; i++) {
    digitalWrite(cLEDRPin, HIGH);   // sets the LED on
    delay(500);                     // 
    digitalWrite(cLEDRPin, LOW);    // sets the LED off
    delay(500);
    wdt_reset();
  }  
  // init keyboard values
  BTNLvalue = digitalRead(cBTNLPin);
  BTNRvalue = digitalRead(cBTNRPin);
  
  DEBUG_MSG("S1");
  // cmdMessenger.discard_LF_CR(); // Useful if your terminal appends CR/LF, and you wish to remove them
  // cmdMessenger.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor

  // Attach default / generic callback methods
  cmdMessenger.attach(kARDUINO_READY, arduino_ready);
  cmdMessenger.attach(unknownCmd);

  // Attach my application's user-defined callback methods
  cmdMessenger.attach('R', exeSENS_READ);
  cmdMessenger.attach('V', exeVOLT_BATT);
  cmdMessenger.attach('C', exeRELE_SET);
  cmdMessenger.attach('S', exeRELE_STATUS);
  cmdMessenger.attach('T', exeRELE_LIMIT);
  cmdMessenger.attach('M', exeRELE_MEMORY);
  cmdMessenger.attach('Z', exeRELE_MEMORY_DEL);
  cmdMessenger.attach('W', exeRELE_SAVE);
  cmdMessenger.attach('D', exeRELE_RESTORE);
  cmdMessenger.attach('L', exeLED_SET);
  cmdMessenger.attach('B', exeBUZZER);
  cmdMessenger.attach('G', exeSTARTUP);
  cmdMessenger.attach('K', exeSET_RTC);
  cmdMessenger.attach('I', exeREAD_RTC);
  cmdMessenger.attach('E', exeMANAGE_LOG);
  cmdMessenger.attach('F', exeREAD_LOG);
  cmdMessenger.attach('Q', exeBUTTON);
  cmdMessenger.attach('U', exeSENS);
  cmdMessenger.attach('X', exeVOLT_LIGHT);
#ifdef LOG_DEBUG
  cmdMessenger.attach('J', exeDUMP);
#endif

  DEBUG_MSG("S2");
  
  // load from EEPROM
  MagikEEPROM();
  // LOG file
  bLogDump = logDUMP_OFF;      // nessun log in dump
#ifdef LOG_ENABLE
  exeINIT_LOG();
#endif
  DEBUG_MSG("S3");
  // TEST BUZZER
  exeBUZZER();
#ifdef RELE_TEST_74HC595
  // TEST RELE - solo debug
  testRELE();
#endif
  DEBUG_MSG("S4");
}

void loop() {
  unsigned long curMillis = millis();          // Get current time
  
  wdt_reset(); // max 8 secs cycle
  
  // KEYBOARD PROCESS ********************************************************
  BTNLbouncer.update();
  BTNRbouncer.update();
  //Serial << "L " << codeRelPin(digitalRead(cBTNLPin)) << endl;  
  if (BTNLvalue != BTNLbouncer.read()) {
    BTNLvalue = BTNLbouncer.read();
    digitalWrite(cLEDLPin, !BTNLvalue);
    if (BTNLvalue == HIGH) {
      Serial.println("BTNLR");
    } 
    else {
      Serial.println("BTNLP");
    }
  }
  
  //Serial << "R " << codeRelPin(digitalRead(cBTNRPin)) << endl;
  if (BTNRvalue != BTNRbouncer.read()) {
    BTNRvalue = BTNRbouncer.read();
    digitalWrite(cLEDRPin, !BTNRvalue);
    if (BTNRvalue == HIGH) {
      Serial.println("BTNRR");
    } 
    else {
      Serial.println("BTNRP");
    }
  }
  
  // CONSOLE PROCESS *********************************************************
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData(); 

  // LED PROCESS *************************************************************
  if (curMillis - blinkMillis >= BLINKSTEP) {  // Time to toggle the LED state?
    ledState ^= 1;
    // digitalWrite(ledPin, ledState); NO, ora ce il 74HC595
    if (LEDLblink) {
      digitalWrite(cLEDLPin, ledState);
    } else {
      digitalWrite(cLEDLPin, !BTNLvalue);
    }
    if (LEDRblink) {
      digitalWrite(cLEDRPin, ledState);
    } else {
      digitalWrite(cLEDRPin, !BTNRvalue);
    }
    blinkMillis = curMillis;
  }
  // SENSORS PROCESS ********************************************************
  // non-blocking calls
  if (curMillis - trhMillis >= TRHSTEP) {      // Time for new measurements?
    // *** BAT MONITOR PROCESS START ***
    readVoltBatt();
    if (vBAT < VBAT_LOWBATT) {
        Serial << "LOWB" << endl;
    }
    // *** BAT MONITOR PROCESS END *****
    readVoltLight();

#ifdef DEV_SENSIRION_SHT1x
    measActive = true;
    measType = TEMP;
    // Start temp measurement
    if (errorStatus = sht.meas(TEMP, &rawData, NONBLOCK)) logError(errorStatus);  
#endif

#ifdef DEV_LM35DZ
#ifdef DEV_HCZ-5D-A
    measureLM35DZ();
    measureHCZ5DA();
    dewpoint = calcDewpoint(humidity, temperature);
#endif
#endif

#ifdef DEV_DHT11
   // se ce un time out qui per DTH11 che non risponde si riporta 
   // l'errore al prossimo watchdog scattato, le variabili interne si resettano
   // usare una sRAM protetta esterna
   i2c_write_byte(aSRAM, SENS_FAULT_ADDR, SENS_FAULT);
   if (errorStatus = dht11.read(humidity, temperature)) logError(errorStatus);
   i2c_write_byte(aSRAM, SENS_FAULT_ADDR, SENS_OK);
   dewpoint = calcDewpoint(humidity, temperature);
   //exeSENS_READ(); // debug
#endif
    
    trhMillis = curMillis;
  }
#ifdef DEV_SENSIRION_SHT1x
  // Check measurement status
  if (measActive && (errorStatus = sht.measRdy())) { 
    if (errorStatus != S_Meas_Rdy) logError(errorStatus);
    if (measType == TEMP) {                    // Process temp or humi ?
      // debug sensiron raw data
      //Serial.print("T raw: ");
      //Serial.println(rawData, HEX);
      // -----------------------
      measType = HUMI;
      temperature = sht.calcTemp(rawData);     // Convert raw sensor data
      // Start humi measurement
      if (errorStatus = sht.meas(HUMI, &rawData, NONBLOCK)) logError(errorStatus);
    } 
    else {
      // debug sensiron raw data
      //Serial.print("H raw: ");
      //Serial.println(rawData, HEX);
      // -----------------------
      measActive = false;
      humidity = sht.calcHumi(rawData, temperature); // Convert raw sensor data
      dewpoint = sht.calcDewpoint(humidity, temperature);
    }
  }
#endif 

    // ultimi aggiornamenti dai sensori
    readVoltBatt();
    readVoltLight();
    // calcolo soglie reles
    ComputeLimits();

#ifdef LOG_ENABLE
    processLOG_TIME();
#endif

  /** MOON PHASE *******************/
  ComputeMoonPhase();
}

#ifdef DEV_SENSIRION_SHT1x
// The following code is only used with error checking enabled
void logError(byte error) {
  switch (error) {
  case S_Err_NoACK:
    Serial.println("S_Err_NoACK");
    break;
  case S_Err_CRC:
    Serial.println("S_Err_CRC");
    break;
  case S_Err_TO:
    Serial.println("S_Err_TO");
    break;
  default:
    Serial.println("S_Err_UNK");
    break;
  }
}
#endif

#ifdef DEV_DHT11
void logError(byte error) {
  switch (error) {
  case 1:
    Serial.println("S_Err_NoACK");
    break;
  case 4:
    Serial.println("S_Err_CRC");
    break;
  case -1:
    Serial.println("S_Err_TO");
    break;
  default:
    Serial.println("S_Err_UNK");
    break;
  }
}
#endif

int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10) Serial << '0';
  Serial << digits;
}

void showRTC() {
  DateTime now = RTC.now();
  
  Serial << now.year() << "-";
  printDigits(now.month());
  Serial << "-";
  printDigits(now.day());
  Serial << " ";
  printDigits(now.hour());
  Serial << ":";
  printDigits(now.minute());
  Serial << ":";
  printDigits(now.second());
  Serial << "/";
  printDigits(moonPhase);
  Serial << "/" << moonPhaseStr[moonPhase];
}

#ifdef RELE_TEST_74HC595
void testRELE() {
  char releSt;
  
  pinMode(latch595Pin, OUTPUT);
  pinMode(clock595Pin, OUTPUT);
  pinMode(data595Pin,  OUTPUT);
  
  for (int i = 0; i < 10; i++) {
    Serial << "i=" << int(i) << endl;
    digitalWrite(latch595Pin, LOW);
    digitalWrite(cLEDLPin, LOW);
    delay(500);
    digitalWrite(latch595Pin, HIGH);
    digitalWrite(cLEDLPin, HIGH);
    delay(500);
    wdt_reset();
  }
  digitalWrite(cLEDLPin, LOW);
  
  for (int i = 0; i < 1000; i++) {
    for (int releNr = 1; releNr <= 4; releNr++) {
      wdt_reset();
      digitalWrite(cLEDRPin, HIGH);
      releSt = 'Y';
      if (exeRELE_SETProcedure(releNr, releSt)) {
          writeHC595(hc595);
          Serial << "i=" << int(i) << " C " << int(releNr) << " " << releSt << endl;
      }
      delay(3000);
      digitalWrite(cLEDRPin, LOW);
      releSt = 'N';
      if (exeRELE_SETProcedure(releNr, releSt)) {
          writeHC595(hc595);
          Serial << "i=" << int(i) << " C " << int(releNr) << " " << releSt << endl;
      }
      delay(3000);
    }
  }
  digitalWrite(cLEDRPin, LOW);
}
#endif

// I2C functions begin *****************
void i2c_set_address_long(const uint32_t i2cAddress) {
  dwordType eeAddress;
  
  eeAddress.dword = i2cAddress;   
  Wire.beginTransmission(EE24AA1025_ADDRESS_L | (eeAddress.byte.byte2 << 3)); // in pos. B0
  Wire.write(eeAddress.byte.byte0);
  Wire.write(eeAddress.byte.byte1);
  Wire.endTransmission();
} 

void i2c_set_address(const uint8_t i2cAddress, const uint8_t regAddress)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(regAddress);	
  Wire.endTransmission();
}

uint8_t i2c_read_byte(const uint8_t i2cAddress, const uint8_t regAddress)
{
  i2c_set_address(i2cAddress, regAddress);
  Wire.requestFrom((uint8_t) i2cAddress, (uint8_t) sizeof(uint8_t));
  return Wire.read();
}


void i2c_write_byte(const uint8_t i2cAddress, const uint8_t regAddress, const uint8_t value)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(regAddress);
  Wire.write(value);
  Wire.endTransmission();
}

uint16_t i2c_read_word(const uint8_t i2cAddress, const uint8_t regAddress)
{
  wordType x;
  
  i2c_set_address(i2cAddress, regAddress);  
  Wire.requestFrom(i2cAddress, sizeof(x));
  x.byte.lo = Wire.read();
  x.byte.hi = Wire.read();
  return x.word;
}

uint32_t i2c_read_long(const uint8_t i2cAddress, const uint8_t regAddress)
{
  dwordType x;
  
  i2c_set_address(i2cAddress, regAddress);  
  Wire.requestFrom(i2cAddress, sizeof(x));
  x.byte.byte0 = Wire.read();
  x.byte.byte1 = Wire.read();
  x.byte.byte2 = Wire.read();
  x.byte.byte3 = Wire.read();
  return x.dword;
}

void i2c_write_word(const uint8_t i2cAddress, const uint8_t regAddress, const uint16_t value)
{
  wordType x;
    
  x.word = value;
  Wire.beginTransmission(i2cAddress);
  Wire.write(regAddress);
  Wire.write(x.byte.lo);
  Wire.write(x.byte.hi);
  Wire.endTransmission();
}

void i2c_write_long(const uint8_t i2cAddress, const uint8_t regAddress, const uint32_t value)
{
  dwordType x;
    
  x.dword = value;
  Wire.beginTransmission(i2cAddress);
  Wire.write(regAddress);
  Wire.write(x.byte.byte0);
  Wire.write(x.byte.byte1);
  Wire.write(x.byte.byte2);
  Wire.write(x.byte.byte3);
  Wire.endTransmission();
}

// 24AA1025 functions ********************
uint8_t i2c_read_byte_24AA1025(uint32_t regAddress32) 
{
  byte rdata = 0xFF;
  uint16_t regAddress16;
  uint8_t i2cAddress;
  
  regAddress32 &= 0x0001FFFF;
  regAddress16 = regAddress32;

  if (regAddress32 > 0xFFFF) {
    i2cAddress = EE24AA1025_ADDRESS_H;
  } else {
    i2cAddress = EE24AA1025_ADDRESS_L;
  }
  Wire.beginTransmission(i2cAddress);
  Wire.write((int)(regAddress16 >> 8));   // MSB
  Wire.write((int)(regAddress16 & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom((uint8_t) i2cAddress, (uint8_t) 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void i2c_write_byte_24AA1025(uint32_t regAddress32, const uint8_t value)
{
   uint16_t regAddress16;
   uint8_t i2cAddress;
   
   regAddress32 &= 0x0001FFFF;
   regAddress16 = regAddress32;
   
   if (regAddress32 > 0xFFFF) {
    i2cAddress = EE24AA1025_ADDRESS_H;
   } else {
    i2cAddress = EE24AA1025_ADDRESS_L;
   }
   Wire.beginTransmission(i2cAddress);
   Wire.write((int)(regAddress16 >> 8));   // MSB
   Wire.write((int)(regAddress16 & 0xFF)); // LSB
   Wire.write(value);
   Wire.endTransmission();
   delay(5);
}

void i2c_write_buffer_24AA1025(uint32_t regAddress32, byte *data, int data_len) {
   for (int i = 0; i < data_len; i++) {
     i2c_write_byte_24AA1025(regAddress32, data[i]);
     regAddress32++;
   }
}

uint16_t i2c_read_word_24AA1025(uint32_t regAddress32)
{
  wordType x;
  
  x.byte.lo = i2c_read_byte_24AA1025(regAddress32++);
  x.byte.hi = i2c_read_byte_24AA1025(regAddress32++);
  return x.word;
}

uint32_t i2c_read_long_24AA1025(uint32_t regAddress32)
{
  dwordType x;
  
  x.byte.byte0 = i2c_read_byte_24AA1025(regAddress32++);
  x.byte.byte1 = i2c_read_byte_24AA1025(regAddress32++);
  x.byte.byte2 = i2c_read_byte_24AA1025(regAddress32++);
  x.byte.byte3 = i2c_read_byte_24AA1025(regAddress32++);
  return x.dword;
}

// *** LOG EEPROM functions begin ***
// usa la SRAM dell'RTC di 56 bytes
// e la EEPROM ESTERNA 24AA1025
// I2C EEPROM 128K x 8 address: 00000-1FFFF, 
// paginata 64K e paginata 128 byte
//
// --- comandi gestiti ---
// E S <delay min>     log start
// E S 0               log stop / pause
// E R <key>           log clear con chiave key = 12379
// E L 0               log status	

void exeMANAGE_LOG() {
  byte      variant;
  dwordType dw;
  wordType  wparam;
  
  variant = cmdMessenger.readChar();
  wparam.word = cmdMessenger.readInt();
  
  switch (variant) {
     case 'S':
       if (wparam.word > 0) {
           // log start
           if (i2c_read_byte(aSRAM, LOG_SRAM_VALID_ADDR) == LOG_SRAM_VALID_VAL) {
             DateTime now = RTC.now();
             
             // setup log
             i2c_write_word(aSRAM, LOG_SRAM_DELAY_ADDR, wparam.word);
             i2c_write_long(aSRAM, LOG_SRAM_TIME_ADDR, now.unixtime());          
             Serial << "LOG START ";
             showTime(i2c_read_long(aSRAM, LOG_SRAM_TIME_ADDR));
             Serial << endl;
             LEDRblink = 1; // log running feedback  
           }        
       } else {
           // log stop / pause
           i2c_write_word(aSRAM, LOG_SRAM_DELAY_ADDR, 0); // timer = 0
           Serial << "LOG STOP" << endl;
           LEDRblink = 0; // log stopped feedback
       }
       break;
       
     case 'R':
       if (wparam.word == 12379) {
         clearLOG();
       } else {
         unknownCmd();
       }
       break;
       
     case 'L':
       // answers:
       // LOG OFF <timings> <nr readings> <last timestamps>
       // LOG ON  <timings> <nr readings> <last timestamps>
       wparam.word = i2c_read_word(aSRAM, LOG_SRAM_DELAY_ADDR);
       dw.dword    = i2c_read_long(aSRAM, LOG_FILE_NR_READINGS) - 1; // è la succ. lettura (++)
       
       Serial << "LOG ";
       if (wparam.word) {
          Serial << "ON";
       } else {
          Serial << "OFF";
       }
       Serial << " " << wparam.word << " " << dw.dword;
       if (wparam.word) {
         // ON
         Serial << " ";
         showTime(i2c_read_long(aSRAM, LOG_SRAM_TIME_ADDR));
       }
       Serial << endl; 
       break;
       
     default:
       unknownCmd();
  }
  arduino_ready();
}

void unpackRelStatus(byte code) {
  Serial <<  "C1" << codeRelPin((code & 0b00000001))
         << ",C2" << codeRelPin((code & 0b00000010) >> 1)
         << ",C3" << codeRelPin((code & 0b00000100) >> 2)
         << ",C4" << codeRelPin((code & 0b00001000) >> 3); 
}

// bisogna inviare un comando F per ogni record che si vuole scaricare
// non inviare altri comandi mentre si scarica il log
// se si invia un comando G il log dump viene interrotto
void exeREAD_LOG() {
  uint8_t  recordType;
  
  switch (bLogDump) {
    case logDUMP_OFF: // start log dump
      bLogDump = logDUMP_RUN_1;
      iLogAddress = i2c_read_long(aSRAM, LOG_SRAM_EEPTRS_ADDR);
      iLogIndex = 1;
      Serial << endl << "<log>" << endl;
      break;
      
    case logDUMP_RUN_1:
    case logDUMP_RUN_2:
      recordType = i2c_read_byte_24AA1025(iLogAddress++);
      //Serial << "->" << recordType << " " << char(recordType) << " " << iLogAddress << endl;
      switch (recordType) {
        case 'S': {
            int fraPart, intPart, check, vlLog;
            byte errorStatusLog, relCode;
            float temperatureLog, humidityLog, dewpointLog, vbatLog;
            dwordType timestamp;
            
            check = recordType;      
            timestamp.dword = i2c_read_long_24AA1025(iLogAddress);  // timestamp
            check ^= timestamp.byte.byte0;
            check ^= timestamp.byte.byte1;
            check ^= timestamp.byte.byte2;
            check ^= timestamp.byte.byte3;
            iLogAddress += 4;
     
            fraPart = i2c_read_byte_24AA1025(iLogAddress++);  // TL temperature low byte: 00..99
            intPart = i2c_read_byte_24AA1025(iLogAddress++);  // TH temperature high byte: -127..+127
            check ^= byte(fraPart) ^ byte(int(intPart));
            temperatureLog = intPart + fraPart / 100.0;
            
            fraPart = i2c_read_byte_24AA1025(iLogAddress++);  // HL humidity low byte: 00..99
            intPart = i2c_read_byte_24AA1025(iLogAddress++);  // HH humidity high byte: -127..+127
            check ^= byte(fraPart) ^ byte(int(intPart));
            humidityLog = intPart + fraPart / 100.0;
            
            fraPart = i2c_read_byte_24AA1025(iLogAddress++);  // DL dew point low byte: 00..99
            intPart = i2c_read_byte_24AA1025(iLogAddress++);  // DH dew point high byte: -127..+127
            check ^= byte(fraPart) ^ byte(int(intPart));
            dewpointLog = intPart + fraPart / 100.0;
            
            relCode = i2c_read_byte_24AA1025(iLogAddress++);
            check ^= byte(relCode);
            
            errorStatusLog = i2c_read_byte_24AA1025(iLogAddress++);
            check ^= byte(errorStatusLog);
            
            fraPart = i2c_read_byte_24AA1025(iLogAddress++);  // V batt low byte: 00..99
            intPart = i2c_read_byte_24AA1025(iLogAddress++);  // V batt high byte: -127..+127
            check ^= byte(fraPart) ^ byte(int(intPart));
            vbatLog = intPart + fraPart / 100.0;   
            
            fraPart = i2c_read_byte_24AA1025(iLogAddress++);  // V light low byte: 0..32768
            intPart = i2c_read_byte_24AA1025(iLogAddress++);  // V light high byte
            check ^= byte(fraPart) ^ byte(int(intPart));
            vlLog = intPart * 256 + fraPart;        
            
            //Serial << "CRCR:" << int(check) << " CRCR:" << i2c_read_byte_24AA1025(iLogAddress) << endl;
            if (check != i2c_read_byte_24AA1025(iLogAddress++))  { 
              errorStatusLog |= logSTAT_CRC;
            } 
            Serial << " <n>" << int(iLogIndex++) << ",";
            showTime(timestamp.dword);
            Serial << ",T[C]:" << temperatureLog << ",H[%]:" << humidityLog << ",D[C]:" << dewpointLog <<
                      ",V:" << vbatLog << ",L:" << int(vlLog) << ",";
            unpackRelStatus(relCode);
            Serial << "," << int(errorStatusLog) << "</n>" << endl;
            
            nextRecord();
          }
          break;
        
       case 'X': {
            bLogDump = logDUMP_END;     
          }
          break;
          
       default: {
            // trovato altro: skip record
            iLogAddress += LOG_SLOT_LEN - 1;
            nextRecord();
          }
          break;
      }
      break;
      
    case logDUMP_END:
      Serial << "</log>" << endl;
      bLogDump = logDUMP_OFF;
      break;
  }
}

void nextRecord() {
    if ((bLogDump == logDUMP_RUN_1) && 
        (iLogAddress > EE24AA1025_END_ADDR)) {
           iLogAddress = EE24AA1025_START_ADDR;
           bLogDump = logDUMP_RUN_2;
           
    } else if (bLogDump == logDUMP_RUN_2) {
           uint32_t iLogEndAddress;
           iLogEndAddress = i2c_read_long(aSRAM, LOG_SRAM_EEPTRE_ADDR);
           if (iLogAddress > iLogEndAddress) {
              bLogDump = logDUMP_END;
           }
    }  
}

void exeINIT_LOG() {	
  // test go/nogo per la sram / rtc 
  if (i2c_read_byte(aSRAM, LOG_SRAM_VALID_ADDR) != LOG_SRAM_VALID_VAL) {
      // init blank log
      i2c_write_byte(aSRAM, LOG_SRAM_VALID_ADDR, LOG_SRAM_VALID_VAL);
      clearLOG();
  }
  if (i2c_read_byte(aSRAM, LOG_SRAM_VALID_ADDR) != LOG_SRAM_VALID_VAL) {
    Serial << endl << "SRAM_FAULT" << endl;
    logStatus |= logSTAT_SRAM;
  }
  // test go/nogo per la eeprom esterna 
  if (i2c_read_byte_24AA1025(EE24AA1025_VALID_ADDR) != EE24AA1025_VALID_VAL) {
      i2c_write_byte_24AA1025(EE24AA1025_VALID_ADDR, EE24AA1025_VALID_VAL);
  }
  if (i2c_read_byte_24AA1025(EE24AA1025_VALID_ADDR) != EE24AA1025_VALID_VAL) {
    Serial << endl << "EEPROM_FAULT" << endl;
    logStatus |= logSTAT_EEPR;
  }
  DateTime now = RTC.now();          
  // setup log
  i2c_write_long(aSRAM, LOG_SRAM_TIME_ADDR, now.unixtime()); 
  logStatus |= logSTAT_POW;
  // feedback log
  wordType  delayMin;
  delayMin.word = i2c_read_word(aSRAM, LOG_SRAM_DELAY_ADDR);
  
  if (((logStatus & logSTAT_SRAM) == 0) &&
      ((logStatus & logSTAT_EEPR) == 0) &&
      (((int)delayMin.word) != 0)) {
     Serial << endl << "LOG START ";
     showTime(i2c_read_long(aSRAM, LOG_SRAM_TIME_ADDR));
     Serial << endl;
     LEDRblink = 1; // log running feedback      
  }
  arduino_ready();
}

void clearLOG() {
  Serial << endl << "WAIT" << endl;
  i2c_write_word(aSRAM, LOG_SRAM_DELAY_ADDR, 0);                      // timer = 0
  i2c_write_long(aSRAM, LOG_SRAM_EEPTRS_ADDR, EE24AA1025_START_ADDR); // log reset pointers
  i2c_write_long(aSRAM, LOG_SRAM_EEPTRE_ADDR, EE24AA1025_START_ADDR);
  i2c_write_long(aSRAM, LOG_SRAM_TIME_ADDR, 0);
  i2c_write_long(aSRAM, LOG_FILE_NR_READINGS, 1);                     // è sempre un numero ++
  
  // place 'X' (deleted mark) on all log slots
  for (uint32_t regAddress32 = EE24AA1025_START_ADDR; 
       regAddress32 <= EE24AA1025_END_ADDR; 
       regAddress32 += LOG_SLOT_LEN) {   
         wdt_reset(); // max 8 secs cycle
         if (i2c_read_byte_24AA1025(regAddress32) != 'X') {
           // for eeprom endurance
           i2c_write_byte_24AA1025(regAddress32, 'X');
         }
  } 
  Serial << "LOG CLEAR" << endl;
  arduino_ready();
}

void processLOG_TIME() {
  dwordType dw;
  wordType  delayMin;
  DateTime  now = RTC.now();
  
  if (i2c_read_byte(aSRAM, LOG_SRAM_VALID_ADDR) != LOG_SRAM_VALID_VAL) {
    // SRAM ext fault
    logStatus |= logSTAT_SRAM;
    return;
  }
  
  if (i2c_read_byte_24AA1025(EE24AA1025_VALID_ADDR) != EE24AA1025_VALID_VAL) {
    // EEPROM ext fault
    logStatus |= logSTAT_EEPR;
    return;
  }
  
  delayMin.word = i2c_read_word(aSRAM, LOG_SRAM_DELAY_ADDR);

  if (((int)delayMin.word) != 0) {
    uint32_t timestamp = i2c_read_long(aSRAM, LOG_SRAM_TIME_ADDR) + 60 * delayMin.word;
    
//    Serial << "DBG ";
//    showTime(timestamp);
//    Serial << " ";
//    timestamp += 60 * delayMin.word;
//    showTime(timestamp);
//    Serial << " ";
//    showTime(now.unixtime());
//    Serial << endl;
    
    if (timestamp <= now.unixtime()) {
      byte   check;
      double intPart,fraPart;
       
      LEDRblink = 1; // log running feedback
       
      // save log on EEPROM      
      logRecord[0] = 'S';               // type record
      dw.dword = now.unixtime();        // timestamp
      logRecord[1]  = dw.byte.byte0;
      logRecord[2]  = dw.byte.byte1;
      logRecord[3]  = dw.byte.byte2;
      logRecord[4]  = dw.byte.byte3;
      fraPart = modf(temperature, &intPart);  
      logRecord[5]  = int(fraPart * 100);    // TL temperature low byte: 00..99
      logRecord[6]  = int(intPart);          // TH temperature high byte: -127..+127
      fraPart = modf(humidity, &intPart);     
      logRecord[7]  = int(fraPart * 100);    // HL humidity low byte: 00..99
      logRecord[8]  = int(intPart);          // HH humidity high byte: -127..+127
      fraPart = modf(dewpoint, &intPart);
      logRecord[9]  = int(fraPart * 100);    // DL dew point low byte: 00..99
      logRecord[10] = int(intPart);          // DH dew point high byte: -127..+127
      logRecord[11] = packRelStatus();       // RL rele status, bit position: 00004321
      logRecord[12] = logStatus;
      fraPart = modf((((float)vBAT) / 100.0), &intPart);
      logRecord[13] = int(fraPart * 100);    // V batt low byte: 00..99
      logRecord[14] = int(intPart);          // V batt high byte: -127..+127
      logRecord[15] = lowByte(vLIGHT);       // VL light low byte: 0..32768
      logRecord[16] = highByte(vLIGHT);      // VL light high byte
      
      check = 0;
      for (byte i = 0; i < LOG_SLOT_LEN - 1; i++) {
        check ^= byte(logRecord[i]);
      }
      logRecord[LOG_SLOT_LEN - 1] = check;   // CRC
      log_record_end(LOG_SLOT_LEN);
      
      //Serial << "CRCW:" << int(check) << endl;
      
      // log timer expired, insert new timestamp
      i2c_write_long(aSRAM, LOG_SRAM_TIME_ADDR, timestamp);
      // nr reading increments (1)
      dw.dword = i2c_read_long(aSRAM, LOG_FILE_NR_READINGS);
      // signal log write
      Serial << "WLOG " << dw.dword << " ";
      showTime(i2c_read_long(aSRAM, LOG_SRAM_TIME_ADDR));
      Serial << endl;
      // nr reading increments (2)
      i2c_write_long(aSRAM, LOG_FILE_NR_READINGS, ++dw.dword);
      // reset log status
      logStatus = logSTAT_OK;
    }
  }
}

// va messo per chiudere il record e registrare in definitivo
// la pagina su EEPROM I2C esterna 128k x 8
// il buffer Wire.h in tx è generalmente dichiarato a 32 byte
// siamo sempre sicuri che la scrittura cade nella pagina EEPROM
// dichiarare quanti bytes si intendono scrivere in totale
// byte = 1, int/word = 2, long = 4
// prima della chiusura del record
void log_record_end(int data_len) {
  uint32_t  regAddress32 = i2c_read_long(aSRAM, LOG_SRAM_EEPTRE_ADDR);
  
  if ((regAddress32+data_len) > EE24AA1025_END_ADDR) {
       regAddress32 = EE24AA1025_START_ADDR;
  }
  i2c_write_buffer_24AA1025(regAddress32, logRecord, data_len);
  
  regAddress32 = (regAddress32+data_len) & 0x0001FFFF; // att! Bitwise AND
  //Serial << "SIMULATE RECORDING " << (unsigned long)regAddress32 << " - " << data_len << endl;
  i2c_write_long(aSRAM, LOG_SRAM_EEPTRE_ADDR, regAddress32);
}

#ifdef LOG_DEBUG
unsigned long htoi(char f[]) {
    unsigned long  z, n;
    n = 0;

    for (z = 0; f[z] >= '0' && f[z] <= 'F'; ++z) 
        if (f[z] >= 'A' && f[z] <= 'F')
            n = 10 + 16 * n + (f[z] - 'A');
        else
            n = 16 * n + (f[z] - '0');
    return n;
}

//////////////////////////////////////////////////////////////
// formato:
// J <chip> <from> <to>
// J 0
// J 1 <from> <to>
// J 2 log status
// chip = 0 -> sram   00->3F
// chip = 1 -> eeprom 00000->1FFFF, esprimere in interi hex
//////////////////////////////////////////////////////////////
void exeDUMP() {
  byte chip = cmdMessenger.readInt();
  uint8_t ii;
  byte theValue;
  char textString[10];
  char asciiDump[18];
  unsigned long toAddr;
  dwordType fromAddr;
  
  if (chip == 0) {
    // dump external SRAM
    Serial << "SRAM ";
    fromAddr.dword = 0;
    toAddr = 63;
  } else if (chip == 1) {
    // dump external EEPROM
    Serial << "EEPROM ";
    char addr32[8+1];
    
    cmdMessenger.copyString(addr32,8);
    fromAddr.dword = htoi(addr32);
    cmdMessenger.copyString(addr32,8);
    toAddr = htoi(addr32);
  }
  
  if (chip <= 1) {
    // SRAM / EEPROM DUMP
    sprintf(textString, "%05lX", fromAddr);
    Serial << textString << " ";
    sprintf(textString, "%05lX", toAddr);
    Serial << textString << endl;
    
    while (toAddr >= fromAddr.dword) {
      wdt_reset();
      sprintf(textString, "%05lX - ", fromAddr);
      Serial << textString;
      asciiDump[0] = 0;
      for (ii=0; ii<16; ii++) {
        if (chip == 0) {
          theValue = i2c_read_byte(DS1307_ADDRESS, fromAddr.dword);
        } else if (chip == 1) {
          theValue = i2c_read_byte_24AA1025(fromAddr.dword); 
        }
        sprintf(textString, "%02X ", theValue);
        Serial << textString;
        if ((theValue >= 0x20) && (theValue < 0x7f)) {
	    asciiDump[ii % 16] = theValue;
        } else {
	    asciiDump[ii % 16] = '.';
	}
        fromAddr.dword++;
     }
     asciiDump[16] = 0;
     Serial << asciiDump << endl;
   }  
  } else if (chip == 2) {
    // LOG STATUS
    theValue = i2c_read_byte(aSRAM, LOG_SRAM_VALID_ADDR);
    Serial << "L:V " << (unsigned int)theValue << endl;
    Serial << "L:D " << (unsigned long)i2c_read_word(aSRAM, LOG_SRAM_DELAY_ADDR)  << endl;
    Serial << "L:S " << (unsigned long)i2c_read_long(aSRAM, LOG_SRAM_EEPTRS_ADDR) << endl;
    Serial << "L:E " << (unsigned long)i2c_read_long(aSRAM, LOG_SRAM_EEPTRE_ADDR) << endl;
    Serial << "L:T ";
    showTime(i2c_read_long(aSRAM, LOG_SRAM_TIME_ADDR));
    Serial << endl;
    theValue = i2c_read_byte(aSRAM, LOG_FILE_SRAM_STAT);
    Serial << "L:S " << (unsigned int)theValue << endl;
  }
  arduino_ready();
}
#endif

#ifdef DEV_SENSIRION_SHT1x
void getMeasureTest() {
    temperature = random(100) - 50 + random(100) / 100;
    humidity = 40 + random(50) / 5 - 2.5;
    dewpoint = 12 + random(100) / 5 - 5;  
    if (random(100) > 90) {
      errorStatus = S_Err_NoACK;
    } else {
      errorStatus = S_Meas_Rdy;
    }
}
#endif

void showTime(const uint32_t value) {
  DateTime now = DateTime(value);
  
  Serial << now.year() << "-";
  printDigits(now.month());
  Serial << "-";
  printDigits(now.day());
  Serial << " ";
  printDigits(now.hour());
  Serial << ":";
  printDigits(now.minute());
  Serial << ":";
  printDigits(now.second());
}

#if 0
void testeeprom() {
  byte theValue;
  unsigned int eeaddress = 0;
  
   theValue = 1;
   for (int i = 0; i < 10; i++) {
     Wire.beginTransmission(EE24AA1025_ADDRESS | logPAGE_1);
     Wire.send((int)(eeaddress >> 8));   // MSB
     Wire.send((int)(eeaddress & 0xFF)); // LSB
     Wire.send(theValue);
     Wire.endTransmission();
     delay(5);
     
     Serial << eeaddress << " W:" << (int)theValue << endl;

     eeaddress++;
     theValue++;
   }

   for (unsigned int i = 0; i < 10; i++) {
     theValue = readEEPROM(EE24AA1025_ADDRESS | logPAGE_1, i);
     Serial << i << " R:" << (int)theValue << endl;
   }
}
#endif

#if 0
void testeeprombyte() {
  byte value = 0;
  int count = 0;
  
  for (long addr = 0; addr <= 0x1FFFF; addr++) {
     i2c_write_byte_24AA1025(addr, value++);
     if (value > 12) { value = 0; }
     if (count++ > 500) {
       Serial << "W: " << addr << endl;
       count = 0;
     }
     wdt_reset();
  }
}
#endif

#ifdef DEV_LM35DZ
void initLM35DZ() {
  pinMode(LM35DZ_PIN, INPUT);
}

#define LM35_READINGS_NR  20
void measureLM35DZ() {
  unsigned long t1_measure = 0;
  
  for (byte i = 0; i < LM35_READINGS_NR; i++) {
    wdt_reset();
    t1_measure += analogRead(LM35DZ_PIN);
  }
  t1_measure /= LM35_READINGS_NR;
  // conversione: + 10mV/C
  temperature = (t1_measure * 5.0 / 1024.0) * 100.0;
  
  // debug
  Serial << "TL: " << temperature << endl;
}
#endif

#ifdef DEV_HCZ-5D-A
void initHCZ5DA() {
  pinMode(HCZ_5D_A_DRV1_PIN, OUTPUT);
  digitalWrite(HCZ_5D_A_DRV1_PIN, LOW);
  pinMode(HCZ_5D_A_DRV2_PIN, OUTPUT);
  digitalWrite(HCZ_5D_A_DRV2_PIN, LOW);  
}

#define HCZ5DA_READINGS_NR  30
void measureHCZ5DA() {
  unsigned long h1_measure = 0;
  unsigned long h2_measure = 0;
  
  // +5->DRV1->R->SENS->GND: PULL DOWN
  pinMode(HCZ_5D_A_DRV1_PIN, OUTPUT);
  pinMode(HCZ_5D_A_DRV2_PIN, INPUT);
  digitalWrite(HCZ_5D_A_DRV2_PIN, LOW);  // GND->DRV2
  tone(HCZ_5D_A_DRV1_PIN, 1000);         // +5->DRV1
  for (byte i = 0; i < HCZ5DA_READINGS_NR; i++) {
    h1_measure += analogRead(HCZ_5D_A_IN_PIN);
    delay(100);
  }
  noTone(HCZ_5D_A_DRV1_PIN);
  h1_measure /= HCZ5DA_READINGS_NR;
  
  // +5->DRV2->SENS->R->GND: PULL UP
  pinMode(HCZ_5D_A_DRV1_PIN, INPUT);
  pinMode(HCZ_5D_A_DRV2_PIN, OUTPUT);
  digitalWrite(HCZ_5D_A_DRV1_PIN, LOW);  // GND->DRV1
  tone(HCZ_5D_A_DRV2_PIN, 1000);         // +5->DRV2
  for (byte i = 0; i < HCZ5DA_READINGS_NR; i++) {
    h2_measure += analogRead(HCZ_5D_A_IN_PIN);
    delay(100);
  }
  noTone(HCZ_5D_A_DRV2_PIN);
  h2_measure /= HCZ5DA_READINGS_NR;
  
  initHCZ5DA();
  
  // calculation...
  
  // debug
  Serial << "H1: " << h1_measure << endl;
  Serial << "H2: " << h2_measure << endl;
}

float calcDewpoint(float humidity, float temperature) {
  return 0;
}
#endif;

#ifdef DEV_DHT11
// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm
float calcDewpoint(float humidity, float celsius)
{
   double RATIO = 373.15 / (273.15 + celsius);  // RATIO was originally named A0, possibly confusing in Arduino context
   double SUM = -7.90298 * (RATIO - 1);
   SUM += 5.02808 * log10(RATIO);
   SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
   SUM += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
   SUM += log10(1013.246);
   double VP = pow(10, SUM - 3) * humidity;
   double T = log(VP/0.61078);   // temp var
   return (241.88 * T) / (17.558 - T);
}
#endif

#ifdef HC_05_PROGRAM
// HC-05 program sequence
// 1. definire HC_05_PROGRAM
// 2. PIN 24 KEY = OFF (= GND) = switch KEY inserito
// 3. BT switch OFF
// 4. compilare e programmare arduino
// 5. spegnere arduino e scollegare USB
// 6. PIN 24 KEY = ON (= 3V3) = switch KEY in 3V3 esterno
// 7. BT switch ON
// 8. power on arduino
// 9. attendere il lampeggio alternato dei leds
// 10.spegnere arduino
// 11.PIN 24 KEY = OFF (= GND) = switch KEY inserito
// 12.commentare HC_05_PROGRAM
// 13.BT switch OFF
// 14.compilare e programmare arduino
// 15.BT switch ON
// 8. power on arduino con il programma finale
void programHC_05() {
  pinMode(cLEDLPin, OUTPUT);
  pinMode(cLEDRPin, OUTPUT);
  digitalWrite(cLEDLPin, HIGH);
  digitalWrite(cLEDRPin, HIGH);
  
  Serial.end();
  delay(20);
  Serial.begin(38400);
  delay(20);
  Serial << "AT+UART=" << cBAUDRATE << "\r\n";
  delay(1000);
  Serial << "AT+NAME=HC-05-" << cSerialCode << "\r\n";
  wdt_reset();
  delay(1000);
  Serial << "AT+DISC\r\n";
  delay(1000);
  wdt_reset();
  
  while(1) {
     wdt_reset(); // max 8 secs cycle
     digitalWrite(cLEDLPin, HIGH);
     digitalWrite(cLEDRPin, LOW);
     delay(200);
     digitalWrite(cLEDLPin, LOW);
     digitalWrite(cLEDRPin, HIGH);
     delay(200);
  }
}
#endif

#ifdef HC_05_BAUDRATE_FIND 
void printBaudRateHC_05() {
  Serial.end();
  delay(20);
  Serial.begin(9600);
  delay(20);
  Serial << "BR: " << 9600 << endl;
  exeSTARTUP();
  
  Serial.end();
  delay(20);
  Serial.begin(38400);
  delay(20);
  Serial << "BR: " << 38400 << endl;
  exeSTARTUP();
  
  Serial.end();
  delay(20);
  Serial.begin(57600);
  delay(20);
  Serial << "BR: " << 57600 << endl;
  exeSTARTUP();
  
  Serial.end();
  delay(20);
  Serial.begin(115200);
  delay(20);
  Serial << "BR: " << 115200 << endl;
  exeSTARTUP();
  
  Serial.end();
  delay(20);
  Serial.begin(cBAUDRATE);
  delay(20);
  Serial << "BR* " << cBAUDRATE << endl;
  exeSTARTUP();
}
#endif

// compute the phase day (0 to 29, where 0=new moon, 15=full etc.)
void ComputeMoonPhase() {
  DateTime now = RTC.now();
  
  if ((moonDD != now.day()) || (moonMM != now.month()) || (moonYY != now.year())) {
    moonDD = now.day();
    moonMM = now.month();
    moonYY = now.year();
  
    float n = floor(12.37 * (moonYY - 1900 + ((1.0 * moonMM - 0.5)/12.0)));
    #define RAD (3.14159265/180.0)
    float t = n / 1236.85;
    float t2 = t * t;
    float as = 359.2242 + 29.105356 * n;
    float am = 306.0253 + 385.816918 * n + 0.010730 * t2;
    float xtra = 0.75933 + 1.53058868 * n + ((1.178e-4) - (1.55e-7) * t) * t2 
          + (0.1734 - (3.93e-4) * t) * sin(RAD * as) - 0.4068 * sin(RAD * am);
    uint32_t i = (xtra > 0.0 ? floor(xtra) : ceil(xtra - 1.0));
    uint32_t j1 = julday(moonYY, moonMM, moonDD);
    uint32_t jd = (2415020 + 28 * n) + i;
    moonPhase = (j1-jd + 30)%30;
  }
}

uint32_t julday(int year, byte month, byte day) {
  if (year < 0) { year ++; }
  int  jy = year;
  byte jm = month + 1;
  if (month <= 2) { jy--; jm += 12; } 
  uint32_t jul = floor(365.25 * jy) + floor(30.6001 * jm) + day + 1720995;
  if (day+31*(month+12*year) >= (15+31*(10+12*1582))) {
    int ja = floor(0.01 * jy);
    jul = jul + 2 - ja + floor(0.25 * ja);
  }
  return jul;
}

