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
const int MAX_ROCKS = 5;

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
const unsigned long TIME_MENU_DEBOUNCE = 200;
const unsigned long TIME_STAGE_SCREEN = 2600;   // Duración de la pantalla "ETAPA N"
const unsigned long TIME_GAMEOVER_LOCK = 800;   // Evita saltarse el Game Over sin querer

const unsigned long TIME_COOLDOWN_RALPH = 2000;       // Entre lanzamientos en la etapa 1
const unsigned long TIME_COOLDOWN_RALPH_STEP = 150;   // Se acorta esto en cada etapa
const unsigned long TIME_COOLDOWN_RALPH_MIN = 900;
const unsigned long TIME_COOLDOWN_START = 1000;
const unsigned long TIME_POWERUP_HAMMER = 4000;
const unsigned long TIME_INVINCIBLE = 1500;
const unsigned long TIME_FLICKER = 100;

const unsigned long TIME_HOP = 120;          // Duración del salto de Felix entre ventanas
const int HOP_HEIGHT = 6;                    // Altura del arco del salto (px)
const unsigned long TIME_REPAIR = 250;       // Lo que tarda una reparación; Felix no puede moverse
const unsigned long TIME_RALPH_WINDUP = 350; // Ralph levanta los brazos antes de soltar la roca

// Área que se recompone alrededor de Felix (cabe su posición anterior y la nueva)
const int ACTOR_BUF_W = 48;
const int ACTOR_BUF_H = 64;
const int FELIX_RECT_TOP = 24;  // Por encima del centro: sprite + indicador de martillo

// Paleta (RGB565)
#define RGB565(r, g, b) ((uint16_t)((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)))
const uint16_t COLOR_SKY = RGB565(10, 14, 40);
const uint16_t COLOR_BRICK = RGB565(152, 72, 52);
const uint16_t COLOR_MORTAR = RGB565(96, 44, 32);
const uint16_t COLOR_FRAME = RGB565(226, 220, 200);
const uint16_t COLOR_SILL = RGB565(170, 164, 148);
const uint16_t COLOR_GLASS = RGB565(110, 196, 236);
const uint16_t COLOR_GLASS_SHINE = RGB565(214, 240, 252);
const uint16_t COLOR_ROOM = RGB565(22, 22, 44);
const uint16_t COLOR_SKIN = RGB565(248, 190, 140);
const uint16_t COLOR_HAIR = RGB565(110, 60, 26);
const uint16_t COLOR_DENIM = RGB565(40, 90, 210);
const uint16_t COLOR_DENIM_DARK = RGB565(24, 52, 130);
const uint16_t COLOR_SHIRT = RGB565(206, 44, 36);
const uint16_t COLOR_BOOT = RGB565(92, 56, 30);
const uint16_t COLOR_STEEL = RGB565(176, 180, 190);
const int BRICK_W = 16;  // Divide la separación entre columnas: mismo dibujo en las tres
const int BRICK_H = 8;

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
TFT_eSprite sprFelixFix = TFT_eSprite(&tft);
TFT_eSprite sprRalph = TFT_eSprite(&tft);
TFT_eSprite sprRalphThrow = TFT_eSprite(&tft);
TFT_eSprite sprRock = TFT_eSprite(&tft);
TFT_eSprite sprWinFixed = TFT_eSprite(&tft);
TFT_eSprite sprWinBroken = TFT_eSprite(&tft);
TFT_eSprite sprWinFullyBroken = TFT_eSprite(&tft);
TFT_eSprite sprHammer = TFT_eSprite(&tft);
// Búferes de composición: cada zona se dibuja entera en RAM y se vuelca de una
// vez, así nada se borra y repinta a la vista (sin parpadeo).
TFT_eSprite sprColumn = TFT_eSprite(&tft);  // Una columna del edificio (rocas)
TFT_eSprite sprActor = TFT_eSprite(&tft);   // Entorno de Felix (saltos entre ventanas)

// ================= ESTRUCTURAS Y MODOS =================
enum GameState { START, PLAYING, STAGE_CLEAR, GAMEOVER };
enum GameMode { ONE_PLAYER, TWO_PLAYERS };

GameState currentState = START;
GameMode selectedMode = ONE_PLAYER;
GameMode activeMode = ONE_PLAYER;

struct Window { int state; };  // 0 intacta, 1 rota, 2 muy rota

struct PlayerFelix {
  int col, row;   // Celda lógica: cambia al instante y es la que cuenta para colisiones
  int lives;
  long score;
  int px, py;     // Centro en pantalla; va con retraso respecto a la celda durante el salto
  int fromX, fromY;
  unsigned long hopStart;
  bool hopping;
};

struct PlayerRalph {
  int col, oldCol;
  unsigned long lastThrowTime;
  bool moved;
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
PlayerFelix felix = {1, 3, 3, 0, 0, 0, 0, 0, 0, false};
PlayerRalph ralph = {1, 1, 0, true, 0, false};
Rock rocks[MAX_ROCKS];
bool colDirty[COLS];
bool felixDirty = false;
int felixDrawnX = 0, felixDrawnY = 0;  // Dónde está pintado Felix ahora mismo

bool felixFixing = false;
unsigned long felixFixUntil = 0;
bool ralphThrowing = false;
unsigned long ralphThrowUntil = 0;

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
void updateInvincibility(unsigned long now);
void drawBricks(TFT_eSPI &g, int x0, int y0, int w, int h);

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
// Los personajes se definen como mapas de caracteres (un carácter = un píxel
// del dibujo) y se escalan al crear el sprite. '.' es el fondo.
uint16_t artColor(char ch, uint16_t background) {
  switch (ch) {
    case 'y': return TFT_YELLOW;
    case 's': return COLOR_SKIN;
    case 'k': return TFT_BLACK;
    case 'w': return TFT_WHITE;
    case 'b': return COLOR_DENIM;
    case 'B': return COLOR_DENIM_DARK;
    case 'r': return COLOR_SHIRT;
    case 'h': return COLOR_HAIR;
    case 'n': return COLOR_BOOT;
    case 'g': return COLOR_STEEL;
    default:  return background;
  }
}

void buildArtSprite(TFT_eSprite &spr, const char *const *rows, int size, int scale, uint16_t background) {
  spr.createSprite(size * scale, size * scale);
  spr.fillSprite(background);
  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      uint16_t color = artColor(rows[y][x], background);
      if (color != background) spr.fillRect(x * scale, y * scale, scale, scale, color);
    }
  }
}

const char *const ART_FELIX[12] = {
  "....yyyy....",
  "...yyyyyy...",
  "..yyyyyyyyy.",
  "...sksssk...",
  "...sssssss..",
  "....sssss...",
  "..bbbbbbbb..",
  ".sbbwbbwbbs.",
  ".sbbbbbbbbs.",
  "..bbbbbbbb..",
  "..BBB..BBB..",
  ".nnnn..nnnn.",
};

const char *const ART_FELIX_FIX[12] = {
  "....yyyy..gg",
  "...yyyyyy.gg",
  "..yyyyyyyyn.",
  "...sksssk.n.",
  "...sssssssn.",
  "....sssss.s.",
  "..bbbbbbbbb.",
  ".sbbwbbwbb..",
  ".sbbbbbbbb..",
  "..bbbbbbbb..",
  "..BBB..BBB..",
  ".nnnn..nnnn.",
};

const char *const ART_RALPH[16] = {
  "....hhhhhhhh....",
  "...hhhhhhhhhh...",
  "...hsssssssshh..",
  "...sskssssksss..",
  "...sssssssssss..",
  "....sskkkkkss...",
  ".....sssssss....",
  "..rrrrrrrrrrrr..",
  ".rrrrrrrrrrrrrr.",
  "sssrrrrrrrrrrsss",
  "sssrrrrrrrrrrsss",
  "sss.rrrrrrrr.sss",
  "....rrrrrrrr....",
  "....nnnnnnnn....",
  "....nnn..nnn....",
  "...kkkk..kkkk...",
};

const char *const ART_RALPH_THROW[16] = {
  "sss.hhhhhhhh.sss",
  "ssshhhhhhhhhhsss",
  ".rrhsssssssshrr.",
  ".rrsskssssksssr.",
  ".rrsssssssssssr.",
  ".rrrsskkkkkssrr.",
  "..rrrsssssssrr..",
  "..rrrrrrrrrrrr..",
  "...rrrrrrrrrr...",
  "....rrrrrrrr....",
  "....rrrrrrrr....",
  "....rrrrrrrr....",
  "....rrrrrrrr....",
  "....nnnnnnnn....",
  "....nnn..nnn....",
  "...kkkk..kkkk...",
};

// Marco, cristal, travesaños y alféizar comunes a los tres estados de ventana
void drawWindowBase(TFT_eSprite &spr, uint16_t glass) {
  spr.createSprite(WIN_WIDTH, WIN_HEIGHT);
  spr.fillSprite(COLOR_FRAME);
  spr.fillRect(3, 3, WIN_WIDTH - 6, 40, glass);
  spr.fillRect(0, 45, WIN_WIDTH, 5, COLOR_SILL);
  spr.drawFastHLine(0, 44, WIN_WIDTH, COLOR_MORTAR);
}

void drawWindowMullions(TFT_eSprite &spr) {
  spr.fillRect(19, 3, 2, 40, COLOR_FRAME);
  spr.fillRect(3, 22, WIN_WIDTH - 6, 2, COLOR_FRAME);
}

void buildSprites() {
  // Felix se dibuja sobre el edificio: fondo magenta = transparente.
  buildArtSprite(sprFelix, ART_FELIX, 12, 2, TFT_MAGENTA);
  buildArtSprite(sprFelixFix, ART_FELIX_FIX, 12, 2, TFT_MAGENTA);
  // Ralph solo se dibuja sobre el cielo: sprite opaco, se pinta sin borrar antes.
  buildArtSprite(sprRalph, ART_RALPH, 16, 2, COLOR_SKY);
  buildArtSprite(sprRalphThrow, ART_RALPH_THROW, 16, 2, COLOR_SKY);

  sprRock.createSprite(ROCK_SIZE, ROCK_SIZE);
  sprRock.fillSprite(TFT_MAGENTA);
  sprRock.fillCircle(6, 6, 5, TFT_DARKGREY);
  sprRock.fillRect(2, 3, 8, 6, TFT_DARKGREY);
  sprRock.fillRect(3, 2, 4, 2, TFT_LIGHTGREY);
  sprRock.drawLine(5, 6, 8, 9, TFT_BLACK);
  sprRock.drawPixel(4, 8, TFT_BLACK);

  // Ventana intacta: cristal con reflejos
  drawWindowBase(sprWinFixed, COLOR_GLASS);
  sprWinFixed.drawLine(6, 16, 15, 7, COLOR_GLASS_SHINE);
  sprWinFixed.drawLine(6, 20, 17, 9, COLOR_GLASS_SHINE);
  sprWinFixed.drawLine(24, 40, 33, 31, COLOR_GLASS_SHINE);
  drawWindowMullions(sprWinFixed);

  // Ventana rota: grietas y un hueco
  drawWindowBase(sprWinBroken, COLOR_GLASS);
  sprWinBroken.fillTriangle(22, 4, 36, 4, 30, 18, COLOR_ROOM);
  sprWinBroken.drawLine(30, 18, 24, 30, TFT_BLACK);
  sprWinBroken.drawLine(24, 30, 34, 40, TFT_BLACK);
  sprWinBroken.drawLine(4, 10, 16, 20, TFT_BLACK);
  sprWinBroken.drawLine(16, 20, 8, 38, TFT_BLACK);
  sprWinBroken.drawLine(16, 20, 24, 30, TFT_BLACK);
  drawWindowMullions(sprWinBroken);

  // Ventana muy rota: sin travesaños, solo quedan esquirlas en las esquinas
  drawWindowBase(sprWinFullyBroken, COLOR_ROOM);
  sprWinFullyBroken.fillTriangle(3, 3, 16, 3, 3, 18, COLOR_GLASS);
  sprWinFullyBroken.fillTriangle(36, 3, 36, 14, 26, 3, COLOR_GLASS);
  sprWinFullyBroken.fillTriangle(3, 42, 3, 30, 12, 42, COLOR_GLASS);
  sprWinFullyBroken.fillTriangle(36, 42, 36, 26, 22, 42, COLOR_GLASS);
  sprWinFullyBroken.fillRect(19, 3, 2, 9, COLOR_FRAME);
  sprWinFullyBroken.fillRect(19, 36, 2, 7, COLOR_FRAME);

  sprHammer.createSprite(HAMMER_SIZE, HAMMER_SIZE);
  sprHammer.fillSprite(TFT_MAGENTA);
  sprHammer.fillRect(1, 2, 14, 6, TFT_BLACK);
  sprHammer.fillRect(2, 3, 12, 4, TFT_YELLOW);
  sprHammer.fillRect(6, 8, 4, 8, TFT_BLACK);
  sprHammer.fillRect(7, 8, 2, 7, TFT_WHITE);

  if (sprColumn.createSprite(WIN_WIDTH, BUILD_H) == nullptr ||
      sprActor.createSprite(ACTOR_BUF_W, ACTOR_BUF_H) == nullptr) {
    Serial.println("Sin memoria para los buferes de composicion");
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

// Tiempo entre lanzamientos de Ralph: se acorta con cada etapa
unsigned long ralphCooldown() {
  unsigned long reduction = (unsigned long)(stage - 1) * TIME_COOLDOWN_RALPH_STEP;
  if (reduction >= TIME_COOLDOWN_RALPH - TIME_COOLDOWN_RALPH_MIN) return TIME_COOLDOWN_RALPH_MIN;
  return TIME_COOLDOWN_RALPH - reduction;
}

void giveHammer() {
  hammerSpawned = false;
  felixHasHammer = true;
  hammerEndTime = millis() + TIME_POWERUP_HAMMER;
}

void resetActors() {
  felix.col = 1; felix.row = 3;
  felix.px = colX[felix.col]; felix.py = rowY[felix.row];
  felix.hopping = false;
  felixDrawnX = felix.px; felixDrawnY = felix.py;
  felixFixing = false;
  ralphThrowing = false;
  ralph.col = 1; ralph.oldCol = 1; ralph.moved = true;
  ralph.delayActive = false;

  // El primer lanzamiento llega tras TIME_COOLDOWN_START en vez del cooldown completo
  unsigned long initialOffset = ralphCooldown() - TIME_COOLDOWN_START;
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
  tft.fillScreen(COLOR_SKY);
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

// Termina la reparación en curso: aquí es cuando la ventana mejora de verdad
void finishRepair() {
  felixFixing = false;
  felixDirty = true;

  Window &w = building[felix.col][felix.row];
  if (w.state == 0) return;
  w.state--;
  felix.score += (w.state == 0) ? SCORE_FIX_FULL : SCORE_FIX_PARTIAL;
  markDirty(felix.col);
  playSFX(NOTE_G4, 150);
  if (allWindowsFixed()) beginStageClear();
}

void handleFelixInput(const Inputs &in, bool fixPressed, unsigned long now) {
  // Mientras repara, Felix se queda quieto (y expuesto a las rocas)
  if (felixFixing) return;

  // Una reparación por pulsación (flanco), no por mantener el botón
  if (fixPressed && building[felix.col][felix.row].state > 0) {
    felixFixing = true;
    felixFixUntil = now + TIME_REPAIR;
    felixDirty = true;
    playSFX(NOTE_E4, 60);
    return;
  }

  if (now - lastMoveTime < TIME_DEBOUNCE) return;

  int newCol = felix.col;
  int newRow = felix.row;
  if (in.left) { if (newCol > 0) newCol--; }
  else if (in.right) { if (newCol < COLS - 1) newCol++; }
  else if (in.up) { if (newRow > 0) newRow--; }
  else if (in.down) { if (newRow < ROWS - 1) newRow++; }

  if (newCol == felix.col && newRow == felix.row) return;

  felix.col = newCol;
  felix.row = newRow;
  felix.fromX = felix.px;
  felix.fromY = felix.py;
  felix.hopStart = now;
  felix.hopping = true;
  lastMoveTime = now;

  if (hammerSpawned && felix.col == hammerCol && felix.row == hammerRow) {
    giveHammer();
    markDirty(hammerCol);
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

// Ralph levanta los brazos; la roca sale cuando los baja (ver updateTimedEffects).
// Ese aviso es lo que permite a Felix reaccionar.
void startThrow(unsigned long now) {
  ralphThrowing = true;
  ralphThrowUntil = now + TIME_RALPH_WINDUP;
  ralph.lastThrowTime = now;
  ralph.moved = true;
  playSFX(110, 70);
}

void handleRalph(const Inputs &in, unsigned long now) {
  if (ralphThrowing) return;  // Clavado en el sitio mientras prepara el lanzamiento

  bool cooldownReady = now >= ralph.lastThrowTime && (now - ralph.lastThrowTime >= ralphCooldown());

  if (activeMode == TWO_PLAYERS) {
    if (in.ralphThrow && cooldownReady) {
      startThrow(now);
    } else if (now - lastRalphMoveTime > TIME_DEBOUNCE) {
      if (in.ralphLeft && ralph.col > 0) {
        moveRalph(ralph.col - 1);
        lastRalphMoveTime = now;
      } else if (in.ralphRight && ralph.col < COLS - 1) {
        moveRalph(ralph.col + 1);
        lastRalphMoveTime = now;
      }
    }
    return;
  }

  // IA de 1 jugador: camina hacia la columna de Felix, de una en una y con un
  // retraso de reacción en cada paso
  if (felix.col == ralph.col) {
    ralph.delayActive = false;
  } else if (!ralph.delayActive) {
    ralph.nextMoveTime = now + random(IA_DELAY_MIN, IA_DELAY_MAX);
    ralph.delayActive = true;
  } else if (now >= ralph.nextMoveTime) {
    moveRalph(ralph.col + (felix.col > ralph.col ? 1 : -1));
    ralph.delayActive = false;
  }

  if (cooldownReady) startThrow(now);
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
    felixFixing = false;  // El golpe interrumpe la reparación
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
  // Salto de Felix hacia su celda, con un pequeño arco
  if (felix.hopping) {
    float t = (float)(now - felix.hopStart) / TIME_HOP;
    if (t >= 1.0f) { t = 1.0f; felix.hopping = false; }
    felix.px = felix.fromX + (int)((colX[felix.col] - felix.fromX) * t);
    felix.py = felix.fromY + (int)((rowY[felix.row] - felix.fromY) * t) - (int)(HOP_HEIGHT * sinf(PI * t));
  }

  if (ralphThrowing && now >= ralphThrowUntil) {
    ralphThrowing = false;
    ralph.moved = true;
    spawnRock();
  }

  if (felixHasHammer && now >= hammerEndTime) {
    felixHasHammer = false;
    felixDirty = true;
  }

  if (isInvincible) {
    updateInvincibility(now);
  }

  // Al final: puede completar la etapa y cambiar de estado
  if (felixFixing && now >= felixFixUntil) finishRepair();
}

void updateInvincibility(unsigned long now) {
  if (now >= invincibilityEndTime) {
    isInvincible = false;
    flickerVisible = true;
    felixDirty = true;
  } else if (now - lastFlickerTime > TIME_FLICKER) {
    flickerVisible = !flickerVisible;
    lastFlickerTime = now;
    felixDirty = true;
  }
}

void updateLivesLEDs() {
  digitalWrite(LED_LIFE_1, felix.lives >= 1 ? HIGH : LOW);
  digitalWrite(LED_LIFE_2, felix.lives >= 2 ? HIGH : LOW);
  digitalWrite(LED_LIFE_3, felix.lives >= 3 ? HIGH : LOW);
}

// ================= RENDERIZADO TFT =================
// Dibuja en `g` el trozo de pared de ladrillo que cae en la zona de pantalla
// (x0, y0, w, h). El origen de `g` coincide con la esquina de esa zona.
void drawBricks(TFT_eSPI &g, int x0, int y0, int w, int h) {
  g.fillRect(0, 0, w, h, COLOR_BRICK);

  int firstCourse = (y0 - BUILD_Y) / BRICK_H;
  for (int course = firstCourse; BUILD_Y + course * BRICK_H < y0 + h; course++) {
    int y = BUILD_Y + course * BRICK_H - y0;  // Junta horizontal de esta hilada
    g.drawFastHLine(0, y, w, COLOR_MORTAR);

    int stagger = (course % 2) ? BRICK_W / 2 : 0;
    int firstJoint = (x0 - BUILD_X - stagger) / BRICK_W;
    for (int j = firstJoint; ; j++) {
      int x = BUILD_X + stagger + j * BRICK_W - x0;
      if (x >= w) break;
      if (x >= 0) g.drawFastVLine(x, y + 1, BRICK_H - 1, COLOR_MORTAR);
    }
  }
}

// Compone en `buf` todo lo que hay en la zona de pantalla que empieza en
// (x0, y0) y mide lo que el búfer (pared, ventanas, martillo, Felix, rocas) y
// la vuelca. La zona debe quedar dentro del edificio.
void renderRegion(TFT_eSprite &buf, int x0, int y0) {
  int w = buf.width();
  int h = buf.height();
  drawBricks(buf, x0, y0, w, h);

  for (int c = 0; c < COLS; c++) {
    int wx = colX[c] - WIN_OFFSET_X - x0;
    if (wx >= w || wx + WIN_WIDTH <= 0) continue;
    for (int r = 0; r < ROWS; r++) {
      int wy = rowY[r] - WIN_OFFSET_Y - y0;
      if (wy >= h || wy + WIN_HEIGHT <= 0) continue;

      TFT_eSprite &win = (building[c][r].state == 2) ? sprWinFullyBroken
                       : (building[c][r].state == 1) ? sprWinBroken
                       : sprWinFixed;
      win.pushToSprite(&buf, wx, wy);

      if (hammerSpawned && hammerCol == c && hammerRow == r) {
        sprHammer.pushToSprite(&buf, wx + 12, wy + 14, TFT_MAGENTA);
      }
    }
  }

  if (flickerVisible) {
    int fx = felix.px - x0;
    int fy = felix.py - y0;
    TFT_eSprite &frame = felixFixing ? sprFelixFix : sprFelix;
    frame.pushToSprite(&buf, fx - FELIX_OFFSET, fy - FELIX_OFFSET, TFT_MAGENTA);
    if (felixHasHammer) buf.fillRect(fx - 4, fy - 22, 8, 4, TFT_YELLOW);
  }

  for (int i = 0; i < MAX_ROCKS; i++) {
    if (!rocks[i].active) continue;
    sprRock.pushToSprite(&buf, colX[rocks[i].col] - ROCK_OFFSET - x0, (int)rocks[i].y - ROCK_OFFSET - y0, TFT_MAGENTA);
  }

  buf.pushSprite(x0, y0);
}

void renderColumn(int c) {
  renderRegion(sprColumn, colX[c] - WIN_OFFSET_X, BUILD_Y);
  colDirty[c] = false;
}

// Recompone una caja del tamaño de sprActor que contiene el punto (x, y)
// dado como esquina superior izquierda, sin salirse del edificio.
void renderActorBox(int x, int y) {
  x = constrain(x, BUILD_X, BUILD_X + BUILD_W - ACTOR_BUF_W);
  y = constrain(y, BUILD_Y, SCREEN_MAX_Y - ACTOR_BUF_H);
  renderRegion(sprActor, x, y);
}

// Repinta a Felix: una sola caja si su posición anterior y la nueva caben
// juntas (lo normal), o dos si se ha movido mucho entre fotogramas.
void renderFelixArea() {
  int minX = min(felixDrawnX, felix.px) - FELIX_OFFSET;
  int maxX = max(felixDrawnX, felix.px) + FELIX_OFFSET;
  int minY = min(felixDrawnY, felix.py) - FELIX_RECT_TOP;
  int maxY = max(felixDrawnY, felix.py) + FELIX_OFFSET;

  if (maxX - minX <= ACTOR_BUF_W && maxY - minY <= ACTOR_BUF_H) {
    renderActorBox(minX, minY);
  } else {
    renderActorBox(felixDrawnX - FELIX_OFFSET, felixDrawnY - FELIX_RECT_TOP);
    renderActorBox(felix.px - FELIX_OFFSET, felix.py - FELIX_RECT_TOP);
  }

  felixDrawnX = felix.px;
  felixDrawnY = felix.py;
  felixDirty = false;
}

void drawRalph() {
  if (ralph.oldCol != ralph.col) {
    tft.fillRect(colX[ralph.oldCol] - RALPH_OFFSET, RALPH_FIXED_Y, RALPH_SIZE, RALPH_SIZE, COLOR_SKY);
    ralph.oldCol = ralph.col;
  }
  TFT_eSprite &frame = ralphThrowing ? sprRalphThrow : sprRalph;
  frame.pushSprite(colX[ralph.col] - RALPH_OFFSET, RALPH_FIXED_Y);
  ralph.moved = false;
}

void drawBuildingBase() {
  // Pared entera una vez (incluye los tramos entre columnas, que luego no se tocan)
  tft.setViewport(BUILD_X, BUILD_Y, BUILD_W, BUILD_H);
  drawBricks(tft, BUILD_X, BUILD_Y, BUILD_W, BUILD_H);
  tft.resetViewport();

  for (int c = 0; c < COLS; c++) renderColumn(c);
  felixDrawnX = felix.px;
  felixDrawnY = felix.py;
  felixDirty = false;
  ralph.moved = true;
}

void drawGame() {
  if (ralph.moved) drawRalph();

  for (int c = 0; c < COLS; c++) {
    if (colDirty[c]) renderColumn(c);
  }

  if (felixDirty || felix.px != felixDrawnX || felix.py != felixDrawnY) renderFelixArea();
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
  if (timeSinceThrow < ralphCooldown()) {
    barWidth = map(timeSinceThrow, 0, ralphCooldown(), 0, OLED_WIDTH);
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
      if (currentState != PLAYING) break;

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
