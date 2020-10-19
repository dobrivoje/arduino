

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

  asyncExecuteSignals(RED_LED_PIN, HIGH, 2000, &currentMillis_R, &previousMillis_R, LOW, 2500, &currentMillis_R_SIG2, &previousMillis_R_SIG2, threadSync1);
  asyncExecuteSignals(GREEN_LED_PIN, HIGH, 3000, &currentMillis_G, &previousMillis_G, LOW, 3500, &currentMillis_G_SIG2, &previousMillis_G_SIG2, threadSync3);
  asyncExecuteSignals(BLUE_LED_PIN, HIGH, 30, &currentMillis_B, &previousMillis_B, LOW, 2000, &currentMillis_B_SIG2, &previousMillis_B_SIG2, threadSync2);
}

void asyncExecuteSignals(int threadId, byte sig1Value, unsigned long sig1Duration, unsigned long* currentSig1Millis, unsigned long* previousSig1Millis
                         , byte sig2Value, unsigned long sig2Duration, unsigned long* currentSig2Millis, unsigned long* previousSig2Millis, boolean threadSync) {

  *currentSig1Millis = *currentSig2Millis = millis();

  unsigned long dDig1 = *currentSig1Millis - *previousSig1Millis;
  if (!threadSync && (dDig1 >= sig1Duration)) {
    *previousSig1Millis = *currentSig1Millis;
    threadSync = true;

    digitalWrite(threadId, sig2Value);
  }

  unsigned long dDig2 = *currentSig2Millis - *previousSig2Millis;
  if (threadSync && (dDig2 >= sig2Duration)) {
    *previousSig2Millis = *currentSig2Millis;
    threadSync = false;

    digitalWrite(threadId, sig1Value);
  }
}
