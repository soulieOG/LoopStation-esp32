# LoopStation-esp32
To see the English version of this README.md click below.  
[English documentation](README.md)

## Índice
* [Introducción](#introduccion)
* [Defining the project](#define)
* [Materiales](#materiales)
* [La lógica del sistema](#logica)
* [Alcanzando las espectativas con el código](#code)
* [El hardware](#hardware)
* [El funcionamiento](#functioning)
* [Documentación extra](#documentation)

<a name="introduccion"></a>
## Introducción
Se trata de un proyecto de índole pequeño que pretende montar un dispositivo portable que actúa como un tipo de "loop station". Planteado para la clase de Sistemas Empotrados, se utilizará una ESP32 como foco central para construir la loop station. A lo largo del documento se especificarán los materiales y su coste (estimando a la alta y teniendo en cuenta el coste del material proporcionado por los profesores también), el planteamiento del funcionamiento y demás puntos de interés para entender y/o replicar el dispositivo con facilidad. Sígase como se estime conveniente.  

Seguir este README para observar el proceso llevado a cabo para la creación final de la implementación de su primer prototipo con el siguiente código:  
[Código de la Loop Station](/loopStation.ino)


<a name="define"></a>
## Definiendo el proyecto  
Primero, para entender la naturaleza del proyecto, se debe definir el concepto de loop station.  

### Qué es una loop station?
Tradicionalmente hablando, este concepto suele asociarse (y puede que estés familiarizado con ello) a los pedales de guitarra. Sin embargo, ese no es el único contexto que tiene una loop station. Para generalizar y sentar las bases de este proyecto, se definirá una loop station como un dispositivo que permite al usuario grabar o capturar una serie de audios (pistas, eventos MIDI, etc.) y reproducirlos en bucle (loop). En el caso concreto de esta estación de bucles, se encargará de grabar audios.

De esta manera, se pueden crear bucles multicapa, dando un amplio margen para que la creatividad tome el control (armonizar, grabar una base de fondo, etc.).

### Defining its limits
Una loop station profesional puede variar en su composición (número de pistas, opciones de manipulación, etc.). Debido a esto, es necesario establecer las bases de las limitaciones y expectativas de esta mini versión:  

- 3 pistas funcionales (grabación, overdub y control de volumen).  
- Graban audios a través del módulo de micrófono.  
- La primera pista será una pista "maestra" (define la longitud del bucle), y las otras dos serán sus esclavas.  

Ahora bien, hay que tener en cuenta diferentes límites en la sección de hardware. Debido a la gestión de memoria (PSRAM), se establecerá que:

- Los bucles tienen una duración máxima de 20 segundos.

Esto implica que cuando la loop station se configura para grabar la primera pista del bucle (necesariamente la Pista 1, ya que es la maestra), se puede establecer una duración de cualquier valor entre 0 y 20 segundos. Esto se hará pulsando el botón de grabación por segunda vez para detenerla, o dejando que grabe hasta el final natural del bucle. Estas expectativas se logran mediante delimitaciones por software (al pulsar grabar por primera vez, si se alcanzan los 20 segundos de grabación, la loop station sale del estado de grabación automáticamente). De esta manera, durante todo el bucle, los audios se ajustan a la duración establecida.

Este proyecto está diseñado para que pueda desmontarse fácilmente, ya que algunos componentes deben ser devueltos. Por lo tanto, tiene un concepto de prototipo que no es definitivo y presenta algunos problemas de hardware que podrían resolverse soldando todos los componentes correctamente en una placa.

<a name="materiales"></a>
## Materiales  
En esta seccion los requirimientos tanto de software como de hardware serán especificados. Tal y como se presenta, no es una necesidad estricta usar los mismos, y el proyecto se puede ajustar a diferentes módulos. Sin embargo, esta documentación sigue el proceso de manera fiel a las opciones especificadas que se escogieron por el grupo de trabajo y grandes modificacions del planteamiento del proyecto podrían conllevar un gran impacto en los requerimientos de software y hardware.  

This project is designed so it can be easily put apart, since some components must be returned, therefore it has a prototype concept that is not final, and it deals with some hardware issues that could be resolved by soldering everything properly in a board.  
Es te proyecto está diseñado para que se pueda desmontar de manera fácil, ya que algunos componentes deben ser devueltos o reotulizados, por lo tanto es un concepto de prototipo no final, que lidia con algunos problemas de hardware que se podrían solventar con una adecuada soldadura de los componentes en una placa.  

### Componentes  
Han sido pedidos por medios más fiables a coste de un posible mayor precio, sin arriesgar a que llegaran dañados. Varios de los componentes fueron comprados en packs de varias unidades, por lo que se reflejará el precio estimado de una unidad del pack. No todos los materiales usados en esta versión son necesarios o indispensables, ya que el montaje ha sido hecho de manera subjetiva para mantener una limpieza y separación del circuito. Por lo tanto, no es una guía estricta, sino orientativa, del hardware implicado en el proyecto.  

| Nombre | Cantidad | Coste Estimado (euros) | Comprado/Proporcionado |
|:--- |:--- |:--- |:--- |
| ESP32-S3 DevKitC-1 N16R8 | 1 | 8€ | Comprado |
| PCM5102 | 1 | 4,7€ | Comprado |
| INMP441 | 1 | 4€ | Comprado |
| Potenciómetros | 3 | 0,5€ | Comprado |
| LEDs (azul)  | 3 | 0,05€ | Proporcionado |
| LEDs (verde)  | 1 | 0,05€ | Proporcionado |
| LEDs (amarillo)  | 1 | 0,05€ | Proporcionado |
| Botones  | 4 | 0,04€ | Proporcionado |
| Cables Dupont  | 1 | 15€ | Comprado |
| Protoboard  | 2 | x | Proporcionado x1, Bought x1 |
| Mini Protoboard  | 1 | x | Proporcionado |  
 
La "x" se refiere a la posibilidad de comprar los componentes maracados con precio "x" en un pack de aproximadamente 13€.
  
Se refiere entonces el precio total estimado a la suma del precio de todos los materiales usados en este proyecto. El precio total invertido es el precio total real restándole los materiales que fueron proporcionados o fueron comprados previamente para otros proyectos, de manera que solo se ve reflejado en éste el dinero invertido desde el inicio del proyecto a final.

| Precio Total Estimado | Precio Total Invertido |
|:--- |:--- |
| 47€ | 33,5€ |

Estos precios, si el proyecto fuera a empezarse de cero, son algo surrealistas. Se recomendaría comprar packs de componentes, subiendo algo el precio. De todas maneras, el dinero realmente invertido en este proyecto particular se ve reflejado en los precios de las tablas. La diferencia que marcaría añadir el precio entero de packs con componentes extras no constituye un gasto real en el proyecto, ya que ese dinero solo se desperdiciaría exclusivamente en este proyecto si no se fueran a usar en ningún futuro proyecto. También hay que considerar la posibilidad de que un componente se rompa o no funcione.  

### Software
Respecto a la parte de código del proyecyto, se ha utilizado Arduino IDE. Se harán recordatorios a lo largo del documento para ciertos específicos de la configuración, pero hay dos principales focos que tener en cuenta:  
- La elección de board debe estar descargada si no la tiene por defecto Arduino.  
- El Puerto (Port) correcto está seleccionado (asegurarse de quqe el COM es correcto y ante la duda si hay vrios, conectar y desconectar la board para ver cuál es). 

<a name="logica"></a>
## La lógica del sistema  
Ahora que los objetivos están claros, se puede definir una lógica para el sistema. Esto se consigue mediante dos preguntas simples:  
- ¿Qué estoy haciendo?

![IMG](https://github.com/user-attachments/assets/7c5a5b7b-5497-40b6-ad98-fd4cd1dbc56b)


Es importante recalcar que una vez que la loop station empieza un loop (graba el primer track), el dispositivo SIEMPRE está reproduciendo en bucle. Eso no significa que si se cambia a grabando la loop station deja de reproducir, simplemente cambia su foco principal a la tarea de grabar. Si la loop station se encuenta en el estado de reproducir, eso simplemente significa que 1- Se ha establecido un bucle 2- Actualmente no está grabando nada.  

- ¿Dónde estoy?

![IMG](https://github.com/user-attachments/assets/8f85534d-6779-4fba-9019-fdd935b59e33)


El programa responde a un doble estado. Cuando el dispositivo se inicia, entra un estado de reposo (IDLE) donde su único propósito es detectar si el usuario quiere empezar a grabar (presionar el botón de grabar). Cada vez que se quiera grabar (se ha presionado el botón de grabar), al principio de cada loop (por coordinación), la loop station entra en su estado de grabar. Cuando termina de grabar, pasa a reproducir, lo que implica que ya hay un bucle definido (tiene longitud de loop, looplength, definida por lo menos, da igual si se ha grabado silencio en la pista 1).  

De igual manera, responde a dónde se encuentra. ¿Estoy en la pista 1, 2 o 3? Esto ayuda a determinar dónde está grabando cuando captura la entrada (input) y decide dónde pone la salida (output) de las pistas. Esto también ayuda a la dinámica maestro-esclavo. Sólo se podrá comenzar a grabar cada bucle si la loop station se encuentra en la pista 1.  

Mientras que se podría añadir una restricción para ignorar si se intenta cambiar de track al principio de cada bucle (estado de espera, IDLE) ya que se debe estar en la pista uno para grabar (igual que cuando se graba se ignora un cambio de pista), no se implementa porque ayuda a la detección temprana de mal funcionamiento del hardware (los botones se pueden pulsar nada más se enciende la loop station para controlar si cambia de pista de manera exitosa).  

Con esta información, se introducen dos lógicas interesantes de la loop station:  

#### Pending recording
- Si se presiona el botón de grabar en cualquier momento que no sea justo al principio del bucle, el sistema entra en un "estado" intermedio introducido por una variable nooleana (se especificará en la sección de software). Este "estado" intermedio es simplemente una alerta para que el sistema sepa que cuando vuelve al principio del loop tiene que empezar a grabar, asegurando que se cambie el estado.  
- Esto solo pasa si se ha establecido un loop. Cuando se inicializa/conecta, el bucle no está definido y no está intentando leer or reproducir nada, así que empieza a grabar inmediatamente tras pulsar el botón de grabar.  

Con este concepto similar a las interrupciones pendientes, no se molesta al sistema hasta que se encuentra en el principio del bucle, ya que se ha establecido que no se grabará en ningún otro momento para una sincronización adecuada de la loop station que siga la naturaleza establecida.  

#### Panic!
- Pulsar dos botones de pista a la vez manda una señal de "PANIC!" (pánico), que resetea la ESP32.  

Qué pasa si se pulsan botones de manera simultanea de pistas? Como el hardware está pensado para ser introducido en una caja al final, se aprovecha este caso límite para un reseteo rápido de la ESP32 ya que no es osible pulsar el propio botón de reset de la placa una vez metida en la caja y cerrada. Hay diferentes lógicas que se podían introducir para gestionar este evento, pero se aprovecha la oportunidad para el reseteo rápido. Se plantea un posible quinto botón para esta funcionalidad, pero para simplificar el hardware y minimizar los componentes, se establece el pulsar dos botones o más de track a la vez como una señal de "PANIC!".  

<a name="code"></a>
## Alcanzando las espectativas con el código  
Una vez la lógica ha sido clarificada, el código se puede empezar a cosntruir siguiéndola. se cubrirán diferentes casos de uso, y una explicación más exhaustiva de el comportamiento exacto del disporitivo se reflejará en esta sección. Los pines se mencionarán más adelante en la siguiente sección de hardware. A lo largo del código se realizarán diferentes impresiones por pantalla que servirán de comprobación del correcto funcionamiento, además de servir de preámbulo para la adición de una posible pantalla.  

La especificación de los define de los pines en el código se encuentra aquí: [El hardware](#hardware).  

Sólo hay un include que se necesita para esta versión:  
#include "driver/i2s.h"  


### Definición de constantes
Justo después de la declaración de los pines se encuentra una sección que declara lo siguiente:  
- Constantes que definen la duración máxima del bucle en 20 segundos para un fácil reajuste y para el cçalculo de la frecuencia.  
- Las dos máquinas de estados por medio de tipo Enumeración.
- pendingRecord (grabación pendiente) is una variable booleana para indicarle a la loop station que tiene que empezar a grabar una vez vuelva al principio del bucle.  
- stopAtLoopEnd (parar al final del bucle) se declara para que cuando se grabe la primera isntancia de la pista 1, pare automáticamente a los 20 segundos si no se presiona el botón de grabar una segunda vez antes de eso.  
- Las instancias de las máquinas de estado se declaran e inicializan (trackFocus, pista de focus y currentStatus, estado actual).  
- MAX_SAMPLES es el resultado de operar con la duración máxima del bucle y la frecuencia establecida, para encontrar el final del bucle que empieza en 0.  
- Los punteros para los buffers de cada track, inicializados a null.  
- loopLength (longitu del bucle) y currentPos (posición actual) se definen e inicializan a 0. Estas variables determinan la duración del bucle que se pondrá una vez se grabe la pista maestra por primera vez (siendo MAX_SAMPLES su valor máximo) y currentPost marcará la posición que irá aumentando según avanza el loop (empezando en 0 y acabando en looplength).  


```
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
int16_t* track1 = NULL;
int16_t* track2 = NULL;
int16_t* track3 = NULL;

// Variables que definen mi loop
uint32_t loopLength = 0;
uint32_t currentPos = 0;
```

### El setup
En esta parte del código, se hacen todas las inicializaiones del reto de cosas. Los pines se asignarán como entrada o salida (input o output) y se inicializarán los buffers de cada track a la longitud máxima establecida (peor caso). Para comprobar que la memoria se ha podido reservar correctamente, se pone un mensaje de error para la PSRAM- Después de todo eso, invoca al [setupI2S](#setupI2S), que se explica a continuación y a [updateTrackLEDs](#updateTrackLEDs) para inicializar los LEDs.  

```
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
```


<a name="setupI2S"></a>
#### El I2S setup   
Esta secciónd del código está altamente influencuada por las configuraciones de otros proyectos que usan estos componentes, por lo tanto, se pueden aplicar cambios, pero el setup propuesto debería funcionar de manera adecuada con los componentes propuestos.  

Se tienen que hacer consideraciones en esta parte si el micrófono que se vaya a usar no es mono (un pequeño ajuste). El INMP441 es un micrófono mono y en este escenario se le va a conectar el L/R al GND, por lo que el canal por el que vaya será el izquierdo. Si se conectan auriculares al PCM5102, solo se oirá por el izquierdo (.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT).  

El  .dma_buf_count = 4 y .dma_buf_len = 128 tienen valores bajos para evitar retrasos en el audio al grabar y reproducir (desfase) por el bloqueo que generan las funciones que se encargan de eso (read y write).  


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
Las variables que va a necesitar el bucle se declaran (e inicializan a 0 si es necesario).  

```
void loop() {
  int16_t sampleIn = 0; // Variable de entrada de audio
  int32_t mixed = 0;    // Variable de salida de audio
  size_t bytesRead, bytesWritten;

  static float v1 = 1.0, v2 = 1.0, v3 = 1.0;

```

La loop station solo comprueba los botones mientras no haya empezado con el bucle aún.  

```
  if(currentStatus == IDLE){
    buttons();

```

Para mantener la constacia en el bucle, empieza a escuchar al micrófono, aunque sólo recogerá la entrada si está grabando. También contiene la primera de varias sentencias de control pensadas para mejorar la calidad de el audio en la medida de lo posible.  

```
  }else{
    // ENTRADA DE AUDIO (Leo el micro todo el rato por constancia) 
    i2s_read(I2S_NUM_0, &sampleIn, sizeof(int16_t), &bytesRead, portMAX_DELAY);

    sampleIn = sampleIn >> 2; // Ajuste para evitar ruidos de distorsion y demas
```

Simplemente para el control seguro de la coordinación del sistema, algunos valores solo se comprueban cada x muestras. No es realmente un programa exigente que necesite FreeRTOs, pero por asegurar el funcionamiento fluido, bastantes sentencias de control son implementadas. En este caso recibe el valor del volumen.  

```
    if (currentPos % 1024 == 0) { // Actualiza volumen cada 1024 muestras
      float tmp;
      v1 = ((tmp = getVolume(POT_T1)) != v1) ? tmp : v1;
      v2 = ((tmp = getVolume(POT_T2)) != v2) ? tmp : v2;
      v3 = ((tmp = getVolume(POT_T3)) != v3) ? tmp : v3;
    }
```

De igual manera, los botones solo se comprueban cada ciertas muestras (diferente del volumen por alternar).  

```
    if (currentPos % 441 == 0){ // Comprobamos botones y hacemos update de LEDS cada ciertas muestras
      buttons();
    }
```

Si está grabando, entonces necesita procesar la muestra de entrada que recibe del micrófono en la pista que corresponda. Como está contemplado que mientras se graba no se puede mover de pista (especificado en la función auxiliar de [buttons](#buttons)) no se producen mezclas raras.  

```
    // LOGICA DE GRABACION
    if (currentStatus == RECORDING) { // Si estoy grabando, guardo donde me encuentro lo que estoy recogiendo del input del micro
      if (trackFocus == TRACK1) track1[currentPos] = sampleIn;
      else if (trackFocus == TRACK2) track2[currentPos] = sampleIn;
      else if (trackFocus == TRACK3) track3[currentPos] = sampleIn;
    }
```

Como ya ha leído el volumen arriba, en caso de que haya cambiado al último valor que ha leído, se actualizan como corresponda multiplicando el volumen (la proporción establecida).  

```
    // LOGICA DE REPRODUCCION
    // Calculo del volumen y control de mute forzado
    int32_t s1 = (v1 < 0.01f) ? 0 : (int32_t)(track1[currentPos] * v1);
    int32_t s2 = (v2 < 0.01f) ? 0 : (int32_t)(track2[currentPos] * v2);
    int32_t s3 = (v3 < 0.01f) ? 0 : (int32_t)(track3[currentPos] * v3);
```

Esta sección monta la salida del audio, añadiendo las tres pistas. Si alguna no ha grabado aún, supondrá no añadir nada, por lo que no causa problemas.  

```
    // Reproduzco todas las pistas que no esten grabando en ese momento
    if (currentStatus == RECORDING) {
        if (trackFocus == TRACK1)      mixed = s2 + s3;
        else if (trackFocus == TRACK2) mixed = s1 + s3;
        else if (trackFocus == TRACK3) mixed = s1 + s2;
    } else {
        mixed = s1 + s2 + s3;
    }
```

Variass secciones de control implementadas para manejar de manera segura el volumen de la salida para intentar acotar comportamiento indeseado del hardware. Los límites son genéricos y funcionan, pero se pueden ajustar sin problemas.  

```
    // Limitador de audio para la salida
    if (mixed > 32767) mixed = 32767;
    if (mixed < -32768) mixed = -32768;
    int16_t sampleOut = (int16_t)mixed;
```

La siguiente línea reproduce la salida de audio, mandándola al PCM5102.  

```
    // SALIDA DE AUDIO
    i2s_write(I2S_NUM_0, &sampleOut, sizeof(int16_t), &bytesWritten, portMAX_DELAY);
```

Esta gran sección del código es la parte complicada de la lógica. Las sentencias *if* comprueban ciertps valores que se pueden cambiar por comprobaciones equivalentes o ajustados con cambios de la lógica. Primero mueve la posición a la siguiente. A continuación, parte el código en dos: 1) Hay un bucle ya establecido (looplength mayor que 0 o que no está en IDLE). 2) Está grabando la pissta maestra por primera vez, y por lo tanto no hay un bucle establecido.  

Dentro de la primera parte también hace una partición. Primero, comprueba si ha llegado al final del bucle, para que resetee la posición al 0. Después comprueba si hay alguna grabación pendiente, para actualizar el sistema a grabando (si se ha pulsado el botón de grabar). Despueés de manejar las grabaciones pendientes (actualizar LEDs, estado, variables booleanas...) necesita comprobar si está grabando, porque si ha llegado al final de bucle necesita parar de grabar de manera automática (cambiar el estado, los LEDs...).  

Dentro de la segunda condición se comprueba si ha llegado a la longitud máxima del bucle (MAX_SAMPLES), ya que el sistema sabe porque lo tiene controlado que tiene que encontrarse grabando en la primera pista por primera vez. Si este es el caso, actualiza las variables acorde con ello, similar a los ejemplos anteriores.  

```
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
```

### Funciones auxiliares 

<a name="buttons"></a>
#### 1) buttons  
La primera cosa que hace es guardar el estado bajo en una variable booleana, para ser usada más adelante en el código en detectar flancos de subida. También establece una variable booleana por cada botón de cada pista para leer su estado.  

```
void buttons() {
  static bool lastState = LOW; 
  
  // READ THE BUTTON STATES
  bool track1 = digitalRead(BTN_T1) == HIGH;
  bool track2 = digitalRead(BTN_T2) == HIGH;
  bool track3 = digitalRead(BTN_T3) == HIGH;
                          ...
```

La siguiente cosa es detectar si ha habido un PANIC!, para para inmediatamente de hacer cosas e ir directamente al reseteo.  

```
  // Panic! detection
  int pressedCount = (int)track1 + (int)track2 + (int)track3;
  if (pressedCount >= 2) {
    panicReset();
    return;
  }
                          ...
```

La siguiente sección actualiza la posición y los LEDs solo si 1) ha cambiado 2) No está grabando.  

```
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
                            ...
```

El botón de grabar tiene un control por flanco de subida para poder lidiar con el botón siendo mantenido en vez de pulsado, lo que daría un problema que no surge en el caso de los botones de pistas.  


```
  // COMPROBAR SI HEMOS PULSADO GRABAR
  bool currentRecState = digitalRead(BTN_REC);

  if (currentRecState == HIGH && lastState == LOW) { //Flanco de subida?
    handleActionBtn();
    delay(50);
  }
  lastState = currentRecState;
```


#### 2) handleActionBtn 
Esta función es llamada cada vez que detecta que se ha pulsado el botón de grabar. Se extrae en una función para mantener claridad y organización en el código. Su trabajo es determinar qué conlleva que se haya presionado el botón de grabar, dependiendo de los estados de la loop station.  

En el primer caso, el estado tras la inicialización o reseteo.  La loop station simplemente se encuentra esperando a que se presione el botón de grabar. Por lo tanto, si se encuentra en estado de espera (IDLE), que solo ocurre si se acaba de conectar o resetear, y la pista en la que se encuentra es la primera (se contempla poder cambiar de track para comprobar el comportamiento adecuado de los botones, por ello no se capa), puede empezar a grabar.  Grabar conlleva cambiar el estado, posicionar al principio del bucle la posición y actualizar el LED de grabar, para activar el verde.  

```
void handleActionBtn() {
    // CASO A: Es la primera grabacion (Track 1 vacio)
    if (currentStatus == IDLE && trackFocus == TRACK1) {
        currentStatus = RECORDING;
        currentPos = 0;
        updateRECLED();
        Serial.println("Grabando Pista Maestra...");
    } 
                          ...
```

El usuario quiere establecer la longitud por debajo de los 20 segundos, por lo tanto necesita pulsar el botón de grabar una segunda vez (segundo caso). Debe encontrarse grabando, y como ya ha comprobado en la anterior que si ha entrado a grabar se encuentra en la pista uno, se comprueba que aún no hay una longitud establecida para determinar que no hay un bucle aún (no ha terminado de grabar).  

```
    // CASO B: Estamos grabando la Pista Maestra y pulsamos para cerrarla
    else if (currentStatus == RECORDING && loopLength == 0) {
        loopLength = currentPos; // Definimos la duración AQUÍ
        currentStatus = PLAYING;
        updateRECLED();
        currentPos = 0; // Reiniciamos al inicio para empezar el playback
        Serial.printf("Loop definido: %u muestras. Play.\n", loopLength);
    }
                          ...
```

Está grabando cualquier pista (no es la primera iteración de la pista maestra). Cambia todas las variables acorde con la actualización de la loop station, activando la variable booleana que le indica que quiere grabar, encendiendo también el LED amarillo encargado de avisar.  

```
    // CASO C: El loop ya existe y queremos grabar una capa (Overdub)
    else if (currentStatus == PLAYING) {
        pendingRecord = true;
        digitalWrite(LED_PENDING, HIGH); 
        stopAtLoopEnd = true; // Forzamos a que se detenga solo al final
        Serial.println("Grabación armada: Empezará y terminará en el ciclo.");
    }
}
```


#### 3) getVolume  
Esta función se encarga de controlar la entrada del potenciómetro destinado al volumen de una pista. Debido al ruido en el sistema y a la imprecisión del hardware, se establece una "zona muerta" (dead zone). Se han implementado varias sentencias de control, pero esto se puede ajustar de muchas maneras. Se podría aplicar una fórmula cuadrática o cualquier otra que el usuario considere más adecuada. En este caso, el código actual funciona perfectamente para el objetivo previsto, por lo que realizar cambios en esta función no es una prioridad.

```
float getVolume(int pin) {
  int raw = analogRead(pin);
  if (raw < 250) { //Zona muerta para asegurar que se queda al 0
    return 0.0f; 
  }else{
    return (float)(raw - 200) / (4095.0f - 200.0f);
  }
}
```  


<a name="updateTrackLEDs"></a>

#### 4) updateTrackLEDs  
Esta función se invoca únicamente cuando se pulsa un botón que afecta al estado de los LEDs de las pistas. De esta forma, se asegura de apagar el LED anterior y encender el correcto.

```
void updateTrackLEDs() {   // LEDs de Pistas
  digitalWrite(LED_T1, (trackFocus == TRACK1) ? HIGH : LOW);
  digitalWrite(LED_T2, (trackFocus == TRACK2) ? HIGH : LOW);
  digitalWrite(LED_T3, (trackFocus == TRACK3) ? HIGH : LOW);
}
```


#### 5) updateRECLED
Esta función ha sido creada específicamente para ser invocada solo cuando el estado de grabación se actualiza (ya sea al entrar o salir de dicho estado), de modo que enciende o apaga el LED según corresponda.

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
Esta función se invoca cuando buttons detecta que se ha pulsado más de un botón de pista al mismo tiempo. Reinicia el ESP32 (una alternativa mejor que reset();). Esto se puede hacer en cualquier momento.

```
void panicReset() {
  Serial.println("!!! PANIC: Reseteando sistema...");
  ESP.restart();
}
```

## Nota para Arduino IDE (Recordatorio)
Este código fue escrito con Arduino IDE. Para ejecutarlo, se deben comprobar tres cosas fundamentales:  

- El módulo ESP32S3 Dev Module debe estar instalado como opción de placa.
- La PSRAM debe estar activada (OPI).
- El puerto debe estar seleccionado correctamente (asegúrate de que sea el COM adecuado).  

Como nota adicional, asegúrase de tener instalada la única librería necesaria (#include "driver/i2s.h"). 


<a name="hardware"></a>
## El hardware
Dado que el software ya está diseñado, lo único que queda es conectar el hardware. CUIDADO: pueden surgir problemas si no pruebas los componentes por separado. Se recomienda testearlos de antemano para poder descartar fallos por componentes defectuosos (o cableado/soldadura incorrectos) y ahorrarte tiempo más adelante. Para ayudarte en esta tediosa tarea, aquí se proporcionan códigos de prueba:
[Test codes for INMP441 and PCM5102](/tests) 

Las pruebas se ejecutan con el ESP32S3, pero el código y los pines deberían poder adaptarse fácilmente a placas similares (se debe tener especial cuidado con el uso de la memoria que permite la placa, ya que la utilizada en este proyecto puede usar OPI PSRAM).

- Pequeña indicación para el ESP32S3: ¡asegúrate de que el cable esté conectado a la entrada USB y no a la de COM!

### Probando el hardware
Para que las pruebas sean lo más sencillas posible, los pines utilizados en el código final serán los mismos que se usan en los códigos de prueba, de esa manera, se puede cambiar un código por otro y cargarlo para una comprobación rápida del funcionamiento correcto del componente.

#### PCM5102
El objetivo aquí es reproducir una escala para asegurarse de que el módulo funciona correctamente. Para ello, el PCM5102 puede conectarse directamente a los pines especificados en la imagen de abajo. Si se conectan unos auriculares (o incluso un altavoz) al módulo y el programa se ejecuta (asegúrate de seleccionar el puerto correcto en el IDE de Arduino), debería empezar a sonar una escala en bucle.

Si no se escucha ninguna escala, primero comprueba la soldadura y los cables. Si eso está bien, comprueba de nuevo la conexión I2S, ya que es la causa más probable del silencio y fue el principal problema durante el desarrollo de este proyecto.

![PINS IMAGE](https://github.com/user-attachments/assets/645e0754-2b16-48e0-ac9e-b72c79c2bd8f)


```
// Pines I2S
#define I2S_BCLK      41
#define I2S_WS        40
#define I2S_DIN_MIC   47
#define I2S_DOUT_DAC  42
```


#### INMP441
Para observar correctamente si el micrófono funciona, esta vez será necesaria la ayuda del Serial Plotter (para visualizar las ondas). Si el micrófono está funcionando, deberían aparecer ondas que reaccionen a los sonidos. Si algo va mal, se mostrará una línea plana. Si ocurre esto último, comprueba la soldadura y los cables. Si se garantiza una soldadura adecuada y los cables parecen funcionar bien, entonces podría tratarse de un módulo defectuoso.  

- Pequeña indicación: si no funciona, comprueba que el puerto sea el correcto y que la opción USB CDC On Boot esté en "Enabled" (Activado).

![PINS IMAGE](https://github.com/user-attachments/assets/d0ef23d3-a252-45be-8292-fc218910dd54)

#### LEDs, botones y potenciómetros
Estos componentes se prueban fácilmente con el propio código. Los LEDs se pueden testear de forma sencilla con un circuito básico que no requiere el ESP, pero seguirá el mismo esquema simple tradicional de LED, resistencia y batería.  

Y los pines para estos componentes deberían ser los siguientes:  

![PINS IMAGE](https://github.com/user-attachments/assets/df9f681d-0422-4225-afe1-7667cf9dbacc)

 ```
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
```

Una vez se haya asegurado de que los LEDs funcionan, se puede determinar si los botones también lo hacen si se arranca la loop station y, mientras permanece en modo IDLE, se presionan diferentes botones de pista (¡NO simultáneamente, para evitar un PANIC!). Si los LEDs responden en consecuencia, los botones funcionan correctamente.


### Montaje de la loop station
Ahora que se ha asegurado que todo funciona correctamente, esta parte solo requiere el máximo cuidado al conectar cada cosa en su lugar. Los mayores problemas suelen surgir aquí, así que procede con precaución. Este es el esquema presentado en la versión de la loop station que se entregó para la clase. Siguiendo las imágenes de los pines insertadas anteriormente, el circuito debería verse algo parecido a esto (sin contrar el INMP441 y el PCM5102):  

![WOKWI VERSION OF THE CIRCUIT](https://github.com/user-attachments/assets/ba320ab9-42cf-4e1c-8756-6e0e3c1e4beb)

- Ten en cuenta que hay muchas formas de conectar esto, pero está limitado por los cables, el uso de una placa de expansión u otros factores. En este caso, este es el ejemplo del montaje realizado intentando lograr una buena división por secciones de las partes del hardware. Este esquema se traduce en muchas versiones de la vida real, siendo esta la que se hizo para la presentación del proyecto:  

![IRL HARDWARE PIC](https://github.com/user-attachments/assets/2a8f897d-ad34-49a3-9066-f8b8dadb690e)  

Como regla general, uno no debe tener miedo de utilizar intermediarios como protoboards o placas de expansión, que simplifican este proceso. Lo único que hay que tener en cuenta son las posibles interferencias derivadas del tipo de cables, su longitud y el posible impacto que esto tenga en la entrada/salida de audio.  

Si surgen problemas, como un silencio total en la loop station, lo más probable es que el problema principal sean los cables. Durante la creación de este proyecto, se conectó un GND (tierra) común, y el cable que conectaba el GND de la protoboard al ESP32S3 fallaba, junto con el pin de mute del PCM5102. El hardware es el principal enemigo de este proyecto, ya que el código es funcional tal y como se presentó. Por lo tanto, si tienes problemas, el hardware es el principal culpable. Si se realizaron cambios en el código, eso podría generar nuevos problemas que habría que revisar.

<a name="functioning"></a>
## Funcionamiento

Esta sección detallará diferentes casos de uso, mostrando así el funcionamiento esperado del sistema.  

### 1) Arranque de la ESP32
Una vez que la ESP32 se enciende (conectado al portátil o a baterías), el programa comienza a ejecutarse. Se inicia en un estado de reposo (IDLE), se posiciona en la pista 1 (encendiendo el LED) y configura todos los pines y opciones para los dispositivos de entrada y salida. No se espera nada más de él, por lo que no hace otra cosa que verificar la función de los botones para determinar si alguno ha sido pulsado.  

### 2) Salida del estado IDLE
Para salir del estado IDLE, se deben cumplir dos condiciones. Esto es para que, una vez que salga de ese estado, nunca vuelva a consultar el inicio del código.  

- La loop station está en la Pista 1.
- Se ha pulsado el botón de grabación (record).

Esto cambia inmediatamente el estado a RECORDING (Grabando). No hay otra forma de pasar de IDLE a PLAY (Reproducción), necesariamente debe pasar por la fase de grabación. Este caso se ramifica en dos situaciones distintas:

#### 2.1) La pista maestra establece una duración de bucle de MAX_SAMPLES
Para llegar a este escenario, tras pulsar el botón de grabación por primera vez, no se debe pulsar nada más, dejando que el bucle llegue a su fin por sí solo. Esto establece automáticamente el bucle a su duración máxima y actualiza su estado a PLAYING (Reproduciendo).  

#### 2.2) La pista maestra detiene la grabación antes de MAX_SAMPLES
Para llegar a este escenario, se debe pulsar el botón de grabación una segunda vez antes de que alcance la duración máxima, estableciendo la longitud del bucle en cualquier tiempo inferior a 20 segundos.  

### 3) Grabación de la segunda o tercera pista tras la pista maestra
Este escenario es idéntico en ambos casos. El único cambio es en qué pista se encuentra una vez que comienza a grabar. La loop station está reproduciendo la pista maestra en bucle (con el volumen que tenga configurado) y entonces se pulsa el botón de grabación. En cuanto llega al inicio del bucle, la loop station entra en estado de grabación y sale de él al alcanzar la longitud del bucle. Cualquier pulsación de otro botón durante esta fase de grabación es completamente ignorada.  

### 4) Overdub de cualquier pista
Si el objetivo es sobrescribir cualquiera de las pistas existentes, todo lo demás seguirá ocurriendo con normalidad, pero la pista regrabada dejará de reproducirse, ya que se está grabando de nuevo. Se debe pulsar el botón de la pista que se desea y también el botón de grabación (el orden no es relevante, siempre y cuando se haya pulsado el botón de pista correcto antes de que el bucle llegue de nuevo al inicio y comience a grabar).

### 5) PANIC! se invoca
El modo "Panic", ya sea por pulsar accidentalmente dos botones de pista al mismo tiempo o como una forma intencionada de realizar un reinicio rápido, ocurre al pulsar o mantener pulsados dos botones (o más) de pista distintos simultáneamente. Puede ocurrir en cualquier momento.  


<a name="documentation"></a>
## Documentación extra
Se recomienda consultar otra documentación que ayude con los componentes específicos utilizados en el proyecto. En este escenario, se enlazarán referencias externas para ayudar a la mejor comprensión del hardware utilizado:
- [ESP32-S3 Hardware Pinout and Details (Luis Llamas)](https://www.luisllamas.es/en/esp32-s3-hardware-details-pinout/)
- [SPARKLEIOT XH-S3E Ultra-Low Power WiFi Module User Manual (Manuals+)](https://manuals.plus/es/sparkleiot/xh-s3e-ultra-low-power-wifi-module-manual)
- [Lesson 2: Testing Your Microphone (INMP441)(OceanLabz)](https://www.oceanlabz.in/lesson-2-testing-your-microphone-inmp441/?srsltid=AfmBOoqxJcil_28kHzQjYyT23pxv0jRdq-Ltm8145vbhw94lhPC2eP9x)
