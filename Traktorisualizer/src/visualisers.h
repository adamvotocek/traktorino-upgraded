//Led Strips
uint8_t hue = 0;
uint8_t saturation = 100;
uint8_t brightness = 40;
uint8_t numLedsOn = 1; //How many leds are lit up at the time

//Hue change timings
unsigned long lastHueChange = 0;

//colors
uint8_t visColor = 0;


void setNumLedsOn(boolean centered){
    if (centered)
    {
        if (NUM_LEDS % 2)
        {
            numLedsOn = map(audioLevel, 0, maxLevelRecieved, 0, NUM_LEDS / 2 + 1);
        }
        else
        {
            numLedsOn = map(audioLevel, 0, maxLevelRecieved, 0, NUM_LEDS / 2);
        }
    }
    else
    {
        numLedsOn = map(audioLevel, 0, maxLevelRecieved, 0, NUM_LEDS);
    }
}

void setVisLeds(uint8_t hue, uint8_t saturation, uint8_t brightness, boolean centered){
    if (centered)
    {
        if (NUM_LEDS % 2){
            for (int i = NUM_LEDS/2; i < numLedsOn + NUM_LEDS/2; i++)
            {
                leds[i] = CHSV(hue, saturation, brightness);
            }
            for (int i = NUM_LEDS/2; i > NUM_LEDS/2 - numLedsOn; i--)
            {
                leds[i] = CHSV(hue, saturation, brightness);
            }
        }else{
            for (int i = NUM_LEDS/2; i < numLedsOn + NUM_LEDS/2; i++)
            {
                leds[i] = CHSV(hue, saturation, brightness);
            }
            for (int i = NUM_LEDS/2 - 1; i >= NUM_LEDS/2 - numLedsOn; i--)
            {
                leds[i] = CHSV(hue, saturation, brightness);
            }
        }
    }
    else
    {
        for (int i = 0; i < numLedsOn; i++)
        {
            leds[i] = CHSV(hue, saturation, brightness);
        }
    }
}

void setRainbowValues(boolean desaturation, boolean centered)
{
    setNumLedsOn(centered);
    
    brightness = map(audioLevel, 0, maxLevelRecieved, minRainbowBrightness, maxRainbowBrightness);
    if (desaturation)
    {
        saturation = map(audioLevel, 0, maxLevelRecieved, minRainbowSaturation, maxRainbowSaturation);
    }
    else
    {
        saturation = 255;
    }
}

void setPulseBrightness(){
    if(audioLevel != 0){
        if(audioLevel <= maxLevelRecieved - maxLevelRecieved*portionDisplayed){
            brightness = minPulseBrightness;
        }else{
            brightness = map(audioLevel, maxLevelRecieved - maxLevelRecieved*portionDisplayed, maxLevelRecieved, minPulseBrightness, maxPulseBrightness);
        }        
    }else{
        brightness = 0;
    }
}

void rainbowVis(unsigned long hueDelay, uint8_t hueIncrements, boolean desaturation, boolean centered)
{
    if (audioLevel != audioLevelPrevious || visModeCurrent != visModePrevious)
    {
        setRainbowValues(desaturation, centered);
        audioLevelPrevious = audioLevel;
        visModePrevious = visModeCurrent;
    }
    FastLED.clear();

    setVisLeds(hue, saturation, brightness, centered);

    if (millis() - lastHueChange > hueDelay)
    {
        lastHueChange = millis();
        hue += hueIncrements;
    }
    FastLED.show();
}

void colorVis(uint8_t color, boolean centered)
{
    if (audioLevel != audioLevelPrevious || visModeCurrent != visModePrevious)
    {
        setNumLedsOn(centered);
        audioLevelPrevious = audioLevel;
        visModePrevious = visModeCurrent;
    }
    FastLED.clear();

    setVisLeds(colors[color][0], colors[color][1], colors[color][2], centered);

    FastLED.show();
}

void pulseVis(uint8_t color){
    setPulseBrightness();
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CHSV(colors[color][0], colors[color][1], brightness);
    }
    FastLED.show();
}

void selectRainbow(boolean centered)
{
    switch (rainbowType)
    {
    case 0:
        rainbowVis(hueDelayS, hueIncrementsS, true, centered);
        break;

    case 1:
        rainbowVis(hueDelayN, hueIncrementsN, true, centered);
        break;

    case 2:
        rainbowVis(hueDelayF, hueIncrementsF, true, centered);
        break;

    case 3:
        rainbowVis(hueDelayS, hueIncrementsS, false, centered);
        break;

    case 4:
        rainbowVis(hueDelayN, hueIncrementsN, false, centered);
        break;

    case 5:
        rainbowVis(hueDelayF, hueIncrementsF, false, centered);
        break;

    default:
        break;
    }
}
