#include "driver/i2s.h"

// Pines I2S
#define I2S_BCLK      41
#define I2S_WS        40
#define I2S_DIN_MIC   47
#define I2S_DOUT_DAC  42

// Pines potenciometros
#define POT_T1 12  
#define POT_T2 13  
#define POT_T3 14

// Pines de botones
#define BTN_T1 9   
#define BTN_T2 10  
#define BTN_T3 11  
#define BTN_REC 15

// Pines LEDs
#define LED_T1  16  
#define LED_T2  17
#define LED_T3  18
#define LED_REC 8
#define LED_PENDING 3

// Audio
#define MAX_LOOP_SECONDS 20
#define SAMPLE_RATE   48000

// Estados
enum SelectedTrack { TRACK1, TRACK2, TRACK3 };  // Donde estoy?
enum SystemStatus { IDLE, RECORDING, PLAYING }; // Que estoy haciendo?

bool pendingRecord = false; // Grabacion pendiente
bool stopAtLoopEnd = false; // Parada automatica de grabar cuando llego al final del loop

// Variables para mis estados
SelectedTrack trackFocus = TRACK1;
SystemStatus currentStatus = IDLE;

// Duracion maxima establecida (arbitrario)
const uint32_t MAX_SAMPLES = MAX_LOOP_SECONDS * SAMPLE_RATE;

// Buffers en PSRAM
int16_t* track1 = NULL; // no debería ser int32_t?
int16_t* track2 = NULL;
int16_t* track3 = NULL;

// Variables que definen mi loop
uint32_t loopLength = 0;
uint32_t currentPos = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  //Potenciometros
  analogReadResolution(12); // Establece resolucion de 0 a 4095 (De 10 a 12 bits)

  // Entrada de los potenciometros
  pinMode(POT_T1, INPUT);
  pinMode(POT_T2, INPUT);
  pinMode(POT_T3, INPUT);

  // Reserva de memoria para las pistas
  size_t bufferSize = MAX_SAMPLES;
  
  track1 = (int16_t*) ps_calloc(bufferSize, sizeof(int16_t));
  track2 = (int16_t*) ps_calloc(bufferSize, sizeof(int16_t));
  track3 = (int16_t*) ps_calloc(bufferSize, sizeof(int16_t));

  // Verificar si la PSRAM se pudo reservar
  if (track1 == NULL || track2 == NULL || track3 == NULL) {
    Serial.println("¡ERROR! No se pudo asignar PSRAM. ¿Está activada en Tools?");
    while(1); 
  }

  // Salida de los LEDS
  pinMode(LED_T1, OUTPUT);
  pinMode(LED_T2, OUTPUT);
  pinMode(LED_T3, OUTPUT);
  pinMode(LED_REC, OUTPUT);
  pinMode(LED_PENDING, OUTPUT);

  // Entrada de los botones
  pinMode(BTN_T1, INPUT_PULLDOWN);
  pinMode(BTN_T2, INPUT_PULLDOWN);
  pinMode(BTN_T3, INPUT_PULLDOWN);
  pinMode(BTN_REC, INPUT_PULLDOWN);

  // Inicializacion de I2S
  setupI2S();
  updateTrackLEDs(); // Estado inicial de los LEDs de los tracks
}

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

void loop() {
  int16_t sampleIn = 0; // Variable de entrada de audio
  int32_t mixed = 0;    // Variable de salida de audio
  size_t bytesRead, bytesWritten;

  static float v1 = 1.0, v2 = 1.0, v3 = 1.0;
  if(currentStatus == IDLE){
    buttons();
  }else{
    // ENTRADA DE AUDIO (Leo el micro todo el rato por constancia) 
    i2s_read(I2S_NUM_0, &sampleIn, sizeof(int16_t), &bytesRead, portMAX_DELAY);

    sampleIn = sampleIn >> 2; // Ajuste para evitar ruidos de distorsion y demas

    if (currentPos % 1024 == 0) { // Actualiza volumen cada 1024 muestras
      float tmp;
      v1 = ((tmp = getVolume(POT_T1)) != v1) ? tmp : v1;
      v2 = ((tmp = getVolume(POT_T2)) != v2) ? tmp : v2;
      v3 = ((tmp = getVolume(POT_T3)) != v3) ? tmp : v3;
    }

    if (currentPos % 441 == 0){ // Comprobamos botones y hacemos update de LEDS cada ciertas muestras
      buttons();
    }

    // LOGICA DE GRABACION
    if (currentStatus == RECORDING) { // Si estoy grabando, guardo donde me encuentro lo que estoy recogiendo del input del micro
      if (trackFocus == TRACK1) track1[currentPos] = sampleIn;
      else if (trackFocus == TRACK2) track2[currentPos] = sampleIn;
      else if (trackFocus == TRACK3) track3[currentPos] = sampleIn;
    }

    // LOGICA DE REPRODUCCION
    // Calculo del volumen y control de mute forzado
    int32_t s1 = (v1 < 0.01f) ? 0 : (int32_t)(track1[currentPos] * v1);
    int32_t s2 = (v2 < 0.01f) ? 0 : (int32_t)(track2[currentPos] * v2);
    int32_t s3 = (v3 < 0.01f) ? 0 : (int32_t)(track3[currentPos] * v3);

    // Reproduzco todas las pistas que no esten grabando en ese momento
    if (currentStatus == RECORDING) {
        if (trackFocus == TRACK1)      mixed = s2 + s3;
        else if (trackFocus == TRACK2) mixed = s1 + s3;
        else if (trackFocus == TRACK3) mixed = s1 + s2;
    } else {
        mixed = s1 + s2 + s3;
    }
    
    // Limitador de audio para la salida
    if (mixed > 32767) mixed = 32767;
    if (mixed < -32768) mixed = -32768;
    int16_t sampleOut = (int16_t)mixed;

    // SALIDA DE AUDIO
    i2s_write(I2S_NUM_0, &sampleOut, sizeof(int16_t), &bytesWritten, portMAX_DELAY);

    // LOGICA DEL LOOP
    currentPos++;
    if (loopLength > 0) {
        if (currentPos >= loopLength) { // Si hemos llegado al final del loop, vuelvo al principio
            currentPos = 0;
            if (pendingRecord) {        // Si hay grabacion pendiente, iniciamos al principio del loop
                currentStatus = RECORDING;
                pendingRecord = false;
                digitalWrite(LED_PENDING, LOW);
                updateRECLED();
                Serial.println(">>> Iniciando grabación de capa.");
            } 
            else if (currentStatus == RECORDING && stopAtLoopEnd) { // Si estoy grabando, como ya he llegado al final del loop, paro de grabar
                currentStatus = PLAYING;
                updateRECLED();
                stopAtLoopEnd = false;
                Serial.println(">>> Grabación completada y sincronizada.");
            }
        }
    } else if (currentPos >= MAX_SAMPLES && currentStatus == RECORDING) { // Parada automatica de la primera grabacion al llegar al limite establecido
        loopLength = MAX_SAMPLES;
        currentStatus = PLAYING;
        updateRECLED();
        currentPos = 0;
        Serial.println(">>> Grabación completada y sincronizada.");
    }
  }
}

void buttons() {
  static bool lastState = LOW; 
  
  // COMPROBAR SI HEMOS CAMBIADO EL FOCUS
  bool track1 = digitalRead(BTN_T1) == HIGH;
  bool track2 = digitalRead(BTN_T2) == HIGH;
  bool track3 = digitalRead(BTN_T3) == HIGH;

  // Deteccion de Panic!!
  int pressedCount = (int)track1 + (int)track2 + (int)track3;
  if (pressedCount >= 2) {
    panicReset();
    return;
  }

  // Cambio el focus solo si realmente ha cambiado y no estoy grabando
  if(currentStatus != RECORDING){
    if (track1 && trackFocus != TRACK1) {
      trackFocus = TRACK1;
      updateTrackLEDs();
      Serial.println(">>> TRACK 1 SELECCIONADO: ");
    } else if (track2 && trackFocus != TRACK2) {
      trackFocus = TRACK2;
      updateTrackLEDs();
      Serial.println(">>> TRACK 2 SELECCIONADO: ");
    } else if (track3 && trackFocus != TRACK3) {
      trackFocus = TRACK3;
      updateTrackLEDs();
      Serial.println(">>> TRACK 3 SELECCIONADO: ");
    }
  }

  // COMPROBAR SI HEMOS PULSADO GRABAR
  bool currentRecState = digitalRead(BTN_REC);

  if (currentRecState == HIGH && lastState == LOW) { //Flanco de subida?
    handleActionBtn();
    delay(50);
  }
  lastState = currentRecState;
}

void handleActionBtn() {
    // CASO A: Es la primera grabacion (Track 1 vacio)
    if (currentStatus == IDLE && trackFocus == TRACK1) {
        currentStatus = RECORDING;
        currentPos = 0;
        updateRECLED();
        Serial.println("Grabando Pista Maestra...");
    } 
    
    // CASO B: Estamos grabando la Pista Maestra y pulsamos para cerrarla
    else if (currentStatus == RECORDING && loopLength == 0) {
        loopLength = currentPos; // Definimos la duración AQUÍ
        currentStatus = PLAYING;
        updateRECLED();
        currentPos = 0; // Reiniciamos al inicio para empezar el playback
        Serial.printf("Loop definido: %u muestras. Play.\n", loopLength);
    }

    // CASO C: El loop ya existe y queremos grabar una capa (Overdub)
    else if (currentStatus == PLAYING) {
        pendingRecord = true;
        digitalWrite(LED_PENDING, HIGH); 
        stopAtLoopEnd = true; // Forzamos a que se detenga solo al final
        Serial.println("Grabación armada: Empezará y terminará en el ciclo.");
    }
}

float getVolume(int pin) {
  int raw = analogRead(pin);
  if (raw < 200) { //Zona muerta para asegurar que se queda al 0
    return 0.0f; 
  }else{
    return (float)(raw - 200) / (4095.0f - 200.0f);
  }
}

void updateTrackLEDs() {   // LEDs de Pistas
  digitalWrite(LED_T1, (trackFocus == TRACK1) ? HIGH : LOW);
  digitalWrite(LED_T2, (trackFocus == TRACK2) ? HIGH : LOW);
  digitalWrite(LED_T3, (trackFocus == TRACK3) ? HIGH : LOW);
}

void updateRECLED(){      // LED de REC
  if (currentStatus == RECORDING) {
    digitalWrite(LED_REC, HIGH);
  } else {
    digitalWrite(LED_REC, LOW);
  }
}

void panicReset() {
  Serial.println("!!! PANIC: Reseteando sistema...");
  ESP.restart();
}