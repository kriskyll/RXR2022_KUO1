#define pwm_a 3
#define pwm_b 6
#define dir_a0 4
#define dir_a1 5
#define dir_b0 7
#define dir_b1 8

int velocity;

void setup() {
	Serial.begin(9600);

	// Moottorit
	pinMode(pwm_a, OUTPUT);
	pinMode(pwm_b, OUTPUT);
	pinMode(dir_a0, OUTPUT);
	pinMode(dir_a1, OUTPUT);
	pinMode(dir_b0, OUTPUT);
	pinMode(dir_b1, OUTPUT);

	velocity = 50
}

void loop() {
	if (Serial.available() > 0) {
		String data = Serial.readStringUntil('\n');

		if (data == 'f') {
			forward(velocity)
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
