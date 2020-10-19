
#include <util/atomic.h>
// #include <math.h>
// #include <LiquidCrystal.h>

uint8_t RED_LED_PIN  = 8;
unsigned long currentMillis_R = 0;
unsigned long previousMillis_R = 0;
unsigned long currentMillis_R_SIG2 = 0;
unsigned long previousMillis_R_SIG2 = 0;

uint8_t GREEN_LED_PIN = 9;
unsigned long currentMillis_G = 0;
unsigned long previousMillis_G = 0;
unsigned long currentMillis_G_SIG2 = 0;
unsigned long previousMillis_G_SIG2 = 0;

uint8_t BLUE_LED_PIN = 10;
unsigned long currentMillis_B = 0;
unsigned long previousMillis_B = 0;
unsigned long currentMillis_B_SIG2 = 0;
unsigned long previousMillis_B_SIG2 = 0;

boolean threadSync1 = false;
boolean threadSync2 = false;
boolean threadSync3 = false;

enum digitalPin {
  RED1 = 2312,
  GREEN2 = 7975
};

void setup() {
  Serial.begin(9600);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
}

void loop() {
  Serial.println(digitalPin(RED1));

  // digitalWriteToPinAsync(RED_LED_PIN, 5000, 1000, &currentMillis_R, &previousMillis_R);
  asyncExecuteSignals(RED_LED_PIN, HIGH, 2000, &currentMillis_R, &previousMillis_R, LOW, 2500, &currentMillis_R_SIG2, &previousMillis_R_SIG2, threadSync1);

  // digitalWriteToPinAsync(GREEN_LED_PIN, 5000, 1500, &currentMillis_G, &previousMillis_G);
  asyncExecuteSignals(GREEN_LED_PIN, HIGH, 2000, &currentMillis_G, &previousMillis_G, LOW, 2500, &currentMillis_G_SIG2, &previousMillis_G_SIG2, threadSync3);

  // digitalWriteToPinAsync(BLUE_LED_PIN, 5000, 3000, &currentMillis_B, &previousMillis_B);
  asyncExecuteSignals(BLUE_LED_PIN, HIGH, 300, &currentMillis_B, &previousMillis_B, LOW, 1500, &currentMillis_B_SIG2, &previousMillis_B_SIG2, threadSync2);
}

void digitalWriteToPinAsync(int pinId, long offDuration, long onDuration,  unsigned long* currentMillis, unsigned long* previousMillis) {
  asyncExecute(pinId, onDuration, currentMillis, previousMillis);
  // alternateDigitalPin(pinId, offDuration, onDuration);
}

void asyncExecute(int threadId, unsigned long duration, unsigned long* currentMillis, unsigned long* previousMillis) {
  *currentMillis = millis();

  if (*currentMillis - *previousMillis >= duration) {
    *previousMillis = *currentMillis;

    /*
      Serial.print("Thread id: "); Serial.print(threadId); Serial.println(" ");
      Serial.print("curr mills: "); Serial.print(*currentMillis); Serial.print(" ("); Serial.print((unsigned int)currentMillis, HEX); Serial.println(")");
      Serial.print("prev mills: "); Serial.print(*previousMillis); Serial.print(" "); Serial.print(" ("); Serial.print((unsigned int)previousMillis, HEX); Serial.println(")");
      Serial.print("interval: "); Serial.println(duration);
    */
    digitalWrite(threadId, HIGH);
  } else {
    digitalWrite(threadId, LOW);
  }
}

void asyncExecuteSignals(int threadId, byte sig1Value, unsigned long sig1Duration, unsigned long* currentSig1Millis, unsigned long* previousSig1Millis
                         , byte sig2Value, unsigned long sig2Duration, unsigned long* currentSig2Millis, unsigned long* previousSig2Millis, boolean threadSync) {
  *currentSig1Millis = *currentSig2Millis = millis();

  if (!threadSync && (*currentSig1Millis - *previousSig1Millis >= sig1Duration)) {
    *previousSig1Millis = *currentSig1Millis;
    threadSync = true;

    digitalWrite(threadId, sig2Value);
  }

  if (threadSync && (*currentSig2Millis - *previousSig2Millis >= sig2Duration)) {
    *previousSig2Millis = *currentSig2Millis;
    threadSync = false;

    digitalWrite(threadId, sig1Value);
  }
}

void digitalWriteToPin(int pinId, byte value, long duration) {
  digitalWrite(pinId, value);
  delay(duration);
}

void alternateDigitalPin(int pinId, long offDuration, long onDuration) {
  digitalWriteToPin(pinId, HIGH, offDuration);
  digitalWriteToPin(pinId, LOW, onDuration);
}

void swap(int* a, int* b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}
