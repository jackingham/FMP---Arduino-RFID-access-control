#include <Ethernet.h>
#include <FastLED.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <MD5.h>

#define NUM_LEDS 8
#define DATA_PIN A0

CRGB leds[NUM_LEDS];

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          8         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

bool isPINRequired = false;
char* secretCode = "7777";
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 2, 1, 0}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

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




void accessGranted() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Green;
  }
  FastLED.show();
  Serial.println("Access is granted");
  delay(3000);
}

void accessDenied() {
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
    client.print(String("GET ") + "/checkuuid.php?uuid=" + currTag + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n"); //GET request
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


  if (rcv.equals("[Access granted]")  ) {
    if (isPINRequired == false) {
      accessGranted();
      return;
    } else if (isPINRequired == true) {
      for (int dot = 0; dot < (NUM_LEDS / 2); dot++) {
        leds[dot] = CRGB::Green;
      }
      FastLED.show();
      String currentInput = "";
      boolean inKeypadLoop = true;
      while (inKeypadLoop) {
        char key = keypad.getKey();
        if (key == 'C') {
          if (currentInput == "") {
            Serial.println("Cleared out");
            inKeypadLoop = false;
          } else {
            Serial.println("Clearing input");
            currentInput = "";
            for (int dot = 0; dot < NUM_LEDS; dot++) {
              leds[dot] = CRGB::Black;
            }
            FastLED.show();
            delay(100);
            for (int dot = 0; dot < (NUM_LEDS / 2); dot++) {
              leds[dot] = CRGB::Green;
            }
            FastLED.show();
            delay(100);
            for (int dot = 0; dot < NUM_LEDS; dot++) {
              leds[dot] = CRGB::Black;
            }
            FastLED.show();
            delay(100);
            for (int dot = 0; dot < (NUM_LEDS / 2); dot++) {
              leds[dot] = CRGB::Green;
            }
            FastLED.show();
          }
        } else if (key == '#') {

          unsigned char* hash = MD5::make_hash(currentInput);
          char *md5str = MD5::make_digest(hash, 16);
          free(hash);

          client.print(String("GET ") + "/checkPIN.php?uuid=" + currTag + "&hash=" + *md5str + "HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n"); //GET request
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

        if (rcv.equals("[Access granted]")  ) {
          accessGranted();
        } else {
         accessDenied();
        }
        currentInput = "";
        inKeypadLoop = false;
        break;




      } else {
        if (key != NO_KEY) {
          currentInput += key;
          for (int dot = 0; dot < NUM_LEDS; dot++) {
            leds[dot] = CRGB::Black;
          }
          FastLED.show();
          delay(100);
          for (int dot = 0; dot < (NUM_LEDS / 2); dot++) {
            leds[dot] = CRGB::Green;
          }
          FastLED.show();
        }
      }
    }
  }
} else {
  accessDenied();
}
}
