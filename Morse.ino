
/*
  PROGRAM TO DECIPHER MORSE CODE USING A PUSH BUTTON AND DISPLAY IT ON THE SERIAL MONITOR
  DATE: 20 JANUARY 2017
  AUTHORS: PINAKI SADHUKHAN AND PRIYANKA SADHUKHAN
  EDITED BY: SHIVANGI SINGH
*/

unsigned long signal_len,t1,t2;   //time for which button is pressed
int inputPin = 2;                 //input pin for push button
int ledPin = 4;                   //output pin for LED
int ledPin2 = 13;
String code = "";                 //string in which pattern of dots and dashes are stored
String Mode = "Morse";            // Mode for decrypting

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP); //internal pullup resistor is used to simplify the circuit
  pinMode(ledPin,OUTPUT);          //first led
  pinMode(ledPin2,OUTPUT);         //second led
}

void loop()

{
NextDotDash:
  while (digitalRead(inputPin) == HIGH) {}
  t1 = millis();                            //time at button press
  if (Mode == "Morse") {
    digitalWrite(ledPin, HIGH);            //if in Morse mode first pin is HIGH otherwise second pin is HIGH
    }
  else {
    digitalWrite(ledPin2, HIGH);
    }
                 
  while (digitalRead(inputPin) == LOW) {}
  t2 = millis();                            //time at button release
  if (Mode == "Morse") {
      digitalWrite(ledPin, LOW);           
    }
  else {
    digitalWrite(ledPin2, LOW);
    }    
                
  signal_len = t2 - t1;                     //time for which button is pressed
  if (signal_len > 50) {                      //to account for switch debouncing
    code += readio();                       //function to read dot or dash
    }
    
  while ((millis() - t2) < 500) {          //if time between button press greater than 0.5sec, skip loop and go to next alphabet   
    if (digitalRead(inputPin) == LOW) {
      goto NextDotDash;
      }
    }
    
  if (code == ".....") {                   // initialize the Emoji mode on appropriate input
    Mode = "Emoji";
    digitalWrite(ledPin2, HIGH);           // the leds light up to indicate that mode has changed for easier usability
    delay(200);
    digitalWrite(ledPin2, LOW);
    }
  else if (code == "......") {             // initialize the Morse mode on appropriate input
      Mode = "Morse";
      digitalWrite(ledPin, HIGH);          // the leds light up to indicate that mode has changed for easier usability
      delay(200);
      digitalWrite(ledPin, LOW);
    }


  if (Mode == "Morse") {                   //interpret signal in Alphabetical letters if in morse mode
      convertor();
    }
    else if (Mode == "Emoji"){             //interpret signal in Emojis if in emoji mode
      emoji_convertor();
    }
                              
}

char readio()
{
  if (signal_len < 600 && signal_len > 50)
  {
    return '.';                        //if button press less than 0.6sec, it is a dot
  }
  else if (signal_len > 600)
  {
    return '-';                        //if button press more than 0.6sec, it is a dash
  }
}

void convertor()
{
  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".....", "E"
                            };
  int i = 0;

  if (code == ".-.-.-")
  {
    Serial.print(".");          //for break
  }
  else
  {
    while (letters[i] != "E")  //loop for comparing input code with letters array
    {
      if (letters[i] == code)
      {
        Serial.print(char('A' + i));
        break;
      }
      i++;
    }
    if (letters[i] == "E")
    {
      Serial.println("<Wrong input>");  //if input code doesn't match any letter, error
    }
  }
  code = "";                            //reset code to blank string
}

void emoji_convertor()  // this is the emoji convertor fuction, interprets emojis as indicated by dots and dash patterns
{
  if (code == "..") {
    Serial.println(":-)");
  }
  else if (code == "-") {
    Serial.println(":-(");
  }
  else if (code == "...") {
    Serial.println(":-O");
  }
  else if (code == "--"){
    Serial.println(":-|");
  }
  code = "";
}
