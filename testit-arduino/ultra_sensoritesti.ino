/*
	Testaus ultraääniantureille
	Käytössä olevat pinnit:
	- Vasen anturi
		Trig 2
		Echo 11
	- Keski anturi
		Trig 9
		Echo 12
	- Oikea anturi
		Trig 10
		Echo 13
*/

#define trigV 2
#define echoV 11
#define trigK 9
#define echoK 12
#define trigO 10
#define echoO 13

long duration;
int distance, max, min;

void setup() {
	Serial.begin(9600);
	pinMode(trigV, OUTPUT);
	pinMode(echoV, INPUT);
	pinMode(trigK, OUTPUT);
	pinMode(echoK, INPUT);
	pinMode(trigO, OUTPUT);
	pinMode(echoO, INPUT);

	max = 30;
	min = 10;
}

void loop() {
	delay(500);
	measure(2, 11);
	measure(9, 12);
	measure(10, 13);
	Serial.println();
}

void measure(int trig, int echo) {
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	duration = pulseIn(echo, HIGH);
	distance = duration * 0.034 / 2;
	
	if (distance > max) {
		distance = max;
	}
	else if (distance < min) {
		distance = 0;
	}
	
	// Printit testaukseen
	if (trig == 2) {
		Serial.print("Vasen: ");
	}
	else if (trig == 9) {
		Serial.print("Keski: ");
	}
	else if (trig == 10) {
		Serial.print("Oikea: ");
	}
	Serial.print(distance);
	Serial.println(" cm \t");
}
