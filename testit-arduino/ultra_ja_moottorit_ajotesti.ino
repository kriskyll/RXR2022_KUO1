/*
	Testi moottorien ohjaamiselle etäisyysanturien signaalin mukaan.
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
	
	Moottorit käyttää pinnejä shieldin mukaan (pinnit 3-8).
*/

// Ultrasonic-anturit
#define trigV 2
#define echoV 11
#define trigK 9
#define echoK 12
#define trigO 10
#define echoO 13

// Moottoreiden ohjaus
#define pwm_a 3
#define pwm_b 6
#define dir_a0 4
#define dir_a1 5
#define dir_b0 7
#define dir_b1 8

long duration;
int distance, max, velocity;

void setup() {
	// Testailuun seriali
	//Serial.begin(9600);
	// Ultrasonicit
	pinMode(trigV, OUTPUT);
	pinMode(echoV, INPUT);
	pinMode(trigK, OUTPUT);
	pinMode(echoK, INPUT);
	pinMode(trigO, OUTPUT);
	pinMode(echoO, INPUT);
	// Moottorit
	pinMode(pwm_a, OUTPUT);
	pinMode(pwm_b, OUTPUT);
	pinMode(dir_a0, OUTPUT);
	pinMode(dir_a1, OUTPUT);
	pinMode(dir_b0, OUTPUT);
	pinMode(dir_b1, OUTPUT);

	// Raja-arvot ajeluun
	max = 20;
	velocity = 50;
}

// Main functionality
void loop() {
	if (measureV() > max and measureK() > max and measureO() > max) {
		forward(velocity);
	}
	else {
		reverse(velocity);
	}
}

int measureV() {
	return measure(2, 11);
}

int measureK() {
	return measure(9, 12);
}

int measureO() {
	return measure(10, 13);
}

int measure(int trig, int echo) { // Etäisyyden mittaaminen
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	duration = pulseIn(echo, HIGH);
	distance = duration * 0.034 / 2;
	
	/* Testausta varten alla olevat
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
	*/

	if (distance > max) {
		distance = max;
	}
	
	return distance;
}

void forward(int speed) // Move Forward
{ 

	digitalWrite(dir_a0, 0);
	digitalWrite(dir_a1, 1);
	digitalWrite(dir_b0, 0);
	digitalWrite(dir_b1, 1);

	analogWrite(pwm_a, speed); 
	analogWrite(pwm_b, speed); 

}

void reverse(int speed) // Move Backward 
{

	digitalWrite(dir_a0, 1);
	digitalWrite(dir_a1, 0);
	digitalWrite(dir_b0, 1);
	digitalWrite(dir_b1, 0);

	analogWrite(pwm_a, speed); 
	analogWrite(pwm_b, speed); 

}

void turnL(int speed) // Turn Left while moving forward
{ 

	digitalWrite(dir_a0, 0);
	digitalWrite(dir_a1, 1);
	digitalWrite(dir_b0, 0);
	digitalWrite(dir_b1, 1);

	analogWrite(pwm_a, speed); 
	analogWrite(pwm_b, speed/4); 

}

void turnR(int speed) // Turn Right while moving forward
{ 

	digitalWrite(dir_a0, 0);
	digitalWrite(dir_a1, 1);
	digitalWrite(dir_b0, 0);
	digitalWrite(dir_b1, 1);

	analogWrite(pwm_a, speed/4); 
	analogWrite(pwm_b, speed); 

}

void drive(int leftSensorValue, int centralSensorValue, int rightSensorValue) { // Drive forward & turn based on ultrasound distance signal. For avoiding objects.
	digitalWrite(dir_a0, 0);
	digitalWrite(dir_a1, 1);
	digitalWrite(dir_b0, 0);
	digitalWrite(dir_b1, 1);

	// Tämän kirjoittelu on kesken
	// Pitäisi lisätä se moodi, että kääntää mieluummin vähän enempi oikialle jos keskisensori aktivoituu.
	// Onkohan tässä jotain ongelmia noiden ultraääniantureiden muodostamien keilojen kanssa? :hmm

	analogWrite(pwm_a, velocity*(leftSensorValue/max));
	analogWrite(pwm_b, velocity*(rightSensorValue/max));


}

void spinL(int speed) // Spin Left in place
{ 

digitalWrite(dir_a0, 0);
digitalWrite(dir_a1, 1);
digitalWrite(dir_b0, 1);
digitalWrite(dir_b1, 0);

analogWrite(pwm_a, speed/2); 
analogWrite(pwm_b, speed/2); 

}

void spinR(int speed) // Spin Right in place
{ 

digitalWrite(dir_a0, 1);
digitalWrite(dir_a1, 0);
digitalWrite(dir_b0, 0);
digitalWrite(dir_b1, 1);

analogWrite(pwm_a, speed/2); 
analogWrite(pwm_b, speed/2); 

}