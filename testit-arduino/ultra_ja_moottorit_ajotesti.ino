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
#define dir_a0 5
#define dir_a1 4
#define dir_b0 8
#define dir_b1 7

long duration;
int distance, max, min, velocity;

void setup() {
	// Testailuun seriali
	Serial.begin(9600);
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
	// Maksimietäisyys mihin asti ultraääni mittaa
	max = 30;
	// Minimietäisyys mistä asti ultraääni mittaa - turvaetäisyys ettei osu
	min = 10;
	// Moottorien maksiminopeus
	velocity = 150;
}

// Main functionality
void loop() {
	drive(measureV(), measureK(), measureO());
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
	
	// Testausta varten alla olevat
	if (trig == 2) {
		Serial.print("Vasen: ");
	}
	else if (trig == 9) {
		Serial.print("Keski: ");
	}
	else {
		Serial.print("Oikea: ");
	}
	Serial.print(distance);
	Serial.println(" cm");
	

	if (distance > max) {
		distance = max-min;
	}
	if (distance < min) {
		distance = 0;
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
	if (centralSensorValue == 0) {
		reverse(velocity);
		delay(1000);
		shutoff();
		spinR(velocity);
		delay(1000);
		shutoff();
	}
	
	digitalWrite(dir_a0, 0);
	digitalWrite(dir_a1, 1);
	digitalWrite(dir_b0, 0);
	digitalWrite(dir_b1, 1);

	// Tämän kirjoittelu on kesken
	// Pitäisi lisätä se moodi, että kääntää mieluummin vähän enempi oikialle jos keskisensori aktivoituu.
	// Onkohan tässä jotain ongelmia noiden ultraääniantureiden muodostamien keilojen kanssa? :hmm

	analogWrite(pwm_a, velocity*(leftSensorValue/(max-min)));
	analogWrite(pwm_b, velocity*(rightSensorValue/(max-min)));


}

void spinL(int speed) // Spin Left in place
{ 

digitalWrite(dir_a0, 0);
digitalWrite(dir_a1, 1);
digitalWrite(dir_b0, 1);
digitalWrite(dir_b1, 0);

analogWrite(pwm_a, speed); 
analogWrite(pwm_b, speed); 

}

void spinR(int speed) // Spin Right in place
{ 

digitalWrite(dir_a0, 1);
digitalWrite(dir_a1, 0);
digitalWrite(dir_b0, 0);
digitalWrite(dir_b1, 1);

analogWrite(pwm_a, speed); 
analogWrite(pwm_b, speed); 

}

void brake() // Short brake
{ 

digitalWrite(dir_a0, 1);
digitalWrite(dir_a1, 1);
digitalWrite(dir_b0, 1);
digitalWrite(dir_b1, 1);

analogWrite(pwm_a, 0); 
analogWrite(pwm_b, 0);

}

void shutoff() // Stop Motors w/o braking
{ 

digitalWrite(dir_a0, 0);
digitalWrite(dir_a1, 0);
digitalWrite(dir_b0, 0);
digitalWrite(dir_b1, 0);

analogWrite(pwm_a, 0); 
analogWrite(pwm_b, 0);

}