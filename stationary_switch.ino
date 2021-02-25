#include <IRremote.h>

//IR codes for on/off
//byte irOff = 0x0020DF18E7;
//byte irOn= 0x0020DFE817;


byte receiver = 7;
IRrecv irrecv(receiver);
decode_results results;


class Commuter {
  
  private:
  byte toggle;
  byte relay;
  int _toggleStatus;
  int _relayStatus;
  byte _irOn;
  byte _irOff;
  
  
  public:
  Commuter(byte toggle, byte relay, byte irOn, byte irOff){
    this->toggle = toggle;
    this->relay= relay;
    this->_irOff=irOff;
    this->_irOn=irOn;
    this->_relayStatus=_relayStatus;
    this->_toggleStatus=_toggleStatus;
    init(); 
    }

  void init (){  
    //inits the conmuter
    pinMode (toggle , INPUT);
    pinMode (relay , OUTPUT);
  }

  
void printStatus (){
  Serial.print( "Estado relay #");
          Serial.print(relay);
          Serial.print( ": ");
          Serial.println(_relayStatus);
}

  void setRelayStatus(){
    //Changes status negating the actual state of the relay

        if (_relayStatus == HIGH){
          _relayStatus= LOW;
        }else {
          if (_relayStatus== LOW){
          _relayStatus= HIGH;
            }
        }
          digitalWrite (relay, _relayStatus);
          printStatus();
        }
  
          
  void setToggleStatus(){
      if (_toggleStatus == HIGH){
          _toggleStatus= LOW;
        }else {
          _toggleStatus= HIGH;
        }        
  }

  void setIr(){
    if (irrecv.decode(&results)){
      if (results.value == 0x0020DFE817){
        _relayStatus =HIGH;
      }else {
        if (results.value== 0x0020DF18E7){
          _relayStatus =LOW;
          }
        }
        Serial.println (results.value);
      digitalWrite(relay, _relayStatus);
      printStatus();
    }
  irrecv.resume();
  }

  void setCommuter(){
    int toggleReading= digitalRead(toggle);
    if (toggleReading != _toggleStatus){
      delay(50);
      setRelayStatus();
      setToggleStatus();
    }
  }
};


Commuter Conmutador1 (4,13, 0x0020DFE817, 0x0020DF18E7);


void setup() {
  Serial.begin (9600);
  irrecv.enableIRIn();


}

void loop() {

Conmutador1.setIr();
Conmutador1.setCommuter();


}
