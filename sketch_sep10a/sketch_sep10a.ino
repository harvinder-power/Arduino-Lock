#include <Servo.h>
Servo myServo;

//Assigment of variable to allow for streamlined modification of code later on e.g. button_delay_rate. This can be lowered to reduce time needed to press a button to register a value.
boolean lock_state = false;
int passcode[4];
int input_passcode[4];
int array_counter = 0;
int button_delay_rate = 500;
int servo_delay_rate = 1000;
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
    delay(button_delay_rate);
  }
  else if (value >= 995 && value <= 1005){
    input_passcode[array_counter] = 2;
    array_counter += 1;
    delay(button_delay_rate);
  }
  else if (value >= 960 && value <= 980){
    input_passcode[array_counter] = 3;
    array_counter += 1;
    delay(button_delay_rate);
  }
  else if (value >= 920 && value <= 940){
    input_passcode[array_counter] = 4;
    array_counter += 1;
    delay(button_delay_rate);
  }
  else if (value >= 500 && value <= 520){
    input_passcode[array_counter] = 5;
    array_counter += 1;
    delay(button_delay_rate);
  }
  else if (value >= 5 && value <= 15){
    input_passcode[array_counter] = 6;
    array_counter += 1;
    delay(button_delay_rate);
  }
  else if (value == 0){
    delay(button_delay_rate);
  }
}




 
void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  myServo.write(0);
  delay(servo_delay_rate);
}

void loop() {
  int lock_val = analogRead(A0);
  int reset_val = analogRead(A1);
  Serial.print("Resetval=");
  Serial.print(reset_val);
  Serial.println();
  /*Serial.println("Lock val =");
  Serial.print(lock_val);*/
  button_mapper(lock_val);
  
  //Serial.println("Array length =");
  //Serial.print(array_counter);
  
  Serial.print("Input Passcode =");
  Serial.print(input_passcode[0]);
  Serial.print(input_passcode[1]);
  Serial.print(input_passcode[2]);
  Serial.print(input_passcode[3]);
  
  //once there are 4 values in the input_passcode array, the array passcode is set to be equal to the array input_passcode, iterating through values using a for loop.
  if (lock_state == false){
    if (array_counter == 4){
      myServo.write(90);
      delay(servo_delay_rate);
    for (int n=0; n<4; n++){
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
    
  //if the system is locked, and there have been 4 values inputted, then checks to see if all numbers are correct, and in the right sequence.
  if (lock_state == true && array_counter == 4){
    for (int n=0; n<4; n++){
      if (input_passcode[0] == passcode[0] && input_passcode[1] == passcode[1] && input_passcode[2] == passcode[2] && input_passcode[3] == passcode[3]){
        Serial.println("Passcode verified");
        myServo.write(0);
        lock_state = false;
        delay(10000);
        myServo.write(90);
        lock_state = true;
        array_counter = 0;
        input_passcode[n] = 0;
      }
      //if wrong passcode is inputted, then the system resets the input_passcode variable, and allows the user to try again.
      if (input_passcode[n] != passcode[n]){
        Serial.println("Incorrect passcode");
        input_passcode[n] = 0;
        array_counter = 0;
      }
    }
  }
  //resets the system if the reset button is pressed.
  if (reset_val == 0){
    array_counter = 0;
    myServo.write(0);
    Serial.println("system reset!");
    for (int n=0; n<4; n++){
      passcode[n] = 0;
      input_passcode[n] = 0;
    }
  }

   
}

