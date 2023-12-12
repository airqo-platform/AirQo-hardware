#include "configuration.h"
String option;
unsigned long lastInputTime = 0;  // Variable to store the time of the last input
void clear_eeprom()
{
  Serial.println(EEPROM.length());
  unsigned long startMillis = millis();
  for (int i = 0 ; i < EEPROM.length() ; i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  // The time spent can be very short (4-5ms) if the EEPROM is not dirty.
  // For F767ZI, the time is around 1.1s for 16384 bytes of emulated-EEPROM
  Serial.print("Done clearing emulated EEPROM. Time spent (ms) = ");
  Serial.println(millis() - startMillis);
  Serial.print(F("Cleared "));Serial.print(EEPROM.length());Serial.println(F(" bytes of memory"));
}
void configure_device_params()
{
    Serial.println(F("****************** Entered Device Configuration Mode ******************"));
    Serial.println(BOARD_NAME);
    Serial.println(F("Options:"));
    Serial.println(F("  1. Configure Device Name"));
    Serial.println(F("  2. Configure LORAWAN DEVEUI"));
    Serial.println(F("  3. Configure LORAWAN APPKEY"));
    Serial.println(F("  4. Configure Device Channel"));
    Serial.println(F("  5. Configure Deployment Mode"));
    Serial.println(F("  6. Clear saved values"));
    Serial.println(F("  X. Exit Configuration Mode"));
    Serial.println(F("Enter the option number or 'H' for help menu"));
    Serial.println(F("Configuration mode will exit automatically if no input is made in one minute."));

    while (1)
    {
        option = "";  // Reset the option before reading a new input

        if (Serial.available())
        {
            option = Serial.readString();
            lastInputTime = millis();  // Update the last input time
        }

        // Check if one minute has passed without input
        if (millis() - lastInputTime > 60000)
        {
            Serial.println(F("\nNo input received for one minute. Exiting configuration mode."));
            break;
        }

        if (option.equalsIgnoreCase("H"))
        {
            // Display help menu
            Serial.println(F("\nOptions:"));
            Serial.println(F("  1. Configure DEVEUI"));
            Serial.println(F("  2. Configure APPKEY"));
            Serial.println(F("  3. Configure Channel"));
            Serial.println(F("  4. Configure Deployment Mode"));
            Serial.println(F("  5. Configure Deployment Mode"));
            Serial.println(F("  6. Clear saved values"));
            Serial.println(F("  X. Exit Configuration Mode"));
        }
        else if (option.equalsIgnoreCase("X"))
        {
            // Exit the configuration loop
            Serial.println(F("\nExiting configuration mode."));
            break;
        }
        else if (option.equals("1"))
        {
            // Configure DEVEUI
            // Implement the code to configure DEVEUI
            Serial.print(F("enter device name:"));
            while(1)
            {
                if (Serial.available())
                {
                    option = Serial.readString();
                    lastInputTime = millis();  // Update the last input time
                    //save eeprom data here;
                }

                // Check if 15 seconds have passed without input and exit this loop
                if (millis() - lastInputTime > 15000)
                {
                    Serial.println(F("\nNo input received for 15 seconds Exiting back to configuration mode."));
                    break;
                }
            }
        }
        else if (option.equals("2"))
        {
            // Configure APPKEY
            // Implement the code to configure APPKEY
        }
        else if (option.equals("3"))
        {
            // Configure channel
            // Implement the code to configure channel
        }
        else if (option.equals("4"))
        {
            // Configure deployment mode
            // Implement the code to configure deployment mode
        }
        else if (option.equals("5"))
        {
            // Configure deployment mode
            // Implement the code to configure deployment mode
        }
        else if (option.equals("6"))
        {
            // Configure deployment mode
            // Implement the code to configure deployment mode
            Serial.println(F("Clearing saved configurations in eeprom"));
            clear_eeprom();
        }
    }
}
