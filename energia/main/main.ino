#include "LiquidCrystal.h"

LiquidCrystal lcd(7, 2, 14, 3, 4, 19, 38);

//int beat_old = 0;
//int BPM = 0;
//float beats[500];
//int beatIndex;
//
//int value;
//
//float threshold = 2481.0;
//bool belowThreshold = true;
//
//byte chars[8] = {
//  0b00000,
//  0b00000,
//  0b00000,
//  0b00000,
//  0b00000,
//  0b00000,
//  0b00000,
//  0b00000
//};
//
//void setup() {
//  // put your setup code here, to run once:
//  lcd.begin(16, 2);
//  lcd.print("THE DOCTOR IS IN.");
//  lcd.print("BPM");
//
//
//  Serial.begin(9600);
//
//  pinMode(A1, INPUT);
//  pinMode(23, INPUT);
//  pinMode(24, INPUT);
//  
//}
//
//
//
//void loop() {
//  // put your main code here, to run repeatedly: 
//  Serial.println(map(analogRead(A1), 0, 4095, 0, 7));
//
//
//  lcd.setCursor(0, 1);  
//  lcd.print("BPM: ");
//  
//  if((digitalRead(23) == 1)||(digitalRead(24) == 1)){
//    lcd.print("---      ");
//    
//  } else {
//    
//    value = analogRead(A1);
//
//    if (millis() % 128 == 0)
//      BPM = map(value, 0, 4095, 0, 200);
//
//    if (BPM < 300) {
//      lcd.print(BPM);
//      lcd.print("    ");
//    }
//      
//    
//    if (value > threshold && belowThreshold == true) {
////      calculateBPM();
//      belowThreshold = false;
//      
//    } else if(value < threshold) {
//      belowThreshold = true;
//    }
//  }
//
//  delay(1);
//}
//
//void calculateBPM() {
//  int beat_new = millis();    // get the current millisecond
//  int diff = beat_new - beat_old;    // find the time between the last two beats
//  float currentBPM = 60000 / diff;    // convert to beats per minute
//  beats[beatIndex] = currentBPM;  // store to array to convert the average
//  float total = 0.0;
//  for (int i = 0; i < 500; i++){
//    total += beats[i];
//  }
//  BPM = int(total / 500);
//  beat_old = beat_new;
//  beatIndex = (beatIndex + 1) % 500;  // cycle through the array instead of using FIFO queue
//
//}

// make some custom characters:

byte pixel[16][9];

void setup() {
  // set up the lcd's number of columns and rows: 
  lcd.begin(16, 2);
  pinMode(A1, INPUT);
  Serial.begin(9600);


  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 8; j++) {
      pixel[i][j] = 0b00000;
    }
  }
}

void loop() {
  lcd.setCursor(1, 1);
  // draw the little man, arms down:
  

  int value = map(analogRead(A1), 0, 4095, 0, 7);
//  Serial.println(value);
//  byte chars[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00001};
  
//  lcd.createChar(1, chars);
//  delay(100);
//  lcd.write(1);
//  draw(value);

  for (int i = 0; i < 8; i++)
    draw(i);
  
}

byte chars[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};

void draw(int val) {
  int _delay = 50;

  val = shiftPixel(val, pixel[0]);
  lcd.createChar(1, pixel[0]);
  lcd.setCursor(0, 1);
  lcd.write(1);

  val = shiftPixel(val, pixel[1]);
  lcd.createChar(2, pixel[1]);
  lcd.setCursor(1, 1);
  lcd.write(2);

  val = shiftPixel(val, pixel[2]);
  lcd.createChar(3, pixel[2]);
  lcd.setCursor(2, 1);
  lcd.write(3);

  val = shiftPixel(val, pixel[3]);
  lcd.createChar(4, pixel[3]);
  lcd.setCursor(3, 1);
  lcd.write(4);

  val = shiftPixel(val, pixel[4]);
  lcd.createChar(5, pixel[4]);
  lcd.setCursor(4, 1);
  lcd.write(5);

  val = shiftPixel(val, pixel[5]);
  lcd.createChar(6, pixel[5]);
  lcd.setCursor(5, 1);
  lcd.write(6);
  
  val = shiftPixel(val, pixel[6]);
  lcd.createChar(7, pixel[6]);
  lcd.setCursor(6, 1);
  lcd.write(7);
  
//  for (int i = 0; i < 16; i++) {
//    val = shiftPixel(val, pixel[i]);
//    Serial.println(val);
//    lcd.createChar(1, pixel[i]);
//    lcd.setCursor(i, 1);
//    lcd.write(1);
//  }

  delay(_delay);
}

int shiftPixel(int val, byte pixel[]) {
  byte originalPixel[8];

  // Copy the pixel
  for (int i = 0; i < 8; i++) {
    originalPixel[i] = pixel[i];
  }

  // Shift current pixel to the right
  for (int i = 0; i < 8; i++) {
    pixel[i] = pixel[i] >> 1;
  }


  // Clear the left col of the pixel
  for (int i = 0; i < 9; i++) {
    bitClear(pixel[i], 4);
  }

  // Set the left col of the pixel according to the val
  for (int i = map(val, 0, 7, 8, 0); i <= 7; i++) {
    bitSet(pixel[i], 4);
  }

  int nextVal = 0;
  bool valFound = false;
  while (!valFound && nextVal < 8) {
    if (bitRead(originalPixel[nextVal], 0)) {
      valFound = true;
    }
    nextVal++; 
  }

  nextVal--;

  nextVal = map(nextVal, 0, 7, 8, 0);
  Serial.println(nextVal);
  return nextVal;
}
