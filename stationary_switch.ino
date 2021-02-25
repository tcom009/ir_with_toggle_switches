const int toggle1 = 4;
const int momentarium = 7;
const int led = 13;
int toggle1_status;
int estado_momentario;
int led_status;


void setup() {

  pinMode (toggle1, INPUT);
  pinMode (momentarium, INPUT);
  pinMode (led, OUTPUT);
  digitalWrite (led, led_status);
  Serial.begin (9600);

}

void setRelayStatus(int relay, int relay_status) {
  //sets each relay's virtual status 
  switch (relay) {
    case 13:
    led_status = relay_status;
    break;
  }


}

void changeRelayStatus(int relay, int relay_status) {
  //changes phisical status of the relay 
  if (relay_status == LOW) {

    setRelayStatus (relay, HIGH);
    Serial.println ("LED encendido");
  }
  else {
    if (relay_status == HIGH) {

      setRelayStatus (relay, LOW);
      Serial.println ("LED apagado");
    }
  }
  digitalWrite(relay, relay_status);

}

void setToggleStatus (int toggleNumber, int toggle_status){
  //sets each toggle's virtual status H/L
  switch (toggleNumber){
    case 4:
    toggle1_status=toggle_status;
    }
  
  
  }
void switchState(int toggle, int toggle_status, int relay, int relay_status) {

  //this function reads the toggle input and if it has changed, it sets the status of the toggle and change the relay position
  int toggle_read = digitalRead (toggle);
  delay (100);
  if (toggle_read != toggle_status) {
    setToggleStatus(toggle, toggle_read);
    changeRelayStatus(relay, relay_status);

  }
}



void loop() {


  switchState(toggle1, toggle1_status, led, led_status);
//Momentary switch only for test propouses 
  int leer_momentary = digitalRead (momentarium);
  if (leer_momentary != LOW) {
    delay(100);
    changeRelayStatus(led, led_status);
    estado_momentario = leer_momentary;
  }



}
