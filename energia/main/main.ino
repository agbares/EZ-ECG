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
byte pixel0[8] {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte pixel1[8] {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte pixel2[8] {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte pixel3[8] {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte pixel4[8] {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte pixel5[8] {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  // set up the lcd's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the lcd.
  lcd.print("I "); 
//  lcd.write(0);
  lcd.print(" Arduino! ");
//  lcd.write(1);
  pinMode(A1, INPUT);

  Serial.begin(9600);
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
  draw(value);
}

//void loop() {
//  // start the Thumbs Up part:
//  thumbsup();
//}     
//
byte chars[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
void draw(int val) {


  int max = 7;

//  for (int i = 0; i < 5; i++) {
//    for (int j = map(val, 0, 7, 7, 0); j <= max; j++) {
//      bitSet(chars[j], i);  
//      lcd.createChar(1, chars);
//      lcd.setCursor(4,1);
//      lcd.write(1);
//    }
//
//  }
  
  shiftPixel(val, chars, chars, true);
  lcd.createChar(1, chars);
  lcd.setCursor(4, 1);
  lcd.write(1);
  
//  Serial.println(chars[val]);


  delay(50);
}

void shiftPixel(int val, byte pixel[], byte previousPixel[], bool isLast) {
  byte originalPixel[8];

  for (int i = 0; i < 8; i++) {
    originalPixel[i] = pixel[i];
  }

  for (int i = 0; i < 8; i++) {
    pixel[i] = pixel[i] << 1;
  }
  
  // Set previousPixel
  if (!isLast) {
    for (int i = 0; i < 8; i++) {
      bitWrite(previousPixel[i], 0, bitRead(originalPixel[i], 4));
    }
  }


  for (int i = 0; i < 8; i++) {
    bitClear(pixel[i], 0);
  }

  for (int i = map(val, 0, 7, 7, 0); i <= 7; i++) {
    bitSet(pixel[i], 0);
  }
  
}
