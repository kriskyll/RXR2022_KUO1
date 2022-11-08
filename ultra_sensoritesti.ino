/*
	Testaus ultraääniantureille
	Käytössä olevat pinnit:
	- Vasen anturi
		Trig 5
		Echo 11
	- Keski anturi
		Trig 6
		Echo 12
	- Oikea anturi
		Trig 7
		Echo 13
*/

#define trigV 5
#define echoV 11
#define trigK 6
#define echoK 12
#define trigO 7
#define echoO 13

long duration;
int distance;

void setup() {
	Serial.begin(9600);
	pinMode(trigV, OUTPUT);
	pinMode(echoV, INPUT);
	pinMode(trigK, OUTPUT);
	pinMode(echoK, INPUT);
	pinMode(trigO, OUTPUT);
	pinMode(echoO, INPUT);
}

void loop() {
	measure(5, 11);
	measure(6, 12);
	measure(7, 13);
}

void measure(int trig, int echo) {
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	duration = pulseIn(echo, HIGH);
	distance = duration * 0.034 / 2;
	
	if (trig == 5) {
		Serial.print("Vasen: ");
	}
	else if (trig == 6) {
		Serial.print("Keski: ");
	}
	else {
		Serial.print("Oikea: ");
	}
	Serial.print(distance);
	Serial.println(" cm");
}
