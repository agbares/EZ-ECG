#include "LiquidCrystal.h"

/* Pin Mapping */
const int PIN_ECG_DATA = A1;  // PE2
const int PIN_ECG_LO_N = 23;  // PD0
const int PIN_ECG_LO_P = 24;  // PD1
const int PIN_LCD_RS = 7;     // PB4
const int PIN_LCD_RW = 2;     // PB5
const int PIN_LCD_EN = 14;    // PB6
const int PIN_LCD_D4 = 3;     // PB0
const int PIN_LCD_D5 = 4;     // PB1
const int PIN_LCD_D6 = 19;    // PB2
const int PIN_LCD_D7 = 38;    // PB3

// Instantiating LCD object with specified pins
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_RW, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

// For use with BPM calculations
int beat_old = 0;
float beats[500];
int beatIndex;
int BPM = 0;

// Pixel & ECG constants
const int NUM_OF_ECG_PIXELS = 7;
const int ECG_START_PIXEL = 9;
const int NUM_OF_ROWS_PER_PIXEL = 8;

// Pixel array to display for ECG
byte pixel[NUM_OF_ECG_PIXELS][NUM_OF_ROWS_PER_PIXEL + 1];

void setup() {
  // set up the lcd's number of columns and rows: 
  const int ROWS = 16;
  const int COLS = 2;
  lcd.begin(ROWS, COLS);

  // Init ECG Pins
  pinMode(PIN_ECG_DATA, INPUT);
  pinMode(PIN_ECG_LO_N, INPUT);
  pinMode(PIN_ECG_LO_P, INPUT);

  // Init LCD Text
  initLCDText();

  // Init the pixels
  initPixels(pixel);

  // Serial used for debugging purposes
  Serial.begin(9600);
}

void loop() {
  // Delay between each drawing
  const int DELAY = 100;
  
  // We're manually setting the lower and upper bound analog values to be mapped as 0 and 7 on a pixel, respectively
  const int LOWER_THRESHOLD = 1790;
  const int UPPER_THRESHOLD = 2300;
  
  int value = analogRead(PIN_ECG_DATA);
  int mappedValue = mapToPixel(value, LOWER_THRESHOLD, UPPER_THRESHOLD);

  // The cursor needs to be reset back to after the 'BPM: ' text
  lcd.setCursor(5, 1);
  
  if((digitalRead(PIN_ECG_LO_N) == 1)||(digitalRead(PIN_ECG_LO_P) == 1)) {
    // Leads off has been detected
    lcd.print("---");
    
    // Flat line the ECG monitor
    draw(4);
    
  } else {
    // The leads are on so draw the ECG
    draw(mappedValue);

    if (millis() % 10 == 0) {
      // We're only updating the BPM ever so often
      // Analog values are being mapped to a range of 0 to 200
      BPM = map(value, 0, 4095, 0, 200);
      
      // BPM = calculateBPM();
    }
    
    if (BPM < 300) {
      // Print out the BPM
      
      // This clears out the last displayed BPM
      lcd.print("   ");
      lcd.setCursor(5, 1);
      lcd.print(BPM);
    }
    
  }
  delay(DELAY);
}

// This function maps an anlog value onto one of the 8 rows of the LCD's pixel.
// This function essentially emphasizes where ECG's actual range.
// Values that are below the threshold are set to 0
// Values that are above the threshold are set to 7
int mapToPixel(int value, int lowerThreshold, int upperThreshold) {
  int mappedValue = 0;
  
  if (value >= lowerThreshold && value <= upperThreshold) {
    mappedValue = map(value, lowerThreshold - 1, upperThreshold + 1, 0, 7);
    
  } else if (value < lowerThreshold) {
    mappedValue = 0;
    
  } else {
    mappedValue = 7;
  }

  return mappedValue;
}

// This function draws out the ECG monitor onto the display from beginning to end position
// as defined by ECG_START_PIXEL upto NUM_OF_ECG_PIXELS.
// val is the mapped analog value to draw onto the left most pixel
void draw(int val) {
  for (int i = 0; i < NUM_OF_ECG_PIXELS; i++) {
    val = shiftPixel(val, pixel[i]);
    lcd.createChar(i + 1, pixel[i]);
    lcd.setCursor(ECG_START_PIXEL + i, 1);
    lcd.write(i + 1);
  }
}

// This function shifts the columns of a pixel to the right by one, then
// fills the respective rows of the leftmost column for the new val
// The function returns an integer denoting the value that was shifted
// out to the right
int shiftPixel(int val, byte pixel[]) {
  byte originalPixel[8];

  // Copy the pixel
  for (int i = 0; i < 8; i++) {
    originalPixel[i] = pixel[i];
  }

  // Shift current pixel's cols to the right
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
  return nextVal;
}

// Init function that 0's out the pixel byte 2D array
void initPixels(byte pixel[][NUM_OF_ROWS_PER_PIXEL + 1]) {
  for (int i = 0; i < NUM_OF_ECG_PIXELS; i++) {
    for (int j = 0; j < 9; j++) {
      pixel[i][j] = 0b00000;
    }
  }
}

// Init function to print greeting text to LCD
void initLCDText() {
  lcd.print("The Dr. is in.");
  lcd.setCursor(0, 1);
  lcd.print("BPM: ");
}

// Function to calculate BPMs
// Adapted for the TM4C123 from: 
// https://github.com/sparkfun/AD8232_Heart_Rate_Monitor/blob/master/Software/Heart_Rate_Display_Processing/Heart_Rate_Display/Heart_Rate_Display.pde
// Does not work as expected, unfortunately
int calculateBPM() {
  int beat_new = millis();    // get the current millisecond
  int diff = beat_new - beat_old;    // find the time between the last two beats
  float currentBPM = 60000 / diff;    // convert to beats per minute
  beats[beatIndex] = currentBPM;  // store to array to convert the average
  float total = 0.0;
  
  for (int i = 0; i < 500; i++){
    total += beats[i];
  }
  
  beat_old = beat_new;
  beatIndex = (beatIndex + 1) % 500;  // cycle through the array instead of using FIFO queue
  
  return int(total / 500);
}
