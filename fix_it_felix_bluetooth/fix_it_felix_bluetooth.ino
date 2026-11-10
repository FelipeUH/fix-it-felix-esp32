// Fix-it Felix! - versión Bluetooth (Bluepad32)
// 1 jugador: Felix con gamepad o con los pulsadores de la placa.
// 2 jugadores: Felix con gamepad, Ralph con los pulsadores.
// Pausa: Start en el gamepad; en la placa, Izquierda + Derecha (1 jugador) o Abajo (2 jugadores).
// La lógica del juego está en felix_game.h (enlace a ../shared/felix_game.h), común con la versión RF.
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>
#include <Bluepad32.h>

#include "felix_game.h"

const int STICK_THRESHOLD = 200;

// Entradas mapeadas del gamepad Bluetooth
bool padUp = false;
bool padDown = false;
bool padLeft = false;
bool padRight = false;
bool padA = false;
bool padStart = false;

// ================= GESTIÓN DE CONTROLES BLUEPAD32 =================
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("Control conectado, index=%d\n", i);
      myControllers[i] = ctl;
      return;
    }
  }
  Serial.println("Control conectado, pero no hay espacio libre.");
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("Control desconectado de index=%d\n", i);
      myControllers[i] = nullptr;
      return;
    }
  }
  Serial.println("Control desconectado, pero no encontrado en memoria.");
}

void readGamepadInputs() {
  padUp = padDown = padLeft = padRight = padA = padStart = false;
  for (auto ctl : myControllers) {
    if (ctl && ctl->isConnected() && ctl->isGamepad()) {
      padUp = (ctl->dpad() & DPAD_UP) || (ctl->axisY() < -STICK_THRESHOLD);
      padDown = (ctl->dpad() & DPAD_DOWN) || (ctl->axisY() > STICK_THRESHOLD);
      padLeft = (ctl->dpad() & DPAD_LEFT) || (ctl->axisX() < -STICK_THRESHOLD);
      padRight = (ctl->dpad() & DPAD_RIGHT) || (ctl->axisX() > STICK_THRESHOLD);
      padA = ctl->a();
      padStart = ctl->miscStart();
      break;
    }
  }
}

// ================= CAPA DE ENTRADA =================
void platformSetup() {
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // Los mandos emparejados se recuerdan entre reinicios. Para olvidarlos
  // (emparejar otro mando), mantener FIX pulsado al encender.
  if (digitalRead(BTN_FIX) == LOW) {
    BP32.forgetBluetoothKeys();
    Serial.println("Emparejamientos Bluetooth borrados.");
  }
  BP32.enableVirtualDevice(false);
  Serial.println("Bluetooth iniciado. Esperando sincronización...");
}

void platformPoll() {
  BP32.update();
  readGamepadInputs();
}

const char* platformBootMessage() { return "ESPERANDO BT..."; }
const char* platformFixLabel() { return "FIX o B"; }

void platformRumble(int durationMs) {
  for (auto ctl : myControllers) {
    if (ctl && ctl->isConnected() && ctl->isGamepad()) {
      ctl->playDualRumble(0 /* inicio */, durationMs, 0x80 /* motor débil */, 0xFF /* motor fuerte */);
    }
  }
}

void readInputs(Inputs &in, bool ralphIsHuman) {
  bool btnUp = digitalRead(BTN_UP) == LOW;
  bool btnDown = digitalRead(BTN_DOWN) == LOW;
  bool btnLeft = digitalRead(BTN_LEFT) == LOW;
  bool btnRight = digitalRead(BTN_RIGHT) == LOW;
  bool btnFix = digitalRead(BTN_FIX) == LOW;

  in.up = padUp;
  in.down = padDown;
  in.left = padLeft;
  in.right = padRight;
  in.fix = padA;
  in.pause = padStart;

  if (ralphIsHuman) {
    // 2 jugadores: los pulsadores son de Ralph
    in.ralphLeft = btnUp;
    in.ralphRight = btnRight;
    in.ralphThrow = btnFix;
    in.pause |= btnDown;  // Abajo no tiene otro uso para Ralph
  } else {
    // Menús y 1 jugador: los pulsadores también manejan a Felix
    in.up |= btnUp;
    in.down |= btnDown;
    in.left |= btnLeft;
    in.right |= btnRight;
    in.fix |= btnFix;
    in.pause |= btnLeft && btnRight;  // Izquierda + Derecha a la vez
  }
}

void setup() { gameSetup(); }
void loop() { gameLoop(); }
