//usign IRremote 2.0.1
#include <IRremote.h>

byte receiver = 7;
IRrecv irrecv(receiver);
decode_results results;
//

class Commuter
{

private:
  byte push;
  byte toggle;
  byte relay;
  byte _toggleStatus;
  byte _relayStatus;
  byte _pushStatus;
  byte _lastPushStatus;
  String deviceName = "";
  bool _is_open;
  long _irOn;
  long _irOff;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;

  void printStatus()
  {
    //String device = Commuter::getDeviceType();
    Serial.print("Device: ");
    Serial.println(deviceName);
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
  Commuter(byte relay) //long irOn, long irOff)
  {
    this->relay = relay;
    // this->_irOff = irOff;
    // this->_irOn = irOn;
    this->_relayStatus = _relayStatus;
    this->deviceName = "OUTLET";
    init();
  }

  Commuter(byte push, bool is_open, byte relay) //, long irOn, long irOff)
  {
    this->push = push;
    this->_is_open = is_open;
    this->relay = relay;
    // this->_irOff = irOff;
    // this->_irOn = irOn;
    this->_relayStatus = _relayStatus;
    this->deviceName = "PUSH";
    _is_open == true ? this->_lastPushStatus = LOW : this->_lastPushStatus = HIGH;
    init();
  }
  Commuter(byte toggle, byte relay) //, long irOn, long irOff)
  {
    this->toggle = toggle;
    this->_toggleStatus = _toggleStatus;
    this->relay = relay;
    // this->_irOff = irOff;
    // this->_irOn = irOn;
    this->deviceName = "TOGGLE";
    this->_relayStatus = _relayStatus;
    init();
  }

  /*void setFromRTC(bool state)
  {
    if (state)
    {
      digitalWrite(relay, HIGH);
      //printStatus();
    }
    else
    {
      digitalWrite(relay, LOW);
      //printStatus();
    }
  }
  void isScheduledON()
  {
    DateTime now = rtc.now();

    int hour = now.hour();
    /*int minutes = now.minute();

  bool hourCondition = ((hour > onHour) && (hour < offHour));
  // Miercoles, Sabado o Domingo
  //bool dayCondition = (weekDay == 3 || weekDay == 6 || weekDay == 0);
  if (hourCondition)
  {
    Serial.println("luz encendida");
    setFromRTC(true);
  }
  else
  {

    setFromRTC(false);
  }
} */
  /*String getDeviceType()
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
  }*/

  void init()
  {
    //String device = Commuter::getDeviceType();
    if (deviceName == "TOGGLE")
    {
      pinMode(toggle, INPUT);
    }
    if (deviceName == "PUSH")
    {
      pinMode(push, INPUT);
    }

    pinMode(relay, OUTPUT);
  }

  void setIr(bool isOn)
  {
    if (isOn)
    {
      _relayStatus = HIGH;
      digitalWrite(relay, _relayStatus);
      printStatus();
    }
    else
    {
      _relayStatus = LOW;
      digitalWrite(relay, _relayStatus);
      printStatus();
    }
  }

  void
  setCommuter()
  {
    //detects if any change has happend in toggle status
    //String device = Commuter::getDeviceType();

    if (deviceName == "TOGGLE")
    {
      int toggleReading = digitalRead(toggle);
      if (toggleReading != _toggleStatus)
      {
        lastDebounceTime = millis();
      }
      if ((millis() - lastDebounceTime) > debounceDelay)
      {
        setRelayStatus();
        setToggleStatus();
      }
    }

    if (deviceName == "PUSH")
    {
      int pushReading = digitalRead(push);
      if (pushReading != _lastPushStatus)
      {
        lastDebounceTime = millis();
      }
      if ((millis() - lastDebounceTime) > debounceDelay)
      {
        setRelayStatus();
      }
    }
  }

  /* void printDevice()
  {
    String device = Commuter::getDeviceType();
    Serial.print(device);
  }*/
};
//byte toggleSwitchPins[7] = {2, 3, 4, 5, 13, 14,15 };
// byte relayPins[8] = {6, 10, 8, 9, 11, 12, 17, 18};

Commuter luzBalcon(2, 6);
Commuter luzRepisas(3, 10);
Commuter luzBar(4, 8);
Commuter luzTV(5, 9);
Commuter luzEscaleras(13, 11);
Commuter luzComedor(14, 12);
Commuter luzCuadro(15, 19);
Commuter luzCerramiento(18);

void IRFunction()
{
  if (irrecv.decode())
  {
    switch (irrecv.decodedIRData.decodedRawData)
    {
    case 0x24DBBF40:
      luzBalcon.setIr(true);
      break;
    case 0x26D9BF40:
      luzRepisas.setIr(true);
      break;
    case 0x27D8BF40:
      luzBar.setIr(true);
      break;
    case 0x28D7BF40:
      luzTV.setIr(true);
      break;
    case 0x1FEBF40:
      luzComedor.setIr(true);
      break;
    case 0x29D6BF40:
      luzCuadro.setIr(true);
      break;
    case 0x2AD5BF40:
      luzEscaleras.setIr(true);
      break;
    case 0x2BD4BF40:
      luzBalcon.setIr(false);
      break;
    case 0x38C7BF40:
      luzRepisas.setIr(false);
      break;
    case 0xF00FBF40:
      luzBar.setIr(false);
      break;
    case 0x3AC5BF40:
      luzTV.setIr(false);
      break;
    case 0x3BC4BF40:
      luzEscaleras.setIr(false);
      break;
    case 0xFF0BF40:
      luzComedor.setIr(false);
      break;
    case 0xA758BF40:
      luzCuadro.setIr(false);
      break;
    case 0xEF1BF40:
      luzCerramiento.setIr(true);
      break;
    case 0x3FC0BF40:
      luzCerramiento.setIr(false);
      break;
    }
  }
  irrecv.resume();
}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("---Listo para recibir ordenes---");
}

void loop()
{
  IRFunction();
  luzBalcon.setCommuter();
  luzRepisas.setCommuter();
  luzTV.setCommuter();
  luzBar.setCommuter();
  luzEscaleras.setCommuter();
  luzComedor.setCommuter();
  luzCuadro.setCommuter();
}
