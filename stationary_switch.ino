//usign IRremote 2.0.1
#include <IRremote.h>
#include "RTClib.h"

RTC_DS1307 rtc;
byte receiver = 7;
IRrecv irrecv(receiver);
decode_results results;
unsigned long onIRCodes[8] = {
    0xA888E157,
    0xDA28F020,
    0x2307B446,
    0xC0E821D2,
    0x5D8011C8,
    0xFD213B16,
    0x4B3D41B9,
    0x2AD99623,

} unsigned long offIRCodes[8] = {
    0xDCB93B93,
    0xE9D77D18,
    0x35974D2F,
    0x5061D043,
    0x71D06673,
    0x15E972C1,
    0xA4A34BB,
    0x7AD0864A,

}

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
  byte _hourOn;
  byte _hourOff;
  bool _is_open;
  long _irOn;
  long _irOff;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;

  void printStatus()
  {
    String device = Commuter::getDeviceType();
    Serial.print("Device: ");
    Serial.println(device);
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
  Commuter(byte relay) //, byte hourOn, byte hourOff, long irOn, long irOff)
  {
    //this->hourOn = hourOn;
    //this->hourOff = hourOff;
    this->relay = relay;
    //this->_irOff = irOff;
    //this->_irOn = irOn;
    pinMode(relay, OUTPUT);
  }
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
    this->_is_open = is_open;
    this->relay = relay;
    this->_irOff = irOff;
    this->_irOn = irOn;
    this->_relayStatus = _relayStatus;
    _is_open == true ? this->_lastPushStatus = LOW : this->_lastPushStatus = HIGH;
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

  void setFromRTC(bool state)
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
    /*int minutes = now.minute();*/

    bool hourCondition = (hour > 19);
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
    String device = Commuter::getDeviceType();
    setIr();

    if (device == "TOGGLE")
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

    if (device == "PUSH")
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

  void printDevice()
  {
    String device = Commuter::getDeviceType();
    Serial.print(device);
  }
};
/*Commuter instance recives, toggle input pin, relay output pin and ir on and off codes

Commuter lucesProgramadas(13, 7, 6, 0xFE01BF40, 0xFD02BF40);*/
Commuter lucesProgramadas(13);

void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn();
  Serial.println("---Listo para recibir ordenes---");
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}
//ScheduledOutput outlet(13);
void loop()
{
  DateTime now = rtc.now();
  int hora = now.hour();
  int minutos = now.minute();
  lucesProgramadas.isScheduledON();
  Serial.print("la hora es: ");
  Serial.print(hora);
  Serial.print(":");
  Serial.println(minutos);
}
