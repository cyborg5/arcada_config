/* Illustrate weird problem with Arcada configuration file system.
 * If you run this program it should look for "/arcada_configure.json"
 * And if it doesn't find it, create it and write the volume and back light
 * Brightness. Then you can edit that file and on subsequent runs it will
 * use those values to initialize.
 * 
 * If the file already exists, it works perfectly. But if the file doesn't
 * exist, it says that it creates but when you go into  Windows Explorer
 * you could not see the file. Yet when it prints out the list of existing files,
 * it definitely says the file is there. If you run the program a second time it finds
 * the file it created on the previous run which you couldn't see.
 * After the second run, it continues to work properly you CAN the the file now.
 * 
 * We are running this program on PyGamer/M4 Express.
 */
#define USE_SD_CARD true 
#if(USE_SD_CARD)
  #define FILESYSTEM ARCADA_FILESYS_SD
#else
  #define FILESYSTEM ARCADA_FILESYS_QSPI
#endif
#ifndef USE_TINYUSB
  #error In the Arduino IDE change to Tools->USB Stack->TinyUSB
#endif  

#include <Adafruit_Arcada.h>

Adafruit_Arcada Device;

void fatal(const char *message, uint16_t blinkDelay) {
  Serial.println(message);
  for (bool ledState = HIGH;; ledState = !ledState) {
    digitalWrite(LED_BUILTIN, ledState);
    delay(blinkDelay);
  }
}

void setup(void) {
  Serial.begin(115200);//sometimes you have to move this after the filesysBeginMSD
  Device.arcadaBegin();
  if (!Device.filesysBeginMSD(FILESYSTEM)) fatal("No filesystem found!", 250);
  delay(1000);
#if(USE_SD_CARD)
  Serial.println("Using SD card");
#else
  Serial.println("Using internal QSPI memory");
#endif
  delay(12000);
  Serial.println("\n\n\nAttempting to read configuration");
  if (Device.loadConfigurationFile(ARCADA_DEFAULT_CONFIGURATION_FILENAME)){
    Serial.println("Configuration Loaded");
  } else {
    Serial.println("Could not load configuration.");
  }
  Serial.printf("Volume=%d\n",Device.getVolume());
  Serial.printf("Brightness=%d\n", Device.getBacklight());
  delay(1000);
  //Presumably now we do something in our program and when we are done we save
//  Value1++;
//  Value2--;
  //now we save
  Serial.println ("\nAttempting to save configuration");
  if (Device.saveConfigurationFile(ARCADA_DEFAULT_CONFIGURATION_FILENAME)){
    Serial.println("Configuration Saved");
  } else {
    Serial.println("Failed to save configuration");
  }
  Serial.printf("Found %d files.\n",Device.filesysListFiles());
}
void loop(void) {
};
