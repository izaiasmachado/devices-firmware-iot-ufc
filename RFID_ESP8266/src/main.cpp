#define SS_PIN D2  // D2
#define RST_PIN D1 // D1

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

WiFiClient client;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup()
{
  Serial.begin(115200);
  SPI.begin();        // Initiate  SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522

  WiFi.begin(IOT_CONFIG_WIFI_SSID, IOT_CONFIG_WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop()
{

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    Serial.println("No Card detected");
    delay(100);
    return;
  }

  Serial.print("Card UID:");
  String card_uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (i > 0)
    {
      card_uid += " ";
    }
    if (mfrc522.uid.uidByte[i] < 0x10)
    {
      card_uid += "0";
    }
    card_uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  card_uid.toUpperCase(); // Convert to uppercase if needed

  Serial.println(card_uid); // Debug output to verify correct card UID

  if (client.connect(IOT_SERVER, 80))
  {
    String path = "/rfid";
    String body = "{\"id\": \"" + card_uid + "\"}";

    client.print("POST " + path + " HTTP/1.1\r\n");
    client.print("Host: " + String(IOT_SERVER) + "\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(body.length()) + "\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(body);

    Serial.println("Request sent");
    Serial.println("Card UID: " + card_uid);
    Serial.println("Body: " + body);

    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
    }
    client.stop();
  }
  else
  {
    Serial.println("Connection failed!");
  }

  Serial.println();
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
