#include <Adafruit_NeoPixel.h>
#include <Wire.h>

// pin 14 is equal to pin A0 on Arduino UNO
const unsigned char MIC_ANALOG_PIN = 14;
int MIC_ANALOG_VALUE = 0;

// pin 0 is the WS2812B input signal from the led strip
const unsigned char LED_CONNECTION_PIN = 2;
const unsigned char LED_STRIP_SIZE = 13;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_STRIP_SIZE, LED_CONNECTION_PIN, NEO_GRB + NEO_KHZ800);
// pin 1 is the inboard built-in led used for testing and feedback on program health and state
const unsigned char INBOARD_LED_PIN = 1;

short delay_val = 50;
bool is_on = false;

void show_led_mic(int mic_rounded){
  pixels.setPixelColor((mic_rounded), pixels.Color(255,255,255));
}

void setup() {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(9600);
  pinMode(MIC_ANALOG_PIN, INPUT);
  // initialize the pixels instance
  pixels.begin();
  // attach interrupt to all signals received from the car
  // may take care for constant signals not overwhelming the
  // interruptions
  //attachInterrupt(digitalPinToInterrupt(LED_CONNECTION_PIN),change_state, RISING);
  Serial.println("");
}

double get_mic_value(void){
  MIC_ANALOG_VALUE = analogRead(MIC_ANALOG_PIN);
  Serial.println(MIC_ANALOG_VALUE-500);
  return (MIC_ANALOG_VALUE-500)/5;
}

// the loop routine runs over and over again forever:
void loop() {
  /* Get a new sensor event */ 
  pixels.clear();
  show_led_mic(round(get_mic_value()));
  pixels.show();

  if(is_on){
    digitalWrite(INBOARD_LED_PIN, LOW);   // turn the LED off by making the voltage LOW
  }
  else{
    digitalWrite(INBOARD_LED_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  }

  delay(delay_val);
}

//void change_state(){
//  pulses++;
//}
