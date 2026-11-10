# Fix-it Felix! - ESP32

Recreación del juego clásico de arcade **Fix-it Felix, Jr.** para **ESP32**, con pantalla TFT a color, display OLED, efectos de sonido, LEDs y dos versiones de control inalámbrico para el segundo jugador (Ralph).

## Versiones del proyecto

El repositorio contiene dos versiones del mismo juego:

| Carpeta | Medio de comunicación | Librería adicional |
|---|---|---|
| `fix_it_felix_rf/` | Radiofrecuencia (433 MHz) | RCSwitch |
| `fix_it_felix_bluetooth/` | Bluetooth (gamepad) | Bluepad32 |

Ambos comparten la misma mecánica, sprites, lógica de juego y periféricos. La única diferencia es cómo se controla a cada personaje.

Toda la lógica del juego vive en un único archivo, `shared/felix_game.h`. Cada carpeta de sketch contiene solo su capa de control (`.ino`) y un **enlace simbólico** `felix_game.h` que apunta al archivo compartido, de modo que cualquier cambio en el juego aplica a las dos versiones.

> En Windows, Git solo crea enlaces simbólicos si está activado `core.symlinks` (requiere Modo desarrollador). Si `felix_game.h` aparece como un archivo de texto con una ruta dentro, copia `shared/felix_game.h` sobre él en cada carpeta.

---

## Hardware requerido

| Componente | Especificación | Cantidad |
|---|---|---|
| Microcontrolador | **NodeMCU ESP32S** | 1 |
| Pantalla TFT | **ST7789** 2", 240x320 píxeles, interfaz SPI | 1 |
| Display OLED | **SSD1306** 0.91", 128x32 píxeles, interfaz I2C (dirección 0x3C) | 1 |
| Buzzer | Pasivo (tipo piezoeléctrico, señal PWM) | 1 |
| LEDs | Color rojo, indicadores de vidas | 3 |
| Resistencias | 220 Ω para los LEDs | 3 |
| Pulsadores | NA (normalmente abiertos), con pull-up interno del ESP32 | 5 |
| Módulo RF | **Receptor RXB6 433 MHz** (solo versión RF) | 1 |
| Control RF | Control de carro por radiofrecuencia 433 MHz (solo versión RF) | 1 |
| Control BT | Gamepad Bluetooth (PS4, PS5, Xbox One S, Switch Pro, etc.) (solo versión BT) | 1 |
| Fuente de alimentación | 5V vía USB | 1 |

---

## Pinout (NodeMCU ESP32S)

| Pin NodeMCU | GPIO | Conexión |
|---|---|---|
| D23 | GPIO 23 | TFT MOSI |
| D19 | GPIO 19 | TFT MISO |
| D18 | GPIO 18 | TFT SCLK |
| D15 | GPIO 15 | TFT CS |
| D2  | GPIO 2  | TFT DC |
| D4  | GPIO 4  | TFT RST |
| D16 | GPIO 16 | Buzzer |
| D25 | GPIO 25 | Botón Arriba (Felix: mover arriba / Ralph RF: izquierda / Menú: arriba) |
| D26 | GPIO 26 | Botón Abajo (Menú: abajo) |
| D27 | GPIO 27 | Botón Izquierda (Felix: mover izquierda) |
| D14 | GPIO 14 | Botón Derecha (Felix: mover derecha / Ralph: mover derecha) |
| D12 | GPIO 12 | Botón FIX / Acción (reparar ventana, seleccionar menú, lanzar roca) |
| D32 | GPIO 32 | LED Vida 1 |
| D33 | GPIO 33 | LED Vida 2 |
| D5  | GPIO 5  | LED Vida 3 |
| D13 | GPIO 13 | **(solo RF)** Módulo RXB6 - DATA |
| D21 | GPIO 21 | OLED SDA (I2C) |
| D22 | GPIO 22 | OLED SCL (I2C) |

---

## Instalación del entorno

### 1. Arduino IDE

1. Descargar e instalar **Arduino IDE** desde https://www.arduino.cc/en/software/
2. Agregar soporte para ESP32:
   - Archivo → Preferencias → "URLs adicionales de gestor de placas"
   - Agregar: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Herramientas → Placa → Gestor de tarjetas → buscar "ESP32" → instalar **"ESP32 by Espressif Systems"**
3. Seleccionar la placa: **ESP32 Dev Module** (o **NodeMCU-32S** si aparece en la lista) en Herramientas → Placa

### 2. Librerías requeridas (ambas versiones)

Abrir el Gestor de librerías (Sketch → Incluir Librería → Gestionar librerías) e instalar:

| Librería | Versión recomendada | Instalación |
|---|---|---|
| **TFT_eSPI** | ≥ 2.5.0 | Buscar "TFT_eSPI" de Bodmer |
| **Adafruit SSD1306** | ≥ 2.5.0 | Buscar "Adafruit SSD1306" |
| **Adafruit GFX** | ≥ 1.11.0 | Buscar "Adafruit GFX" |

### 3. Configurar TFT_eSPI

La librería TFT_eSPI necesita un archivo de configuración con los pines del display:

1. Localizar la carpeta de la librería TFT_eSPI:
   - Windows: `%USERPROFILE%\Documents\Arduino\libraries\TFT_eSPI`
   - Linux: `~/Arduino/libraries/TFT_eSPI`
   - macOS: `~/Documents/Arduino/libraries/TFT_eSPI`

2. Abrir el archivo `User_Setup.h` en esa carpeta.

3. **Reemplazar** todo su contenido por el archivo `User_Setup.h` incluido en este repositorio.

   > El archivo `User_Setup.h` ya está en la raíz del proyecto con la configuración correcta. Solo hay que copiarlo a la carpeta de la librería TFT_eSPI.

   La configuración incluye:
   - Driver **ST7789** con orden de color BGR
   - Resolución **240x320**
   - Pines SPI: MISO=19, MOSI=23, SCLK=18, CS=15, DC=2, RST=4
   - Frecuencia SPI: 40 MHz
   - Carga de fuentes GLCD, Font2, Font4, Font6, Font7, Font8 y GFXFF

### 4. Librerías específicas por versión

#### Versión RF (`fix_it_felix_rf/`)
| Librería | Versión | Instalación |
|---|---|---|
| **RCSwitch** | ≥ 2.6.4 | Buscar "rc-switch" de sui77 |

#### Versión Bluetooth (`fix_it_felix_bluetooth/`)
| Librería | Versión | Instalación |
|---|---|---|
| **Bluepad32** | ≥ 3.6.0 | Requiere instalación especial (ver sección aparte) |

### 5. Instalación de Bluepad32 (para la versión Bluetooth)

Bluepad32 **no** se instala desde el gestor de librerías estándar. Requiere añadir un paquete de placas específico. Sigue estos pasos:

1. En el IDE de Arduino, ve a **Archivo → Preferencias**.
2. En "Gestor de URLs adicionales de tarjetas", añade **ambas** URLs (separadas por líneas):
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json

text
3. Abre **Herramientas → Placa → Gestor de tarjetas**.
4. Busca e instala **ambos** paquetes:
- `esp32` (de Espressif Systems)
- `ESP32 + Bluepad32` (de Ricardo Quesada)
5. Una vez instalados, para los proyectos que usen Bluepad32 selecciona una placa de la categoría:
- **Herramientas → Placa → ESP32 + Bluepad32 Arduino**

> 📖 Para más detalles, consulta la [documentación oficial de Bluepad32](https://bluepad32.readthedocs.io/).

---

## Carga del programa

1. Abrir el archivo `.ino` correspondiente en Arduino IDE:
   - Para RF: `fix_it_felix_rf/fix_it_felix_rf.ino`
   - Para Bluetooth: `fix_it_felix_bluetooth/fix_it_felix_bluetooth.ino`
2. Conectar el ESP32 por USB
3. Seleccionar el puerto COM correspondiente (Herramientas → Puerto)
4. Ajustar las siguientes opciones en Herramientas:
   - **Board**: "ESP32 Dev Module" o "NodeMCU-32S" (para RF) / "ESP32 + Bluepad32" (para Bluetooth)
   - **Flash Size**: "4MB"
   - **Partition Scheme**: "Default 4MB with spiffs" (o "No OTA (Large APP)")
   - **Upload Speed**: "115200" (o 921600 si tu cable lo soporta)
5. Presionar **Subir** (→)

---

## Controles

### Modo 1 jugador

| Control | Felix |
|---|---|
| BTN_LEFT (GPIO 27) | Mover izquierda |
| BTN_RIGHT (GPIO 14) | Mover derecha |
| BTN_UP (GPIO 25) | Mover arriba |
| BTN_DOWN (GPIO 26) | Mover abajo |
| BTN_FIX (GPIO 12) | Reparar ventana / Seleccionar |

Ralph es controlado por la **IA** del ESP32. Persigue a Felix y lanza rocas automáticamente.

### Modo 2 jugadores

#### Versión RF (`fix_it_felix_rf/`)

**Jugador 1 (Felix):** usa los pulsadores en la placa (mismos que en 1 jugador).

**Jugador 2 (Ralph):** usa un **control remoto RF de 3 botones** con los siguientes códigos:

| Botón | Código RF | Acción |
|---|---|---|
| Botón 1 | `5735105` | Mover Ralph a la izquierda |
| Botón 2 | `5735106` | Mover Ralph a la derecha |
| Botón 3 | `5735108` | Lanzar roca |

> Para cambiar los códigos RF, modificar las constantes `RF_LEFT`, `RF_RIGHT` y `RF_THROW` en el código. Se puede usar el ejemplo `ReceiveDemo_Advanced` de la librería RCSwitch para obtener los códigos de tu mando.

#### Versión Bluetooth (`fix_it_felix_bluetooth/`)

En **1 jugador**, Felix se maneja con el gamepad Bluetooth **o** con los pulsadores de la placa (mismos que en la tabla de 1 jugador).

El mando emparejado se recuerda entre reinicios. Para borrar los emparejamientos (por ejemplo, para usar otro mando), mantén pulsado **FIX** mientras enciendes o reinicias el ESP32.

**Jugador 1 (Felix):** usa un **gamepad Bluetooth** conectado al ESP32.

| Control | Acción |
|---|---|
| D-Pad / Joystick izquierdo | Mover Felix |
| Botón A / B | Reparar ventana / Seleccionar |

**Jugador 2 (Ralph):** usa los pulsadores físicos:

| Control | Acción |
|---|---|
| BTN_UP (GPIO 25) | Mover Ralph a la izquierda |
| BTN_RIGHT (GPIO 14) | Mover Ralph a la derecha |
| BTN_FIX (GPIO 12) | Lanzar roca |

---

## Funcionamiento del juego

1. **Pantalla de inicio**: muestra el título, el récord máximo y la selección de modo (1 o 2 jugadores). Usar Arriba/Abajo para cambiar y FIX/B para confirmar.
2. **Objetivo**: reparar todas las ventanas rotas del edificio antes de que Ralph te golpee con sus rocas.
3. **Ventanas**: pueden estar en 3 estados: intactas (0), parcialmente rotas (1) y totalmente rotas (2). Felix repara un nivel por cada pulsación de FIX (mantenerlo pulsado no repite). Cada reparación tarda 0,25 s, durante los que Felix no puede moverse; si recibe un golpe, la reparación se pierde. Cada etapa empieza con al menos 3 ventanas rotas.
4. **Ralph**: en modo 1 jugador tiene IA; en modo 2 jugadores es controlado por una persona. Camina de columna en columna y, antes de cada lanzamiento, levanta los brazos durante 0,35 s: la roca cae cuando los baja. Las rocas caen verticalmente.
5. **Martillo**: power-up que aparece aleatoriamente. Al recogerlo, Felix es invencible por 4 segundos y puede destruir rocas al instante.
6. **Invencibilidad**: al recibir un golpe, Felix parpadea durante 1.5 segundos sin poder ser dañado.
7. **Vidas**: 3 vidas indicadas por LEDs. Al perder todas, Game Over.
8. **Etapas**: al reparar todas las ventanas, se avanza de etapa. Cada etapa aumenta la velocidad de las rocas y la probabilidad de ventanas rotas, y acorta el tiempo entre lanzamientos de Ralph (de 2 s hasta un mínimo de 0,9 s).
9. **Puntuación**: 50 pts por reparación parcial, 100 pts por reparación completa, 500 pts de bono por etapa. El récord se guarda en la memoria no volátil (Preferences), por separado para 1 y 2 jugadores, al superar cada etapa y al terminar la partida.

---

## Información adicional en el OLED

El display OLED de 128x32 muestra en tiempo real:
- **P:** Puntuación actual
- **E:** Etapa actual
- **V:** Vidas restantes
- Barra de progreso del cooldown de Ralph (cuándo podrá lanzar la próxima roca)
- Temporizador del martillo (si está activo)

---

## Diagrama de conexiones (esquemático básico)

```
NodeMCU ESP32S                  TFT ST7789 (2")
─────────────                   ──────────────
D23 (GPIO 23) ───────────────── MOSI
D19 (GPIO 19) ───────────────── MISO
D18 (GPIO 18) ───────────────── SCLK
D15 (GPIO 15) ───────────────── CS
D2  (GPIO 2)  ───────────────── DC
D4  (GPIO 4)  ───────────────── RST
3.3V         ───────────────── VCC, LED
GND          ───────────────── GND

NodeMCU ESP32S                  OLED SSD1306 (0.91")
─────────────                   ───────────────────
D21 (GPIO 21) ───────────────── SDA
D22 (GPIO 22) ───────────────── SCL
3.3V         ───────────────── VCC
GND          ───────────────── GND

NodeMCU ESP32S                  Buzzer Pasivo
─────────────                   ─────────────
D16 (GPIO 16) ───────────────── (+) (ánodo)
GND          ───────────────── (-) (cátodo)

NodeMCU ESP32S                  LEDs Rojos (x3)
─────────────                   ───────────────
D32 (GPIO 32) ──┤ 220Ω ├──────── LED1 (+) (rojo)
D33 (GPIO 33) ──┤ 220Ω ├──────── LED2 (+) (rojo)
D5  (GPIO 5)  ──┤ 220Ω ├──────── LED3 (+) (rojo)
GND           ────────────────── (-) de todos los LEDs

NodeMCU ESP32S                  Pulsadores (x5)
─────────────                   ────────────────
D25 (GPIO 25) ──┤ botón ├────── GND  (BTN_UP)
D26 (GPIO 26) ──┤ botón ├────── GND  (BTN_DOWN)
D27 (GPIO 27) ──┤ botón ├────── GND  (BTN_LEFT)
D14 (GPIO 14) ──┤ botón ├────── GND  (BTN_RIGHT)
D12 (GPIO 12) ──┤ botón ├────── GND  (BTN_FIX)

NodeMCU ESP32S (solo RF)       Módulo Receptor RXB6 433 MHz
─────────────                   ────────────────────────────
D13 (GPIO 13) ───────────────── DATA
3.3V         ───────────────── VCC
GND          ───────────────── GND
```

---

## Solución de problemas

| Problema | Posible causa | Solución |
|---|---|---|
| Pantalla TFT en blanco | `User_Setup.h` incorrecto | Verificar que los pines coinciden con el archivo `User_Setup.h` del repositorio |
| OLED no muestra nada | Dirección I2C incorrecta | Ejecutar scanner I2C para verificar; si es `0x3D`, cambiar `OLED_ADDR` |
| No compila Bluepad32 | Toolchain no instalado correctamente | Seguir la guía de instalación de Bluepad32 (sección 5). Asegurarse de haber instalado el paquete "ESP32 + Bluepad32" y seleccionado la placa correspondiente |
| No recibe RF | Códigos del mando distintos | Usar el ejemplo `ReceiveDemo_Advanced` de RCSwitch para obtener los códigos reales y actualizar `RF_LEFT`, `RF_RIGHT`, `RF_THROW`. Verificar también el level shifter |
| Gamepad BT no conecta | Emparejamiento antiguo o ESP32 en modo conflicto | Reiniciar el ESP32 manteniendo FIX pulsado para borrar los emparejamientos y volver a emparejar el gamepad. Asegurarse de haber seleccionado la placa "ESP32 + Bluepad32" |
| Al subir: `Failed to communicate with the flash chip` | El circuito alimentado desde el pin de 5V interfiere en modo de carga | Desconectar el cable de 5V que va del ESP32 al circuito, subir el programa y volver a conectarlo |
| El ESP32 se reinicia al conectar RXB6 | Voltaje de 5V en pin GPIO | Revisar urgentemente el nivel lógico: usar level shifter obligatorio |

