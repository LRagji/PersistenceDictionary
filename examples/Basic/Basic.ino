/*
  Basic.ino - Example sketch for PersistenceDictionary Library
  Created by Laukik Ragji, November 13, 2016.
  Released with MIT.
*/

#include "PersistenceDictionary.h"
#include <SPI.h>
#include <SD.h>

#define SD_CS 10

void InitCard()
{
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed, No card found.");
    return;
  }
  Serial.println("completed");
}

bool SerialAssert(String expected, String actual)
{
  if (actual != expected)
  {
    Serial.print("Failled! Expected: \"");
    Serial.print(expected);
    Serial.print("\" was: \"");
    Serial.print(actual);
    Serial.println("\"");
    return false;
  }
  else
  {
    Serial.println("Pass.");
    return true;
  }
}

void SingleKVPTests()
{
  String filePath = "Single.csv";
  String key = "Hello";
  String value = "World";

  PersistenceDictionary dic;
  dic.Begin(filePath);
  SD.remove(filePath);//Clean the stuff

  Serial.println("Setting value");
  dic.SetValue(key, value);

  Serial.print("Getting Value ");
  if (!SerialAssert(value, dic.GetValue(key)))return;

  Serial.println("Deleting  Value");
  dic.DeleteValue(key);

  Serial.print("Getting Deleted Value ");
  if (! SerialAssert("", dic.GetValue(key)))return;

  Serial.println("Setting value");
  dic.SetValue(key, value);

  Serial.print("Getting Value ");
  if (!SerialAssert(value, dic.GetValue(key)))return;

  value = "NewValue";
  Serial.println("Updating Value");
  dic.SetValue(key, value);

  Serial.print("Getting Value ");
  if (!SerialAssert(value, dic.GetValue(key))) return;

  Serial.println("Deleting  Value");
  dic.DeleteValue(key);

  Serial.print("Getting Deleted Value ");
  if (! SerialAssert("", dic.GetValue(key)))return ;

  Serial.println("Single key value Tests Completed");

  StressTests();
}

void StressTests()
{
  unsigned long ctr = 1;
  String filePath = "Stress.csv";
  String key = "Hello";
  String value = "World";

  PersistenceDictionary dic;
  dic.Begin(filePath);
  SD.remove(filePath);//Clean the stuff

  unsigned int stressLimit = 100;
  long starttime = millis();
  while (ctr <= stressLimit)
  {
    String counter = String(ctr);
    long starttimei = millis();
    dic.SetValue(key + counter , value + counter);
    Serial.print("Writting ");
    Serial.print(ctr);
    Serial.print(" values completed in ");
    Serial.print(millis() - starttimei);
    Serial.println(" ms");
    ctr++;

  }
  Serial.print("Total Writting ");
  Serial.print(stressLimit);
  Serial.print(" values completed in ");
  Serial.print(millis() - starttime);
  Serial.println(" ms");
  ctr = 1;
  starttime = millis();
  while (ctr <= stressLimit)
  {
    String counter = String(ctr);
    long starttimei = millis();
    String actual = dic.GetValue(key + counter);
    Serial.print("Reading ");
    Serial.print(ctr);
    Serial.print(" values completed in ");
    Serial.print(millis() - starttimei);
    Serial.println(" ms");
    if (!SerialAssert(actual, value + counter))
    {
      return;
    }
    ctr++;
  }
  Serial.print("Total Reading ");
  Serial.print(stressLimit);
  Serial.print(" values completed in ");
  Serial.print(millis() - starttime);
  Serial.println(" ms");
  Serial.println("Stress Tests Completed");
}

void setup() {
  Serial.begin(9600);

    //  while (!Serial);

    InitCard();

    SingleKVPTests();
  }

  void loop() {
  // put your main code here, to run repeatedly:

}
