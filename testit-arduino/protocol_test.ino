char action;
char speed;

void setup() {

    Serial.begin(9600);
    Serial.setTimeout(100);
    delay(200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    if(Serial.available() == 2) {
        action = Serial.read();
        speed = Serial.read();
        if (action == 'd') {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(500);
        }
        if (speed == 89) {
            digitalWrite(LED_BUILTIN, LOW);
        }
        byte forSend[2] = {action, speed};
        Serial.write(forSend, 2);
    }
}