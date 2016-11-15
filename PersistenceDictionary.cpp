/*
  PersistenceDictionary.cpp - Library for Persistence Dictionary on SD cards.
  Created by Laukik Ragji, November 13, 2016.
  Released with MIT.
*/

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"
#include "PersistenceDictionary.h"

PersistenceDictionary::PersistenceDictionary()
{
}

void PersistenceDictionary::Begin(String dictionaryFilePath)
{
  _dictionaryFileLocation = dictionaryFilePath;
}

String PersistenceDictionary::GetValue(String key)
{
  String returnValue = "";
  File file = SD.open(_dictionaryFileLocation, FILE_READ);
  if (file == NULL)
  {
    //Serial.println(_dictionaryFileLocation);
    //Serial.println("File is null");
    return "";
  }
  key = 'A' + key;
  SeekValueStartPosition(file, key);
  if (file.position() != file.size())
  {
    int bytesRead = file.read();
    while (bytesRead > 0 & ((char)bytesRead) != '\r')
    {
      returnValue += String((char)bytesRead);
      bytesRead = file.read();
    }
  }
  file.close();
  return returnValue;
}

void PersistenceDictionary::SetValue(String key, String value)
{
  key = 'A' + key;
  File file = SD.open(_dictionaryFileLocation, FILE_WRITE);
  SeekValueStartPosition(file, key);
  if (file.position() != file.size())
  {
    //Serial.println("Deleting exiting");
    //Update Mode so delete first and make entry for new one.
    MarkDeleted(file, key.length());
  }
  //Create Mode
  file.seek(file.size());
  file.println(key + "," + value);
  file.close();
}

void PersistenceDictionary::DeleteValue(String key)
{
  File file = SD.open(_dictionaryFileLocation, FILE_WRITE);
  if (file == NULL)
  {
    //Serial.println("File is null");
    return;
  }
  key = 'A' + key;
  SeekValueStartPosition(file, key);
  if (file.position() != file.size())
  {
    MarkDeleted(file, key.length());
  }
  file.close();
}

void PersistenceDictionary::MarkDeleted(File &file, int keyLength)
{
  file.seek(file.position() - (1 + keyLength)); //Should Bring to start of the line.
  file.write('D');
}

bool PersistenceDictionary::Matches(String key, byte buff[])
{
  int index = 0;
  while (index < key.length())
  {
    //    Serial.print(index);
    //    Serial.print(" ");
    //    Serial.print(key[index]);
    //    Serial.print(" ");
    //    Serial.println(buff[index]);
    if (key[index] != buff[index])
    {
      return false;
    }
    index++;
  }
  return true;
}

void PersistenceDictionary::SeekValueStartPosition(File &fileToSearch, String key)
{
  fileToSearch.seek(0);
  bool matches = false;
  byte sdbuffer[key.length() + 1];
  int bytesRead = fileToSearch.read(sdbuffer, sizeof(sdbuffer));
  while (bytesRead > 0)
  {
    matches = Matches((key + ","), sdbuffer);
//    Serial.print("Match ");
//    Serial.print(key + ", ");
//    Serial.println(matches);
    if (!matches)
    {
      //read until end line
      bytesRead = fileToSearch.read(sdbuffer, 1);
      while (bytesRead > 0 & ((char)sdbuffer[0]) != '\n')
      {
        bytesRead = fileToSearch.read(sdbuffer, 1);
      }

      bytesRead = fileToSearch.read(sdbuffer, sizeof(sdbuffer));
    }
    else
    {
      return;
    }
  }
}
