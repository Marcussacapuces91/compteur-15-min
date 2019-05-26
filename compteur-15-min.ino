#include "lib-ws2812b/lib-ws2812b.h"

enum {
  LED = LED_BUILTIN,
  RELAIS = 8,   // Position du relais : D8
  PUSH = 3,     // Position du bouton poussoir : D3
  DUREE = 10,   // Temps de décompte
  PRE = 1,
  PB_LED = 4,   // Port B : PB4 = D12
  MAX_LED = 12, // 12 leds sur une ligne
};


WS2812b<PB_LED, MAX_LED, 1> line;

void sonner(const unsigned aDuree) {
// Flash rouge
  //for (byte i = 0; i < MAX_LED ; ++i) line.setRGB(i, { 255,64,64 });
//  line.flush();

// Sonnerie
  digitalWrite(RELAIS, HIGH);
  const unsigned long st = millis();
  while (millis() - st < aDuree) {
    for (byte i = 0; i < MAX_LED ; ++i) line.setRGB(i, { 255,32,32 });
    line.flush();
    delay(50);
    for (byte i = 0; i < MAX_LED ; ++i) line.setRGB(i, { 0,0,0 });
    line.flush();
    delay(50);
  }
  digitalWrite(RELAIS, LOW);

}

void setup() {
  pinMode(RELAIS, OUTPUT);
  pinMode(PUSH, INPUT_PULLUP);
  
  line.setup();
}

void loop() {
  const unsigned long st = millis();
  while ((millis() - st) < (DUREE * 60000UL)) {
    const unsigned long t = (millis() - st) / 10;
//    Serial.println(t);

    switch (DUREE * 6000UL - t) {
      case PRE * 6000 : // PRE min de la fin
        sonner(200);
        break;
      case 5 * 100 :  // les 5 dernière secondes
      case 4 * 100 :
      case 3 * 100 :
      case 2 * 100 :
      case 1 * 100 :
        sonner(50);
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
    yield();
  }

  sonner(2500);
}
