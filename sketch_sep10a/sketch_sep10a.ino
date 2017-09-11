#include <Servo.h>
Servo myServo;

boolean lock_state = false;
//using an array based system, by initialising two empty arrays to be populated with values from the user (becomes the array passcode). Then the user inputs a test sequence (input_passcode).
int passcode[4];
int input_passcode[4];

//counting to 4 before then initiating other sections of the code
int array_counter = 0;

//Assigment of variables to allow for streamlined modification of code later on e.g. button_delay_rate. This can be lowered to reduce time needed to press a button to register a value.
int button_delay_rate = 500;
int servo_delay_rate = 1000;


//function to map inputs from a resistor ladder to numbers assigned to each button.
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



//setting up the servo and initialisning the serial monitor
void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  myServo.write(0);
  delay(servo_delay_rate);
}

void loop() {
  
  //probes are used to obtain values for lock_val and reset_val. reset_val is outputting a constant value of 1023 until pressed, when it becomes 0. lock_val is dependent on the ladder.
  int lock_val = analogRead(A0);
  int reset_val = analogRead(A1);
  
  button_mapper(lock_val);
  
  //Serial.println("Array length =");
  //Serial.print(array_counter);

  //printing out the passcode
  Serial.print("Input Passcode =");
  Serial.print(input_passcode[0]);
  Serial.print(input_passcode[1]);
  Serial.print(input_passcode[2]);
  Serial.print(input_passcode[3]);
  
  //once there are 4 values in the input_passcode array, the array passcode is set to be equal to the array input_passcode, iterating through values using a for loop.
  if (lock_state == false){
    //if the system has not been locked, and receives 4 numbers, then the system is locked
    if (array_counter == 4){
      myServo.write(90);
      delay(servo_delay_rate);
    for (int n=0; n<4; n++){
      //the 4 numbers inputted by the user were initially held in the array input_passcode, however, this moves them to the passcode array
      passcode[n] = input_passcode[n];
      // iterate through the values held in the array input_passcode, and reset them all back to 0, ready to receive the passcode from the user to test against the array "passcode".
      input_passcode[n] = 0;
    }
    //print out the newly set passcode
    Serial.println("Passcode set to:");
    Serial.print(passcode[0]);
    Serial.print(passcode[1]);
    Serial.print(passcode[2]);
    Serial.print(passcode[3]);
    lock_state = true;
    //Serial.println("lock state set to");
    //Serial.print(lock_state);
    array_counter = 0;
  }
  }
    
  //if the system is locked, and there have been 4 values inputted, then checks to see if all numbers are correct, and in the right sequence.
  if (lock_state == true && array_counter == 4){
    //for loop used to set the array back to 0, enabling the passcode to be entered again.
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

