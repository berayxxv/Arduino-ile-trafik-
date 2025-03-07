// --------------- Donanım Soyutlama Katmanı (HAL) --------------- //
#define RED_LED 2
#define YELLOW_LED 3
#define GREEN_LED 4
#define BUTTON 5

void setupHardware() {
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);
}

void setLights(bool red, bool yellow, bool green) {
    digitalWrite(RED_LED, red);
    digitalWrite(YELLOW_LED, yellow);
    digitalWrite(GREEN_LED, green);
}

// --------------- İş Mantığı Katmanı (BLL) --------------- //
enum State {RED, GREEN, YELLOW};
State currentState = RED;

unsigned long previousMillis = 0;
unsigned long stateDuration = 5000;  // Kırmızı süresi başlangıçta 5s
bool pedestrianPressed = false;

void updateState() {
    unsigned long currentMillis = millis();

    // Buton kontrolü (LOW = basıldı)
    if (digitalRead(BUTTON) == LOW) {
        pedestrianPressed = true;
    }

    switch (currentState) {
        case RED:
            setLights(true, false, false); // Kırmızı yanıyor
            if (currentMillis - previousMillis >= stateDuration) {
                previousMillis = currentMillis;
                currentState = GREEN;
                stateDuration = 5000;  // Yeşil süresi
                pedestrianPressed = false;  // Buton sıfırlanır
            }
            break;

        case GREEN:
            setLights(false, false, true); // Yeşil yanıyor
            if (pedestrianPressed) {
                stateDuration = 3000;  // Süreyi kısaltıyoruz (örneğin 3 saniyeye)
            }
            if (currentMillis - previousMillis >= stateDuration) {
                previousMillis = currentMillis;
                currentState = YELLOW;
                stateDuration = 2000;  // Sarı süresi
            }
            break;

        case YELLOW:
            setLights(false, true, false); // Sarı yanıyor
            if (currentMillis - previousMillis >= stateDuration) {
                previousMillis = currentMillis;
                currentState = RED;
                stateDuration = 5000;  // Kırmızı süresi tekrar 5 saniye
            }
            break;
    }
}

// --------------- Uygulama Katmanı (Application Layer) --------------- //
void setup() {
    setupHardware();
}

void loop() {
    updateState();
}
