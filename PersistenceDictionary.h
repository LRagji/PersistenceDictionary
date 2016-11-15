/*
  PersistenceDictionary.h - Library for Persistence Dictionary on SD cards.
  Created by Laukik Ragji, November 13, 2016.
  Released with MIT.
*/

#ifndef PersistenceDictionary_h
#define PersistenceDictionary_h

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"

class PersistenceDictionary
{
  public:
    PersistenceDictionary();
    void Begin(String dictionaryFilePath);
    String GetValue(String key);
    void SetValue(String key, String value);
    void DeleteValue(String key);
  private:
    String _dictionaryFileLocation;
    void MarkDeleted(File &file, int keyLength);
    void SeekValueStartPosition(File &fileToSearch, String key);
    bool Matches(String key, byte buff[]);
};

#endif


