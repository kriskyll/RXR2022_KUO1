/*
Testaus moottoreiden ohjaamiseen sarjaportin yli.

Ei käytä ultraääniantureita mihinkään.
*/

// Moottoreiden ohjaus
#define pwm_a 3
#define pwm_b 6
#define dir_a0 4
#define dir_a1 5
#define dir_b0 7
#define dir_b1 8

char inbit; // A place to store serial input


void setup()
{

  Serial.begin(9600); // Pour a bowl of serial

  pinMode(pwm_a, OUTPUT);  // Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a0, OUTPUT);
  pinMode(dir_a1, OUTPUT);
  pinMode(dir_b0, OUTPUT);
  pinMode(dir_b1, OUTPUT);

  draw(); // Draw the driving instructions to the serial terminal
  
}

void loop()
{ 

if(Serial.available()){ // Wait for serial input

  inbit = Serial.read();
 
  switch(inbit){ // Switch based on serial in

    case 'w': // Move Forward

      forward(200);
      delay(30);
      shutoff();
      break;

    case 's': // Move Backward

      reverse(200);
      delay(30);
      shutoff();
      break;

    case 'q': // Turn Left while moving forward

      turnL(200);
      delay(30);
      shutoff();
      break;
        
    case 'e': // Turn Right while moving forward

      turnR(200);
      delay(30);
      shutoff();
      break;

    case 'a': // Spin Left in place

      spinL(200);
      delay(30);
      shutoff();
      break;
  
    case 'd': // Spin Right in place

      spinR(200);
      delay(30);
      shutoff();
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

void draw() // Serial Instructions
{
  Serial.println("          DuckBot 2015          ");
  Serial.println("                                ");
  Serial.println("   -------------------------    ");
  Serial.println("   |       |       |       |    ");
  Serial.println("   |   Q   |   W   |   E   |    ");
  Serial.println("   | turnL |forward| turnR |    ");
  Serial.println("   -------------------------    ");
  Serial.println("   |       |       |       |    ");
  Serial.println("   |   A   |   S   |   D   |    ");
  Serial.println("   | spinL |reverse| spinR |    ");
  Serial.println("   -------------------------    ");
  Serial.println("   |       |       |       |    ");
  Serial.println("   |   Z   |   X   |   C   |    ");
  Serial.println("   |       | brake |       |    ");
  Serial.println("   -------------------------    ");
  Serial.println("                                ");
}