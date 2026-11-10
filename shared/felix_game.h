// Lógica común del juego para las versiones RF y Bluetooth.
// Cada sketch incluye este archivo e implementa la capa de entrada declarada abajo.
#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>

// ================= CAPA DE ENTRADA (la implementa cada sketch) =================
struct Inputs {
  bool up, down, left, right, fix;         // Felix y navegación de menús
  bool ralphLeft, ralphRight, ralphThrow;  // Ralph controlado por una persona
};

void platformSetup();                 // Inicializa el control inalámbrico
void platformPoll();                  // Se llama una vez por iteración del loop
// Rellena `in`. Si ralphIsHuman es false (menús y 1 jugador) todos los controles
// disponibles manejan a Felix; si es true, cada sketch reparte Felix/Ralph.
void readInputs(Inputs &in, bool ralphIsHuman);
const char* platformBootMessage();    // Texto del OLED durante el arranque
const char* platformFixLabel();       // Nombre del botón de acción en pantalla

// ================= PINES =================
#define PIN_BUZZER 16

#define BTN_UP 25
#define BTN_DOWN 26
#define BTN_LEFT 27
#define BTN_RIGHT 14
#define BTN_FIX 12

#define LED_LIFE_1 32
#define LED_LIFE_2 33
#define LED_LIFE_3 5

// ================= CONFIGURACIÓN Y CONSTANTES =================
const int OLED_WIDTH = 128;
const int OLED_HEIGHT = 32;
const int OLED_ADDR = 0x3C;
const int SCREEN_MAX_Y = 320;

const int FELIX_SIZE = 24;
const int FELIX_OFFSET = 12;
const int RALPH_SIZE = 32;
const int RALPH_OFFSET = 16;
const int ROCK_SIZE = 12;
const int ROCK_OFFSET = 6;
const int HAMMER_SIZE = 16;
const int MAX_ROCKS = 3;

const int WIN_WIDTH = 40;
const int WIN_HEIGHT = 50;
const int WIN_OFFSET_X = 20;
const int WIN_OFFSET_Y = 30;

const int BUILD_X = 20;
const int BUILD_Y = 40;
const int BUILD_W = 200;
const int BUILD_H = 280;
const int RALPH_FIXED_Y = BUILD_Y - RALPH_SIZE;  // Ralph de pie sobre la azotea

const unsigned long TICK_GAME = 30;
const unsigned long TICK_OLED = 200;
const unsigned long TIME_DEBOUNCE = 150;        // Repetición de movimiento al mantener
const unsigned long TIME_FIX_COOLDOWN = 120;    // Mínimo entre dos reparaciones
const unsigned long TIME_MENU_DEBOUNCE = 200;
const unsigned long TIME_STAGE_SCREEN = 2600;   // Duración de la pantalla "ETAPA N"
const unsigned long TIME_GAMEOVER_LOCK = 800;   // Evita saltarse el Game Over sin querer

const unsigned long TIME_COOLDOWN_RALPH = 2000;
const unsigned long TIME_COOLDOWN_START = 1000;
const unsigned long TIME_POWERUP_HAMMER = 4000;
const unsigned long TIME_INVINCIBLE = 1500;
const unsigned long TIME_FLICKER = 100;

const unsigned long IA_DELAY_MIN = 100;
const unsigned long IA_DELAY_MAX = 301;

const float ROCK_SPEED_BASE = 4.0;
const float ROCK_SPEED_INC = 0.5;
const float ROCK_SPEED_MAX = 10.0;
const int COLLISION_TOLERANCE = 15;

const long SCORE_FIX_PARTIAL = 50;
const long SCORE_FIX_FULL = 100;
const long SCORE_STAGE_BONUS = 500;

const int MIN_BROKEN_WINDOWS = 3;  // Ninguna etapa puede empezar ya resuelta

const int PROB_BROKEN_BASE = 35;
const int PROB_BROKEN_INC = 5;
const int PROB_BROKEN_MAX = 65;

const int PROB_DBL_BROKEN_BASE = 10;
const int PROB_DBL_BROKEN_INC = 7;
const int PROB_DBL_BROKEN_MAX = 65;

const int PROB_HAMMER_BASE = 15;
const int PROB_HAMMER_INC = 5;
const int PROB_HAMMER_MAX = 60;
const int PROB_HAMMER_STAGE_1 = 20;

const int COLS = 3;
const int ROWS = 4;
const int colX[COLS] = {40, 120, 200};
const int rowY[ROWS] = {90, 160, 230, 300};

// ================= SISTEMA DE AUDIO =================
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_C5  523
#define REST     0

const int bgmNotes[] = {NOTE_C4, REST, NOTE_E4, REST, NOTE_G4, REST, NOTE_E4, REST};
const int bgmDurations[] = {150, 50, 150, 50, 150, 50, 150, 50};
const int bgmLength = 8;

const int jingleStageNotes[] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5};
const int jingleStageDurations[] = {120, 120, 120, 300};
const int jingleStageLength = 4;

int currentNote = 0;
unsigned long noteStartMillis = 0;
unsigned long sfxEndMillis = 0;
bool buzzerOn = false;

// Melodía corta que tiene prioridad sobre la música de fondo
const int *jingleNotes = nullptr;
const int *jingleDurations = nullptr;
int jingleLength = 0;
int jinglePos = 0;
unsigned long jingleNoteStart = 0;

// ================= PANTALLAS Y OBJETOS GLOBALES =================
TFT_eSPI tft = TFT_eSPI();
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
Preferences preferences;

TFT_eSprite sprFelix = TFT_eSprite(&tft);
TFT_eSprite sprRalph = TFT_eSprite(&tft);
TFT_eSprite sprRock = TFT_eSprite(&tft);
TFT_eSprite sprWinFixed = TFT_eSprite(&tft);
TFT_eSprite sprWinBroken = TFT_eSprite(&tft);
TFT_eSprite sprWinFullyBroken = TFT_eSprite(&tft);
TFT_eSprite sprHammer = TFT_eSprite(&tft);
// Búfer de una columna del edificio: se compone entera en RAM y se vuelca de una
// vez, así nada se borra y repinta a la vista (sin parpadeo).
TFT_eSprite sprColumn = TFT_eSprite(&tft);

// ================= ESTRUCTURAS Y MODOS =================
enum GameState { START, PLAYING, STAGE_CLEAR, GAMEOVER };
enum GameMode { ONE_PLAYER, TWO_PLAYERS };

GameState currentState = START;
GameMode selectedMode = ONE_PLAYER;
GameMode activeMode = ONE_PLAYER;

struct Window { int state; };  // 0 intacta, 1 rota, 2 muy rota

struct PlayerFelix {
  int col, row;
  int lives;
  long score;
};

struct PlayerRalph {
  int col, oldCol;
  unsigned long lastThrowTime;
  bool moved;
  int targetCol;
  unsigned long nextMoveTime;
  bool delayActive;
};

struct Rock {
  bool active;
  int col;
  float y;
};

// ================= VARIABLES DE ESTADO =================
Window building[COLS][ROWS];
PlayerFelix felix = {1, 3, 3, 0};
PlayerRalph ralph = {1, 1, 0, true, 1, 0, false};
Rock rocks[MAX_ROCKS];
bool colDirty[COLS];

int stage = 1;
long highScore[2] = {0, 0};  // Récord por modo, indexado por GameMode
const char* const HIGHSCORE_KEYS[2] = {"highscore", "highscore2p"};

bool isInvincible = false;
unsigned long invincibilityEndTime = 0;
unsigned long lastFlickerTime = 0;
bool flickerVisible = true;

bool hammerSpawned = false;
int hammerCol = 0, hammerRow = 0;
bool felixHasHammer = false;
unsigned long hammerEndTime = 0;

bool startScreenDrawn = false;
bool prevFix = false;

unsigned long lastGameUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long menuDebounceTime = 0;
unsigned long lastMoveTime = 0;
unsigned long lastFixTime = 0;
unsigned long lastRalphMoveTime = 0;
unsigned long stageClearStart = 0;
unsigned long gameOverStart = 0;

// ================= PROTOTIPOS =================
void updateLivesLEDs();
void updateOLED();
void drawStartScreen();
void showGameOverScreen();
void drawBuildingBase();
void renderColumn(int c);

// ================= GESTIÓN DE AUDIO =================
void buzz(int freq) {
  if (freq == REST) {
    if (buzzerOn) noTone(PIN_BUZZER);
    buzzerOn = false;
  } else {
    tone(PIN_BUZZER, freq);
    buzzerOn = true;
  }
}

void playSFX(int freq, int duration) {
  buzz(freq);
  sfxEndMillis = millis() + duration;
}

void playJingle(const int *notes, const int *durations, int length) {
  jingleNotes = notes;
  jingleDurations = durations;
  jingleLength = length;
  jinglePos = 0;
  jingleNoteStart = millis();
  sfxEndMillis = 0;
  buzz(notes[0]);
}

void handleAudio(unsigned long now) {
  if (jingleNotes != nullptr) {
    if (now - jingleNoteStart >= (unsigned long)jingleDurations[jinglePos]) {
      jinglePos++;
      if (jinglePos >= jingleLength) {
        jingleNotes = nullptr;
        buzz(REST);
      } else {
        jingleNoteStart = now;
        buzz(jingleNotes[jinglePos]);
      }
    }
    return;
  }

  if (now < sfxEndMillis) return;

  if (currentState != PLAYING) {
    buzz(REST);
    currentNote = 0;
    return;
  }

  if (now - noteStartMillis >= (unsigned long)bgmDurations[currentNote]) {
    currentNote++;
    if (currentNote >= bgmLength) currentNote = 0;
    noteStartMillis = now;
    buzz(bgmNotes[currentNote]);
  }
}

// ================= INICIALIZACIÓN DE SPRITES =================
void buildSprites() {
  sprFelix.createSprite(FELIX_SIZE, FELIX_SIZE);
  sprFelix.fillSprite(TFT_MAGENTA);
  sprFelix.fillRect(4, 8, 16, 16, TFT_BLUE);
  sprFelix.fillRect(6, 2, 12, 10, TFT_ORANGE);
  sprFelix.fillRect(2, 0, 20, 4, TFT_YELLOW);

  sprRalph.createSprite(RALPH_SIZE, RALPH_SIZE);
  sprRalph.fillSprite(TFT_MAGENTA);
  sprRalph.fillRect(0, 10, 32, 22, TFT_RED);
  sprRalph.fillRect(8, 2, 16, 12, TFT_ORANGE);
  sprRalph.fillCircle(12, 6, 2, TFT_BLACK);
  sprRalph.fillCircle(20, 6, 2, TFT_BLACK);

  sprRock.createSprite(ROCK_SIZE, ROCK_SIZE);
  sprRock.fillSprite(TFT_MAGENTA);
  sprRock.fillCircle(6, 6, 6, TFT_DARKGREY);
  sprRock.drawCircle(6, 6, 4, TFT_LIGHTGREY);

  sprWinFixed.createSprite(WIN_WIDTH, WIN_HEIGHT);
  sprWinFixed.fillSprite(TFT_CYAN);
  sprWinFixed.drawRect(0, 0, WIN_WIDTH, WIN_HEIGHT, TFT_NAVY);
  sprWinFixed.fillRect(18, 0, 4, WIN_HEIGHT, TFT_NAVY);

  sprWinBroken.createSprite(WIN_WIDTH, WIN_HEIGHT);
  sprWinBroken.fillSprite(TFT_DARKCYAN);
  sprWinBroken.drawRect(0, 0, WIN_WIDTH, WIN_HEIGHT, TFT_NAVY);
  sprWinBroken.drawLine(0, 10, 20, 30, TFT_BLACK);
  sprWinBroken.drawLine(20, 30, 40, 20, TFT_BLACK);
  sprWinBroken.drawLine(20, 30, 15, 50, TFT_BLACK);

  sprWinFullyBroken.createSprite(WIN_WIDTH, WIN_HEIGHT);
  sprWinFullyBroken.fillSprite(TFT_DARKCYAN);
  sprWinFullyBroken.drawRect(0, 0, WIN_WIDTH, WIN_HEIGHT, TFT_NAVY);
  sprWinFullyBroken.drawLine(0, 10, 20, 30, TFT_BLACK);
  sprWinFullyBroken.drawLine(20, 30, 40, 20, TFT_BLACK);
  sprWinFullyBroken.drawLine(20, 30, 15, 50, TFT_BLACK);
  sprWinFullyBroken.drawLine(5, 5, 25, 15, TFT_BLACK);
  sprWinFullyBroken.drawLine(35, 45, 20, 35, TFT_BLACK);
  sprWinFullyBroken.drawLine(10, 45, 30, 10, TFT_BLACK);
  sprWinFullyBroken.fillRect(18, 23, 5, 5, TFT_BLACK);

  sprHammer.createSprite(HAMMER_SIZE, HAMMER_SIZE);
  sprHammer.fillSprite(TFT_MAGENTA);
  sprHammer.fillRect(2, 4, 12, 5, TFT_YELLOW);
  sprHammer.fillRect(7, 9, 2, 7, TFT_WHITE);

  if (sprColumn.createSprite(WIN_WIDTH, BUILD_H) == nullptr) {
    Serial.println("Sin memoria para el bufer de columna");
  }
}

// ================= PUNTUACIÓN =================
void loadHighScores() {
  preferences.begin("felix_game", true);
  for (int m = 0; m < 2; m++) highScore[m] = preferences.getLong(HIGHSCORE_KEYS[m], 0);
  preferences.end();
}

void saveHighScoreIfBeaten() {
  if (felix.score <= highScore[activeMode]) return;
  highScore[activeMode] = felix.score;
  preferences.begin("felix_game", false);
  preferences.putLong(HIGHSCORE_KEYS[activeMode], felix.score);
  preferences.end();
}

// ================= LÓGICA DEL JUEGO =================
void markDirty(int c) { colDirty[c] = true; }

void giveHammer() {
  hammerSpawned = false;
  felixHasHammer = true;
  hammerEndTime = millis() + TIME_POWERUP_HAMMER;
}

void resetActors() {
  felix.col = 1; felix.row = 3;
  ralph.col = 1; ralph.oldCol = 1; ralph.moved = true;
  ralph.delayActive = false;

  // El primer lanzamiento llega tras TIME_COOLDOWN_START en vez del cooldown completo
  unsigned long initialOffset = TIME_COOLDOWN_RALPH - TIME_COOLDOWN_START;
  ralph.lastThrowTime = (millis() >= initialOffset) ? millis() - initialOffset : 0;

  for (int i = 0; i < MAX_ROCKS; i++) rocks[i].active = false;

  isInvincible = false;
  flickerVisible = true;
  felixHasHammer = false;
  hammerEndTime = 0;
}

// Genera ventanas rotas y martillo para la etapa actual
void generateStage() {
  int brokenChance = PROB_BROKEN_BASE;
  int doubleBrokenChance = PROB_DBL_BROKEN_BASE;
  int hammerChance = PROB_HAMMER_STAGE_1;
  if (stage > 1) {
    brokenChance = min(PROB_BROKEN_BASE + stage * PROB_BROKEN_INC, PROB_BROKEN_MAX);
    doubleBrokenChance = min(PROB_DBL_BROKEN_BASE + stage * PROB_DBL_BROKEN_INC, PROB_DBL_BROKEN_MAX);
    hammerChance = min(PROB_HAMMER_BASE + stage * PROB_HAMMER_INC, PROB_HAMMER_MAX);
  }

  int broken = 0;
  for (int c = 0; c < COLS; c++) {
    for (int r = 0; r < ROWS; r++) {
      if (random(100) < brokenChance) {
        building[c][r].state = (random(100) < doubleBrokenChance) ? 2 : 1;
        broken++;
      } else {
        building[c][r].state = 0;
      }
    }
  }
  while (broken < MIN_BROKEN_WINDOWS) {
    Window &w = building[random(COLS)][random(ROWS)];
    if (w.state == 0) { w.state = 1; broken++; }
  }

  hammerSpawned = random(100) < hammerChance;
  if (hammerSpawned) {
    hammerCol = random(COLS);
    hammerRow = random(ROWS);
    if (felix.col == hammerCol && felix.row == hammerRow) giveHammer();
  }
}

void initGame() {
  stage = 1;
  felix.lives = 3;
  felix.score = 0;
  resetActors();
  generateStage();
  updateLivesLEDs();
}

void enterPlaying() {
  currentState = PLAYING;
  tft.fillScreen(TFT_BLACK);
  drawBuildingBase();
  updateOLED();
  noteStartMillis = millis();
  lastGameUpdate = millis();
}

bool allWindowsFixed() {
  for (int c = 0; c < COLS; c++)
    for (int r = 0; r < ROWS; r++)
      if (building[c][r].state > 0) return false;
  return true;
}

void beginStageClear() {
  stage++;
  felix.score += SCORE_STAGE_BONUS;
  saveHighScoreIfBeaten();

  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(40, 110);
  tft.print("ETAPA ");
  tft.print(stage);

  tft.setTextSize(2);
  tft.setCursor(35, 180);
  tft.print("+"); tft.print(SCORE_STAGE_BONUS); tft.print(" PTS BONO");

  updateOLED();
  playJingle(jingleStageNotes, jingleStageDurations, jingleStageLength);
  stageClearStart = millis();
  currentState = STAGE_CLEAR;
}

void beginGameOver() {
  saveHighScoreIfBeaten();
  playSFX(150, 1000);
  showGameOverScreen();
  updateOLED();
  gameOverStart = millis();
  currentState = GAMEOVER;
}

void handleFelixInput(const Inputs &in, bool fixPressed, unsigned long now) {
  // Reparar: una reparación por pulsación (flanco), no por mantener el botón
  if (fixPressed && now - lastFixTime >= TIME_FIX_COOLDOWN) {
    Window &w = building[felix.col][felix.row];
    if (w.state > 0) {
      w.state--;
      lastFixTime = now;
      felix.score += (w.state == 0) ? SCORE_FIX_FULL : SCORE_FIX_PARTIAL;
      markDirty(felix.col);
      playSFX(NOTE_G4, 150);
      if (allWindowsFixed()) beginStageClear();
      return;
    }
  }

  if (now - lastMoveTime < TIME_DEBOUNCE) return;

  int newCol = felix.col;
  int newRow = felix.row;
  if (in.left) { if (newCol > 0) newCol--; }
  else if (in.right) { if (newCol < COLS - 1) newCol++; }
  else if (in.up) { if (newRow > 0) newRow--; }
  else if (in.down) { if (newRow < ROWS - 1) newRow++; }

  if (newCol == felix.col && newRow == felix.row) return;

  markDirty(felix.col);
  felix.col = newCol;
  felix.row = newRow;
  markDirty(felix.col);
  lastMoveTime = now;

  if (hammerSpawned && felix.col == hammerCol && felix.row == hammerRow) {
    giveHammer();
    playSFX(NOTE_C5, 400);
  }
}

void spawnRock() {
  for (int i = 0; i < MAX_ROCKS; i++) {
    if (!rocks[i].active) {
      rocks[i].active = true;
      rocks[i].col = ralph.col;
      rocks[i].y = BUILD_Y;
      markDirty(ralph.col);
      break;
    }
  }
}

void moveRalph(int newCol) {
  ralph.oldCol = ralph.col;
  ralph.col = newCol;
  ralph.moved = true;
}

void handleRalph(const Inputs &in, unsigned long now) {
  bool cooldownReady = now >= ralph.lastThrowTime && (now - ralph.lastThrowTime >= TIME_COOLDOWN_RALPH);

  if (activeMode == TWO_PLAYERS) {
    if (now - lastRalphMoveTime > TIME_DEBOUNCE) {
      if (in.ralphLeft && ralph.col > 0) {
        moveRalph(ralph.col - 1);
        lastRalphMoveTime = now;
      } else if (in.ralphRight && ralph.col < COLS - 1) {
        moveRalph(ralph.col + 1);
        lastRalphMoveTime = now;
      }
    }
    if (in.ralphThrow && cooldownReady) {
      spawnRock();
      ralph.lastThrowTime = now;
    }
    return;
  }

  // IA de 1 jugador: persigue la columna de Felix con un retraso de reacción
  if (felix.col != ralph.col) {
    if (!ralph.delayActive || ralph.targetCol != felix.col) {
      ralph.targetCol = felix.col;
      ralph.nextMoveTime = now + random(IA_DELAY_MIN, IA_DELAY_MAX);
      ralph.delayActive = true;
    }
  }

  if (ralph.delayActive && now >= ralph.nextMoveTime) {
    if (ralph.targetCol != ralph.col) moveRalph(ralph.targetCol);
    ralph.delayActive = false;
  }

  if (cooldownReady) {
    spawnRock();
    ralph.lastThrowTime = now;
  }
}

void updateRocks() {
  float currentSpeed = min(ROCK_SPEED_BASE + stage * ROCK_SPEED_INC, ROCK_SPEED_MAX);

  for (int i = 0; i < MAX_ROCKS; i++) {
    if (!rocks[i].active) continue;
    rocks[i].y += currentSpeed;
    if (rocks[i].y > SCREEN_MAX_Y + ROCK_OFFSET) rocks[i].active = false;
    markDirty(rocks[i].col);
  }
}

void checkCollisions() {
  if (isInvincible || felixHasHammer) return;

  for (int i = 0; i < MAX_ROCKS; i++) {
    if (!rocks[i].active) continue;
    if (rocks[i].col != felix.col || fabsf(rocks[i].y - rowY[felix.row]) >= COLLISION_TOLERANCE) continue;

    rocks[i].active = false;
    markDirty(rocks[i].col);
    felix.lives--;
    updateLivesLEDs();

    if (felix.lives <= 0) {
      beginGameOver();
    } else {
      playSFX(200, 300);
      isInvincible = true;
      invincibilityEndTime = millis() + TIME_INVINCIBLE;
      lastFlickerTime = millis();
      flickerVisible = false;
    }
    return;
  }
}

void updateTimedEffects(unsigned long now) {
  if (felixHasHammer && now >= hammerEndTime) {
    felixHasHammer = false;
    markDirty(felix.col);
  }

  if (!isInvincible) return;
  if (now >= invincibilityEndTime) {
    isInvincible = false;
    flickerVisible = true;
    markDirty(felix.col);
  } else if (now - lastFlickerTime > TIME_FLICKER) {
    flickerVisible = !flickerVisible;
    lastFlickerTime = now;
    markDirty(felix.col);
  }
}

void updateLivesLEDs() {
  digitalWrite(LED_LIFE_1, felix.lives >= 1 ? HIGH : LOW);
  digitalWrite(LED_LIFE_2, felix.lives >= 2 ? HIGH : LOW);
  digitalWrite(LED_LIFE_3, felix.lives >= 3 ? HIGH : LOW);
}

// ================= RENDERIZADO TFT =================
// Compone una columna completa (pared, ventanas, martillo, Felix, rocas) en el
// búfer y la vuelca a la pantalla. Coordenadas locales: origen en la esquina
// superior izquierda de la columna.
void renderColumn(int c) {
  sprColumn.fillSprite(TFT_BROWN);

  for (int r = 0; r < ROWS; r++) {
    int y = rowY[r] - WIN_OFFSET_Y - BUILD_Y;
    TFT_eSprite &win = (building[c][r].state == 2) ? sprWinFullyBroken
                     : (building[c][r].state == 1) ? sprWinBroken
                     : sprWinFixed;
    win.pushToSprite(&sprColumn, 0, y);

    if (hammerSpawned && hammerCol == c && hammerRow == r) {
      sprHammer.pushToSprite(&sprColumn, 12, y + 17, TFT_MAGENTA);
    }
  }

  if (felix.col == c && flickerVisible) {
    int fy = rowY[felix.row] - BUILD_Y;
    sprFelix.pushToSprite(&sprColumn, WIN_OFFSET_X - FELIX_OFFSET, fy - FELIX_OFFSET, TFT_MAGENTA);
    if (felixHasHammer) sprColumn.fillRect(WIN_OFFSET_X - 4, fy - 22, 8, 4, TFT_YELLOW);
  }

  for (int i = 0; i < MAX_ROCKS; i++) {
    if (rocks[i].active && rocks[i].col == c) {
      sprRock.pushToSprite(&sprColumn, WIN_OFFSET_X - ROCK_OFFSET, (int)rocks[i].y - ROCK_OFFSET - BUILD_Y, TFT_MAGENTA);
    }
  }

  sprColumn.pushSprite(colX[c] - WIN_OFFSET_X, BUILD_Y);
  colDirty[c] = false;
}

void drawBuildingBase() {
  tft.fillRect(BUILD_X, BUILD_Y, BUILD_W, BUILD_H, TFT_BROWN);
  for (int c = 0; c < COLS; c++) renderColumn(c);
  ralph.moved = true;
}

void drawGame() {
  if (ralph.moved) {
    tft.fillRect(colX[ralph.oldCol] - RALPH_OFFSET, RALPH_FIXED_Y, RALPH_SIZE, RALPH_SIZE, TFT_BLACK);
    sprRalph.pushSprite(colX[ralph.col] - RALPH_OFFSET, RALPH_FIXED_Y, TFT_MAGENTA);
    ralph.moved = false;
  }

  for (int c = 0; c < COLS; c++) {
    if (colDirty[c]) renderColumn(c);
  }
}

void drawStartScreen() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 40);
  tft.print("FIX IT");
  tft.setCursor(20, 80);
  tft.print("FELIX!");

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(20, 125);
  tft.print("RECORD MAXIMO: "); tft.print(highScore[selectedMode]);

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);

  tft.setCursor(30, 165);
  tft.print(selectedMode == ONE_PLAYER ? "> 1 JUGADOR" : "  1 JUGADOR");

  tft.setCursor(30, 205);
  tft.print(selectedMode == TWO_PLAYERS ? "> 2 JUGADORES" : "  2 JUGADORES");

  tft.setTextSize(1);
  tft.setCursor(25, 270);
  tft.print("Arriba/Abajo para elegir");
  tft.setCursor(25, 290);
  tft.print("Presiona "); tft.print(platformFixLabel());
}

void showGameOverScreen() {
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(40, 70);
  tft.print("GAME OVER");

  tft.setTextSize(2);
  if (activeMode == TWO_PLAYERS) {
    tft.setCursor(40, 115);
    tft.print("Gana Ralph!");
  }

  tft.setCursor(40, 150);
  tft.print("Pts: ");
  tft.print(felix.score);

  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(40, 190);
  tft.print("Max: ");
  tft.print(highScore[activeMode]);

  tft.setTextColor(TFT_WHITE);
  tft.setCursor(20, 250);
  tft.print("Presiona "); tft.print(platformFixLabel());
}

// ================= RENDERIZADO OLED =================
void updateOLED() {
  oled.clearDisplay();

  oled.setCursor(0, 0);
  oled.print("P:"); oled.print(felix.score);
  oled.setCursor(55, 0);
  oled.print("E:"); oled.print(stage);
  oled.setCursor(95, 0);
  oled.print("V:"); oled.print(felix.lives);

  unsigned long now = millis();
  oled.setCursor(0, 16);
  if (felixHasHammer && now < hammerEndTime) {
    oled.print("MARTILLO: ");
    oled.print((hammerEndTime - now) / 1000 + 1);
    oled.print("s");
  } else {
    oled.print("Ralph Cooldown:");
  }

  unsigned long timeSinceThrow = (now >= ralph.lastThrowTime) ? now - ralph.lastThrowTime : 0;
  int barWidth = OLED_WIDTH;
  if (timeSinceThrow < TIME_COOLDOWN_RALPH) {
    barWidth = map(timeSinceThrow, 0, TIME_COOLDOWN_RALPH, 0, OLED_WIDTH);
  }
  oled.drawRect(0, 26, OLED_WIDTH, 6, WHITE);
  oled.fillRect(0, 26, barWidth, 6, WHITE);

  oled.display();
}

// ================= SETUP Y LOOP =================
void gameSetup() {
  Serial.begin(115200);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_FIX, INPUT_PULLUP);

  pinMode(LED_LIFE_1, OUTPUT);
  pinMode(LED_LIFE_2, OUTPUT);
  pinMode(LED_LIFE_3, OUTPUT);
  updateLivesLEDs();

  platformSetup();
  loadHighScores();

  if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Fallo OLED");
  }
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setTextSize(1);
  oled.setCursor(10, 10);
  oled.print(platformBootMessage());
  oled.display();

  tft.init();
  tft.setRotation(0);
  buildSprites();
}

void gameLoop() {
  platformPoll();

  unsigned long now = millis();
  Inputs in = {};
  readInputs(in, currentState == PLAYING && activeMode == TWO_PLAYERS);
  bool fixPressed = in.fix && !prevFix;
  prevFix = in.fix;

  handleAudio(now);

  switch (currentState) {
    case START:
      if (!startScreenDrawn) {
        drawStartScreen();
        startScreenDrawn = true;
      }
      if (now - menuDebounceTime <= TIME_MENU_DEBOUNCE) break;

      if (in.up || in.down) {
        selectedMode = (selectedMode == ONE_PLAYER) ? TWO_PLAYERS : ONE_PLAYER;
        drawStartScreen();
        menuDebounceTime = now;
        playSFX(NOTE_E4, 50);
      } else if (fixPressed) {
        playSFX(NOTE_C5, 200);
        activeMode = selectedMode;
        initGame();
        enterPlaying();
      }
      break;

    case PLAYING:
      handleFelixInput(in, fixPressed, now);
      if (currentState != PLAYING) break;

      handleRalph(in, now);
      updateTimedEffects(now);

      if (now - lastGameUpdate >= TICK_GAME) {
        lastGameUpdate = now;
        updateRocks();
        checkCollisions();
        if (currentState != PLAYING) break;
        drawGame();
      }

      if (now - lastDisplayUpdate >= TICK_OLED) {
        updateOLED();
        lastDisplayUpdate = now;
      }
      break;

    case STAGE_CLEAR:
      if (now - stageClearStart >= TIME_STAGE_SCREEN) {
        resetActors();
        generateStage();
        enterPlaying();
      }
      break;

    case GAMEOVER:
      if (fixPressed && now - gameOverStart >= TIME_GAMEOVER_LOCK) {
        playSFX(NOTE_C5, 200);
        currentState = START;
        startScreenDrawn = false;
        menuDebounceTime = now;
      }
      break;
  }
}
