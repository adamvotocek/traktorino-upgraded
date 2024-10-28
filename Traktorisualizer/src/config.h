#define LED_PIN 3
#define NUM_LEDS 21

const uint8_t buttonPin = 6;

//Power limit
//THIS IS FOR 1 LED STRIP. IN REALITY, THE POWER CONSUMPTION IS GOING TO BE DOUBLED
const uint8_t voltage = 5;       //Used voltage in VOLTS
const uint16_t maxCurrent = 500; //Maximum current in MILIAMPS

//Button
const unsigned long writePressDuration = 5000; //in ms; for saving settings to EEPROM
const unsigned long resetPressDuration = 10000; //in ms; for resetting the EEPROM
const unsigned long secondClickDisplacement = 250; //in ms; time from the beginning of the first click to the beginning of the second click

//VISUALISERS

//Rainbow
//ranges
const uint8_t maxRainbowSaturation = 255;
const uint8_t minRainbowSaturation = 0;
const uint8_t maxRainbowBrightness = 150;
const uint8_t minRainbowBrightness = 5;
//slow
const unsigned long hueDelayS = 15;
const uint8_t hueIncrementsS = 2;
//normal
const unsigned long hueDelayN = 15;
const uint8_t hueIncrementsN = 7;
//fast
const unsigned long hueDelayF = 15;
const uint8_t hueIncrementsF = 12;

//Colors
const uint8_t colorBrightness = 130;
const uint8_t nOfColors = 9;
uint8_t colors[nOfColors][3] = 
{{HUE_RED, 255, colorBrightness}, 
{HUE_ORANGE, 255, colorBrightness}, 
{HUE_YELLOW, 255, colorBrightness}, 
{HUE_GREEN, 255, colorBrightness}, 
{HUE_AQUA, 255, colorBrightness}, 
{HUE_BLUE, 255, colorBrightness}, 
{/*HUE_PURPLE*/200, 255, colorBrightness}, 
{HUE_PINK, 255, colorBrightness}, 
{0, 0, colorBrightness}};

//Pulse
float portionDisplayed = 0.52;
const uint8_t maxPulseBrightness = 255;
const uint8_t minPulseBrightness = 15;