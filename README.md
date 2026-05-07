# LoopStation-esp32
Para ver la versión de este README en español, vaya al siguiente documento:  
[Documentación en español](README.es.md)

## Index
* [Introduction](#introduction)
* [Defining the project](#define)
* [Materials](#materials)
* [The logic behind the system](#logic)
* [Achieving expectations through code](#code)
* [The hardware](#hardware)
* [The functioning](#functioning)
* [Helpful external documentation](#documentation)

<a name="introduction"></a>
## Introduction
This is a small project designed to be a portable version of a functional device that acts as a variant of a loop station, made with an ESP32 as its core. It's layed out as a project for the class of Embedded Systems (college). Throughout this document, focal points of interest for the understanding/replication of the project will be displayed, such as the materials used and their estimated cost (rounding up and trying to account for the cost of the provided components given out by the teachers), the logic behind its functionality and others.  

Following this README will show the process that was followed for the creation of the process that lead to the final implementation its first prototype with this code:  
[Loop Station code](/loopStation.ino)

<a name="define"></a>
## Defining the project
Firstly, to understand the nature of the project, the concept of a loop station must be defined.  

### What is a loop station?
Traditionally speaking, this concept is often associated (and you might be familiar with it) in the form of a guitar pedal. That however, is not the only context a loop station has. To generalize and establish the ground work for this project, a loop station will be defined as a device that allows its user to record/capture a series of audios (tracks, MIDI events, etc.) and loop them. In the case of this particular loop station, it will be record audios.  

This way, multilayered loops can be created, giving ample space for creativity to take hold (armonize, record a background base...).  

### Defining its limits
A professional loop station can vary in its composition (number of tracks, options to play around with, etc.). Because of this, it is necessary to establish a basis of the limitations and expectations of this mini version.  
- 3 functional tracks (record, overdub and volume control).
- They record audios through the mic module.
- The first track will be a "master" track (define the loop length), and the other two its slaves.  

Now, different limits have to be accounted for in the hardware section. Due to memory management (PSRAM), it will be set that:  

- Loops have a maximun of 20s duration.  

This implies that when the loop station is set to record the first track of the loop (necessarily the Track 1, since it is the master), it can establish a duration of anything in between 0-20 seconds by pressing record button a second time to stop the recording or by letting it record until the natural end of the loop. This is accomplished by software delimitations (when record it's pressed for the first time, at reaching 20s of recording, the loop station exits recording state automatically). This way, throughout the whole loop, audios are set to be as long as the established duration.  

This project is designed so it can be easily put apart, since some components must be returned, therefore it has a prototype concept that is not final, and it deals with some hardware issues that could be resolved by soldering everything properly in a board.  

<a name="materials"></a>
## Materials
In this section both hardware and software requirements will be specified. As it follows, it is not a set strict need to use the same ones, and the project can be adjusted to the specific modules being used. However, the documentation will follow closely the specified options that were chosen by the project group and big modifications of the approach to the project could greatly impact the hardware selection and software specifics.  

### Components
Ordered to more liable sources, sacrificing a posible higher price over components potentionally getting damaged during their shipment. Multiple components mentioned were bought in packs, therefore the shown price is an estimated cost per unit. Not every material listed is essential or estrictly necessary for the project. Due to the subjective nature of the chosen display and positioning of components in this version, this is not a strict guide, but rather an orientative documentation of the involved hardware. An audio input is needed, as well as an output, but you may follow as you deem appropiate with the exact models.
  
| Name | Units | Estimated Cost/unit (euros) | Bought/Supplied |
|:--- |:--- |:--- |:--- |
| ESP32-S3 DevKitC-1 N16R8 | 1 | 8€ | Bought |
| PCM5102 | 1 | 4,7€ | Bought |
| INMP441 | 1 | 4€ | Bought |
| Potentiometer | 3 | 0,5€ | Bought |
| LEDs (blue)  | 3 | 0,05€ | Supplied |
| LEDs (green)  | 1 | 0,05€ | Supplied |
| LEDs (yellow)  | 1 | 0,05€ | Supplied |
| Buttons  | 4 | 0,04€ | Supplied |
| Dupont Cables  | 1 | 15€ (a pack) | Bought |
| Protoboard  | 2 | x | Supplied x1, Bought x1 |
| Mini Protoboard  | 1 | x | Supplied |

The "x" refers to the possibility of buying a pack, for around 13€, that includes all the components marked with a price of "x".
  
The total estimated price refers to the sum of all the materials used. The total price invested it's the total estimated, substracting all the materials that were supplied by the teachers or adquired previously for different purposes that could be reused, therefore only the actual expenses from begining to end of the project are reflected in this section.  

| Total Estimated Price (euros) | Total Invested Price (euros) |
|:--- |:--- |
| 47€ | 33,5€ |  

These prices, if the project was to be started from 0 are a bit unrealistec. It would be wiser to buy packs of components and the total price spent would be higher. Still, the true money invested on this project in particular is around the prices above. The difference it would make to add the full price of overpacked options that had extra components is not really reflected on the proyect. Only if they are not going to be reused for any other purposes and one is confident that none of them will fail or break this price would increase the waste on correlation with the actual used components vs the amount that was bought.  

### Software
For the coding part of the project, Arduino IDE was used for the whole duration of it. Reminders will be displayed later on, but the focal points one needs to run the loop station are:
- Whatever board it's being used must be downloaded to be selected if it is not a default one.
- Port is selected (make sure it is the right COM, if one is not sure, disconnecting and connecting shall show what port the board is connected to).  

<a name="logic"></a>
## The logic behind the system  
Now that the objectives are clear, a logic behind what the device does can be defined. This is achieved with two simple questions:
- What am I doing?  
[IMG]  

It is important to note that once the loop station begins a loop (records in the first track), the device is ALWAYS reproducing on loop. That does not mean that if it transitions to recording the loop station stops playing, it only shifts its main focus to the recording task. If the loop station is in the play state, that simply means: 1- A loop has been established 2-It is not currently recording.

- Where am I?   
[IMG]  

The program answers to a doble status method. When the device it's booted, it enters an idle state in which its only purpose is detecting if the user wants to start recording (pressing the record button). Anytime a record wants to be made (the record button was pressed), at the beginning of each loop (for coordination purposes), the loop station enters a recording state. Once it is done recording, it goes into the play status, which implies that it already has a defined loop going (has a defined loopLength at the very least, be it silence or anything that was recorded on Track 1).  

On a similar note, it answers to its whereabouts. Am I on Track 1, 2 or 3? This helps to determine where it's recording when it captures the input and determines the added output of the three tracks. This also helps to aid the Master-slave dynamic. It is restricted to start recording at the beginning of each loop (idle) only if the loopstation is currently residing in Track 1.  

While a restriction could be added to "ignore" any attempt at changing the track if we are in idle state because necessarily Track 1 has to be the recording target (just as it is done when recording, because a change of track should not be happening), it is not implemented because it aids to an early detection of possible hardware malfunction (the buttons can be pressed as soon as it's booted to control if the target track changes via LEDs).  

With this information, an expansion of the logic is extended in two interesting features of the loop station:  

#### Pending recording
- If the record button is pressed anytime other than the exact beginning of the loop, the system enters an in between "status" introduced by a boolean variable (will be defined in the software section). This in between "state" is simply an alert that let's the system know that the user wants to record, so it makes sure that its state changes accordingly at the beginning of the loop.
- This only ever happens if the loop has been established. When it is first booted, the loop is not defined and it is not trying to read or reproduce anything, therefore the recording starts inmediatly upon pressing the button.  

This concept is similar to a "pending" interruption logic. With this logic we do not bother the system until we reach the beginning of the loop, because there will be no recording in any other time frame due to the syncronization of the loop station and its nature.  

#### Panic!
- Pushing two track buttons at the same time sends a "PANIC!" signal that resets the ESP32.  

What happens if multiple track buttons are pressed? Since once the hardware is introduced in a box, pressing the reset button of the board becomes an unreachable task, this limit case scenario is used for a quick reset of the board. If two buttons are pressed, a logic could be introduced where we only listen to the first one or any other alternative, but this case is seized as an opportunity for an easy reset. While a fifth button could be added as well to implement this feat, to simplify the hardware and minimize the components, it is established that pushing two track buttons (or more) at the same time sends what is determined as a "PANIC!" signal.  

<a name="code"></a>
## Achieving expectations through code
Once the logic behind it it's clearer, the code can be constructed following it. Case scenarios will be covered, and a way more extense explanation of the exact behavior of the device will be recollected in this section. The pins will be skipped, since they will be specified in the following section of the document (The hardware). Throughout the code, different prints will be shown to debug the correct functioning of the system and hinting at a possible addition of a screen.  

Specification of define section (pins) of the code here: [The hardware](#hardware).  

There will only be one include needed for this version of the project:  
#include "driver/i2s.h"  


### Constant definitions

Right after the pins there is a section to declare the following: 
- Constants are defined that set the max loop duration to 20 seconds for easy readjustments and to set the sample rate.  
- Both state machines are declared as Enum types.  
- pendingRecord is a boolean defined to enter the middle state that alerts the loop station when it reaches the beginning of the loop that it needs to change to recording.  
- stopAtLoopEnd is declared so when the first instance of the loop is recorded on Track 1 for the first time, it automatically stops at 20 seconds if the record button is not pressed a second time before that.  
- Instances of the state machines are declared (trackFocus and currentStatus), and they are also initialized as the preset state of the looping machine when is booted.  
- MAX_SAMPLES is the result of operating to find what exactly is the end of the loop if its maximun duration is 20 seconds. This way, the loop can check if its positiong (that starts at 0) has reached the end and needs to go back to position 0.  
- The pointers for each track's buffers are initialized to null.  
- loopLength and currentPos are defined and initialized to 0. This variables determine the duration of the loop that will be set after the first iteration happens when record is pressed (being MAX_SAMPLES its maximun value possible), and currentPos will mark the iteration of the samples each time it advances, starting at 0 and ending at whatever loopLength is set to.  

```
// Audio
#define MAX_LOOP_SECONDS 20
#define SAMPLE_RATE   48000

// States
enum SelectedTrack { TRACK1, TRACK2, TRACK3 };  // Donde estoy?
enum SystemStatus { IDLE, RECORDING, PLAYING }; // Que estoy haciendo?

bool pendingRecord = false; // Grabación pendiente
bool stopAtLoopEnd = false; // Parada automatica de grabar cuando llego al final del loop

// State variables
SelectedTrack trackFocus = TRACK1;
SystemStatus currentStatus = IDLE;

// Max length for the loop (Adaptable)
const uint32_t MAX_SAMPLES = MAX_LOOP_SECONDS * SAMPLE_RATE;

// PSRAM buffers
int16_t* track1 = NULL;
int16_t* track2 = NULL;
int16_t* track3 = NULL;

// Loop variables
uint32_t loopLength = 0;
uint32_t currentPos = 0;
```

### The setup
In this part of the code, all the initializations will be done. Pins will be assigned as input or output accordingly and buffers for the audio tracks will be initialized. For debug purposes an error message is set to be able to detect any errors when reserving the PSRAM. After all of that is done, it invokes the [setupI2S](#setupI2S), explained right after this one, and [updateTrackLEDs](#updateTrackLEDs), to initialize the LEDs.

```
void setup() {
  Serial.begin(115200);
  delay(1000);

  //Potenciometros
  analogReadResolution(12); // Resolution established 0 to 4095 (From 10 to 12 bits)

  // Potentiometers input
  pinMode(POT_T1, INPUT);
  pinMode(POT_T2, INPUT);
  pinMode(POT_T3, INPUT);

  // Memory allocation
  size_t bufferSize = MAX_SAMPLES;
  
  track1 = (int16_t*) ps_calloc(bufferSize, sizeof(int16_t));
  track2 = (int16_t*) ps_calloc(bufferSize, sizeof(int16_t));
  track3 = (int16_t*) ps_calloc(bufferSize, sizeof(int16_t));

  // Control sentence to check if ti was properly allocated
  if (track1 == NULL || track2 == NULL || track3 == NULL) {
    Serial.println("¡ERROR! PSRAM ouldn't be assigned. Is it selected on Tools?");
    while(1); 
  }

  // LEDs output
  pinMode(LED_T1, OUTPUT);
  pinMode(LED_T2, OUTPUT);
  pinMode(LED_T3, OUTPUT);
  pinMode(LED_REC, OUTPUT);
  pinMode(LED_PENDING, OUTPUT);

  // Buttons input
  pinMode(BTN_T1, INPUT_PULLDOWN);
  pinMode(BTN_T2, INPUT_PULLDOWN);
  pinMode(BTN_T3, INPUT_PULLDOWN);
  pinMode(BTN_REC, INPUT_PULLDOWN);

  // I2S initialization
  setupI2S();
  updateTrackLEDs(); // Initial state of LEDs
}
```

<a name="setupI2S"></a>
#### The I2S setup  
This section of the code is heavily influenced by the modus operandi of projects that use these components, therefore, changes can be applied but this setup should work perfectly fine if the intended components to use are the same.  

Considerations have to be done if the microphone used is not mono (minor adjusting). The INMP441 is a mono microphone, and in this specific scenario the L/R will be connected to the GND so it is expected to go through the LEFT. If headphones are connected to the PCM5102, it wil be heard only through the left headphone (.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT).  

The .dma_buf_count = 4 and .dma_buf_len = 128 are set on low values to avoid delays between recording and reproducing, due to less retained time by the functions that block the flow (read and write).  

```
void setupI2S() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 128,
    .use_apll = true 
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_DOUT_DAC,
    .data_in_num = I2S_DIN_MIC
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}
```


### The loop 

The variables needed for each loop iteration are declared (initialized to 0 if needed).  

```
void loop() {
  int16_t sampleIn = 0; // Variable audio input
  int32_t mixed = 0;    // Variable audio input
  size_t bytesRead, bytesWritten;

   static float v1 = 1.0, v2 = 1.0, v3 = 1.0;

```

The loop station only checks the buttons if it has not started a loop.  

```
  if(currentStatus == IDLE){
    buttons();

```

To be constant throughout the whole loop, it starts listening to the microphone, but will only save it if it is recording, later on. It also has the first of many control sentences to achieve a clean audio (just to be safe).  

```
  }else{
    // AUDIO INPUT 
    i2s_read(I2S_NUM_0, &sampleIn, sizeof(int16_t), &bytesRead, portMAX_DELAY);

    sampleIn = sampleIn >> 2; // Small ajustment to try and assure a better audio quality
```

Just to safely control the proper coordination of the system, some values are only checked every x samples. This is not a taxing program so there isn't any real need to use FreeRTOs, but just for the sake of safety, a lot of control sentences are implemented. This case receives the volume input value.  

```
    if (currentPos % 1024 == 0) { // Volume updated every 1024 samples
      float tmp;
      v1 = ((tmp = getVolume(POT_T1)) != v1) ? tmp : v1;
      v2 = ((tmp = getVolume(POT_T2)) != v2) ? tmp : v2;
      v3 = ((tmp = getVolume(POT_T3)) != v3) ? tmp : v3;
    }
```

On a similar manner, buttons are only checked every 441 samples.  

```
    if (currentPos % 441 == 0){ // Check buttons
      buttons();
    }
```

If it is recording, then it needs to process the sample it's receiving from the microphone in the correct track. Since it is delimeted that while recording the track focus can't change (specified in the [buttons](#buttons) auxiliar function) no mix ups happen.  

```
    // RECORDING LOGIC
    if (currentStatus == RECORDING) { // If it is recording, it saves in the correct track the sample the mic is sending
      if (trackFocus == TRACK1) track1[currentPos] = sampleIn;
      else if (trackFocus == TRACK2) track2[currentPos] = sampleIn;
      else if (trackFocus == TRACK3) track3[currentPos] = sampleIn;
    }
```

Since it has already read the volume values above, in case it has changed, it updates the volume accordingly by multiplying the volume (proportion) accordingly.  

```
    // SOUND OUTPUT LOGIC
    // Volume control and forced muted control logic reinforced
    int32_t s1 = (v1 < 0.01f) ? 0 : (int32_t)(track1[currentPos] * v1);
    int32_t s2 = (v2 < 0.01f) ? 0 : (int32_t)(track2[currentPos] * v2);
    int32_t s3 = (v3 < 0.01f) ? 0 : (int32_t)(track3[currentPos] * v3);
```

This section mounts the output audio samples, adding the tracks. Since if one has not been recorded it is empy, no issues arise.  

```
    // Reproduce any track that is not being recorded
    if (currentStatus == RECORDING) {
        if (trackFocus == TRACK1)      mixed = s2 + s3;
        else if (trackFocus == TRACK2) mixed = s1 + s3;
        else if (trackFocus == TRACK3) mixed = s1 + s2;
    } else {
        mixed = s1 + s2 + s3;
    }
```

Multiple control sections are implemented to secure a safe handling of the volume output due to unexpected behavior possibly arising from the hardware. Limits are generic, since they work, but they can be tweaked with.  

```
    // Output audio limitation
    if (mixed > 32767) mixed = 32767;
    if (mixed < -32768) mixed = -32768;
    int16_t sampleOut = (int16_t)mixed;
```
This next line reproduces the output audio, sending it to the PCM5102.  

```
    // AUDIO OUTPUT
    i2s_write(I2S_NUM_0, &sampleOut, sizeof(int16_t), &bytesWritten, portMAX_DELAY);
```

This huge section of the code is the tricky bit of the logic behind it. The *if* sentences check certain values that could be traded for equivalents, or could be readjusted wiwth the logic. Firstly, it moves the position. Secondly it parts the code into two sections 1) Already established a loop and recorded the master track (looplength is more than 0, which could also imply that the loop station != IDLE). 2) It is recording the master track for the first time, therefore there isn't an established loop yet.  

Inside the first condition it also has a parted logic. First, it checks if it has reached the end of the loop, so it can reset the position to 0. After setting it to the first position it checks if any pending recordings are waiting (if the REC button was pressed). After managing the pending record logic (updating LEDs and status, boolean variables...) it needs to check if it was recording, because it has reached the end of the loop so it needs to automatically stop the recording (change status, LEds...).  

Inside the second condition it checks if it has naturally reached the end of the loop (MAX_SAMPLES), since it already knows that it has to be recording the first track for the first time. If that is the case, it updates all variables accordingly, similar to the previous explained parts.  

```
    // LOOP LOGIC
    currentPos++;
    if (loopLength > 0) {               // A loop has been established
        if (currentPos >= loopLength) { // It reached the end of the loop
            currentPos = 0;
            if (pendingRecord) {        // If there was any pending recording, manage it
                currentStatus = RECORDING;
                pendingRecord = false;
                digitalWrite(LED_PENDING, LOW);
                updateRECLED();
                Serial.println(">>> Recording track.");
            } 
            else if (currentStatus == RECORDING && stopAtLoopEnd) {       // If it's recording, automatically stop
                currentStatus = PLAYING;
                updateRECLED();
                stopAtLoopEnd = false;
                Serial.println(">>> Recording succesfully done.");
            }
        }
    } else if (currentPos >= MAX_SAMPLES && currentStatus == RECORDING) { // Master track reached the maximun length set
        loopLength = MAX_SAMPLES;
        currentStatus = PLAYING;
        updateRECLED();
        currentPos = 0;
        Serial.println(">>> Master track finalized recording with maximun duration.");
    }
  }
}
```

### Auxiliar functions  

<a name="buttons"></a>
#### 1) buttons  
The very first thing it does it's save the button states in boolean variables, so it can check different conditions throughout the code. An additional boolean variable is declared to effectively check a rising edge of the REC button.

```
void buttons() {
  static bool lastState = LOW; 
  
  // READ THE BUTTON STATES
  bool track1 = digitalRead(BTN_T1) == HIGH;
  bool track2 = digitalRead(BTN_T2) == HIGH;
  bool track3 = digitalRead(BTN_T3) == HIGH;
                          ...
```
The very next thing it does is detect if there was a PANIC! signal thrown, this way it inmediatly stops checking anything else.

```
  // Panic! detection
  int pressedCount = (int)track1 + (int)track2 + (int)track3;
  if (pressedCount >= 2) {
    panicReset();
    return;
  }
                          ...
```

The next section updates the status and LEDs only if 1) It's a different one 2) It is not recording.  

```
  // Update the current track focus only if it has changed and it's not recording
  if(currentStatus != RECORDING){
    if (track1 && trackFocus != TRACK1) {
      trackFocus = TRACK1;
      updateTrackLEDs();
      Serial.println(">>> TRACK 1 Selected: ");
    } else if (track2 && trackFocus != TRACK2) {
      trackFocus = TRACK2;
      updateTrackLEDs();
      Serial.println(">>> TRACK 2 Selected: ");
    } else if (track3 && trackFocus != TRACK3) {
      trackFocus = TRACK3;
      updateTrackLEDs();
      Serial.println(">>> TRACK 3 Selected: ");
    }
  }
                            ...
```

The REC button has a rising edge control sentence so it can deal with the REC button being held instead of pressed briefly. This is not an issue in the track buttons, but it is a worry when it concerns the REC button due to the logic the code follows. This way, an uncontrolled reaction of the loop station is avoided.


```
  // Has the REC button been pressed?
  bool currentRecState = digitalRead(BTN_REC);

  if (currentRecState == HIGH && lastState == LOW) { // Rising edge
    handleActionBtn();
    delay(50);
  }
  lastState = currentRecState;
}
```

#### 2) handleActionBtn 
This function is called whenever the loop detects that the record button has been pressed. It has been extracted as a function for clarity and organization of the code. Its job is to determine what the record button press should translate to depending on both states the loop station uses.

First case scenario, the initialization state. The loop station is simply waiting for the record button to be pressed to change what is doing (IDLE). Therefore, if its currentState its IDLE (it only ever happens if it has just been booted or restarted) and the track it's on is the first one (as it will be mentioned later on, changing tracks for debugging purposes is contemplated, but being on track 1 is a requirement to record for the first time), it can start recording. Recording implies a change of status, in this first scenario, an initialization of the position of the loop at 0 and an update on the RECLED's state, so the green LED in charge of indicating it's recording turns on.  

```
void handleActionBtn() {
    // CASE A: First recording of the loop
    if (currentStatus == IDLE && trackFocus == TRACK1) {
        currentStatus = RECORDING;
        currentPos = 0;
        updateRECLED();
        Serial.println("Recording Master Track...");
    } 
                          ...
```

The user wants to set the record length below 20s so it needs to press REC button a second time. It has to be recording, and since in the previous condition it was already delimited that the track must be the first one, it checks that the looplength is actually 0 to assure it hasn't set a loop yet (hasn't finished recording the first master track).

```
    // CASE B: Master track (1) is recording and the looplength wants to be set <20s by pressing again
    else if (currentStatus == RECORDING && loopLength == 0) {
        loopLength = currentPos; // looplentgh is set (not on seconds)
        currentStatus = PLAYING;
        updateRECLED();
        currentPos = 0;          // It goes back to the beginning to start playing it
        Serial.printf("Master Track recorded, loop length defined");
    }
                          ...
```

It is recording any track (not the first iteration of the master track). Changes all variables accordingly to update the loop station and be able to alert it to record the next time it reaches the beginning, turning the pending record LED as well as a visual indicator.

```
    // CASE C: Recording or overdub of a track.
    else if (currentStatus == PLAYING) {
        pendingRecord = true;
        digitalWrite(LED_PENDING, HIGH); 
        stopAtLoopEnd = true; // Reminder boolean to automatically stop recording at the end of the loop
        Serial.println("Pending Record detected, waiting for the beginning of the loop");
    }
}
```


#### 3) getVolume  
This function is in charge of controlling the input of the intended potentiometer for the volume of a track. Due to noise in the system and innacuracy of the hardware, a dead zone is established. Several control sentences are implemented, but this can be adjusted many different ways. A cuadratic formula anything the user considers more fitting can be placed here. In this case, this works just fine for what was intended, so no further expectations to be met make a necessary change to this function a priority.  

```
float getVolume(int pin) {
  int raw = analogRead(pin);
  if (raw < 250) { // Dead zone for mute
    return 0.0f; 
  }else{
    return (float)(raw - 200) / (4095.0f - 200.0f);
  }
}
```  


<a name="updateTrackLEDs"></a>

#### 4) updateTrackLEDs  
This function is invoked only when a button that affects the track LEDs state is pressed. This way, it makes sure that the previous LED is turned off and the right one on.

```
void updateTrackLEDs() {   // Track LEDs
  digitalWrite(LED_T1, (trackFocus == TRACK1) ? HIGH : LOW);
  digitalWrite(LED_T2, (trackFocus == TRACK2) ? HIGH : LOW);
  digitalWrite(LED_T3, (trackFocus == TRACK3) ? HIGH : LOW);
}
```


#### 5) updateRECLED
This function is specifically created so it is only invoked when the record state is updated (it transitions in or out of the state), that way it turns the LED on or off accordingly.

```
void updateRECLED(){
  if (currentStatus == RECORDING) {
    digitalWrite(LED_REC, HIGH);
  } else {
    digitalWrite(LED_REC, LOW);
  }
}
```


#### 6) panicReset
This function is invoked when buttons detects that more than one of the track button has been pressed at the same time. It restarts the ESP32 (better alternative to reset();). At any moment in time this can be achieved.

```
void panicReset() {
  Serial.println("!!! PANIC: Reseteating system...");
  ESP.restart();
}
```


### Note for Arduino IDE (Reminder)
This code was written with Arduino IDE. To run it you must check three fundamental things:
- ESP32S3 Dev Module must be installed as an option for the board
- PSRAM must be activated (OPI)
- Port is correctly selected (make sure it is the right COM)  

On a side note, make sure the only include (#include "driver/i2s.h") is downloaded.

<a name="hardware"></a>
## The hardware
Since the software is layed out, the only thing left is to connect the hardware. BEWARE, issues might arise if you do not test the components by themselves. It is recommended that you test them before hand so you can discard an issue with a broken component (or incorrect wiring/soldering) and safe yourself time later on. To aid you in this tedious task, test codes will be provided here:  
[Test codes for INMP441 and PCM5102](/tests)  

The tests are run with the ESP32S3, but code and pins should be easily adapted to similar boards (special care should be invested in the use of memory that the board allows, since the one used in this project can use OPI PSRAM).  

- Small indication for the ESP32S3: make sure the cable is connected to the USB entry, not the COM one!

### Testing your hardware
To make testing as easy as possible, pins that are used in the final code will be the same as the ones that are used in the test codes, that way, one code can be changed for another and booted for quick check of proper functioning of the component.

#### PCM5102
The goal here is to reproduce a escale, to make sure that the module is in fact working. To do so, the PCM5102 can be connected directly to the pins that are specified in the image below. If headphones (or even a speaker) are connected to the module and the program runs (make sure proper Port in Arduino IDE is selected), then a scale should start reproducing in a loop.  

If no escale can be heard, first check the soldering and cables. If that is okay, then check agaim de XMS connection, because it's the most likely cause of the silence and it was the main issue of the development of this project.

[PINS IMAGE]

```
// I2S Pins
#define I2S_BCLK      41
#define I2S_WS        40
#define I2S_DIN_MIC   47
#define I2S_DOUT_DAC  42
```


#### INMP441
To properly observe if the microphone works, this time the aid of the serial plotter will be needed (to visualize waves). If the microphone is working, waves should appear (reacting to sounds). If there is something wrong, a flat line should show. If the case is the latter, check the soldering and the cables. If proper soldering is assured and cables seem to work fine, then it might be a dead module.
- Small indication: if it is not working check that the Port is the right one, and that USB CDC On Boot is "Enabled".

[PINS IMAGE]

#### LEDs, buttons and potentiometers
This components are easily tested with the code itself. LEDs can be easily tested with a simple circuit that does not require the ESP, but it will follow the same simple scheme of power, resistance and LED (traditional circuit).

And the pins for this components should be as it follows:  

[PINS IMAGE]

 ```
// Potenciometers pins
#define POT_T1 12  
#define POT_T2 13  
#define POT_T3 14

// Button pins
#define BTN_T1 9   
#define BTN_T2 10  
#define BTN_T3 11  
#define BTN_REC 15

// LED pins
#define LED_T1  16  
#define LED_T2  17
#define LED_T3  18
#define LED_REC 8
#define LED_PENDING 3
```

Once it has been assured that the LEDs work, it can be determined if the buttons do as well if the loop station is booted and, while it stays in IDLE mode, different track buttons are pressed (NOT simultaneously, to prevent a PANIC!), if the LEDs respond accordingly, the buttons work correctly.


### Putting the loopStation together
Now that it was assured that everything works just fine, this part only requires the upmost care when connecting what to where. The biggest issues arise here, so proceed with caution. This is the layout presented in the version of the loop station that was presented for the class. Following the pin images inserted above, the circuit should look comething similar to this (does not include the INMP441 and PCM5102):  

[TINKERCAD VERSION OF THE CIRCUIT]


- Note that there are many ways to connect this, but it is limited by the cables, the use of a board expansion or others. In this case, this is the example of the assemble done trying to achieve a good sectioning of the parts of the hardware. This schematic translates to many real life versions, being the one done for the presentation of the project this one:  

[IRL HARDWARE PIC]  

As a general rule, one should not be afraid of using intermediaries such as protoboards or expansion boards, that simplify this process. The only thing to keep in mind is the possible interferences that come along the cables' type, the length and its possible impacts in the audio input/output.

If issues arise, like total silence from the loop station, the main problem is probably the cables. In the making of this project, a common GND was connected, and the cable that connected the GND from the protoboard to the ESP32S3 was malfunctioning, along with the mute from the PCM5102. The hardware is the main enemy of this project, since the code is functional as it was presented. Therefore, if having issues with it, hardware is the main culprit. If changes were done to the code, that might produce new issues that would need to be checked.

<a name="functioning"></a>
## The functioning

This section will elaborate on different use cases, therefore displaying the expected functioning of the system.

### 1) Boot the ESP32
Once the ESP32 is turned on (is connected to the laptop or batteries), the program starts running. It begins on an IDLE state and positions itself in the track 1 (turning the LED on) and setting up all the pins and options for the in and out devices. Nothing else is expected from it, so it doesn't do anything else other than check the buttons function to determine if any of them has been pressed.

### 2) Exit IDLE state
To exit IDLE state, two conditions must be met, this is so once it exits that state, it never checks the beginning of the code again.
- The loop station is on Track 1.
- The record button has been pressed.
This immediatly changes the state to RECORDING. There is no other way of transitioning from IDLE to PLAY, it has to go through the recording phase. This case then ramifies into two different cases:

#### 2.1) Master track sets a loopduration of MAX_SAMPLES
To reach this case scenario, after pressing the record button for the first time nothing else has to be pressed, letting the loop run to its end by itself. This automatically sets the loop to the maximun duration and updates its state to PLAYING.

#### 2.2) Master track stops recording before MAX_SAMPLES
To reach this case scenario, record button must be pressed a second time before it reaches the max length duration, setting the loop length to anything under 20 seconds.

### 3) Second or third tracks are recorded after the master track
This case scenario is identic in both cases. The only change is which track it's on once it starts recording. The loop station is playing the master track on loop (with whatever volume it's set to), then the record button is pressed. Once it reaches the beginning of the loop, the loop station enters recording state and exits it once it reaches the loop length. Pressing any other button during this recording phase is completely ignored.

### 4) Any track is overdub
If the objective is to overrun any of the existing tracks, everything else will keep happening accordingly, but the overdubbed track stops being reproduced, as it is being recorded again. The track button intended to be overdubbed must be pressed and the record button as well (the order is not relevant as long as the right track button has been pressed before the loop reaches the beginning again and stars recording).

### 5) PANIC! is invoked
Panic, either throught accidentally pressing two track buttons at the same time (or more), or as an intentionate way of a fast reset, happens by pressing or holding two different track buttons at the same time. It can happen at any moment.  


<a name="documentation"></a>
## Helpful documentation
It is recommended to check other documentation that helps with the specific components that are used in the project. In this scenario, external references will be linked below to further aid the understanding of used hardware:
- [ESP32-S3 Hardware Pinout and Details (Luis Llamas)](https://www.luisllamas.es/en/esp32-s3-hardware-details-pinout/)
- [SPARKLEIOT XH-S3E Ultra-Low Power WiFi Module User Manual (Manuals+)](https://manuals.plus/es/sparkleiot/xh-s3e-ultra-low-power-wifi-module-manual)
- [Lesson 2: Testing Your Microphone (INMP441)(OceanLabz)](https://www.oceanlabz.in/lesson-2-testing-your-microphone-inmp441/?srsltid=AfmBOoqxJcil_28kHzQjYyT23pxv0jRdq-Ltm8145vbhw94lhPC2eP9x)
