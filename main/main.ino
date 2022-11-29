/*
Ei käytä ultraääniantureita mihinkään.
*/

// Moottoreiden ohjaus
#define pwm_a 3
#define pwm_b 6
#define dir_a0 5
#define dir_a1 4
#define dir_b0 8
#define dir_b1 7

char action;
char adjusted_speed;

int default_speed;

void setup()
{

  Serial.begin(9600); // Pour a bowl of serial

  pinMode(pwm_a, OUTPUT);  // Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a0, OUTPUT);
  pinMode(dir_a1, OUTPUT);
  pinMode(dir_b0, OUTPUT);
  pinMode(dir_b1, OUTPUT);

  default_speed = 100;
  
}

void loop()
{ 
if(Serial.available() > 1){ // Wait for serial input

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