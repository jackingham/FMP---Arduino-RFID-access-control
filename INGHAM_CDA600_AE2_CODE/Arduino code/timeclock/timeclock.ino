#include <Ethernet.h>
#include <FastLED.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

#define NUM_LEDS 8
#define DATA_PIN A0

CRGB leds[NUM_LEDS];

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          8         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x65, 0x15 }; //arduino mac
IPAddress ip(169, 254, 33, 154); //arduino IP
char server[] = "169.254.33.153"; //server/database IP

String rcv; //response is stored here

EthernetClient client;
int port = 80; //server port

String currTag;

void setup() {
  pinMode(A0, OUTPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Black;
  }
  FastLED.show();
  SPI.begin();      // Init SPI bus
  Ethernet.begin(mac, ip);
  Serial.println("Setting up...");
  mfrc522.PCD_Init();   // Init MFRC522
  delay(5000);
  Serial.println("Ready");
}


void loop() {

  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Blue;
  }
  FastLED.show();
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  currTag = "";
  for (byte j = 0; j < mfrc522.uid.size; j++) {
    currTag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
    currTag.concat(String(mfrc522.uid.uidByte[j], HEX));
  }
  currTag.replace(" ", "");
  currTag.toLowerCase();
  Serial.println("Sending tag: ");
  Serial.println(currTag);
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Black;
  }
  FastLED.show();
  checkUUID();
}




void successLED() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Green;
  }
  FastLED.show();
  Serial.println("Access is granted");
  delay(3000);
}

void failLED() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Red;
  }
  FastLED.show();
  Serial.println("Access is Denied");
  delay(3000);
}


void checkUUID() {
  if (client.connect(server, port)) {
    Serial.println("Connection established 1");
    client.print(String("GET ") + "/timeclock.php?uuid=" + currTag + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n"); //GET request
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 25000) {
        return;
      }
    }
    while (client.available()) {
      rcv = client.readStringUntil('}');
    }
    client.stop();
  }
  else {
    Serial.println("Connection failed");
  }
  rcv.remove(0, (rcv.indexOf('[')));

  Serial.println("Server sent back: ");
  Serial.println(rcv);


  if (rcv.equals("[Swipe logged]")  ) {
    successLED();
  } else {
    failLED();
  }
}
