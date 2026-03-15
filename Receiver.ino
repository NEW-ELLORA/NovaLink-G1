#include <esp_now.h>
#include <WiFi.h>

// Motor A
#define A_RPWM 32
#define A_LPWM 33
#define A_R_EN 26
#define A_L_EN 25

// Motor B
#define B_RPWM 12
#define B_LPWM 13
#define B_R_EN 14
#define B_L_EN 27

// Data structure (same as transmitter)
typedef struct struct_message {
  int motorA;
  int motorB;
  int button;
} struct_message;

struct_message rxData;

// ================= RECEIVE DATA =================
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len)
{
  memcpy(&rxData, incomingData, sizeof(rxData));

  Serial.print("MotorA: ");
  Serial.print(rxData.motorA);
  Serial.print("  MotorB: ");
  Serial.println(rxData.motorB);

  controlMotorA(rxData.motorA);
  controlMotorB(rxData.motorB);
}

// ================= MOTOR A =================
void controlMotorA(int speed)
{
  int pwm = map(abs(speed), 0, 1000, 0, 255);

  if (speed > 0) {
    ledcWrite(A_RPWM, pwm);
    ledcWrite(A_LPWM, 0);
  }
  else if (speed < 0) {
    ledcWrite(A_RPWM, 0);
    ledcWrite(A_LPWM, pwm);
  }
  else {
    ledcWrite(A_RPWM, 0);
    ledcWrite(A_LPWM, 0);
  }
}

// ================= MOTOR B =================
void controlMotorB(int speed)
{
  int pwm = map(abs(speed), 0, 1000, 0, 255);

  if (speed > 0) {
    ledcWrite(B_RPWM, pwm);
    ledcWrite(B_LPWM, 0);
  }
  else if (speed < 0) {
    ledcWrite(B_RPWM, 0);
    ledcWrite(B_LPWM, pwm);
  }
  else {
    ledcWrite(B_RPWM, 0);
    ledcWrite(B_LPWM, 0);
  }
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  pinMode(A_R_EN, OUTPUT);
  pinMode(A_L_EN, OUTPUT);
  pinMode(B_R_EN, OUTPUT);
  pinMode(B_L_EN, OUTPUT);

  digitalWrite(A_R_EN, HIGH);
  digitalWrite(A_L_EN, HIGH);
  digitalWrite(B_R_EN, HIGH);
  digitalWrite(B_L_EN, HIGH);

  // Attach PWM to pins
  ledcAttach(A_RPWM, 20000, 8);
  ledcAttach(A_LPWM, 20000, 8);
  ledcAttach(B_RPWM, 20000, 8);
  ledcAttach(B_LPWM, 20000, 8);

  Serial.println("Receiver Ready");
}

void loop()
{
}