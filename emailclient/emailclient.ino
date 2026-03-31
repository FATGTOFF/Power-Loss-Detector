#include <Arduino.h>

#if defined(ESP32)
  #include <WiFi.h>
	#include <WebServer.h>  // standard library
	#include "ESP32_GPIO.h"
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>
#include "time.h"
#include "WebPage.h"
#include "NetworkSettings.h"

#define TURN_WIFI_ON 1

uint8_t ledPin[GPIO_40]{};

constexpr uint8_t RXp2 {UseWithCautionPins::GPIO::GPIO_16};
constexpr uint8_t TXp2 {UseWithCautionPins::GPIO::GPIO_17};

constexpr long  gmtOffset_sec = 3600; // Example: GMT +1 hour offset
constexpr int   daylightOffset_sec = 3600; // Example: 1 hour daylight saving offset

WebServer server(80);

String TempDataToPass{""};
String VoltageDataToPass{""};

void startServer()
 {

	int switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
	if(HIGH == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], LOW);
	}

	server.on("/", []() 
	{
		server.send(200, "text/html", webPage);
	});
	
	server.on("/TempData", []() 
	{
		server.send(200, "text/plain", TempDataToPass);
		const int switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
		if(LOW == switchStateOfLED)
		{
			digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], HIGH);
		}
	});

	if(HIGH == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], LOW);
	}

	server.on("/VoltageData", []() 
	{
		server.send(200, "text/plain", VoltageDataToPass);
		const int switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
		if(LOW == switchStateOfLED)
		{
			digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], HIGH);
		}
	});
	
	server.begin();
  Serial.println("Webserver started.");
}

void WiFiEvent(WiFiEvent_t event) 
{
  switch(event) 
	{

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi lost connection. Reconnecting...");
      WiFi.reconnect();
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("Reconnected. IP: ");
      Serial.println(WiFi.localIP());
      startServer();   // estart webserver after reconnect
      break;
  }
}

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

int getCurrentMinute()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return -1;
  }
  else
  {
    return timeinfo.tm_min;
  }
}

bool sendEmail(const bool shouldSendEmail)
{
  bool emailSent{false};
  int switchStateOfLED = ledPin[SafePinsToUse::GPIO::GPIO_25];
#if (TURN_WIFI_ON == 1)
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED && shouldSendEmail) 
  {
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
    if(LOW == switchStateOfLED)
    {
	  digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], HIGH);
    }
    else
    {
      digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], LOW);
    }
  }
#endif

  if (WiFi.status() == WL_CONNECTED && shouldSendEmail)
  {
    Serial.println("\nWiFi connected, sending email.");
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_27]);

    if(LOW == switchStateOfLED)
    {
	  digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_27], HIGH);
    }
  }
  else if (WiFi.status() == WL_CONNECTED)
  {
	switchStateOfLED = ledPin[SafePinsToUse::GPIO::GPIO_25];
    if(LOW == switchStateOfLED)
    {
      digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], HIGH);
    }
  }


  // Configure email
  Session_Config config;
  config.server.host_name = SMTP_SERVER;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";

  // Set the message
  SMTP_Message message;
  message.sender.name = "Basement";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "No Power on Basement";
  message.addRecipient("First Last", RECIPIENT_EMAIL_1);
  //message.addRecipient("First Last", RECIPIENT_EMAIL_2);
  //message.addRecipient("First Last", RECIPIENT_EMAIL_3);
  message.text.content = "I detected the loss of power in the basement. Check the circuit breaker #XX";

  // Send the email
  if (!smtp.connect(&config) && shouldSendEmail) 
  {
      Serial.println("SMTP connection failed: " + smtp.errorReason());
      emailSent = false;
	  switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_26]);
      if(LOW == switchStateOfLED)
	  {
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_26], HIGH);
	  }
      return emailSent;
  }

  if (!MailClient.sendMail(&smtp, &message)) 
  {
      Serial.println("Error sending email: " + smtp.errorReason());
      emailSent = false;

	  switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_26]);
      if(LOW == switchStateOfLED)
	  {
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_26], HIGH);
	  }

  }

  else 
  {
      Serial.println("Email sent successfully!");
      emailSent = true;
	  
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
	if(HIGH == switchStateOfLED)
	{
	digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], LOW);
	}

	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_26]);
	if(HIGH == switchStateOfLED)
	{
	digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_26], LOW);
	}	  	
	  
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_33]);
	if(HIGH == switchStateOfLED)
	{
	digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_33], LOW);
	}	  		  
	  
	  switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_27]);
      if(LOW == switchStateOfLED)
	  {
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_27], HIGH);
	  }	 

      // Wait 60 minutes
      // 10 minutes = 600,000 milliseconds      
      vTaskDelay((600000 * 6) / portTICK_PERIOD_MS);

	  switchStateOfLED = digitalRead(ledPin[UseWithCautionPins::GPIO::GPIO_02]);
    if(HIGH == switchStateOfLED)
	  {
		digitalWrite (ledPin[UseWithCautionPins::GPIO::GPIO_02], HIGH);
	  }
	  
	  switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_27]);
    if(HIGH == switchStateOfLED)
	  {
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_27], LOW);
	  }	  	  
  }

  return emailSent;
}

void checkLEDs()
{
	pinMode(ledPin[UseWithCautionPins::GPIO::GPIO_02], OUTPUT);
	int switchStateOfLED = digitalRead(ledPin[UseWithCautionPins::GPIO::GPIO_02]);
	if(LOW == switchStateOfLED)
	{
		digitalWrite (ledPin[UseWithCautionPins::GPIO::GPIO_02], HIGH);
	}	 	

	pinMode(ledPin[SafePinsToUse::GPIO::GPIO_25], OUTPUT);
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
	if(LOW == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], HIGH);
	}	 	

	pinMode(ledPin[SafePinsToUse::GPIO::GPIO_26], OUTPUT);
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_26]);
	if(LOW == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_26], HIGH);
	}

	pinMode(ledPin[SafePinsToUse::GPIO::GPIO_27], OUTPUT);
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_27]);
	if(LOW == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_27], HIGH);
	}

	pinMode(ledPin[SafePinsToUse::GPIO::GPIO_33], OUTPUT);
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_33]);
	if(LOW == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_33], HIGH);
	} 	

	vTaskDelay((3000) / portTICK_PERIOD_MS);
	switchStateOfLED = digitalRead(ledPin[UseWithCautionPins::GPIO::GPIO_02]);
	if(HIGH == switchStateOfLED)
	{
		digitalWrite (ledPin[UseWithCautionPins::GPIO::GPIO_02], LOW);
	}	 	

	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
	if(HIGH == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], LOW);
	}	 	

	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_26]);
	if(HIGH == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_26], LOW);
	}

	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_27]);
	if(HIGH == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_27], LOW);
	}

	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_33]);
	if(HIGH == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_33], LOW);
	} 			
}
// Uncomment if different GPIO
// will be used as UART.
//HardwareSerial mySerial(2);
//String dataToPass{""};

void setup()
{

	Serial.begin(115200);
	Serial.println("");
	
	Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
	Serial2.println("");
	
	for (unsigned countElements = 0; countElements < GPIO_40; countElements++)
	{
		ledPin[countElements] = countElements;
	}
	checkLEDs();	
	
	int switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);

	WiFi.onEvent(WiFiEvent);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	
	while (WiFi.status() != WL_CONNECTED) 
	{
		switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
		if(LOW == switchStateOfLED)
		{
			digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], HIGH);
		}
		else
		{
			digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], LOW);
		}
	}
	
	switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
	if(HIGH == switchStateOfLED)
	{
		digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], LOW);
	}

	// Initialize and get the time
	configTzTime("EST5EDT,M3.2.0/2,M11.1.0", NTP_SERVER);
	static_cast<void>(getCurrentMinute());

	Serial.println("");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

}

void loop()
{
	String stringRead = "0";
	static int switchStateOfLED = digitalRead(ledPin[UseWithCautionPins::GPIO::GPIO_02]);
	bool readyToSendEmail{false};
	static bool emailSent{false};

	if (static_cast<int>(HIGH) == switchStateOfLED)
	{
		digitalWrite(ledPin[UseWithCautionPins::GPIO::GPIO_02], static_cast<uint8_t>(LOW));
	}

	if(Serial2.available() > 0)
	{

		stringRead = Serial2.readStringUntil('\n');
		stringRead.trim();

		if (stringRead == "1")
		{
			Serial.println(stringRead);
			readyToSendEmail = true;
		}
		else if (stringRead == "0")
		{
			readyToSendEmail = false;
		}
		else
		{
			VoltageDataToPass = Serial2.readStringUntil('\n');
			TempDataToPass = stringRead;
			stringRead += "\n";					
			stringRead += VoltageDataToPass;
			
			Serial.println(stringRead);

			server.handleClient();

			switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_25]);
			if(HIGH == switchStateOfLED)
			{
				digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_25], LOW);
			}
		}

		switchStateOfLED = digitalRead(ledPin[SafePinsToUse::GPIO::GPIO_33]);
		if(LOW == switchStateOfLED)
		{
			digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_33], HIGH);
		}
		else
		{
			digitalWrite (ledPin[SafePinsToUse::GPIO::GPIO_33], LOW);
		}    
	}

	if(stringRead == "1" && readyToSendEmail)
	{

		if (sendEmail(readyToSendEmail))
		{
			switchStateOfLED = digitalRead(ledPin[UseWithCautionPins::GPIO::GPIO_02]);
			if(LOW == switchStateOfLED)
			{
				digitalWrite (ledPin[UseWithCautionPins::GPIO::GPIO_02], HIGH);
			}

			readyToSendEmail = false;

			while((Serial2.available() > 0) && (stringRead != "0"))
			{
				stringRead = Serial2.readStringUntil('\n');
				stringRead.trim();
				Serial.println(stringRead);
			}

			switchStateOfLED = digitalRead(ledPin[UseWithCautionPins::GPIO::GPIO_02]);
			if(HIGH == switchStateOfLED)
			{
				digitalWrite (ledPin[UseWithCautionPins::GPIO::GPIO_02], LOW);
			}
				emailSent = true;
		}
		else
		{
			emailSent = false;
			readyToSendEmail = false;
		}

	}

}
