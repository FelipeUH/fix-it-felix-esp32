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
  bool pause;                              // Pausar / reanudar la partida
};

void platformSetup();                 // Inicializa el control inalámbrico
void platformPoll();                  // Se llama una vez por iteración del loop
// Rellena `in`. Si ralphIsHuman es false (menús y 1 jugador) todos los controles
// disponibles manejan a Felix; si es true, cada sketch reparte Felix/Ralph.
void readInputs(Inputs &in, bool ralphIsHuman);
const char* platformBootMessage();    // Texto del OLED durante el arranque
const char* platformFixLabel();       // Nombre del botón de acción en pantalla
void platformRumble(int durationMs);  // Vibración del mando, si lo hay

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
const unsigned long TICK_OLED = 120;
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
const int FELIX_RECT_TOP = 24;   // Por encima del centro: sprite + indicador de martillo
const int FELIX_RECT_HALF = 14;  // A los lados y por debajo: sprite + borde del escudo

// Objetos que aparecen en las ventanas
const unsigned long TIME_POWERUP_SLOW = 5000;      // Rocas y pájaro a media velocidad
const unsigned long TIME_PICKUP_LIFETIME = 8000;   // Desaparece si no se recoge
const unsigned long TIME_PICKUP_MIN = 8000;        // Intervalo entre apariciones
const unsigned long TIME_PICKUP_MAX = 14000;
const float SLOW_FACTOR = 0.5;

const int COMBO_MAX_MULT = 5;  // Multiplicador máximo por reparaciones seguidas sin golpe

// Pájaro que cruza una fila del edificio
const int BIRD_FIRST_STAGE = 3;
const int BIRD_W = 16;
const int BIRD_H = 12;
const float BIRD_SPEED_BASE = 2.0;
const float BIRD_SPEED_INC = 0.2;
const float BIRD_SPEED_MAX = 4.5;
const int BIRD_HIT_RANGE = 16;
const unsigned long TIME_BIRD_MIN = 5000;
const unsigned long TIME_BIRD_MAX = 9000;
const unsigned long TIME_BIRD_FLAP = 150;
const int BIRD_BUF_W = 40;
const int BIRD_BUF_H = 16;

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

// Pantallas de menú: muro en la parte baja con un cartel para el aviso
const int SCREEN_W = 240;
const int MENU_WALL_Y = 262;
const int HINT_Y = 280;
const int HINT_H = 26;
const int MENU_OPTION_X = 30;
const int MENU_OPTION_W = 180;
const int MENU_OPTION_H = 26;
const int MENU_OPTION_Y = 140;      // Primera opción
const int MENU_OPTION_PITCH = 30;
const int MENU_OPTIONS = 3;         // 1 jugador, 2 jugadores, récords
const int MENU_RECORD_Y = 127;
const unsigned long TIME_HINT_BLINK = 500;

// Tabla de mejores puntuaciones (una por modo) e introducción del nombre
const int TOP_COUNT = 5;
const int NAME_LEN = 3;
const char NAME_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-";
const int NAME_CHAR_COUNT = sizeof(NAME_CHARS) - 1;
const int NAME_SLOT_W = 44;
const int NAME_SLOT_H = 56;
const int NAME_SLOT_Y = 104;
const int NAME_SLOT_PITCH = 56;
const int NAME_SLOT_X = (SCREEN_W - (NAME_LEN - 1) * NAME_SLOT_PITCH - NAME_SLOT_W) / 2;
const int SCORE_TABLE_H = 112;
const unsigned long TIME_LED_BLINK = 600;
const unsigned long TIME_LED_SWEEP = 120;
const unsigned long TIME_POPUP = 600;       // Lo que dura el "+100" sobre la ventana

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
// Un solo zumbador: la música de fondo avanza siempre a su ritmo y los efectos
// la tapan mientras suenan, así al terminar el efecto la melodía sigue en su sitio.
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_C6  1047
#define REST     0

struct Note { uint16_t freq; uint16_t ms; };
#define SEQ_LEN(seq) ((int)(sizeof(seq) / sizeof(seq[0])))

// Música: dos melodías de 32 pasos iguales (una alegre, otra más tensa) que se
// alternan cada dos etapas. El paso se acorta con la etapa: va cada vez más rápida.
const int MUSIC_STEPS = 32;
const uint16_t MUSIC_TRACKS[2][MUSIC_STEPS] = {
  {NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5,  NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5,
   NOTE_A4, NOTE_C5, NOTE_E5, NOTE_C5,  NOTE_G4, NOTE_B4, NOTE_D5, NOTE_B4,
   NOTE_F4, NOTE_A4, NOTE_C5, NOTE_A4,  NOTE_G4, NOTE_B4, NOTE_D5, NOTE_G5,
   NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,  NOTE_C5, REST,    NOTE_G4, REST},
  {NOTE_A4, NOTE_C5, NOTE_E5, NOTE_C5,  NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A5,
   NOTE_F4, NOTE_A4, NOTE_C5, NOTE_A4,  NOTE_E4, NOTE_GS4, NOTE_B4, NOTE_GS4,
   NOTE_D4, NOTE_F4, NOTE_A4, NOTE_F4,  NOTE_E4, NOTE_GS4, NOTE_B4, NOTE_E5,
   NOTE_C5, NOTE_B4, NOTE_A4, NOTE_GS4, NOTE_A4, REST,    NOTE_E4, REST},
};
const unsigned long MUSIC_STEP_BASE = 160;  // ms por nota en la etapa 1
const unsigned long MUSIC_STEP_DEC = 6;     // Se acorta esto por etapa
const unsigned long MUSIC_STEP_MIN = 100;
const unsigned long MUSIC_NOTE_GAP = 18;    // Silencio al final de cada nota para separarlas

// Efectos y fanfarrias
const Note SFX_START[] = {{NOTE_G4, 80}, {NOTE_C5, 80}, {NOTE_E5, 80}, {NOTE_G5, 160}};
const Note SFX_STAGE_CLEAR[] = {{NOTE_C5, 100}, {NOTE_E5, 100}, {NOTE_G5, 100}, {NOTE_C6, 180}, {NOTE_G5, 90}, {NOTE_C6, 300}};
const Note SFX_GAME_OVER[] = {{NOTE_G4, 180}, {NOTE_E4, 180}, {NOTE_C4, 180}, {NOTE_A3, 180}, {NOTE_G3, 450}};
const Note SFX_PICKUP[] = {{NOTE_E5, 70}, {NOTE_A5, 70}, {NOTE_C6, 140}};
const Note SFX_HIT[] = {{220, 90}, {160, 90}, {110, 180}};
const Note SFX_SHIELD[] = {{NOTE_C6, 60}, {NOTE_G5, 60}, {NOTE_C6, 100}};
const Note SFX_REPAIR[] = {{NOTE_G5, 60}, {NOTE_C6, 90}};
const Note SFX_BIRD[] = {{1800, 50}, {REST, 30}, {2200, 60}};

uint16_t buzzerFreq = 0;  // Lo que suena ahora mismo (0 = silencio)

const uint16_t *musicTrack = MUSIC_TRACKS[0];
unsigned long musicStepMs = MUSIC_STEP_BASE;
unsigned long musicStepStart = 0;
int musicPos = 0;

const Note *sfxSeq = nullptr;  // Efecto en curso, o nullptr
int sfxLength = 0;
int sfxPos = 0;
unsigned long sfxNoteStart = 0;
Note sfxSingle = {0, 0};       // Para efectos de una sola nota (playSFX)

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
TFT_eSprite sprHeart = TFT_eSprite(&tft);
TFT_eSprite sprClock = TFT_eSprite(&tft);
TFT_eSprite sprShield = TFT_eSprite(&tft);
// Pájaro: [0-1] mirando a la derecha, [2-3] a la izquierda; dos fotogramas de aleteo
TFT_eSprite sprBird[4] = {TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft)};
// Búferes de composición: cada zona se dibuja entera en RAM y se vuelca de una
// vez, así nada se borra y repinta a la vista (sin parpadeo).
TFT_eSprite sprColumn = TFT_eSprite(&tft);  // Una columna del edificio (rocas)
TFT_eSprite sprActor = TFT_eSprite(&tft);   // Entorno de Felix (saltos entre ventanas)
TFT_eSprite sprBirdBuf = TFT_eSprite(&tft); // Entorno del pájaro

// ================= ESTRUCTURAS Y MODOS =================
enum GameState { START, SCORES, PLAYING, PAUSED, STAGE_CLEAR, ENTER_NAME, GAMEOVER };
enum GameMode { ONE_PLAYER, TWO_PLAYERS };

GameState currentState = START;
GameMode selectedMode = ONE_PLAYER;  // Modo resaltado en el menú y en la pantalla de récords
int menuIndex = 0;                   // Opción del menú: 0 y 1 son los modos, 2 los récords
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

enum PickupType { PICKUP_NONE, PICKUP_HAMMER, PICKUP_HEART, PICKUP_SLOW, PICKUP_SHIELD };

struct Pickup {
  PickupType type;  // PICKUP_NONE = no hay ninguno en el edificio
  int col, row;
  unsigned long expires;
};

struct Bird {
  bool active;
  float x;   // Centro en pantalla
  int row;
  int dir;   // +1 hacia la derecha, -1 hacia la izquierda
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
struct ScoreEntry {
  char name[NAME_LEN + 1];
  int32_t score;
};
ScoreEntry topScores[2][TOP_COUNT];  // Por modo (GameMode), de mayor a menor
const char* const TOP_KEYS[2] = {"top1p", "top2p"};
int lastRank = -1;                   // Puesto logrado en la última partida (-1 = fuera de la tabla)

char entryName[NAME_LEN + 1] = "AAA";  // Se recuerda de una partida a otra
int entrySlot = 0;
unsigned long nameEntryStart = 0;
unsigned long pauseStart = 0;
bool prevPause = false;
const char* const HIGHSCORE_KEYS[2] = {"highscore", "highscore2p"};

bool isInvincible = false;
unsigned long invincibilityEndTime = 0;
unsigned long lastFlickerTime = 0;
bool flickerVisible = true;

Pickup pickup = {PICKUP_NONE, 0, 0, 0};
unsigned long nextPickupTime = 0;
bool felixHasHammer = false;
unsigned long hammerEndTime = 0;
bool felixHasShield = false;
unsigned long slowEndTime = 0;
int combo = 0;  // Reparaciones seguidas sin recibir un golpe

// Puntos ganados que se muestran un momento sobre la ventana reparada
struct ScorePopup { bool active; int col, row; long value; unsigned long until; };
ScorePopup popup = {false, 0, 0, 0, 0};

Bird bird = {false, 0, 0, 1};
unsigned long nextBirdTime = 0;
int birdDrawnX = 0;
bool birdOnScreen = false;

unsigned long ledBlinkUntil = 0;

bool startScreenDrawn = false;
bool hintVisible = true;
unsigned long lastHintToggle = 0;
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
void drawNameEntryScreen();
void drawPauseOverlay();
void drawMenuOptions();
void drawStageClearScreen();
void showGameOverScreen();
void updateMenuOLED();
void drawBuildingBase();
void renderColumn(int c);
void updateInvincibility(unsigned long now);
int comboMultiplier();
int birdY();
void drawBricks(TFT_eSPI &g, int x0, int y0, int w, int h);

// ================= GESTIÓN DE AUDIO =================
void buzz(uint16_t freq) {
  if (freq == buzzerFreq) return;
  if (freq == REST) noTone(PIN_BUZZER);
  else tone(PIN_BUZZER, freq);
  buzzerFreq = freq;
}

void playSequence(const Note *seq, int length) {
  sfxSeq = seq;
  sfxLength = length;
  sfxPos = 0;
  sfxNoteStart = millis();
}
#define PLAY(seq) playSequence(seq, SEQ_LEN(seq))

void playSFX(int freq, int duration) {
  sfxSingle = {(uint16_t)freq, (uint16_t)duration};
  playSequence(&sfxSingle, 1);
}

// Arranca la música de la etapa actual desde el principio
void startMusic() {
  musicTrack = MUSIC_TRACKS[((stage - 1) / 2) % 2];
  unsigned long faster = (unsigned long)(stage - 1) * MUSIC_STEP_DEC;
  musicStepMs = (faster >= MUSIC_STEP_BASE - MUSIC_STEP_MIN) ? MUSIC_STEP_MIN : MUSIC_STEP_BASE - faster;
  musicPos = 0;
  musicStepStart = millis();
}

void handleAudio(unsigned long now) {
  if (sfxSeq != nullptr && now - sfxNoteStart >= sfxSeq[sfxPos].ms) {
    sfxPos++;
    sfxNoteStart = now;
    if (sfxPos >= sfxLength) sfxSeq = nullptr;
  }

  uint16_t want = REST;

  if (currentState == PLAYING) {
    if (now - musicStepStart > 1000) musicStepStart = now;  // Tras un parón largo no se "pone al día"
    while (now - musicStepStart >= musicStepMs) {
      musicStepStart += musicStepMs;
      musicPos = (musicPos + 1) % MUSIC_STEPS;
    }
    if (now - musicStepStart < musicStepMs - MUSIC_NOTE_GAP) want = musicTrack[musicPos];
  }

  if (sfxSeq != nullptr) want = sfxSeq[sfxPos].freq;

  buzz(want);
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
    case 'c': return COLOR_GLASS;
    default:  return background;
  }
}

void buildArtSprite(TFT_eSprite &spr, const char *const *rows, int w, int h, int scale,
                    uint16_t background, bool mirror = false) {
  spr.createSprite(w * scale, h * scale);
  spr.fillSprite(background);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      uint16_t color = artColor(rows[y][mirror ? w - 1 - x : x], background);
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

const char *const ART_HEART[8] = {
  ".rr..rr.",
  "rrrrrrrr",
  "rwrrrrrr",
  "rrrrrrrr",
  ".rrrrrr.",
  "..rrrr..",
  "...rr...",
  "........",
};

const char *const ART_CLOCK[8] = {
  "..kkkk..",
  ".kwwwwk.",
  "kwwkwwwk",
  "kwwkwwwk",
  "kwwkkwwk",
  "kwwwwwwk",
  ".kwwwwk.",
  "..kkkk..",
};

const char *const ART_SHIELD[8] = {
  "kkkkkkkk",
  "kbbbbwbk",
  "kbbbbwbk",
  "kbbbbbbk",
  "kbbbbbbk",
  ".kbbbbk.",
  "..kbbk..",
  "...kk...",
};

const char *const ART_BIRD_UP[6] = {
  ".gg.....",
  "..ggg.g.",
  "gggggkgy",
  ".gggggg.",
  "..gggg..",
  "........",
};

const char *const ART_BIRD_DOWN[6] = {
  "......g.",
  "gggggkgy",
  ".gggggg.",
  "..gggg..",
  ".ggg....",
  ".gg.....",
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
  buildArtSprite(sprFelix, ART_FELIX, 12, 12, 2, TFT_MAGENTA);
  buildArtSprite(sprFelixFix, ART_FELIX_FIX, 12, 12, 2, TFT_MAGENTA);
  // Ralph solo se dibuja sobre el cielo: sprite opaco, se pinta sin borrar antes.
  buildArtSprite(sprRalph, ART_RALPH, 16, 16, 2, COLOR_SKY);
  buildArtSprite(sprRalphThrow, ART_RALPH_THROW, 16, 16, 2, COLOR_SKY);

  buildArtSprite(sprHeart, ART_HEART, 8, 8, 2, TFT_MAGENTA);
  buildArtSprite(sprClock, ART_CLOCK, 8, 8, 2, TFT_MAGENTA);
  buildArtSprite(sprShield, ART_SHIELD, 8, 8, 2, TFT_MAGENTA);
  buildArtSprite(sprBird[0], ART_BIRD_UP, 8, 6, 2, TFT_MAGENTA);
  buildArtSprite(sprBird[1], ART_BIRD_DOWN, 8, 6, 2, TFT_MAGENTA);
  buildArtSprite(sprBird[2], ART_BIRD_UP, 8, 6, 2, TFT_MAGENTA, true);
  buildArtSprite(sprBird[3], ART_BIRD_DOWN, 8, 6, 2, TFT_MAGENTA, true);

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
      sprActor.createSprite(ACTOR_BUF_W, ACTOR_BUF_H) == nullptr ||
      sprBirdBuf.createSprite(BIRD_BUF_W, BIRD_BUF_H) == nullptr) {
    Serial.println("Sin memoria para los buferes de composicion");
  }
}

// ================= PUNTUACIÓN =================
long bestScore(GameMode mode) { return topScores[mode][0].score; }

void loadHighScores() {
  preferences.begin("felix_game", true);
  for (int m = 0; m < 2; m++) {
    if (preferences.getBytesLength(TOP_KEYS[m]) == sizeof(topScores[m])) {
      preferences.getBytes(TOP_KEYS[m], topScores[m], sizeof(topScores[m]));
      continue;
    }
    // Sin tabla guardada: vacía, conservando el récord único de versiones anteriores
    for (int i = 0; i < TOP_COUNT; i++) {
      strcpy(topScores[m][i].name, "---");
      topScores[m][i].score = 0;
    }
    topScores[m][0].score = preferences.getLong(HIGHSCORE_KEYS[m], 0);
  }
  preferences.end();
}

// Puesto que ocuparía esta puntuación, o -1 si no entra en la tabla
int rankForScore(GameMode mode, long score) {
  if (score <= 0) return -1;
  for (int i = 0; i < TOP_COUNT; i++) {
    if (score > topScores[mode][i].score) return i;
  }
  return -1;
}

void insertScore(GameMode mode, int rank, const char *name, long score) {
  for (int i = TOP_COUNT - 1; i > rank; i--) topScores[mode][i] = topScores[mode][i - 1];
  strncpy(topScores[mode][rank].name, name, NAME_LEN);
  topScores[mode][rank].name[NAME_LEN] = '\0';
  topScores[mode][rank].score = score;

  preferences.begin("felix_game", false);
  preferences.putBytes(TOP_KEYS[mode], topScores[mode], sizeof(topScores[mode]));
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

void schedulePickup(unsigned long now) {
  nextPickupTime = now + random(TIME_PICKUP_MIN, TIME_PICKUP_MAX);
}

// Coloca un objeto al azar en una ventana donde no esté Felix
void spawnPickup(unsigned long now) {
  // El corazón solo sale si falta alguna vida
  static const PickupType options[] = {PICKUP_HAMMER, PICKUP_SLOW, PICKUP_SHIELD, PICKUP_HEART};
  pickup.type = options[random(felix.lives < 3 ? 4 : 3)];
  if (pickup.type == PICKUP_SHIELD && felixHasShield) pickup.type = PICKUP_HAMMER;

  do {
    pickup.col = random(COLS);
    pickup.row = random(ROWS);
  } while (pickup.col == felix.col && pickup.row == felix.row);

  pickup.expires = now + TIME_PICKUP_LIFETIME;
  markDirty(pickup.col);
}

void collectPickup(unsigned long now) {
  switch (pickup.type) {
    case PICKUP_HAMMER:
      felixHasHammer = true;
      hammerEndTime = now + TIME_POWERUP_HAMMER;
      break;
    case PICKUP_HEART:
      if (felix.lives < 3) felix.lives++;
      updateLivesLEDs();
      break;
    case PICKUP_SLOW:
      slowEndTime = now + TIME_POWERUP_SLOW;
      break;
    case PICKUP_SHIELD:
      felixHasShield = true;
      break;
    case PICKUP_NONE:
      return;
  }
  pickup.type = PICKUP_NONE;
  markDirty(pickup.col);
  felixDirty = true;
  schedulePickup(now);
  PLAY(SFX_PICKUP);
}

bool slowActive(unsigned long now) { return now < slowEndTime; }

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
  slowEndTime = 0;  // El escudo y el combo sí se conservan entre etapas

  pickup.type = PICKUP_NONE;
  schedulePickup(millis());
  popup.active = false;
  bird.active = false;
  birdOnScreen = false;
  nextBirdTime = millis() + random(TIME_BIRD_MIN, TIME_BIRD_MAX);
}

// Genera las ventanas rotas de la etapa actual y, a veces, un objeto inicial
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

  if (random(100) < hammerChance) spawnPickup(millis());
}

void initGame() {
  stage = 1;
  felix.lives = 3;
  felix.score = 0;
  felixHasShield = false;
  combo = 0;
  resetActors();
  generateStage();
  updateLivesLEDs();
}

void enterPlaying() {
  currentState = PLAYING;
  tft.fillScreen(COLOR_SKY);
  drawBuildingBase();
  updateOLED();
  startMusic();
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

  drawStageClearScreen();
  updateOLED();
  PLAY(SFX_STAGE_CLEAR);
  stageClearStart = millis();
  currentState = STAGE_CLEAR;
}

void beginGameOver() {
  PLAY(SFX_GAME_OVER);
  platformRumble(600);

  // Si la puntuación entra en la tabla, primero se pide el nombre
  lastRank = rankForScore(activeMode, felix.score);
  if (lastRank >= 0) {
    currentState = ENTER_NAME;
    entrySlot = 0;
    nameEntryStart = millis();
    drawNameEntryScreen();
  } else {
    currentState = GAMEOVER;
    gameOverStart = millis();
    showGameOverScreen();
  }
  updateMenuOLED();
}

// Al reanudar, todos los plazos pendientes se retrasan lo que duró la pausa
void resumeFromPause(unsigned long now) {
  unsigned long delta = now - pauseStart;
  ralph.lastThrowTime += delta;
  ralph.nextMoveTime += delta;
  ralphThrowUntil += delta;
  felixFixUntil += delta;
  felix.hopStart += delta;
  invincibilityEndTime += delta;
  lastFlickerTime += delta;
  hammerEndTime += delta;
  slowEndTime += delta;
  pickup.expires += delta;
  nextPickupTime += delta;
  nextBirdTime += delta;
  popup.until += delta;
  ledBlinkUntil += delta;
  lastMoveTime = now;  // Que el botón de pausa aún pulsado no mueva a Felix
  lastGameUpdate = now;

  currentState = PLAYING;
  drawBuildingBase();
  felixDirty = true;
  startMusic();
}

void beginPause(unsigned long now) {
  currentState = PAUSED;
  pauseStart = now;
  drawPauseOverlay();
  updateMenuOLED();
}

int comboMultiplier() { return min(combo + 1, COMBO_MAX_MULT); }

// Termina la reparación en curso: aquí es cuando la ventana mejora de verdad
void finishRepair() {
  felixFixing = false;
  felixDirty = true;

  Window &w = building[felix.col][felix.row];
  if (w.state == 0) return;
  w.state--;
  long gained = ((w.state == 0) ? SCORE_FIX_FULL : SCORE_FIX_PARTIAL) * comboMultiplier();
  felix.score += gained;
  combo++;
  popup = {true, felix.col, felix.row, gained, millis() + TIME_POPUP};
  markDirty(felix.col);
  PLAY(SFX_REPAIR);
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

  if (pickup.type != PICKUP_NONE && felix.col == pickup.col && felix.row == pickup.row) {
    collectPickup(now);
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

void updateRocks(unsigned long now) {
  float currentSpeed = min(ROCK_SPEED_BASE + stage * ROCK_SPEED_INC, ROCK_SPEED_MAX);
  if (slowActive(now)) currentSpeed *= SLOW_FACTOR;

  for (int i = 0; i < MAX_ROCKS; i++) {
    if (!rocks[i].active) continue;
    rocks[i].y += currentSpeed;
    if (rocks[i].y > SCREEN_MAX_Y + ROCK_OFFSET) rocks[i].active = false;
    markDirty(rocks[i].col);
  }
}

// A partir de BIRD_FIRST_STAGE, un pájaro cruza de vez en cuando una fila
// entera: obliga a cambiar de fila, no solo de columna.
void updateBird(unsigned long now) {
  if (!bird.active) {
    if (stage < BIRD_FIRST_STAGE || now < nextBirdTime) return;
    bird.active = true;
    bird.row = random(ROWS);
    bird.dir = random(2) ? 1 : -1;
    bird.x = (bird.dir > 0) ? BUILD_X - BIRD_W / 2 : BUILD_X + BUILD_W + BIRD_W / 2;
    PLAY(SFX_BIRD);
    return;
  }

  float speed = min(BIRD_SPEED_BASE + stage * BIRD_SPEED_INC, BIRD_SPEED_MAX);
  if (slowActive(now)) speed *= SLOW_FACTOR;
  bird.x += bird.dir * speed;

  if (bird.x < BUILD_X - BIRD_W / 2 || bird.x > BUILD_X + BUILD_W + BIRD_W / 2) {
    bird.active = false;
    nextBirdTime = now + random(TIME_BIRD_MIN, TIME_BIRD_MAX);
  }
}

int birdY() { return rowY[bird.row] - 4; }

// Felix recibe un golpe de una roca o del pájaro
void hitFelix() {
  felixFixing = false;  // El golpe interrumpe la reparación
  felixDirty = true;

  if (felixHasShield) {
    felixHasShield = false;  // El escudo absorbe el golpe: ni vida ni combo
    PLAY(SFX_SHIELD);
    platformRumble(100);
  } else {
    combo = 0;
    felix.lives--;
    ledBlinkUntil = millis() + TIME_LED_BLINK;  // Parpadea el LED de la vida perdida
    if (felix.lives <= 0) {
      beginGameOver();
      return;
    }
    PLAY(SFX_HIT);
    platformRumble(250);
  }

  isInvincible = true;
  invincibilityEndTime = millis() + TIME_INVINCIBLE;
  lastFlickerTime = millis();
  flickerVisible = false;
}

void checkCollisions() {
  if (isInvincible || felixHasHammer) return;

  for (int i = 0; i < MAX_ROCKS; i++) {
    if (!rocks[i].active) continue;
    if (rocks[i].col != felix.col || fabsf(rocks[i].y - rowY[felix.row]) >= COLLISION_TOLERANCE) continue;

    rocks[i].active = false;
    markDirty(rocks[i].col);
    hitFelix();
    return;
  }

  if (bird.active && bird.row == felix.row && fabsf(bird.x - colX[felix.col]) < BIRD_HIT_RANGE) {
    hitFelix();
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

  if (popup.active && now >= popup.until) {
    popup.active = false;
    markDirty(popup.col);
  }

  // Objetos: caducan si nadie los recoge y reaparecen cada cierto tiempo
  if (pickup.type != PICKUP_NONE) {
    if (now >= pickup.expires) {
      pickup.type = PICKUP_NONE;
      markDirty(pickup.col);
      schedulePickup(now);
    }
  } else if (now >= nextPickupTime) {
    spawnPickup(now);
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

// LEDs de vida: muestran las vidas, con tres animaciones encima:
// parpadeo de la vida recién perdida, barrido al superar una etapa y
// parpadeo lento de los tres en Game Over.
void updateLivesLEDs() {
  static const int pins[3] = {LED_LIFE_1, LED_LIFE_2, LED_LIFE_3};
  unsigned long now = millis();

  for (int i = 0; i < 3; i++) {
    bool on = felix.lives > i;
    if (currentState == STAGE_CLEAR) {
      on = (int)((now / TIME_LED_SWEEP) % 3) == i;
    } else if (currentState == GAMEOVER || currentState == ENTER_NAME) {
      on = (now / 400) % 2 == 0;
    } else if (now < ledBlinkUntil && i == felix.lives) {
      on = (now / 80) % 2 == 0;
    }
    digitalWrite(pins[i], on ? HIGH : LOW);
  }
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

TFT_eSprite &pickupSprite(PickupType type) {
  switch (type) {
    case PICKUP_HEART:  return sprHeart;
    case PICKUP_SLOW:   return sprClock;
    case PICKUP_SHIELD: return sprShield;
    default:            return sprHammer;
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

      if (pickup.type != PICKUP_NONE && pickup.col == c && pickup.row == r) {
        pickupSprite(pickup.type).pushToSprite(&buf, wx + 12, wy + 14, TFT_MAGENTA);
      }
    }
  }

  if (flickerVisible) {
    int fx = felix.px - x0;
    int fy = felix.py - y0;
    TFT_eSprite &frame = felixFixing ? sprFelixFix : sprFelix;
    frame.pushToSprite(&buf, fx - FELIX_OFFSET, fy - FELIX_OFFSET, TFT_MAGENTA);
    if (felixHasHammer) buf.fillRect(fx - 4, fy - 22, 8, 4, TFT_YELLOW);
    if (felixHasShield) buf.drawRoundRect(fx - 14, fy - 14, 28, 28, 6, COLOR_GLASS_SHINE);
  }

  for (int i = 0; i < MAX_ROCKS; i++) {
    if (!rocks[i].active) continue;
    sprRock.pushToSprite(&buf, colX[rocks[i].col] - ROCK_OFFSET - x0, (int)rocks[i].y - ROCK_OFFSET - y0, TFT_MAGENTA);
  }

  if (popup.active) {
    // Sobre los ladrillos, justo encima de la ventana, con sombra para que se lea
    char text[8];
    snprintf(text, sizeof(text), "+%ld", popup.value);
    int tx = colX[popup.col] - x0;
    int ty = rowY[popup.row] - WIN_OFFSET_Y - 6 - y0;
    buf.setTextFont(1);
    buf.setTextSize(1);
    buf.setTextDatum(MC_DATUM);
    buf.setTextColor(TFT_BLACK);
    buf.drawString(text, tx + 1, ty + 1);
    buf.setTextColor(TFT_YELLOW);
    buf.drawString(text, tx, ty);
  }

  if (bird.active) {
    int frame = (millis() / TIME_BIRD_FLAP) % 2 + (bird.dir < 0 ? 2 : 0);
    sprBird[frame].pushToSprite(&buf, (int)bird.x - BIRD_W / 2 - x0, birdY() - BIRD_H / 2 - y0, TFT_MAGENTA);
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
  int minX = min(felixDrawnX, felix.px) - FELIX_RECT_HALF;
  int maxX = max(felixDrawnX, felix.px) + FELIX_RECT_HALF;
  int minY = min(felixDrawnY, felix.py) - FELIX_RECT_TOP;
  int maxY = max(felixDrawnY, felix.py) + FELIX_RECT_HALF;

  if (maxX - minX <= ACTOR_BUF_W && maxY - minY <= ACTOR_BUF_H) {
    renderActorBox(minX, minY);
  } else {
    renderActorBox(felixDrawnX - FELIX_RECT_HALF, felixDrawnY - FELIX_RECT_TOP);
    renderActorBox(felix.px - FELIX_RECT_HALF, felix.py - FELIX_RECT_TOP);
  }

  felixDrawnX = felix.px;
  felixDrawnY = felix.py;
  felixDirty = false;
}

// Repinta la franja por la que vuela el pájaro: cubre donde estaba y donde
// está, y una última vez al irse para borrarlo.
void renderBirdArea() {
  int x = bird.active ? (int)bird.x : birdDrawnX;
  int left = min(x, birdOnScreen ? birdDrawnX : x) - BIRD_W / 2 - 2;
  left = constrain(left, BUILD_X, BUILD_X + BUILD_W - BIRD_BUF_W);
  renderRegion(sprBirdBuf, left, birdY() - BIRD_BUF_H / 2);

  birdDrawnX = x;
  birdOnScreen = bird.active;
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
  if (bird.active || birdOnScreen) renderBirdArea();
}

// ---------- Pantallas de menú ----------
// Todas comparten fondo (cielo con estrellas y un muro abajo) y tipografía:
// la fuente de píxeles escalada, a juego con los sprites.

void drawShadowText(const char *text, int x, int y, int size, uint16_t color, uint16_t shadow) {
  int offset = max(1, size / 2);
  tft.setTextFont(1);
  tft.setTextSize(size);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(shadow);
  tft.drawString(text, x + offset, y + offset);
  tft.setTextColor(color);
  tft.drawString(text, x, y);
}

void drawBackdrop() {
  tft.fillScreen(COLOR_SKY);

  // Estrellas en los márgenes, fuera de las zonas que se redibujan
  static const uint8_t stars[][2] = {
    {8, 10}, {30, 4}, {222, 14}, {200, 6}, {14, 108}, {228, 100}, {6, 160},
    {234, 150}, {12, 226}, {226, 236}, {120, 6}, {74, 12}, {166, 8}, {18, 60}, {224, 64},
  };
  for (auto &star : stars) tft.drawPixel(star[0], star[1], TFT_WHITE);
  tft.fillRect(40, 20, 2, 2, COLOR_GLASS_SHINE);
  tft.fillRect(206, 30, 2, 2, COLOR_GLASS_SHINE);

  int wallH = SCREEN_MAX_Y - MENU_WALL_Y;
  tft.setViewport(0, MENU_WALL_Y, SCREEN_W, wallH);
  drawBricks(tft, BUILD_X, BUILD_Y, SCREEN_W, wallH);
  tft.resetViewport();
  tft.fillRect(0, MENU_WALL_Y, SCREEN_W, 3, COLOR_SILL);
}

// Texto del cartel del muro. Se repinta con color de fondo, así puede
// parpadear sin borrar nada.
void drawHintText(const char *text, bool visible) {
  tft.setTextFont(1);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(visible ? TFT_YELLOW : COLOR_ROOM, COLOR_ROOM);
  tft.drawString(text, SCREEN_W / 2, HINT_Y + HINT_H / 2);
}

void drawHintPlaque() {
  tft.fillRoundRect(10, HINT_Y, SCREEN_W - 20, HINT_H, 5, COLOR_ROOM);
  tft.drawRoundRect(10, HINT_Y, SCREEN_W - 20, HINT_H, 5, COLOR_STEEL);
}

void drawPressFixHint(bool visible) {
  char text[24];
  snprintf(text, sizeof(text), "PULSA %s", platformFixLabel());
  drawHintText(text, visible);
  hintVisible = visible;
  lastHintToggle = millis();
}

void updateHintBlink(unsigned long now) {
  if (now - lastHintToggle >= TIME_HINT_BLINK) drawPressFixHint(!hintVisible);
}

// Panel con filas "ETIQUETA ........ valor"
void drawPanel(int y, int h) {
  tft.fillRoundRect(20, y, SCREEN_W - 40, h, 6, COLOR_ROOM);
  tft.drawRoundRect(20, y, SCREEN_W - 40, h, 6, COLOR_STEEL);
}

void drawPanelRow(const char *label, long value, int y, uint16_t color) {
  tft.setTextFont(1);
  tft.setTextSize(2);
  tft.setTextColor(color);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(label, 32, y);
  tft.setTextDatum(TR_DATUM);
  tft.drawNumber(value, SCREEN_W - 32, y);
}

void drawMenuOption(int index, const char *label) {
  bool selected = (index == menuIndex);
  int y = MENU_OPTION_Y + index * MENU_OPTION_PITCH;
  tft.fillRoundRect(MENU_OPTION_X, y, MENU_OPTION_W, MENU_OPTION_H, 6, selected ? TFT_YELLOW : COLOR_ROOM);
  tft.drawRoundRect(MENU_OPTION_X, y, MENU_OPTION_W, MENU_OPTION_H, 6, selected ? TFT_WHITE : COLOR_STEEL);
  if (selected) {
    int cy = y + MENU_OPTION_H / 2;
    tft.fillTriangle(MENU_OPTION_X + 10, cy - 7, MENU_OPTION_X + 10, cy + 7, MENU_OPTION_X + 20, cy, COLOR_SHIRT);
  }
  tft.setTextFont(1);
  tft.setTextSize(2);
  tft.setTextDatum(ML_DATUM);
  tft.setTextColor(selected ? TFT_BLACK : TFT_WHITE);
  tft.drawString(label, MENU_OPTION_X + 30, y + MENU_OPTION_H / 2);
}

// Parte del menú que cambia al mover la selección: las opciones y el récord
// del modo resaltado. No repinta el resto de la pantalla.
void drawMenuOptions() {
  drawMenuOption(0, "1 JUGADOR");
  drawMenuOption(1, "2 JUGADORES");
  drawMenuOption(2, "RECORDS");

  char text[24];
  const ScoreEntry &best = topScores[selectedMode][0];
  snprintf(text, sizeof(text), "RECORD %s %ld", best.name, (long)best.score);
  tft.fillRect(0, MENU_RECORD_Y - 9, SCREEN_W, 18, COLOR_SKY);
  tft.setTextFont(1);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(COLOR_GLASS_SHINE);
  tft.drawString(text, SCREEN_W / 2, MENU_RECORD_Y);

  updateMenuOLED();
}

void drawStartScreen() {
  drawBackdrop();

  drawShadowText("FIX-IT", SCREEN_W / 2, 42, 5, TFT_YELLOW, COLOR_SHIRT);
  drawShadowText("FELIX!", SCREEN_W / 2, 88, 5, TFT_YELLOW, COLOR_SHIRT);

  // Los dos personajes sobre el muro
  sprFelix.pushSprite(36, MENU_WALL_Y - FELIX_SIZE, TFT_MAGENTA);
  sprRalph.pushSprite(172, MENU_WALL_Y - RALPH_SIZE);

  drawMenuOptions();
  drawHintPlaque();
  drawPressFixHint(true);
}

// Tabla de un modo dentro de un panel que empieza en `y`. `highlight` es el
// puesto a resaltar (-1 = ninguno).
void drawScoreTable(GameMode mode, int y, int highlight) {
  drawPanel(y, SCORE_TABLE_H);
  tft.setTextFont(1);
  tft.setTextSize(2);

  for (int i = 0; i < TOP_COUNT; i++) {
    const ScoreEntry &entry = topScores[mode][i];
    int lineY = y + 10 + i * 19;
    tft.setTextColor(i == highlight ? TFT_YELLOW : (entry.score > 0 ? TFT_WHITE : COLOR_STEEL));
    tft.setTextDatum(TL_DATUM);
    tft.drawNumber(i + 1, 32, lineY);
    tft.drawString(entry.name, 58, lineY);
    tft.setTextDatum(TR_DATUM);
    tft.drawNumber(entry.score, SCREEN_W - 32, lineY);
  }
}

// Pantalla de récords del menú: lo que cambia al pasar de un modo a otro
void drawScoresContent() {
  tft.fillRect(0, 60, SCREEN_W, 24, COLOR_SKY);
  drawShadowText(selectedMode == ONE_PLAYER ? "< 1 JUGADOR >" : "< 2 JUGADORES >", SCREEN_W / 2, 72, 2, TFT_WHITE, COLOR_DENIM_DARK);
  drawScoreTable(selectedMode, 92, -1);
  updateMenuOLED();
}

void drawScoresScreen() {
  drawBackdrop();
  drawShadowText("RECORDS", SCREEN_W / 2, 34, 4, TFT_YELLOW, COLOR_SHIRT);
  drawScoresContent();

  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(COLOR_GLASS_SHINE);
  tft.drawString("ARRIBA/ABAJO: CAMBIAR DE MODO", SCREEN_W / 2, 222);

  sprFelix.pushSprite((SCREEN_W - FELIX_SIZE) / 2, MENU_WALL_Y - FELIX_SIZE, TFT_MAGENTA);
  drawHintPlaque();
  drawPressFixHint(true);
}

// Una casilla de letra de la pantalla de nombre
void drawNameSlot(int slot) {
  bool selected = (slot == entrySlot);
  int x = NAME_SLOT_X + slot * NAME_SLOT_PITCH;
  int cx = x + NAME_SLOT_W / 2;
  int bottom = NAME_SLOT_Y + NAME_SLOT_H;

  // Flechas arriba y abajo solo en la casilla activa
  uint16_t arrow = selected ? TFT_YELLOW : COLOR_SKY;
  tft.fillTriangle(cx - 8, NAME_SLOT_Y - 6, cx + 8, NAME_SLOT_Y - 6, cx, NAME_SLOT_Y - 14, arrow);
  tft.fillTriangle(cx - 8, bottom + 6, cx + 8, bottom + 6, cx, bottom + 14, arrow);

  tft.fillRoundRect(x, NAME_SLOT_Y, NAME_SLOT_W, NAME_SLOT_H, 6, COLOR_ROOM);
  tft.drawRoundRect(x, NAME_SLOT_Y, NAME_SLOT_W, NAME_SLOT_H, 6, selected ? TFT_YELLOW : COLOR_STEEL);

  char letter[2] = {entryName[slot], '\0'};
  tft.setTextFont(1);
  tft.setTextSize(5);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(selected ? TFT_YELLOW : TFT_WHITE);
  tft.drawString(letter, cx + 2, NAME_SLOT_Y + NAME_SLOT_H / 2);
}

void drawNameEntryScreen() {
  drawBackdrop();

  drawShadowText(lastRank == 0 ? "NUEVO RECORD!" : "ENTRAS EN EL TOP!", SCREEN_W / 2, 30, 2, TFT_YELLOW, COLOR_SHIRT);

  char text[24];
  snprintf(text, sizeof(text), "PUESTO %d: %ld", lastRank + 1, felix.score);
  drawShadowText(text, SCREEN_W / 2, 58, 2, TFT_WHITE, COLOR_DENIM_DARK);

  for (int i = 0; i < NAME_LEN; i++) drawNameSlot(i);

  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(COLOR_GLASS_SHINE);
  tft.drawString("ARRIBA/ABAJO: LETRA", SCREEN_W / 2, 196);
  tft.drawString("IZQUIERDA/DERECHA: CASILLA", SCREEN_W / 2, 210);

  sprFelixFix.pushSprite((SCREEN_W - FELIX_SIZE) / 2, MENU_WALL_Y - FELIX_SIZE, TFT_MAGENTA);
  drawHintPlaque();
  drawHintText("FIX: GUARDAR", true);
}

// Cartel de pausa sobre el edificio; al reanudar se repinta el edificio entero
void drawPauseOverlay() {
  tft.fillRoundRect(36, 124, SCREEN_W - 72, 72, 8, COLOR_ROOM);
  tft.drawRoundRect(36, 124, SCREEN_W - 72, 72, 8, TFT_YELLOW);
  drawShadowText("PAUSA", SCREEN_W / 2, 150, 4, TFT_YELLOW, COLOR_SHIRT);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("PAUSA O FIX PARA SEGUIR", SCREEN_W / 2, 180);
}

void drawStageClearScreen() {
  drawBackdrop();

  drawShadowText("BIEN HECHO!", SCREEN_W / 2, 30, 2, TFT_WHITE, COLOR_DENIM_DARK);

  char text[16];
  snprintf(text, sizeof(text), "ETAPA %d", stage);
  drawShadowText(text, SCREEN_W / 2, 76, stage < 10 ? 5 : 4, TFT_YELLOW, COLOR_SHIRT);

  drawPanel(124, 76);
  drawPanelRow("BONO", SCORE_STAGE_BONUS, 138, TFT_YELLOW);
  drawPanelRow("PUNTOS", felix.score, 170, TFT_WHITE);

  sprFelixFix.pushSprite((SCREEN_W - FELIX_SIZE) / 2, MENU_WALL_Y - FELIX_SIZE, TFT_MAGENTA);

  drawHintPlaque();
  drawHintText("PREPARATE...", true);
}

void showGameOverScreen() {
  drawBackdrop();

  drawShadowText("GAME OVER", SCREEN_W / 2, 24, 3, TFT_WHITE, COLOR_SHIRT);
  if (activeMode == TWO_PLAYERS) {
    drawShadowText("GANA RALPH!", SCREEN_W / 2, 48, 1, TFT_YELLOW, COLOR_SHIRT);
  }

  drawPanel(58, 48);
  drawPanelRow("PUNTOS", felix.score, 66, TFT_WHITE);
  drawPanelRow("ETAPA", stage, 86, TFT_WHITE);

  // Tabla del modo jugado, con el puesto recién logrado resaltado
  drawScoreTable(activeMode, 112, lastRank);

  // Ralph celebra sobre el muro
  sprRalphThrow.pushSprite((SCREEN_W - RALPH_SIZE) / 2, MENU_WALL_Y - RALPH_SIZE);

  drawHintPlaque();
  drawPressFixHint(true);
}

// Panel de juego (128x32):
//   arriba:  puntos en grande | corazones de vida y etapa
//   abajo:   radar de rocas | ventanas rotas que quedan | barra de Ralph o martillo
const unsigned char ICON_HEART_FULL[] PROGMEM = {0x6C, 0xFE, 0xFE, 0x7C, 0x38, 0x10};
const unsigned char ICON_HEART_EMPTY[] PROGMEM = {0x6C, 0x92, 0x82, 0x44, 0x28, 0x10};

const int RADAR_CELL = 12;
const int RADAR_PITCH = 14;
const int RADAR_Y = 20;
const int OLED_BAR_X = 88;
const int OLED_BAR_W = 40;

// Una casilla por columna del edificio: marca sobre ella = Ralph (parpadea
// cuando va a lanzar), puntos dentro = rocas cayendo, base gruesa = Felix.
void drawOLEDRadar(unsigned long now) {
  bool blinkOn = (now / 100) % 2 == 0;

  for (int c = 0; c < COLS; c++) {
    int x = c * RADAR_PITCH;
    oled.drawRect(x, RADAR_Y, RADAR_CELL, RADAR_CELL, WHITE);
    if (c == felix.col) oled.fillRect(x, RADAR_Y + RADAR_CELL - 3, RADAR_CELL, 3, WHITE);
    if (c == ralph.col && (!ralphThrowing || blinkOn)) oled.fillRect(x + 2, RADAR_Y - 3, RADAR_CELL - 4, 2, WHITE);
  }

  for (int i = 0; i < MAX_ROCKS; i++) {
    if (!rocks[i].active) continue;
    int y = RADAR_Y + 1 + (int)((rocks[i].y - BUILD_Y) * (RADAR_CELL - 6) / BUILD_H);
    oled.fillRect(rocks[i].col * RADAR_PITCH + 4, constrain(y, RADAR_Y + 1, RADAR_Y + RADAR_CELL - 6), 4, 2, WHITE);
  }
}

void updateOLED() {
  unsigned long now = millis();
  oled.clearDisplay();
  oled.setTextColor(WHITE);

  // Puntos
  oled.setTextSize(2);
  oled.setCursor(0, 0);
  oled.print(felix.score);
  if (comboMultiplier() > 1) {
    oled.setTextSize(1);
    oled.setCursor(oled.getCursorX() + 2, 8);
    oled.print("x"); oled.print(comboMultiplier());
  }

  // Vidas y etapa, alineadas a la derecha
  for (int i = 0; i < 3; i++) {
    oled.drawBitmap(OLED_WIDTH - 34 + i * 12, 0, i < felix.lives ? ICON_HEART_FULL : ICON_HEART_EMPTY, 8, 6, WHITE);
  }
  char stageText[12];
  int len = snprintf(stageText, sizeof(stageText), "ETAPA %d", stage);
  oled.setTextSize(1);
  oled.setCursor(OLED_WIDTH - len * 6, 9);
  oled.print(stageText);

  drawOLEDRadar(now);

  // Ventanas que faltan por reparar
  int broken = 0;
  for (int c = 0; c < COLS; c++)
    for (int r = 0; r < ROWS; r++)
      if (building[c][r].state > 0) broken++;
  oled.drawRect(46, 21, 9, 11, WHITE);
  oled.drawFastVLine(50, 21, 11, WHITE);
  oled.drawFastHLine(46, 26, 9, WHITE);
  oled.setCursor(58, 23);
  oled.print(broken);

  // Barra: tiempo de martillo o de reloj que queda, o cuánto falta para la próxima roca
  int fill;
  if (felixHasHammer && now < hammerEndTime) {
    oled.fillRect(76, 22, 9, 4, WHITE);  // Icono de martillo
    oled.fillRect(79, 26, 3, 6, WHITE);
    fill = map(hammerEndTime - now, 0, TIME_POWERUP_HAMMER, 0, OLED_BAR_W - 4);
  } else if (slowActive(now)) {
    oled.drawCircle(80, 26, 4, WHITE);   // Icono de reloj
    oled.drawFastVLine(80, 23, 4, WHITE);
    oled.drawFastHLine(80, 26, 3, WHITE);
    fill = map(slowEndTime - now, 0, TIME_POWERUP_SLOW, 0, OLED_BAR_W - 4);
  } else {
    oled.fillCircle(80, 26, 4, WHITE);   // Icono de roca
    unsigned long sinceThrow = (now >= ralph.lastThrowTime) ? now - ralph.lastThrowTime : 0;
    unsigned long cooldown = ralphCooldown();
    fill = (sinceThrow >= cooldown) ? OLED_BAR_W - 4 : map(sinceThrow, 0, cooldown, 0, OLED_BAR_W - 4);
    // Ralph con los brazos en alto: la barra entera parpadea como aviso
    if (ralphThrowing) fill = ((now / 100) % 2 == 0) ? OLED_BAR_W - 4 : 0;
  }
  oled.drawRect(OLED_BAR_X, 21, OLED_BAR_W, 11, WHITE);
  oled.fillRect(OLED_BAR_X + 2, 23, fill, 7, WHITE);

  oled.display();
}

// OLED fuera de la partida: modo y récord en el menú, resultado al terminar
void updateMenuOLED() {
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);

  if (currentState == PAUSED) {
    oled.setTextSize(2);
    oled.setCursor(34, 0);
    oled.print("PAUSA");
    oled.setTextSize(1);
    oled.setCursor(0, 22);
    oled.print("Pts:"); oled.print(felix.score);
    oled.print("  Etapa:"); oled.print(stage);
  } else if (currentState == GAMEOVER || currentState == ENTER_NAME) {
    oled.setTextSize(2);
    oled.setCursor(10, 0);
    oled.print("GAME OVER");
    oled.setTextSize(1);
    oled.setCursor(0, 22);
    oled.print("Pts:"); oled.print(felix.score);
    if (lastRank >= 0) { oled.print("  Puesto:"); oled.print(lastRank + 1); }
    else { oled.print("  Etapa:"); oled.print(stage); }
  } else {
    oled.setCursor(28, 0);
    oled.print("FIX-IT FELIX");
    // Opción elegida en una barra invertida
    oled.fillRect(0, 10, OLED_WIDTH, 11, WHITE);
    oled.setTextColor(BLACK);
    oled.setCursor(4, 12);
    if (currentState == START && menuIndex == 2) oled.print("RECORDS");
    else oled.print(selectedMode == ONE_PLAYER ? "1 JUGADOR" : "2 JUGADORES");
    oled.setTextColor(WHITE);
    oled.setCursor(0, 24);
    oled.print("Record: ");
    oled.print(topScores[selectedMode][0].name);
    oled.print(" ");
    oled.print(bestScore(selectedMode));
  }
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

// Pantalla de nombre: arriba/abajo cambian la letra, izquierda/derecha la casilla
void handleNameEntry(const Inputs &in, bool fixPressed, unsigned long now) {
  if (fixPressed && now - nameEntryStart >= TIME_GAMEOVER_LOCK) {
    insertScore(activeMode, lastRank, entryName, felix.score);
    playSFX(NOTE_C5, 200);
    currentState = GAMEOVER;
    gameOverStart = now;
    showGameOverScreen();
    updateMenuOLED();
    return;
  }

  if (now - menuDebounceTime <= TIME_DEBOUNCE) return;

  if (in.up || in.down) {
    int index = strchr(NAME_CHARS, entryName[entrySlot]) - NAME_CHARS;
    index = (index + (in.up ? 1 : NAME_CHAR_COUNT - 1)) % NAME_CHAR_COUNT;
    entryName[entrySlot] = NAME_CHARS[index];
    drawNameSlot(entrySlot);
  } else if (in.left || in.right) {
    int previous = entrySlot;
    entrySlot = (entrySlot + (in.right ? 1 : NAME_LEN - 1)) % NAME_LEN;
    drawNameSlot(previous);
    drawNameSlot(entrySlot);
  } else {
    return;
  }
  menuDebounceTime = now;
  playSFX(NOTE_E4, 30);
}

void gameLoop() {
  platformPoll();

  unsigned long now = millis();
  Inputs in = {};
  bool inMatch = (currentState == PLAYING || currentState == PAUSED);
  readInputs(in, inMatch && activeMode == TWO_PLAYERS);
  bool fixPressed = in.fix && !prevFix;
  prevFix = in.fix;
  bool pausePressed = in.pause && !prevPause;
  prevPause = in.pause;

  handleAudio(now);
  updateLivesLEDs();

  switch (currentState) {
    case START:
      if (!startScreenDrawn) {
        drawStartScreen();
        startScreenDrawn = true;
      }
      updateHintBlink(now);
      if (now - menuDebounceTime <= TIME_MENU_DEBOUNCE) break;

      if (in.up || in.down) {
        menuIndex = (menuIndex + (in.down ? 1 : MENU_OPTIONS - 1)) % MENU_OPTIONS;
        if (menuIndex < 2) selectedMode = (menuIndex == 0) ? ONE_PLAYER : TWO_PLAYERS;
        drawMenuOptions();
        menuDebounceTime = now;
        playSFX(NOTE_E4, 50);
      } else if (fixPressed && menuIndex == 2) {
        playSFX(NOTE_C5, 80);
        currentState = SCORES;
        drawScoresScreen();
        menuDebounceTime = now;
      } else if (fixPressed) {
        PLAY(SFX_START);
        activeMode = selectedMode;
        initGame();
        enterPlaying();
      }
      break;

    case SCORES:
      updateHintBlink(now);
      if (fixPressed) {
        playSFX(NOTE_C5, 80);
        currentState = START;
        startScreenDrawn = false;
        menuDebounceTime = now;
      } else if ((in.up || in.down || in.left || in.right) && now - menuDebounceTime > TIME_MENU_DEBOUNCE) {
        selectedMode = (selectedMode == ONE_PLAYER) ? TWO_PLAYERS : ONE_PLAYER;
        drawScoresContent();
        menuDebounceTime = now;
        playSFX(NOTE_E4, 50);
      }
      break;

    case PLAYING:
      if (pausePressed) {
        beginPause(now);
        break;
      }

      handleFelixInput(in, fixPressed, now);
      if (currentState != PLAYING) break;

      handleRalph(in, now);
      updateTimedEffects(now);
      if (currentState != PLAYING) break;

      if (now - lastGameUpdate >= TICK_GAME) {
        lastGameUpdate = now;
        updateRocks(now);
        updateBird(now);
        checkCollisions();
        if (currentState != PLAYING) break;
        drawGame();
      }

      if (now - lastDisplayUpdate >= TICK_OLED) {
        updateOLED();
        lastDisplayUpdate = now;
      }
      break;

    case PAUSED:
      if (pausePressed || fixPressed) resumeFromPause(now);
      break;

    case STAGE_CLEAR:
      if (now - stageClearStart >= TIME_STAGE_SCREEN) {
        resetActors();
        generateStage();
        enterPlaying();
      }
      break;

    case ENTER_NAME:
      handleNameEntry(in, fixPressed, now);
      break;

    case GAMEOVER:
      updateHintBlink(now);
      if (fixPressed && now - gameOverStart >= TIME_GAMEOVER_LOCK) {
        felix.lives = 3;  // En el menú los tres LEDs vuelven a estar encendidos
        playSFX(NOTE_C5, 200);
        currentState = START;
        startScreenDrawn = false;
        menuDebounceTime = now;
      }
      break;
  }
}
