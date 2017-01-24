/**
 * In order to use this code, you must first download the Adafruit_NeoPixel Library and install. Full instructions 
 * for how to do this can be found by visiting the following URL for NeoPixel Library Installation. Just copy and
 * paste the URL into your browser.
 * 
 * https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation
 *
 */
#include <Adafruit_NeoPixel.h> // LED Ring Library
#ifdef __AVR__
#include <avr/power.h>
#endif
// TCS230 Sensor Parameters
// These variables are used to assign names to the Arduino IO pins connected to the TCS230
const int s0 = 4;
const int s1 = 11;
const int s2 = 7;
const int s3 = 8;
const int out = 10;
// These variables will be used to store sensor data from OUT pin on the TCS230. DO NOT CHANGE.
int red = 0;
int green = 0;
int blue = 0;
// NeoPixel Ring Parameters
// These variables are used to control the color of the NeoPixel Ring (Default WHITE)
int r = 255;
int g = 255;
int b = 255;
// (Default to 12 LED ring on pin 6)
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
// NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
// NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
// NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// NEO_RGBW Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
// Syntax: Adafruit_NeoPixel name = Adafruit_NeoPixel(Parameter 1, Parameter 2, Parameter 3);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 6, NEO_GRB + NEO_KHZ800);
void setup() {

	// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
	#if defined (__AVR_ATtiny85__)
		if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
	#endif
	// End of trinket special code

	Serial.begin(9600); // Begin communication with computer through serial monitor

	// Set IO pin types
	pinMode(s0, OUTPUT);
	pinMode(s1, OUTPUT);
	pinMode(s2, OUTPUT);
	pinMode(s3, OUTPUT);
	pinMode(out, INPUT);

	// Set TCS230 Sensor pins s0 and s1 to HIGH, activating the sensor
	digitalWrite(s0, HIGH);
	digitalWrite(s1, HIGH);

	// Activate the LED ring named "strip"
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}


void loop() {
	// TCS230 Loop Control
	color(); // retrieves data from OUT on TCS230 sensor for red, green, and blue.
	// Print sensor values to serial monitor
	Serial.print("R Intensity:");
	Serial.print(red, DEC);
	Serial.print(" G Intensity: ");
	Serial.print(green, DEC);
	Serial.print(" B Intensity : ");
	Serial.print(blue, DEC);

	// Determine the color being detected on the sensor, & print to serial monitor
	if (red < blue && red < green && red < 20)
	{
		Serial.println(" - (Red Color)");
	}
	else if (blue < red && blue < green)
	{
		Serial.println(" - (Blue Color)");
	}
	else if (green < red && green < blue)
	{
		Serial.println(" - (Green Color)");
	}
	else
	{
		Serial.println();
	}

	// LED Ring Loop Control
	// Some example procedures showing how to display to the pixels:
	for (int i=0; i<12; i++){
		strip.setPixelColor(i, strip.Color(r, g, b));
	}
	strip.setBrightness(255); // Set strip brightness from 0 to 255
	strip.show();
	delay(500);
}

// Function Color() is used to read the color information on the TCS230 Sensor
void color()
{
	digitalWrite(s2, LOW);
	digitalWrite(s3, LOW);
	//count OUT, pRed, RED
	red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
	digitalWrite(s3, HIGH);
	//count OUT, pBLUE, BLUE
	blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
	digitalWrite(s2, HIGH);
	//count OUT, pGreen, GREEN
	green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}