#include <Arduino.h>
#include <FastLED.h>
#include <MIDIUSB.h>
#include <EEPROM.h>

#include <config.h>

CRGB leds[NUM_LEDS]; //array of leds that contains their RGB values

//Audio level
uint8_t audioLevel = 0; //Audio level recieved by MIDI from the computer
uint8_t audioLevelPrevious = 0;
const float audioLevelMultiplier = 1.2; //Maximum is 2 (if more, it won't function properly)
const uint8_t maxLevelRecieved = 127;   //Maximum value of master level (recieved by MIDI)

//modes
uint8_t visModeCurrent = 0;
uint8_t visModePrevious = 0;
uint8_t rainbowType = 0;

//button
boolean buttonCurrentState = 0;
boolean buttonPreviousState = 0;
unsigned long buttonPressed = 0;
boolean secondClick = 0;

//MIDI
const uint8_t midiCCNumber = 5;

//debug
unsigned long lastLevelSim = 0;
unsigned long levelSimDelay = 35;

#include <visualisers.h>

void nextVisMode()
{
    visModeCurrent++;
    if (visModeCurrent == 5)
    {
        visModeCurrent = 0;
    }
}

void nextVisType()
{
    if (visModeCurrent == 0 || visModeCurrent == 1)
    {
        rainbowType++;
        if (rainbowType == 6)
        {
            rainbowType = 0;
        }
    }
    else
    {
        visColor++;
        if (visColor == nOfColors)
        {
            visColor = 0;
        }
    }
}

void storeVisInEEPROM()
{
    EEPROM.update(0, visModeCurrent);
    EEPROM.update(1, visModePrevious);
    EEPROM.update(2, rainbowType);
    EEPROM.update(3, visColor);
}

void clearEEPROM(){
    for (int i = 0; i < 4; i++)
    {
        EEPROM.update(i, 0);
    }
    
}

void applyEEPROMVis()
{
    visModeCurrent = EEPROM.read(0);
    visModePrevious = EEPROM.read(1);
    rainbowType = EEPROM.read(2);
    visColor = EEPROM.read(3);
}

void mode()
{
    buttonCurrentState = !digitalRead(buttonPin);
    if (buttonCurrentState != buttonPreviousState) //if change of state
    {
        if (buttonCurrentState == 1) //if button pressed
        {
            if (millis() - buttonPressed <= secondClickDisplacement)
            {
                secondClick = 1;
            }
            buttonPressed = millis();
        }
        else //if button released
        {
            if(millis() - buttonPressed >= resetPressDuration) //if very long press (eeprom reset)
            {
                clearEEPROM();
            }
            else if (millis() - buttonPressed >= writePressDuration) //if long press (eeprom write)
            {
                secondClick = 0;
                storeVisInEEPROM();
            }
            else //if short press
            {
                if (secondClick)
                {
                    visModeCurrent--;
                    if(visModeCurrent == 255){
                        visModeCurrent = 4;
                    }
                    nextVisType();
                    secondClick = 0;
                }
                else
                {
                    nextVisMode();
                }
            }
        }
    }
    buttonPreviousState = buttonCurrentState;
}

uint8_t applyMultiplier(byte value, float multiplier, uint8_t maximum)
{
    uint16_t outOfRange = value * multiplier;
    if (outOfRange > maximum)
    {
        return maximum;
    }
    else
    {
        return outOfRange;
    }
}

void simulateAudio()
{
    if (millis() - lastLevelSim >= levelSimDelay)
    {
        lastLevelSim = millis();
        if (audioLevel < maxLevelRecieved)
        {
            audioLevel++;
        }
        else
        {
            audioLevel = 0;
        }
    }
}

void readAudioLevel()
{
    midiEventPacket_t rx;
    do
    {
        rx = MidiUSB.read();
        if (rx.header != 0)
        {
            if (rx.byte2 == midiCCNumber)
            {
                //audioLevel = rx.byte3;
                audioLevel = applyMultiplier(rx.byte3, audioLevelMultiplier, maxLevelRecieved);
            }
        }
    } while (rx.header != 0);
}

void visualiser()
{
    switch (visModeCurrent)
    {
    case 0: //rainbow
        selectRainbow(false);
        break;

    case 1: //rainbow centered
        selectRainbow(true);
        break;

    case 2: //color
        colorVis(visColor, false);
        break;

    case 3: //color centered
        colorVis(visColor, true);
        break;

    case 4: //pulse
        pulseVis(visColor);
        break;

    default:
        break;
    }
}
void setup()
{
    //Led Strips
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(voltage, maxCurrent);
    FastLED.clear();
    FastLED.show();

    pinMode(buttonPin, INPUT_PULLUP);

    applyEEPROMVis();
}

void loop()
{
    mode(); //checks the buttons and changes the visualizer mode or type
    //simulateAudio(); //for debugging
    readAudioLevel(); //reads the MIDI input for master level (CC 05, channel 5)
    visualiser(); //makes the lights dance around
}
