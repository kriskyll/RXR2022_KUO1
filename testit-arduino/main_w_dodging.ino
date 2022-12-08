/*
Lisätty Raspin käskyttämään ajosoftaan esteiden väistely ultraäänellä.

Let's see how it works.
*/

#define trigV 2     // Ultrasonic-anturit
#define echoV 11
#define trigK 9
#define echoK 12
#define trigO 10
#define echoO 13

#define pwm_a 3     // Moottoreiden ohjaus
#define pwm_b 6
#define dir_a0 5
#define dir_a1 4
#define dir_b0 8
#define dir_b1 7

char action;        // Moottoreiden muuttujat
char adjusted_speed;
int default_speed;

long duration;      // Ultran muuttujat
int distance, max, min, velocity;

void setup()
{

    Serial.begin(9600); // Pour a bowl of serial

    pinMode(trigV, OUTPUT);     // Ultran alustus
	pinMode(echoV, INPUT);
	pinMode(trigK, OUTPUT);
	pinMode(echoK, INPUT);
	pinMode(trigO, OUTPUT);
	pinMode(echoO, INPUT);

    pinMode(pwm_a, OUTPUT);     // Moottoreiden alustus
    pinMode(pwm_b, OUTPUT);
    pinMode(dir_a0, OUTPUT);
    pinMode(dir_a1, OUTPUT);
    pinMode(dir_b0, OUTPUT);
    pinMode(dir_b1, OUTPUT);

    default_speed = 100;        // Perusnopeus

    max = 30;                   // Ultran raja-arvot
    min = 10;
  
}

void loop()
{ 
    // Väistetään kohteita jos ollaan liian lähellä estettä
    if (measureV() < max or measureK() < max or measureO() < max) {
        drive(measureV(), measureK(), measureO());
    }

    else if (Serial.available() > 1){ // Wait for serial input

        action = Serial.read();
        adjusted_speed = Serial.read();

        char forSend[2] = {action, adjusted_speed};
        Serial.write(forSend, 2);
    
        switch(action){ // Switch based on serial in

            case 'w': // Move Forward

                forward(adjusted_speed);
                break;

            case 's': // Move Backward

                reverse(adjusted_speed);
                break;

            case 'q': // Turn Left while moving forward

                turnL(adjusted_speed);
                break;
            
            case 'e': // Turn Right while moving forward

                turnR(adjusted_speed);
                break;

            case 'a': // Spin Left in place

                spinL(adjusted_speed);
                break;

            case 'd': // Spin Right in place

                spinR(adjusted_speed);
                break;

            case 'x': // Short brake

                brake();
                break;

    }
  }  
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

    analogWrite(pwm_a, default_speed); 
    analogWrite(pwm_b, speed); 

}

void turnR(int speed) // Turn Right while moving forward
{ 

    digitalWrite(dir_a0, 0);
    digitalWrite(dir_a1, 1);
    digitalWrite(dir_b0, 0);
    digitalWrite(dir_b1, 1);

    analogWrite(pwm_a, speed); 
    analogWrite(pwm_b, default_speed); 

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

/*
    Ajometodi esteiden väistelyyn
*/
void drive(int leftSensorValue, int centralSensorValue, int rightSensorValue) { // Drive forward & turn based on ultrasound distance signal. For avoiding objects.
	// Jos keskisensori aktivoituu, niin peruutetaan ja käännytään
    // Tähän pitäisi lisätä ominaisuus, jossa kaaressa ajaa takaisin kohti aiempaa kulkusuuntaansa
    // ... sitä varten tosin pitää mittailla fyysisesti että kuinka paljon se kääntyy
    if (centralSensorValue == 0) {
		reverse(default_speed);
		delay(1000);
		shutoff();
		spinR(default_speed);
		delay(1000);
		shutoff();
	}
	
	digitalWrite(dir_a0, 0);
	digitalWrite(dir_a1, 1);
	digitalWrite(dir_b0, 0);
	digitalWrite(dir_b1, 1);

	analogWrite(pwm_a, default_speed*(leftSensorValue/(max-min)));
	analogWrite(pwm_b, default_speed*(rightSensorValue/(max-min)));


}

/*
    Ultasound-sensorien metodit mittaamiseen
*/
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