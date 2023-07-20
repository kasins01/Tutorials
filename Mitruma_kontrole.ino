#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS1307 RTC;

#define setMoistureValue = A ;
#define minMoistureValue = min;
#define maxMoistureValue = max;
#define settableMoistureValue = Am;                                    
#define switchOnTime = T1, T2, T3, T4;                                 
#define timeCheckPoint = TP1, TP2;                                     
#define setLevelValue = B;                                             
#define rightHour = T;                                                 
#define moistureSensorAnalogReadMaxValue = AMmax;
#define moistureSensorAnalogReadMinValue = AMmin;                
#define levelSensorAnalogReadMaxValue = AUmax;                         
#define levelSensorAnalogReadMinValue = AUmin;                         
#define moistureValue = V1;                                            
#define resultOfComparisonForMoistureValue = SM;                       
#define resultOfComparisonForLevelValue = SU;                          
#define controlAccuracy = Prec;                                        
#define controlSignalForPumpTransistor = Pump;                         
#define inputState = active;                                           


int A, Am, min, max, B, 
    AMmax, AMmin, AUmax, AUmin, V1, SM, SU, Prec;                      
const long TP1=10000, TP2=3000;                                        
bool Up, Down, Enter, Pump, T1, T2, T3, T4, active;                    
bool pressed=0, pressedEnt=0;                                         
int moistureSensorValue                                            ;// Mitruma sensora vērtības
int levelSensorValue                                                  ;// Ūdens līmeņa sensora vērtības
unsigned long entt=0, prest=0;
unsigned long Ti=millis();

String fixZero(int i){
  String ret;                                                          
  if (i < 10) ret += "0";                                              
  ret += i;                                                            
  return ret;
  }

void setup(){
lcd.init();                                                                                 
lcd.begin(16,2);
lcd.backlight();                                                       
lcd.setCursor(0,0);                                                    
      
Wire.begin();                                                          
RTC.begin();                                                           
if (!RTC.isrunning()){
// Laiks, kas uzstādas no datora, ieinstalējot programmu 
RTC.adjust(DateTime(__DATE__, __TIME__));
}

// Arduino pinu inicializēšana
pinMode(13,INPUT_PULLUP)                                     ;//UpPin =13
pinMode(12,INPUT_PULLUP)                                     ;//DownPin =12
pinMode(8,INPUT_PULLUP)                                       ;//EnterPin =8
pinMode(A0,INPUT)                                                     ;//moistureSensorPin = A0 
pinMode(A1,INPUT)                                                     ;//levelSensorPin = A1
pinMode(2,OUTPUT)                                                     ;//moisturePowerPin = 2 
pinMode(4,OUTPUT)                                                     ;//levelPowerPin = 4
pinMode(7,OUTPUT)                                                     ;//PumpPin = 7

//Inicializēšana
Up = true;                                                             
Down = true;                                                           
Enter = true;                                                          
active = false;                                                       
B = 50;                                                                
AMmax = 1023                                                              ;//Sauss                                                  
AMmin = 350                                                               ;//Mitrs
if (moistureSensorValue<AMmin) moistureSensorValue = AMmin;
AUmax = 560                                                               ;//Augsts ūdens līmenis                                                  
AUmin = 0                                                                 ;//Zems ūdens līmenis
min = 0;                                                               
max = 100;                                                             
A = 50;                                                                
Prec = 1;                                                              
Am = 50;                                                                                                                         
V1 = map(moistureSensorValue, AMmax, AMmin, 0, 100);
SM = V1-A;                                                             
if (V1>100) V1=100;                                                    
if (V1<0) V1=0;                                                        
SU = map(levelSensorValue, AUmax, AUmin, 100, 0)-B ;
}

// Reālā laika funkcija
void RightHour(){
  DateTime Now = RTC.now();                                            
  String T = "Time: ";                                                 
  
  int _hour = Now.hour();                                              
  int _minute = Now.minute();                                          
  int _second = Now.second();                                          
  
  T += fixZero(_hour);
  T += ":" ;
  T += fixZero(_minute);                                               
  T += ":" ;                                                           
  T += fixZero(_second);                                              
  
  lcd.clear();                                                         
  lcd.setCursor(0, 0);                                                 
  lcd.print(T);                                                        
  delay(200);
  }
  
//Sūkņa vadība
void Control()  
{
 if ((SM <= -Prec) && (SU <= -Prec))  Pump = true;
 if (Pump ==true) digitalWrite(7, HIGH);
 if ((SM >= Prec) && (SU<= -Prec)) Pump = false;              
 if (Pump == false){
 digitalWrite(7, LOW), digitalWrite(4, LOW),  digitalWrite(2, LOW); }                                       
 if ((SM >= Prec) && (SU >= Prec)) Pump = false;                                                         
 if (Pump == false){
 digitalWrite(7, LOW), digitalWrite(4, LOW),  digitalWrite(2, LOW); }                              
 if ((SM <= -Prec) && (SU >= Prec)) Pump = false;                                                     
 if (Pump == false){ 
 digitalWrite(7, LOW), digitalWrite(4, LOW),  digitalWrite(2, LOW); 
 }}
                                                                      
//Izvade uz ekrānu
void Output() 
{
 if ((SM < 0) && (SU < 0)){
 lcd.clear();                                                          
 lcd.setCursor(0,0);                                                   
 lcd.print("Mitrums");                                                 
 lcd.setCursor(7,0);
 lcd.print("%");
 lcd.setCursor(8,0);
 lcd.print(V1);  
 lcd.setCursor(0,1);
 lcd.print("PUMP:ON");
 lcd.setCursor(11,0);
 lcd.print("Sausa");
 lcd.setCursor(11,1);
 lcd.print("Zeme");
 delay (500);
}
 
 if ((SM >= 0) && (SU < 0)){                                 
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Mitrums");
 lcd.setCursor(7,0);
 lcd.print("%");
 lcd.setCursor(8,0);
 lcd.print(V1);
 lcd.setCursor(0,1);
 lcd.print("PUMP:OFF");
 lcd.setCursor(11,0);
 lcd.print("Mitra");
 lcd.setCursor(11,1);
 lcd.print("Zeme");
 delay(5000)                                                          ;//5s
 }
 if (SU >= 0){
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("   Paarpluude");                                       
 delay(5000)                                                          ;//5s
 }}
 
void Sensors()
{  
  digitalWrite(4, HIGH);
   moistureSensorValue = analogRead(A0);
   levelSensorValue = analogRead(A1); 
   if (moistureSensorValue<AMmin) moistureSensorValue=AMmin;
   V1=map(moistureSensorValue, AMmax, AMmin, 0, 100);
   if (V1>100) V1=100;
   if (V1<0) V1=0;
   SM=V1-A;
   SU=map(levelSensorValue, AUmax, AUmin, 100, 0)-B;
   }

   
//Pogas
void Input()
{
  if ((Up==true || Down==true || Enter==true)) prest=millis();
  else {
  if (millis()>prest+TP1) active=false; 
  if (Enter==false) {pressedEnt=0;}
  return;}

 if (active==false){
  active=true;
  Am=A;}
  else {
 if (Up==true && !pressed){
  Am=Am+1;
  if (Am>max){
  Am=max;}
  pressed=1;}

 if (Down==true && !pressed){
  Am=Am-1;
  if(Am<min){
  Am=min;}
  pressed=1;}

 if (Up==true||Down==true){ 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mitrums");
  lcd.setCursor(7,0);
  lcd.print("%");
  lcd.setCursor(8,0);
  lcd.print(Am);
  delay(200);}
 
 if (Enter==true && !pressedEnt && !pressed){
  pressedEnt=1;
  pressed=1;
  entt=millis();}

 if (Enter==true && pressedEnt==1 && millis()>entt+TP2){
  pressedEnt=0;
  active=false;
  A=Am; 
  lcd.clear();}}
 if ((Up==false || Down==false || Enter==false)) pressed=0;
 }

  
 void loop()
 {
  Up=!digitalRead(13);
  Down=!digitalRead(12);
  Enter=!digitalRead(8);

  Input();

  DateTime myRTC = RTC.now();
  int H = myRTC.hour(); 
  int M = myRTC.minute();
  int S = myRTC.second();

  T1=((H ==22) && (M ==00) && (S == 00));
  T2=((H ==22) && (M ==30) && (S == 00));
  T3=((H ==06) && (M ==00) && (S == 00));
  T4=((H ==06) && (M ==30) && (S == 00));

  if (!active){
  lcd.clear();     
  RightHour();}
  

// Ja zeme ir sausa un ir īstais laiks laistīšanai 
 if(T1==HIGH ||T2==HIGH ||T3==HIGH ||T4==HIGH){                                                    
  digitalWrite(2, HIGH);
  delay(100);
  Sensors();
  Control();
 while(Pump==true){
  Sensors();
  Control();
 if((SM <0)&&(SU <0)){                                                                                                
  Output();}
 if((SM >=0) && (SU<0)){                                                   
  Output();         
  return;}
 else if
  (SU>0){
  Output();
  return ;
  }}}}
 
