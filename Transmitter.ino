#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// ================= RECEIVER MAC =================
uint8_t receiverMac[] = {0xC0, 0xCD, 0xD6, 0x8E, 0x38, 0x6C};

// ================= JOYSTICK =================
#define VRX_PIN 34
#define VRY_PIN 35
#define BTN_PIN 32

#define ADC_MAX 4095
#define DEADZONE 350
#define OUT_MAX 1000

// ================= BUZZER =================
#define BUZZER_PIN 25

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ================= CONNECTION STATE =================
bool connected = false;
bool everConnected = false;
unsigned long lastPacket = 0;

// -------- JOYSTICK CALIB --------
int centerX, centerY;
int maxUp, maxDown, maxLeft, maxRight;

// ================= DATA STRUCT =================
typedef struct {
  int motorA;
  int motorB;
  int button;
} struct_message;

struct_message tx;

// ================= BUZZER =================
void beep(int times)
{
  for(int i=0;i<times;i++)
  {
    digitalWrite(BUZZER_PIN,HIGH);
    delay(120);
    digitalWrite(BUZZER_PIN,LOW);
    delay(150);
  }
}

// ================= DISPLAY =================
void updateDisplay()
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10,25);

  if(connected)
    display.println("Connected");
  else
    display.println("No Link");

  display.display();
}

// ================= ESP-NOW CALLBACK =================
void onSent(const wifi_tx_info_t *info, esp_now_send_status_t status)
{
  if(status == ESP_NOW_SEND_SUCCESS)
  {
    lastPacket = millis();

    if(!connected)
    {
      connected = true;
      everConnected = true;
      beep(1);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  analogReadResolution(12);

  // ================= OLED INIT (YOUR WORKING CODE STYLE) =================
  Wire.begin(21,22);

  if(!display.begin(0x3C, true))
  {
    Serial.println("OLED not detected");
    while(1);
  }

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10,25);
  display.println("Echo 01");
  display.display();

  delay(3000);

  // -------- JOYSTICK CALIBRATION --------
  long sx = 0, sy = 0;
  for (int i = 0; i < 200; i++)
  {
    sx += analogRead(VRX_PIN);
    sy += analogRead(VRY_PIN);
    delay(5);
  }

  centerX = sx / 200;
  centerY = sy / 200;

  maxUp = maxDown = maxLeft = maxRight = 1;

  Serial.println("Move joystick fully in all directions...");

  // ================= ESP-NOW =================
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMac, 6);
  peer.channel = 1;
  peer.encrypt = false;

  if (esp_now_add_peer(&peer) != ESP_OK)
  {
    Serial.println("Failed to add peer");
  }
}

void loop()
{
  int rawX = analogRead(VRX_PIN);
  int rawY = analogRead(VRY_PIN);

  // -------- AUTO LIMIT LEARNING --------
  maxUp    = max(maxUp, centerY - rawY);
  maxDown  = max(maxDown, rawY - centerY);
  maxLeft  = max(maxLeft, centerX - rawX);
  maxRight = max(maxRight, rawX - centerX);

  int x = rawX - centerX;
  int y = rawY - centerY;

  int up = 0, down = 0, left = 0, right = 0;

  int absX = abs(x);
  int absY = abs(y);

  if (absY > absX && absY > DEADZONE)
  {
    if (y < 0)
      up = map(absY, DEADZONE, maxUp, 0, OUT_MAX);
    else
      down = map(absY, DEADZONE, maxDown, 0, OUT_MAX);
  }
  else if (absX > DEADZONE)
  {
    if (x > 0)
      right = map(absX, DEADZONE, maxRight, 0, OUT_MAX);
    else
      left = map(absX, DEADZONE, maxLeft, 0, OUT_MAX);
  }

  up    = constrain(up, 0, OUT_MAX);
  down  = constrain(down, 0, OUT_MAX);
  left  = constrain(left, 0, OUT_MAX);
  right = constrain(right, 0, OUT_MAX);

  // ================= RC MIXING =================
  int throttle = right - left;
  int steering = up - down;

  tx.motorA = constrain(throttle + steering, -OUT_MAX, OUT_MAX);
  tx.motorB = constrain(throttle - steering, -OUT_MAX, OUT_MAX);
  tx.button = (digitalRead(BTN_PIN) == LOW) ? 1 : 0;

  esp_now_send(receiverMac, (uint8_t *)&tx, sizeof(tx));

  // ================= CONNECTION LOSS =================
  if(everConnected && millis() - lastPacket > 2000)
  {
    if(connected)
    {
      connected = false;
      beep(3);
    }
  }

  updateDisplay();

  delay(60);
}