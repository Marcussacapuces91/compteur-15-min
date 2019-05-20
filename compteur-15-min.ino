#include "lib-ws2812b/lib-ws2812b.h"

enum {
  LED = LED_BUILTIN,
  RELAIS = 8,   // Position du relais : D8
  PUSH = 3,     // Position du bouton poussoir : D3
  DUREE = 15,   // Temps de décompte
  PB_LED = 4,   // Port B : PB4 = D12
  MAX_LED = 12, // 12 leds sur une ligne
};


WS2812b<PB_LED, MAX_LED, 1> line;

void setup() {
  pinMode(RELAIS, OUTPUT);
  pinMode(PUSH, INPUT_PULLUP);
  
  line.setup();

  Serial.begin(115200);
}

void loop() {
  const unsigned long st = millis();
  while ((millis() - st) < (DUREE * 60000UL)) {
    const unsigned long t = (millis() - st) / 10;
    Serial.println(t);

    switch (DUREE * 6000UL - t) {
      case 5 * 6000 : // 5 min de la fin
        digitalWrite(RELAIS, HIGH);
        delay(200);
        digitalWrite(RELAIS, LOW);
        break;
      case 5 * 100 :  // les 5 dernière secondes
      case 4 * 100 :
      case 3 * 100 :
      case 2 * 100 :
      case 1 * 100 :
        digitalWrite(RELAIS, HIGH);
        delay(50);
        digitalWrite(RELAIS, LOW);
        break;
    }

    for (byte i = 0; i < MAX_LED ; ++i) { 
      const unsigned long h = (6U * 256) * 1000UL + i * (6U * 256) / MAX_LED - (t * 6U * 256) / 6000;
      line.setHSV(
        (i + 6) % MAX_LED, 
        h % (6U * 256),
        0xFF, 
        i + t * MAX_LED / DUREE / 6000 < MAX_LED ? 0x20 : 0x00
       );
    }
    line.flush();
  }

  digitalWrite(RELAIS, HIGH);
  delay(2500);
  digitalWrite(RELAIS, LOW);
}
