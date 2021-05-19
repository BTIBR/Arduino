String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    if (inputString[4] == '1') {
      digitalWrite(49, HIGH);
    }
    else if (inputString[4] == '0') {
      digitalWrite(49, LOW);
    }

    if (inputString[5] == '1') {
      digitalWrite(47, HIGH);
    }
    else if (inputString[5] == '0') {
      digitalWrite(47, LOW);
    }

    if (inputString[6] == '1') {
      digitalWrite(45, HIGH);
    }
    else if (inputString[6] == '0') {
      digitalWrite(45, LOW);
    }

    if (inputString[7] == '1') {
      digitalWrite(43, HIGH);
    }
    else if (inputString[7] == '0') {
      digitalWrite(43, LOW);
    }

    if (inputString[8] == '1') {
      digitalWrite(41, HIGH);
    }
    else if (inputString[8] == '0') {
      digitalWrite(41, LOW);
    }

    if (inputString[9] == '1') {
      digitalWrite(39, HIGH);
    }
    else if (inputString[9] == '0') {
      digitalWrite(39, LOW);
    }

    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
