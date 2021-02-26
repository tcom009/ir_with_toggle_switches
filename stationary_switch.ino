//usign IRremote 2.0.1
#include <IRremote.h>

byte receiver = 7;
IRrecv irrecv(receiver);
decode_results results;

class Commuter
{

private:
  byte toggle;
  byte relay;
  int _toggleStatus;
  int _relayStatus;
  long _irOn;
  long _irOff;

public:
  //constructor
  Commuter(byte toggle, byte relay, long irOn, long irOff)
  {
    this->toggle = toggle;
    this->relay = relay;
    this->_irOff = irOff;
    this->_irOn = irOn;
    this->_relayStatus = _relayStatus;
    this->_toggleStatus = _toggleStatus;
    init();
  }

  void init()
  {
    //inits the conmuter
    pinMode(toggle, INPUT);
    pinMode(relay, OUTPUT);
  }

  void printStatus()
  {
    Serial.print("Estado relay #");
    Serial.print(relay);
    Serial.print(": ");
    Serial.println(_relayStatus);
  }

  void setRelayStatus()
  {
    //Changes status negating the actual state of the relay

    if (_relayStatus == HIGH)
    {
      _relayStatus = LOW;
    }
    else
    {
      if (_relayStatus == LOW)
      {
        _relayStatus = HIGH;
      }
    }
    digitalWrite(relay, _relayStatus);
    printStatus();
  }

  void setToggleStatus()
  {
    //changes toggle's actual statue
    if (_toggleStatus == HIGH)
    {
      _toggleStatus = LOW;
    }
    else
    {
      _toggleStatus = HIGH;
    }
  }
  //experimental IR management
  void setIr()
  {
    if (irrecv.decode())
    {
      long data = irrecv.decodedIRData.decodedRawData;
      if (data == _irOn)
      {
        _relayStatus = HIGH;
        digitalWrite(relay, _relayStatus);
        printStatus();
      }
      if (data == _irOff)
      {
        _relayStatus = LOW;
        digitalWrite(relay, _relayStatus);
        printStatus();
      }
        }
    irrecv.resume();
  }

  void setCommuter()
  {
    //detects if any change has happend in toggle status
    int toggleReading = digitalRead(toggle);
    if (toggleReading != _toggleStatus)
    {
      delay(50);
      setRelayStatus();
      setToggleStatus();
    }
  }
};

//Commuter instance recives, toggle input pin, relay output pin and ir on and off codes
Commuter Conmutador1(4, 13, 0xFE01BF40, 0xFD02BF40);

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop()
{
  Conmutador1.setIr();
  Conmutador1.setCommuter();
}
