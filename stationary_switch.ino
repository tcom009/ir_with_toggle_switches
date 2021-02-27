//usign IRremote 2.0.1
#include <IRremote.h>

byte receiver = 7;
IRrecv irrecv(receiver);
decode_results results;

class Commuter
{

private:
  byte push;
  byte toggle;
  byte relay;
  byte _toggleStatus;
  byte _relayStatus;
  byte _pushStatus;
  bool is_open = false;
  long _irOn;
  long _irOff;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;

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
    _toggleStatus == HIGH ? _toggleStatus = LOW : _toggleStatus = HIGH;
  }

public:
  Commuter(byte relay, long irOn, long irOff)
  {
    this->relay = relay;
    this->_irOff = irOff;
    this->_irOn = irOn;
    this->_relayStatus = _relayStatus;
    init();
  }

  Commuter(byte push, bool is_open, byte relay, long irOn, long irOff)
  {
    this->push = push;
    this->is_open = is_open;
    this->relay = relay;
    this->_irOff = irOff;
    this->_irOn = irOn;
    this->_relayStatus = _relayStatus;
    this->_pushStatus = _pushStatus;
    init();
  }
  Commuter(byte toggle, byte relay, long irOn, long irOff)
  {
    this->toggle = toggle;
    this->_toggleStatus = _toggleStatus;
    this->relay = relay;
    this->_irOff = irOff;
    this->_irOn = irOn;
    this->_relayStatus = _relayStatus;
    init();
  }

  String getDeviceType()
  {
    if (push == 0 && toggle != 0)
    {

      return "TOGGLE";
    }
    else
    {
      if (push != 0 && toggle == 0)
      {
        return "PUSH";
      }
      else
      {
        if (push == 0 && toggle == 0 && relay != 0)
        {
          return "OUTLET";
        }
      }
    }
  }

  void init()
  {
    String device = Commuter::getDeviceType();
    if (device == "TOGGLE")
    {
      pinMode(toggle, INPUT);
    }
    if (device == "PUSH")
    {
      pinMode(push, INPUT);
    }

    pinMode(relay, OUTPUT);
  }

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
    setIr();
    int toggleReading = digitalRead(toggle);
    if (toggleReading != _toggleStatus)
    {
      delay(50);
      setRelayStatus();
      setToggleStatus();
    }
  }

  void printDevice()
  {
    String device = Commuter::getDeviceType();
    Serial.print(device);
  }
};

//Commuter instance recives, toggle input pin, relay output pin and ir on and off codes
Commuter toggle1(4, 13, 0xFE01BF40, 0xFD02BF40);

void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn();
  Serial.println("---Listo para recibir ordenes---");
}

void loop()
{
  toggle1.setCommuter();
}
