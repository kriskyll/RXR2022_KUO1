String s_input; // A place to store serial input
char action;
int adjusted_speed;

void setup() {

    Serial.begin(9600);
    Serial.setTimeout(100);
    delay(200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    if(Serial.available()){ // Wait for serial input

        s_input = Serial.readString();
        action = s_input.substring(0, 1)[0];
  
        // Meneeköhän tää parsiminen nyt oikein
        String x = s_input.substring(2);
        int l = x.length();
        char y[l+1];
        x.toCharArray(y, l+1);
  
        adjusted_speed = atoi(y);

        if (action == 'd') {
            digitalWrite(LED_BUILTIN, HIGH);
        }
        if (adjusted_speed == 89) {
            digitalWrite(LED_BUILTIN, LOW);
        }
  
        Serial.println(adjusted_speed);
    }
}