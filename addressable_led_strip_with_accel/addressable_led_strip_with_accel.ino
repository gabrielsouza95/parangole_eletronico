#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// pin 0 is the WS2812B input signal from the led strip
const unsigned char LED_CONNECTION_PIN = 2;
const unsigned char LED_STRIP_SIZE = 13;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_STRIP_SIZE, LED_CONNECTION_PIN, NEO_GRB + NEO_KHZ800);
// pin 1 is the inboard built-in led used for testing and feedback on program health and state
const unsigned char INBOARD_LED_PIN = 1;
const unsigned char LED_SECTION_SIZE = 3;
const unsigned char STARTING_LED_SECTION_1 = 0;
const unsigned char STARTING_LED_SECTION_2 = 3;
const unsigned char STARTING_LED_SECTION_3 = 6;
const unsigned char STARTING_LED_SECTION_4 = 9;
short iteration = 0; // used by the for loops. Will use only this one to reserve memory and all the for loops will run separately
short delay_val = 50;
bool is_on = false;
bool do_turn_signal = true;
unsigned char signal_step = 0;
short test_frame = 0;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/*
This method will light all the leds related to the brake light, 
which are the leds 0,1,2 | 3,4,5 | 12
it's color will be Color(255,0,0)
*/
void brake_light(){
  for(
    iteration=STARTING_LED_SECTION_1; 
    iteration < STARTING_LED_SECTION_1 + LED_SECTION_SIZE; 
    iteration ++
    ) {
    pixels.setPixelColor(iteration, pixels.Color(255,0,0));
  }
  for(
    iteration=STARTING_LED_SECTION_2; 
    iteration < STARTING_LED_SECTION_2 + LED_SECTION_SIZE; 
    iteration ++
    ) {
    pixels.setPixelColor(iteration, pixels.Color(255,0,0));
  }
  pixels.setPixelColor(12, pixels.Color(255,0,0));
}

/*
This method will light all the leds related to the light beam,
which are the leds 3,4,5 | 12
it's color will be Color(50,0,0)
*/
void light_beam(){
  for(
    iteration=STARTING_LED_SECTION_2; 
    iteration < STARTING_LED_SECTION_2 + LED_SECTION_SIZE; 
    iteration ++
    ) {
    pixels.setPixelColor(iteration, pixels.Color(25,0,0));
  }
  pixels.setPixelColor(12, pixels.Color(25,0,0));
}

/*
This method will light all the leds related to the turn signal, 
which are the leds 6,7,8
it's color will be Color(255,191,0)
*/
void turn_signal(unsigned char step){
  pixels.setPixelColor(step+STARTING_LED_SECTION_3, pixels.Color(255,191,0));
}

/*
This method will light all the leds related to the reverse light, 
which are the leds 9,10,11
it's color will be Color(255,255,255)
*/
void reverse_light(){
  for(
    iteration=STARTING_LED_SECTION_4; 
    iteration < STARTING_LED_SECTION_4 + LED_SECTION_SIZE; 
    iteration ++
    ) {
    pixels.setPixelColor(iteration, pixels.Color(255,255,255));
  }
}

void show_led_accel(int accel_rounded){
  pixels.setPixelColor(6+(accel_rounded-9), pixels.Color(255,255,255));
}

void test_lights_states(){
  if (test_frame > 0 && test_frame < 100)
  {
    if (test_frame > 50){
      do_turn_signal = true;
    }
    light_beam();
    if(do_turn_signal){
      turn_signal(signal_step);
      signal_step++;
      if (signal_step >= LED_SECTION_SIZE){
        signal_step = 0;
        do_turn_signal = false;
      }
    }
    else {
      signal_step = 0;
    }
    reverse_light();
  }
  if (test_frame >= 100 && test_frame < 200)
  {
    if (test_frame > 150){
      do_turn_signal = true;
    }
    brake_light();
    if(do_turn_signal){
      turn_signal(signal_step);
      signal_step++;
      if (signal_step >= LED_SECTION_SIZE){
        signal_step = 0;
        do_turn_signal = false;
      }
    }
    else {
      signal_step = 0;
    }
    reverse_light();
  }
  if (test_frame >= 200 && test_frame < 300)
  {
    if (test_frame > 250){
      do_turn_signal = true;
    }
    light_beam();
    if(do_turn_signal){
      turn_signal(signal_step);
      signal_step++;
      if (signal_step >= LED_SECTION_SIZE){
        signal_step = 0;
        do_turn_signal = false;
      }
    }
    else {
      signal_step = 0;
    }
  }
  if (test_frame > 300){
    test_frame = 0;
  }
}

void setup() {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(9600);
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

  // initialize the pixels instance
  pixels.begin();
  // attach interrupt to all signals received from the car
  // may take care for constant signals not overwhelming the
  // interruptions
  //attachInterrupt(digitalPinToInterrupt(LED_CONNECTION_PIN),change_state, RISING);
  Serial.println("");
}

double get_accel_vector_module(sensors_event_t& p_event){
  return sqrt(
      (p_event.acceleration.x*p_event.acceleration.x)
      +(p_event.acceleration.y*p_event.acceleration.y)
      +(p_event.acceleration.z*p_event.acceleration.z)
    );
}

// the loop routine runs over and over again forever:
void loop() {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
  
  pixels.clear();
  test_frame++;
  show_led_accel(round(get_accel_vector_module(event)));
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
