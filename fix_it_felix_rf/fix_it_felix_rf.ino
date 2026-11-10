// Fix-it Felix! - versión RF (433 MHz)
// Felix: pulsadores de la placa. Ralph (2 jugadores): mando RF de 3 botones.
// La lógica del juego está en felix_game.h (enlace a ../shared/felix_game.h), común con la versión Bluetooth.
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>
#include <RCSwitch.h>

#include "felix_game.h"

#define PIN_RF 13

// Códigos del mando RF (obtenerlos con el ejemplo ReceiveDemo_Advanced de RCSwitch)
const unsigned long RF_LEFT = 5735105;
const unsigned long RF_RIGHT = 5735106;
const unsigned long RF_THROW = 5735108;

RCSwitch rfSwitch = RCSwitch();

// ================= CAPA DE ENTRADA =================
void platformSetup() {
  rfSwitch.enableReceive(digitalPinToInterrupt(PIN_RF));
  rfSwitch.setReceiveTolerance(60);
}

void platformPoll() {}

const char* platformBootMessage() { return "INICIANDO..."; }
const char* platformFixLabel() { return "FIX"; }
void platformRumble(int durationMs) {}  // El mando RF no vibra

void readInputs(Inputs &in, bool ralphIsHuman) {
  in.up = digitalRead(BTN_UP) == LOW;
  in.down = digitalRead(BTN_DOWN) == LOW;
  in.left = digitalRead(BTN_LEFT) == LOW;
  in.right = digitalRead(BTN_RIGHT) == LOW;
  in.fix = digitalRead(BTN_FIX) == LOW;

  // Los códigos recibidos fuera del modo 2 jugadores se descartan
  if (rfSwitch.available()) {
    unsigned long value = rfSwitch.getReceivedValue();
    rfSwitch.resetAvailable();
    if (ralphIsHuman) {
      in.ralphLeft = (value == RF_LEFT);
      in.ralphRight = (value == RF_RIGHT);
      in.ralphThrow = (value == RF_THROW);
    }
  }
}

void setup() { gameSetup(); }
void loop() { gameLoop(); }
