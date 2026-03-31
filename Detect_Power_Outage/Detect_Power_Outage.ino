#include <dht_nonblocking.h>
#include <LiquidCrystal.h>

struct VoltageResults
{
   float voltageOut{};
   float voltageIn{};
};

bool getVoltageValues(VoltageResults *);

constexpr int DHT_SENSOR_TYPE {DHT_TYPE_11};
constexpr int DHT_SENSOR_PIN {2};

const DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

constexpr int digital_Pin_5 {5};
constexpr int digital_Pin_6 {6};
constexpr int digital_Pin_7 {7};
constexpr int digital_Pin_8 {8};
constexpr int digital_Pin_9 {9};
constexpr int digital_Pin_10 {10};
constexpr int digital_Pin_11 {11};
constexpr int digital_Pin_12 {12};

constexpr int readPin{A0};
constexpr float voltageReference{5.0};
constexpr float resistor1{10000.0}; // R1 value for Ohms
constexpr float resistor2{10000.0}; // R2 value for Ohms
constexpr int seconds{5};
constexpr int milliseconds{seconds * 1000};

// initialize the library with the numbers of the interface pins
const LiquidCrystal lcd(digital_Pin_7, 
                  digital_Pin_8, digital_Pin_9, 
                  digital_Pin_10, digital_Pin_11, digital_Pin_12);
/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
bool measure_environment( const float &temperature, const float &humidity )
{
    if( dht_sensor.measure( &temperature, &humidity ) == true )
    {
      return( true );
    }

  return( false );
}

void setup() {
  Serial.begin(9600);
  Serial.println("");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(readPin, INPUT);

  pinMode(digital_Pin_5, OUTPUT);
  digitalWrite (digital_Pin_5, LOW);

  pinMode(digital_Pin_6, OUTPUT);
  digitalWrite (digital_Pin_6, LOW);

  // Print a message to the LCD.
  lcd.print("Power Detector");

  delay(3000);

}

void loop() 
{
  static float temperature{};
  static float humidity{};

  static long countIteration{0};

  constexpr VoltageResults voltage_results{};
  const VoltageResults *p_voltage_results{&voltage_results};

  static bool keepLooping = false;

  int switchStateOfLED = digitalRead(digital_Pin_6);

  static bool sendEmailNotification = false;

    /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( temperature, humidity ) && getVoltageValues(p_voltage_results))
  {

    lcd.begin(16, 2);
    lcd.print( "T=");

    Serial.print("T=");

    const float FDegs = ((temperature * 9)/5) + 32;
    lcd.print(FDegs, 1);
    Serial.print(FDegs,1);

    lcd.print( "F ");
    Serial.print("F ");
    lcd.print("Vo=");
    lcd.print(p_voltage_results->voltageOut, 2);
    lcd.setCursor(0, 1);

    lcd.print( "H=" );   
    Serial.print("H=");
    lcd.print( humidity, 1 );
    Serial.print(humidity,1);
    lcd.print( "% " );
    lcd.print("Vi=");
    Serial.println( "% " );

    lcd.print(p_voltage_results->voltageIn, 2);

    Serial.print("Voltage In= ");
    Serial.print(p_voltage_results->voltageIn, 2);

    Serial.print(" Voltage Out=");
    Serial.println(p_voltage_results->voltageOut,2);

    switchStateOfLED = digitalRead(digital_Pin_5);
    if(LOW == switchStateOfLED)
    {
      digitalWrite (digital_Pin_5, HIGH);
    }
    else
    {
      digitalWrite (digital_Pin_5, LOW);
    }

    if (p_voltage_results->voltageIn < 6.0)
    {
      keepLooping = true;

        static_cast<void>(getVoltageValues(p_voltage_results));

        if (p_voltage_results->voltageIn < 6.0)
        {
          lcd.begin(16, 2);
          lcd.print("Check Circuit");
          lcd.setCursor(0, 1);
          lcd.print("Breaker # 11");
          Serial.println("Check Circuit Breaker #11");
          sendEmailNotification = true;
          Serial.println(sendEmailNotification);
          switchStateOfLED = digitalRead(digital_Pin_5);
          if(LOW == switchStateOfLED)
          {
            digitalWrite (digital_Pin_5, HIGH);
          }

          delay(milliseconds);
        }
        else
        {
          sendEmailNotification = false;
          Serial.println(sendEmailNotification);
          switchStateOfLED = digitalRead(digital_Pin_5);
          if(LOW == switchStateOfLED)
          {
            digitalWrite (digital_Pin_5, HIGH);
          }
          else
          {
            digitalWrite (digital_Pin_5, LOW);
          }
        }

      switchStateOfLED = digitalRead(digital_Pin_6);
      if(LOW == switchStateOfLED)
      {
        digitalWrite (digital_Pin_6, HIGH);
      }
    
    }
    else if (p_voltage_results->voltageIn >= 6.0 && keepLooping == true)
    {
      keepLooping = false;
      sendEmailNotification = false;
      Serial.println(sendEmailNotification);
          switchStateOfLED = digitalRead(digital_Pin_5);
          if(LOW == switchStateOfLED)
          {
            digitalWrite (digital_Pin_5, HIGH);
          }
          else
          {
            digitalWrite (digital_Pin_5, LOW);
          }
      switchStateOfLED = digitalRead(digital_Pin_6);
      if(HIGH == switchStateOfLED)
      {
        digitalWrite (digital_Pin_6, LOW);
      }
    }
  }

  Serial.println(sendEmailNotification);
          switchStateOfLED = digitalRead(digital_Pin_5);
          if(LOW == switchStateOfLED)
          {
            digitalWrite (digital_Pin_5, HIGH);
          }
          else
          {
            digitalWrite (digital_Pin_5, LOW);
          }
}

bool getVoltageValues(VoltageResults *p_voltage_results)
{
      bool goodReadVO{false};
      bool goodReadVI{false};

      const int sensorValue{analogRead(readPin)};
      p_voltage_results->voltageOut = sensorValue * (voltageReference / 1023.0);

      if (p_voltage_results->voltageOut > 0)
      {
          goodReadVO = true;
      }

      // Use the voltage divider formula to get the original input voltage.
      p_voltage_results->voltageIn = (p_voltage_results->voltageOut * ((resistor1 + resistor2)) / resistor2);

      if (p_voltage_results->voltageIn > 0)
      {
          goodReadVI = true;
      }

      return (goodReadVO && goodReadVI);
}
