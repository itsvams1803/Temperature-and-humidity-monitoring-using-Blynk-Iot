// Include necessary libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define constants and macros
#define BLYNK_TEMPLATE_ID           "TMPL3kQMcajLu"
#define BLYNK_TEMPLATE_NAME         "Temperature and humidity monitoring"
#define BLYNK_AUTH_TOKEN            "foZrmH60w1e5zleo-vK5chmutoz8DF5k"
#define BLYNK_PRINT Serial
#define USE_NODE_MCU_BOARD

#define DHTPIN D4
#define DHTTYPE DHT11

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// WiFi credentials
char ssid[] = "*********";
char pass[] = "*********";

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT sensor
DHT dht(DHTPIN, DHTTYPE);
float t, h;

// Setup function
void setup() {
    pinMode(A0, INPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
    dht.begin();
    Serial.begin(115200);

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    delay(2000);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    delay(2000);
    display.clearDisplay();
    display.setTextColor(WHITE);
}

// Loop function
void loop() {
    delay(5000);
    int a = analogRead(A0);
    Serial.println(a);
    if (a > 710) {
        Blynk.logEvent("smoke", "SMOKE DETECTED");
        digitalWrite(D7, 1);
        digitalWrite(D8, 1);
        delay(2000);
        digitalWrite(D7, 0);
        digitalWrite(D8, 0);
        delay(500);
    }

    Blynk.run();
    h = dht.readHumidity();
    t = dht.readTemperature();
    Serial.println(h);
    Serial.print("%");
    Serial.println(t);
    Serial.print("C");
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
    }

    // clear display
    display.clearDisplay();

    // display temperature
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Temperature: ");
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print(t);
    display.print(" ");
    display.setTextSize(1);
    display.cp437(true);
    display.write(167); // Degree symbol
    display.setTextSize(2);
    display.print("C");

    // display humidity
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Humidity: ");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.print(h);
    display.print(" %");

    display.display();
}
