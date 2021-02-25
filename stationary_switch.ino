

class Commuter {
  private:
  byte toggle;
  byte relay;
  int _toggleStatus;
  int _relayStatus;
  
  public:
  Commuter(byte toggle, byte relay){
   this->toggle = toggle;
    this->relay= relay;
    init(); 
    }

  void init (){  
    //inits the conmuter
    pinMode (toggle , INPUT);
    pinMode (relay , OUTPUT);
  }

  
  void setRelayStatus(byte relay, int relayStatus){
        if (_relayStatus == HIGH){
          _relayStatus= LOW;
        }else {
          if (_relayStatus== LOW){
          _relayStatus= HIGH;
            }
        }
          digitalWrite (relay, _relayStatus);
          Serial.print( "Estado relay #");
          Serial.print(relay);
          Serial.print( ": ");
          Serial.println(relayStatus);
}   
          
  void setToggleStatus(byte toggle, int toggleStatus){
      if (_toggleStatus == HIGH){
          digitalWrite (toggle, toggleStatus);
          _toggleStatus= LOW;
        }else {
          digitalWrite (toggle, toggleStatus);
          _toggleStatus= HIGH;
        }

  }
  void setCommuter(){
    int toggleReading= digitalRead(toggle);
    if (toggleReading != _toggleStatus){
      delay(50);
      setRelayStatus(relay, _relayStatus);
      setToggleStatus(toggle, _toggleStatus);

    }

  }

};


Commuter Conmutador1 (4,13);


void setup() {
  Serial.begin (9600);

}

void loop() {

Conmutador1.setCommuter();


}
