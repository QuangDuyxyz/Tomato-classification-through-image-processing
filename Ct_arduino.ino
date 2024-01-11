#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

int servo_pin1 = 9;
int servo_pin2 = 10;
int servo_pin3 = 11;

int sensor_pin1 = 3;
int sensor_pin2 = 4;
int sensor_pin3 = 5;
int sensor_pin4 = 6;

int conveyor_ena_pin = 12;
int conveyor_in_pin = 13;
String temp_data = "";
String read_data = "";

String result = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo1.attach(servo_pin1);
  servo2.attach(servo_pin2);
  servo3.attach(servo_pin3);
  pinMode(sensor_pin1, INPUT);
  pinMode(sensor_pin2, INPUT);
  pinMode(sensor_pin3, INPUT);
  pinMode(sensor_pin4, INPUT);

  servo1.write(10);
  servo2.write(10);
  servo3.write(5);

  pinMode(conveyor_ena_pin, OUTPUT);
  pinMode(conveyor_in_pin, OUTPUT);

  Serial.print("Setup OK");
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor_data1 = !digitalRead(sensor_pin1);
  int sensor_data2 = !digitalRead(sensor_pin2);
  int sensor_data3 = !digitalRead(sensor_pin3);
  int sensor_data4 = !digitalRead(sensor_pin4);
  chay_bt();

  // Serial.print(sensor_data1);
  // Serial.print("  ");
  // Serial.print(sensor_data2);
  // Serial.print("  ");
  // Serial.print(sensor_data3);
  // Serial.print("  ");
  // Serial.println(sensor_data4);
  // delay(100);

  if (result == "RED" && sensor_data2 == 1) {
    dung_bt();
    delay(300);
    servo1.write(75);
    delay(300);
    servo1.write(10);
    delay(300);
    result = "";
    chay_bt();
  }

  if (result == "YEL" && sensor_data3 == 1) {
    Serial.println("Cbi gat qua vang");
    dung_bt();
    delay(500);
    servo2.write(75);
    delay(500);
    servo2.write(10);
    delay(500);
    result = "";
    chay_bt();
  }

  if (result == "GRE" && sensor_data4 == 1) {
    dung_bt();
    delay(500);
    servo3.write(75);
    delay(500);
    servo3.write(5);
    delay(500);
    result = "";
    chay_bt();
  }


  if (sensor_data1 == 1) {
    dung_bt();
    Serial.println("Start Processing");
    bool done = false;
    while (!done) {
      while (Serial.available() > 0) {

        char data_rec = Serial.read();

        read_data += data_rec;
        if (data_rec == '\n') {
          chay_bt();
          delay(2000);
          if (read_data.substring(0, 3) == "RED") {
            result = "RED";
          } else if (read_data.substring(0, 3) == "YEL") {
            result = "YEL";
          } else if (read_data.substring(0, 3) == "GRE") {
            result = "GRE";
          } else {
            chay_bt();
          }
          read_data = "";
        }
      }
      if (digitalRead(sensor_pin1) == 1){
            done = true;
      }
    }


    Serial.println("Done");
  } else {
    Serial.println("NoThing");
  }
}

void chay_bt() {
  digitalWrite(conveyor_ena_pin, HIGH);
}

void dung_bt() {
  digitalWrite(conveyor_ena_pin, LOW);
}


// #include <Servo.h>

// Servo servo1;
// Servo servo2;
// Servo servo3;

// int servo_pin1 = 9;
// int servo_pin2 = 10;
// int servo_pin3 = 11;

// int sensor_pin1 = 3;
// int sensor_pin2 = 4;
// int sensor_pin3 = 5;
// int sensor_pin4 = 6;

// int conveyor_ena_pin = 12;
// int conveyor_in_pin = 13;
// String temp_data = "";
// String read_data = "";

// String result = "";
// bool processing_started = false; // Biến cờ để kiểm tra đã bắt đầu xử lý hay chưa

// void setup() {
//   Serial.begin(115200);
//   servo1.attach(servo_pin1);
//   servo2.attach(servo_pin2);
//   servo3.attach(servo_pin3);
//   pinMode(sensor_pin1, INPUT);
//   pinMode(sensor_pin2, INPUT);
//   pinMode(sensor_pin3, INPUT);
//   pinMode(sensor_pin4, INPUT);

//   servo1.write(0);
//   servo2.write(0);
//   servo3.write(0);

//   pinMode(conveyor_ena_pin, OUTPUT);
//   pinMode(conveyor_in_pin, OUTPUT);

//   Serial.println("Setup OK");
// }

// void loop() {
//   int sensor_data1 = !digitalRead(sensor_pin1);
//   int sensor_data2 = !digitalRead(sensor_pin2);
//   int sensor_data3 = !digitalRead(sensor_pin3);
//   int sensor_data4 = !digitalRead(sensor_pin4);
//   chay_bt();

//   if (result == "RED" && sensor_data2 == 1) {
//     dung_bt();
//     delay(500);
//     servo1.write(60);
//     delay(500);
//     servo1.write(0);
//     delay(500);
//     result = "";
//     chay_bt();
//   }

//   if (result == "YEL" && sensor_data3 == 1) {
//     Serial.println("Cbi gat qua vang");
//     dung_bt();
//     delay(500);
//     servo2.write(60);
//     delay(500);
//     servo2.write(0);
//     delay(500);
//     result = "";
//     chay_bt();
//   }

//   if (result == "GRE" && sensor_data4 == 1) {
//     dung_bt();
//     delay(500);
//     servo3.write(60);
//     delay(500);
//     servo3.write(0);
//     delay(500);
//     result = "";
//     chay_bt();
//   }

//   if (sensor_data1 == 1 && !processing_started) {
//     dung_bt();
//     Serial.println("Start Processing");
//     processing_started = true; // Đánh dấu đã bắt đầu xử lý
//   } else if (sensor_data1 == 0 && processing_started) {
//     // Reset biến cờ nếu cảm biến không đúng nữa
//     processing_started = false;
//     Serial.println("Done");
//   }

//   if (processing_started) {
//     while (Serial.available() > 0) {

//       char data_rec = Serial.read();

//       read_data += data_rec;
//       if (data_rec == '\n') {
//         chay_bt();
//         delay(2000);
//         if (read_data.substring(0, 3) == "RED") {
//           result = "RED";
//         } else if (read_data.substring(0, 3) == "YEL") {
//           result = "YEL";
//         } else if (read_data.substring(0, 3) == "GRE") {
//           result = "GRE";
//         }
//         else {
//           chay_bt();
//         }
//         read_data = "";
//       }
//     }
//   } else {
//     Serial.println("NoThing");
//   }
// }

// void chay_bt() {
//   digitalWrite(conveyor_ena_pin, HIGH);
// }

// void dung_bt() {
//   digitalWrite(conveyor_ena_pin, LOW);
// }
