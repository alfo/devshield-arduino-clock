/*
    Devshield Arduino Clock - a basic starting point for getting the system time set
                              on your Arduino using BERGCloud and your server's system time

    v1.0 (30th November 2013)

    https://github.com/alfo/devshield-arduino-clock
*/

// Load all the Libraries
#include <BERGCloud.h>
#include <SPI.h>
#include <Time.h>



#define nSSEL_PIN 10
#define VERSION 1

// The Project Key ties this code into a Project on developer.bergcloud.com
const uint8_t PROJECT_KEY[BC_KEY_SIZE_BYTES] =
  {0x00,0x22,0x00,0x9C,0x00,0xC0,0x00,0x6E,0x00,0x0A,0x00,0xE8,0x00,0x00,0x32,0x00};

// Command definitions
#define COMMAND_SEND_TIME 1
#define COMMAND_TRIGGER_ALARM 5

// To keep track of the loop number
int loop_no = 0;

void setup() {

  // Begin all the things
  Serial.begin(115200);
  BERGCloud.begin(&SPI, nSSEL_PIN);
  Serial.println("--- Arduino reset ---");

  // Attempt to connect with our project key and build version
  if (BERGCloud.connect(PROJECT_KEY, VERSION)) {
    Serial.println("Connected to network");
  } else {
    Serial.println("BERGCloud.connect() returned false.");
  }
  
}

// Helper function to format the time
String formattedTime() {
  String formatted_time = "";
  if (hour() < 10)
    formatted_time += "0";
  formatted_time += hour();
  formatted_time += ":";
  if (minute() < 10)
    formatted_time += "0";
  formatted_time += minute();
  formatted_time += ":";
  if (second() < 10)
    formatted_time += "0";
  formatted_time += second();
  return formatted_time;
}

// Helper Function to display the time on the devshield display
void displayTime() {
      String formatted_time;
      formatted_time += formattedTime();
      BERGCloud.clearDisplay();
      BERGCloud.display(formatted_time);
}

// LOOP!
void loop() {
  
  // Increment the loop
  loop_no = loop_no + 1;
  
  // Init variables
  BERGCloudMessage command;
  byte command_id;
  
  // Has it been 30 seconds since the last poll? 
  if (loop_no % 30 == 0) {
    
    // Check for commands from BERGCloud
    if (BERGCloud.pollForCommand(command, command_id)) {
      
      // We've received a command!
      Serial.println("Received Command");
      
      // It is one that sets the time?
      if (command_id == COMMAND_SEND_TIME) {

        // Yes, it is.
        long number;
        long the_hour, min, sec, day, month, year;
  
        // Unpack all the things!
        if (command.unpack(number))
          the_hour = number;
        if (command.unpack(number))
          min = number;
        if (command.unpack(number))
          sec = number;
        if (command.unpack(number))
          day = number;
        if (command.unpack(number))
          month = number;
        if (command.unpack(number))
          year = number;
  
        // Set the Arduino system time using the unpacked vars
        setTime(the_hour, min, sec, day, month, year);
      }
    } else {
       Serial.println("No command"); 
    }
  }
  
  // Display the time
  displayTime();

  // Wait a second
  delay(1000);
}
