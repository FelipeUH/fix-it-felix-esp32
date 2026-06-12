#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RCSwitch.h>
#include <Preferences.h>

// ================= PINES =================
#define PIN_RF 13
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
// Pantallas
const int OLED_WIDTH = 128;
const int OLED_HEIGHT = 32;
const int OLED_ADDR = 0x3C;
const int SCREEN_MAX_Y = 320;

// Dimensiones de Sprites y Offsets (Centro)
const int FELIX_SIZE = 24;
const int FELIX_OFFSET = 12;
const int RALPH_SIZE = 32;
const int RALPH_OFFSET = 16;
const int ROCK_SIZE = 12;
const int ROCK_OFFSET = 6;
const int HAMMER_SIZE = 16;

const int WIN_WIDTH = 40;
const int WIN_HEIGHT = 50;
const int WIN_OFFSET_X = 20;
const int WIN_OFFSET_Y = 30;
const int WIN_COLLISION_BOTTOM = 20; // Ajuste visual de la ventana inferior

// Dimensiones del Edificio (Fondo)
const int BUILD_X = 20;
const int BUILD_Y = 40;
const int BUILD_W = 200;
const int BUILD_H = 280;
const int RALPH_FIXED_Y = 10;

// Temporizadores e Intervalos (Milisegundos)
const unsigned long TICK_GAME = 30;
const unsigned long TICK_OLED = 200;
const unsigned long TIME_DEBOUNCE = 150;
const unsigned long TIME_MENU_DEBOUNCE = 200;
const unsigned long TIME_STAGE_DELAY = 2000;

// Balance de Cooldowns y Efectos
const unsigned long TIME_COOLDOWN_RALPH = 2000;
const unsigned long TIME_COOLDOWN_START = 1000; 
const unsigned long TIME_POWERUP_HAMMER = 4000;
const unsigned long TIME_INVINCIBLE = 1500;
const unsigned long TIME_FLICKER = 100;

// Balance de la IA (Retraso de reacción en ms)
const unsigned long IA_DELAY_MIN = 100;
const unsigned long IA_DELAY_MAX = 301;

// Físicas y Velocidad
const float ROCK_SPEED_BASE = 4.0;
const float ROCK_SPEED_INC = 0.5;
const float ROCK_SPEED_MAX = 10.0;
const int COLLISION_TOLERANCE = 15;

// Sistema de Puntuación
const long SCORE_FIX_PARTIAL = 50;
const long SCORE_FIX_FULL = 100;
const long SCORE_STAGE_BONUS = 500;

// Probabilidades de Eventos (Porcentajes %)
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

// Cuadrícula del juego
const int COLS = 3;
const int ROWS = 4;
const int colX[COLS] = {40, 120, 200};
const int rowY[ROWS] = {90, 160, 230, 300};

// Códigos de RF
const unsigned long RF_LEFT = 5735105;
const unsigned long RF_RIGHT = 5735106;
const unsigned long RF_THROW = 5735108;

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

int currentNote = 0;
unsigned long noteStartMillis = 0;
unsigned long sfxEndMillis = 0;

// ================= PANTALLAS Y OBJETOS GLOBALES =================
TFT_eSPI tft = TFT_eSPI();
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
RCSwitch rfSwitch = RCSwitch();
Preferences preferences; 

TFT_eSprite sprFelix = TFT_eSprite(&tft);
TFT_eSprite sprRalph = TFT_eSprite(&tft);
TFT_eSprite sprRock = TFT_eSprite(&tft);
TFT_eSprite sprWinFixed = TFT_eSprite(&tft);
TFT_eSprite sprWinBroken = TFT_eSprite(&tft);
TFT_eSprite sprWinFullyBroken = TFT_eSprite(&tft); 
TFT_eSprite sprHammer = TFT_eSprite(&tft);          

// ================= ESTRUCTURAS Y MODOS =================
enum GameState { START, PLAYING, GAMEOVER };
enum GameMode { ONE_PLAYER, TWO_PLAYERS };

GameState currentState = START;
GameMode selectedMode = ONE_PLAYER;  
GameMode activeMode = ONE_PLAYER;    

struct Window {
  int state; 
};

struct PlayerFelix {
  int col, row, oldCol, oldRow;
  int lives;
  long score;
  bool moved;
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
  float y, oldY;
};

// ================= VARIABLES DE ESTADO =================
Window building[COLS][ROWS];
PlayerFelix felix = {1, 3, 1, 3, 3, 0, true};
PlayerRalph ralph = {1, 1, 0, true, 1, 0, false};
Rock rocks[3] = {{false, 0, 0, 0}, {false, 0, 0, 0}, {false, 0, 0, 0}};

int stage = 1; 
long highScore = 0; 

bool isInvincible = false;
unsigned long invincibilityEndTime = 0;
unsigned long lastFlickerTime = 0;
bool flickerVisible = true;

bool hammerSpawned = false;
int hammerCol = 0, hammerRow = 0;
bool felixHasHammer = false;
unsigned long hammerEndTime = 0;

bool startScreenDrawn = false;
bool gameOverScreenDrawn = false;

unsigned long lastGameUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long btnDebounceTime = 0;

// ================= PROTOTIPOS DE FUNCIONES =================
void drawBuildingBase();
void drawWindow(int c, int r);
void updateOLED();
void drawStartScreen();
void showGameOverScreen();
void initGame();

// ================= SETUP =================
void setup() {
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

  rfSwitch.enableReceive(digitalPinToInterrupt(PIN_RF));
  rfSwitch.setReceiveTolerance(60);

  preferences.begin("felix_game", true); 
  highScore = preferences.getLong("highscore", 0); 
  preferences.end();

  if(!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Fallo OLED");
  }
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setTextSize(1);
  oled.setCursor(10, 10);
  oled.print("INICIANDO...");
  oled.display();

  tft.init();
  tft.setRotation(0);
  
  buildSprites();
}

// ================= GESTIÓN DE AUDIO =================
void playSFX(int freq, int duration) {
  tone(PIN_BUZZER, freq);
  sfxEndMillis = millis() + duration;
}

void handleAudio(unsigned long currentMillis) {
  if (currentMillis < sfxEndMillis) return; 

  if (currentState != PLAYING) {
    noTone(PIN_BUZZER);
    currentNote = 0;
    return;
  }

  if (currentMillis - noteStartMillis >= bgmDurations[currentNote]) {
    currentNote++;
    if (currentNote >= bgmLength) currentNote = 0;
    
    noteStartMillis = currentMillis;

    if (bgmNotes[currentNote] == REST) {
      noTone(PIN_BUZZER);
    } else {
      tone(PIN_BUZZER, bgmNotes[currentNote]);
    }
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
}

// ================= AVANCE DE ETAPA =================
void advanceStage() {
  noTone(PIN_BUZZER);
  tone(PIN_BUZZER, NOTE_C4); delay(120);
  tone(PIN_BUZZER, NOTE_E4); delay(120);
  tone(PIN_BUZZER, NOTE_G4); delay(120);
  tone(PIN_BUZZER, NOTE_C5); delay(300);
  noTone(PIN_BUZZER);

  stage++;
  felix.score += SCORE_STAGE_BONUS; 
  
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(40, 110);
  tft.print("ETAPA ");
  tft.print(stage);
  
  tft.setTextSize(2);
  tft.setCursor(35, 180);
  tft.print("+"); tft.print(SCORE_STAGE_BONUS); tft.print(" PTS BONO");
  
  delay(TIME_STAGE_DELAY); 
  
  felix.col = 1; felix.row = 3; felix.oldCol = 1; felix.oldRow = 3; felix.moved = true;
  ralph.col = 1; ralph.oldCol = 1; ralph.moved = true;
  ralph.delayActive = false;
  
  unsigned long initialOffset = TIME_COOLDOWN_RALPH - TIME_COOLDOWN_START;
  if (millis() >= initialOffset) {
    ralph.lastThrowTime = millis() - initialOffset;
  } else {
    ralph.lastThrowTime = 0;
  }

  for(int i=0; i<3; i++) rocks[i].active = false;
  
  int brokenChance = PROB_BROKEN_BASE + (stage * PROB_BROKEN_INC);
  if (brokenChance > PROB_BROKEN_MAX) brokenChance = PROB_BROKEN_MAX;
  
  int doubleBrokenChance = PROB_DBL_BROKEN_BASE + (stage * PROB_DBL_BROKEN_INC); 
  if (doubleBrokenChance > PROB_DBL_BROKEN_MAX) doubleBrokenChance = PROB_DBL_BROKEN_MAX;
  
  for(int c=0; c<COLS; c++) {
    for(int r=0; r<ROWS; r++) {
      if (random(100) < brokenChance) {
        building[c][r].state = (random(100) < doubleBrokenChance) ? 2 : 1;
      } else {
        building[c][r].state = 0; 
      }
    }
  }

  int hammerChance = PROB_HAMMER_BASE + (stage * PROB_HAMMER_INC);
  if (hammerChance > PROB_HAMMER_MAX) hammerChance = PROB_HAMMER_MAX;

  if (random(100) < hammerChance) {
    hammerSpawned = true;
    hammerCol = random(COLS);
    hammerRow = random(ROWS);
  } else {
    hammerSpawned = false;
  }
  
  felixHasHammer = false;
  hammerEndTime = 0;

  if (hammerSpawned && felix.col == hammerCol && felix.row == hammerRow) {
    hammerSpawned = false;
    felixHasHammer = true;
    hammerEndTime = millis() + TIME_POWERUP_HAMMER;
  }
  
  tft.fillScreen(TFT_BLACK);
  drawBuildingBase();
  updateOLED();
  
  noteStartMillis = millis(); 
}

void checkStageClear() {
  bool allFixed = true;
  for(int c=0; c<COLS; c++) {
    for(int r=0; r<ROWS; r++) {
      if(building[c][r].state > 0) {
        allFixed = false;
        break;
      }
    }
    if(!allFixed) break;
  }
  if(allFixed) advanceStage();
}

// ================= LOOP PRINCIPAL =================
void loop() {
  unsigned long currentMillis = millis();
  handleAudio(currentMillis);

  switch(currentState) {
    case START:
      if (!startScreenDrawn) {
        drawStartScreen();
        startScreenDrawn = true;
      }

      if (currentMillis - btnDebounceTime > TIME_MENU_DEBOUNCE) {
        if (digitalRead(BTN_UP) == LOW || digitalRead(BTN_DOWN) == LOW) {
          selectedMode = (selectedMode == ONE_PLAYER) ? TWO_PLAYERS : ONE_PLAYER;
          drawStartScreen();
          btnDebounceTime = currentMillis;
          playSFX(NOTE_E4, 50); 
        }

        if(digitalRead(BTN_FIX) == LOW) {
          playSFX(NOTE_C5, 200); 
          activeMode = selectedMode; 
          initGame(); 
          currentState = PLAYING;
          tft.fillScreen(TFT_BLACK);
          drawBuildingBase();
          updateOLED();
          btnDebounceTime = currentMillis;
          delay(300);
        }
      }
      break;

    case PLAYING:
      handleFelixInput();
      
      currentMillis = millis(); // Actualizar tiempo tras input
      handleRalphInput(currentMillis);

      if (felixHasHammer && currentMillis >= hammerEndTime) {
        felixHasHammer = false;
        drawWindow(felix.col, felix.row); 
      }

      if (isInvincible) {
        if (currentMillis >= invincibilityEndTime) {
          isInvincible = false;
          flickerVisible = true; 
        } else {
          if (currentMillis - lastFlickerTime > TIME_FLICKER) { 
            flickerVisible = !flickerVisible;
            lastFlickerTime = currentMillis;
          }
        }
      }

      if(currentMillis - lastGameUpdate > TICK_GAME) {
        updateRocks();
        checkCollisions();
        drawGame();
        lastGameUpdate = currentMillis;
      }
      
      if(currentMillis - lastDisplayUpdate > TICK_OLED) {
        updateOLED();
        lastDisplayUpdate = currentMillis;
      }
      break;

    case GAMEOVER:
      if (!gameOverScreenDrawn) {
        showGameOverScreen();
        gameOverScreenDrawn = true;
      }

      if(currentMillis - btnDebounceTime > TIME_MENU_DEBOUNCE && digitalRead(BTN_FIX) == LOW) {
        playSFX(NOTE_C5, 200);
        currentState = START;
        startScreenDrawn = false; 
        btnDebounceTime = currentMillis;
        delay(300);
      }
      break;
  }
}

// ================= LÓGICA DEL JUEGO =================
void initGame() {
  stage = 1; 
  isInvincible = false;
  flickerVisible = true;
  felixHasHammer = false;
  hammerEndTime = 0;
  
  felix.col = 1; felix.row = 3; felix.oldCol = 1; felix.oldRow = 3;
  felix.lives = 3; felix.score = 0; felix.moved = true;
  
  ralph.col = 1; ralph.oldCol = 1; ralph.moved = true;
  ralph.delayActive = false;

  unsigned long initialOffset = TIME_COOLDOWN_RALPH - TIME_COOLDOWN_START;
  if (millis() >= initialOffset) {
    ralph.lastThrowTime = millis() - initialOffset;
  } else {
    ralph.lastThrowTime = 0;
  }
  
  for(int i=0; i<3; i++) {
    rocks[i].active = false;
    rocks[i].oldY = 0;
  }
  
  for(int c=0; c<COLS; c++) {
    for(int r=0; r<ROWS; r++) {
      if (random(100) < PROB_BROKEN_BASE) {
        building[c][r].state = (random(100) < PROB_DBL_BROKEN_BASE) ? 2 : 1; 
      } else {
        building[c][r].state = 0;
      }
    }
  }

  if (random(100) < PROB_HAMMER_STAGE_1) {
    hammerSpawned = true;
    hammerCol = random(COLS);
    hammerRow = random(ROWS);
  } else {
    hammerSpawned = false;
  }

  if (hammerSpawned && felix.col == hammerCol && felix.row == hammerRow) {
    hammerSpawned = false;
    felixHasHammer = true;
    hammerEndTime = millis() + TIME_POWERUP_HAMMER;
  }

  updateLivesLEDs();
}

void handleFelixInput() {
  if (millis() - btnDebounceTime < TIME_DEBOUNCE) return; 

  bool changed = false;
  felix.oldCol = felix.col;
  felix.oldRow = felix.row;

  if (digitalRead(BTN_LEFT) == LOW && felix.col > 0) { felix.col--; changed = true; }
  else if (digitalRead(BTN_RIGHT) == LOW && felix.col < COLS-1) { felix.col++; changed = true; }
  else if (digitalRead(BTN_UP) == LOW && felix.row > 0) { felix.row--; changed = true; }
  else if (digitalRead(BTN_DOWN) == LOW && felix.row < ROWS-1) { felix.row++; changed = true; }
  else if (digitalRead(BTN_FIX) == LOW) {
    if (building[felix.col][felix.row].state > 0) {
      building[felix.col][felix.row].state--; 
      
      if (building[felix.col][felix.row].state == 0) {
        felix.score += SCORE_FIX_FULL; 
        checkStageClear(); 
      } else {
        felix.score += SCORE_FIX_PARTIAL; 
      }
      
      drawWindow(felix.col, felix.row); 
      playSFX(NOTE_G4, 150); 
    }
    changed = true;
  }

  if (changed) {
    felix.moved = true;
    btnDebounceTime = millis();

    if (hammerSpawned && felix.col == hammerCol && felix.row == hammerRow) {
      hammerSpawned = false;
      felixHasHammer = true;
      hammerEndTime = millis() + TIME_POWERUP_HAMMER; 
      playSFX(NOTE_C5, 400);
      drawWindow(hammerCol, hammerRow); 
    }
  }
}

void handleRalphInput(unsigned long currentMillis) {
  if (activeMode == TWO_PLAYERS) {
    if (rfSwitch.available()) {
      unsigned long value = rfSwitch.getReceivedValue();
      ralph.oldCol = ralph.col;

      if (value == RF_LEFT && ralph.col > 0) { ralph.col--; ralph.moved = true; }
      else if (value == RF_RIGHT && ralph.col < COLS-1) { ralph.col++; ralph.moved = true; }
      else if (value == RF_THROW) {
        if (currentMillis >= ralph.lastThrowTime && (currentMillis - ralph.lastThrowTime >= TIME_COOLDOWN_RALPH)) { 
          spawnRock();
          ralph.lastThrowTime = currentMillis;
        }
      }
      rfSwitch.resetAvailable();
    }
  } else {
    if (felix.col != ralph.col) {
      if (!ralph.delayActive || ralph.targetCol != felix.col) {
        ralph.targetCol = felix.col;
        ralph.nextMoveTime = currentMillis + random(IA_DELAY_MIN, IA_DELAY_MAX); 
        ralph.delayActive = true;
      }
    }

    if (ralph.delayActive && currentMillis >= ralph.nextMoveTime) {
      ralph.oldCol = ralph.col;
      ralph.col = ralph.targetCol;
      ralph.moved = true;
      ralph.delayActive = false;
    }

    if (currentMillis >= ralph.lastThrowTime && (currentMillis - ralph.lastThrowTime >= TIME_COOLDOWN_RALPH)) {
      spawnRock();
      ralph.lastThrowTime = currentMillis;
    }
  }
}

void spawnRock() {
  for(int i=0; i<3; i++) {
    if(!rocks[i].active) {
      rocks[i].active = true;
      rocks[i].col = ralph.col;
      rocks[i].y = BUILD_Y; 
      rocks[i].oldY = BUILD_Y;
      break;
    }
  }
}

void cleanRockTrail(int col, float oldY) {
  int x = colX[col] - ROCK_OFFSET;
  int y = (int)oldY - ROCK_OFFSET;
  
  if(oldY < BUILD_Y) {
    tft.fillRect(x, y, ROCK_SIZE, ROCK_SIZE, TFT_BLACK);
  } else {
    bool overWindow = false;
    for(int r=0; r<ROWS; r++) {
      int winTop = rowY[r] - WIN_OFFSET_Y;
      int winBottom = rowY[r] + WIN_COLLISION_BOTTOM;
      if (oldY >= winTop && oldY <= winBottom) {
        drawWindow(col, r);
        overWindow = true;
        break;
      }
    }
    if(!overWindow) tft.fillRect(x, y, ROCK_SIZE, ROCK_SIZE, TFT_BROWN);
  }
}

void updateRocks() {
  float currentSpeed = ROCK_SPEED_BASE + (stage * ROCK_SPEED_INC);
  if (currentSpeed > ROCK_SPEED_MAX) currentSpeed = ROCK_SPEED_MAX;

  for(int i=0; i<3; i++) {
    if(rocks[i].active) {
      cleanRockTrail(rocks[i].col, rocks[i].oldY);
      rocks[i].oldY = rocks[i].y;
      rocks[i].y += currentSpeed; 
      
      if(rocks[i].y > SCREEN_MAX_Y) {
        rocks[i].active = false;
      }
    }
  }
}

void checkCollisions() {
  if (isInvincible || felixHasHammer) return; 

  for(int i=0; i<3; i++) {
    if(rocks[i].active) {
      if(rocks[i].col == felix.col && abs(rocks[i].y - rowY[felix.row]) < COLLISION_TOLERANCE) {
        rocks[i].active = false;
        cleanRockTrail(rocks[i].col, rocks[i].y);
        felix.lives--;
        updateLivesLEDs();
        
        if(felix.lives <= 0) {
          playSFX(150, 1000); 
          
          if (felix.score > highScore) {
            highScore = felix.score;
            preferences.begin("felix_game", false); 
            preferences.putLong("highscore", highScore);
            preferences.end();
          }

          currentState = GAMEOVER;
          gameOverScreenDrawn = false;
        } else {
          playSFX(200, 300); 
          isInvincible = true;
          invincibilityEndTime = millis() + TIME_INVINCIBLE; 
          lastFlickerTime = millis();
          flickerVisible = false; 
        }
      }
    }
  }
}

void updateLivesLEDs() {
  digitalWrite(LED_LIFE_1, felix.lives >= 1 ? HIGH : LOW);
  digitalWrite(LED_LIFE_2, felix.lives >= 2 ? HIGH : LOW);
  digitalWrite(LED_LIFE_3, felix.lives >= 3 ? HIGH : LOW);
}

// ================= RENDERIZADO TFT =================
void drawBuildingBase() {
  tft.fillRect(BUILD_X, BUILD_Y, BUILD_W, BUILD_H, TFT_BROWN);
  for(int c=0; c<COLS; c++) {
    for(int r=0; r<ROWS; r++) {
      drawWindow(c, r);
    }
  }
}

void drawWindow(int c, int r) {
  int x = colX[c] - WIN_OFFSET_X;
  int y = rowY[r] - WIN_OFFSET_Y;
  
  if(building[c][r].state == 2) {
    sprWinFullyBroken.pushSprite(x, y);
  } else if(building[c][r].state == 1) {
    sprWinBroken.pushSprite(x, y);
  } else {
    sprWinFixed.pushSprite(x, y);
  }

  if (hammerSpawned && hammerCol == c && hammerRow == r) {
    sprHammer.pushSprite(x + 12, y + 17, TFT_MAGENTA);
  }
}

void drawGame() {
  if(ralph.moved) {
    tft.fillRect(colX[ralph.oldCol]-RALPH_OFFSET, RALPH_FIXED_Y, RALPH_SIZE, RALPH_SIZE, TFT_BLACK);
    sprRalph.pushSprite(colX[ralph.col]-RALPH_OFFSET, RALPH_FIXED_Y, TFT_MAGENTA);
    ralph.moved = false;
  }

  if(felix.moved) {
    drawWindow(felix.oldCol, felix.oldRow);
    if (flickerVisible) {
      sprFelix.pushSprite(colX[felix.col]-FELIX_OFFSET, rowY[felix.row]-FELIX_OFFSET, TFT_MAGENTA);
      if (felixHasHammer) tft.fillRect(colX[felix.col]-4, rowY[felix.row]-22, 8, 4, TFT_YELLOW); 
    } else {
      drawWindow(felix.col, felix.row); 
    }
    felix.moved = false;
  } else {
    if (flickerVisible) {
      sprFelix.pushSprite(colX[felix.col]-FELIX_OFFSET, rowY[felix.row]-FELIX_OFFSET, TFT_MAGENTA);
      if (felixHasHammer) tft.fillRect(colX[felix.col]-4, rowY[felix.row]-22, 8, 4, TFT_YELLOW);
    } else {
      drawWindow(felix.col, felix.row);
    }
  }

  for(int i=0; i<3; i++) {
    if(rocks[i].active) {
      sprRock.pushSprite(colX[rocks[i].col]-ROCK_OFFSET, (int)rocks[i].y-ROCK_OFFSET, TFT_MAGENTA);
    }
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
  tft.print("RECORD MAXIMO: "); tft.print(highScore);
  
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  
  tft.setCursor(30, 165);
  if (selectedMode == ONE_PLAYER) tft.print("> 1 JUGADOR");
  else tft.print("  1 JUGADOR");
  
  tft.setCursor(30, 205);
  if (selectedMode == TWO_PLAYERS) tft.print("> 2 JUGADORES");
  else tft.print("  2 JUGADORES");
  
  tft.setTextSize(1);
  tft.setCursor(25, 270);
  tft.print("Arriba/Abajo para elegir");
  tft.setCursor(25, 290);
  tft.print("Presiona FIX para entrar");
}

void showGameOverScreen() {
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(40, 90);
  tft.print("GAME OVER");
  
  tft.setTextSize(2);
  tft.setCursor(40, 150);
  tft.print("Pts: ");
  tft.print(felix.score);
  
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(40, 190);
  tft.print("Max: ");
  tft.print(highScore);
  
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(40, 250);
  tft.print("Presiona FIX");
}

// ================= RENDERIZADO OLED =================
void updateOLED() {
  oled.clearDisplay();
  
  oled.setCursor(0,0);
  oled.print("P:"); oled.print(felix.score);
  oled.setCursor(55,0);
  oled.print("E:"); oled.print(stage);
  oled.setCursor(95,0);
  oled.print("V:"); oled.print(felix.lives);

  if (felixHasHammer && millis() < hammerEndTime) {
    oled.setCursor(0, 16);
    oled.print("MARTILLO: ");
    oled.print((hammerEndTime - millis()) / 1000 + 1);
    oled.print("s    "); 
  } else {
    oled.setCursor(0, 16);
    oled.print("Ralph Cooldown:");
  }
  
  unsigned long now = millis();
  unsigned long timeSinceThrow = 0;
  if (now >= ralph.lastThrowTime) {
    timeSinceThrow = now - ralph.lastThrowTime;
  }
  
  int barWidth = 0;
  if(timeSinceThrow < TIME_COOLDOWN_RALPH) {
    barWidth = map(timeSinceThrow, 0, TIME_COOLDOWN_RALPH, 0, OLED_WIDTH);
  } else {
    barWidth = OLED_WIDTH; 
  }
  oled.drawRect(0, 26, OLED_WIDTH, 6, WHITE);
  oled.fillRect(0, 26, barWidth, 6, WHITE);
  
  oled.display();
}