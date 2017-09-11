#include <Servo.h>
Servo myServo;

boolean lock_state = false;
int passcode[4];
int input_passcode[4];
int array_counter = 0;
/*One way of doing this is to set an initial state of locked = false, then to have a number assigned to each button.
 * If the number is pressed, then the value of another variable is set to another value (x).
 * Using nested if statements, of multiple if statements, then this can be done to progress the lock progress.
 * 
 * 
 */



boolean button_mapper(int value){
  if (value == 1023){
    input_passcode[array_counter] = 1;
    array_counter += 1;
    delay(500);
  }
  else if (value >= 995 && value <= 1005){
    input_passcode[array_counter] = 2;
    array_counter += 1;
    delay(500);
  }
  else if (value >= 960 && value <= 980){
    input_passcode[array_counter] = 3;
    array_counter += 1;
    delay(500);
  }
  else if (value >= 920 && value <= 940){
    input_passcode[array_counter] = 4;
    array_counter += 1;
    delay(500);
  }
  else if (value >= 500 && value <= 520){
    input_passcode[array_counter] = 5;
    array_counter += 1;
    delay(500);
  }
  else if (value >= 5 && value <= 15){
    input_passcode[array_counter] = 6;
    array_counter += 1;
    delay(500);
  }
  else if (value == 0){
    delay(500);
  }
}




 
void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  myServo.write(0);
  delay(1000);
}

void loop() {
  int lock_val = analogRead(A0);
  /*Serial.println("Lock val =");
  Serial.print(lock_val);*/
  button_mapper(lock_val);
  
  Serial.println("Array length =");
  Serial.print(array_counter);
  
  Serial.println("Input Passcode =");
  Serial.print(input_passcode[0]);
  Serial.print(input_passcode[1]);
  Serial.print(input_passcode[2]);
  Serial.print(input_passcode[3]);
  
  //once there are 4 values in the input_passcode array, the array passcode is set to be equal to the array input_passcode, iterating through values using a for loop.
  if (lock_state == false){
    if (array_counter == 4){
      myServo.write(90);
    delay(1000);
    for (int n=0; n<5; n++){
      passcode[n] = input_passcode[n];
      // iterate through the values held in the array input_passcode, and reset them all back to 0, ready to receive the test passcode from the user.
      input_passcode[n] = 0;
    }
    //print out the newly set passcode
    Serial.println("Passcode set to:");
    Serial.print(passcode[0]);
    Serial.print(passcode[1]);
    Serial.print(passcode[2]);
    Serial.print(passcode[3]);
    lock_state = true;
    Serial.println("lock state set to");
    Serial.print(lock_state);
    array_counter = 0;
  }
  }
    

  if (lock_state == true && array_counter == 4){
    for (int n; n<5; n++){
      if (input_passcode[n] == passcode[n]){
        Serial.println("Passcode verified");
        myServo.write(0);
        delay(1000);
      }
      if (input_passcode[n] != passcode[n]){
        Serial.println("Incorrect passcode");
      }
    }
  } 
}

