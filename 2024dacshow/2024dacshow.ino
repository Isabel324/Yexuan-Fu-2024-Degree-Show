#include <SoftwareSerial.h>

SoftwareSerial mySerial1(2, 3); // RX, TX
SoftwareSerial mySerial2(4, 5);
SoftwareSerial mySerial3(6, 7);
SoftwareSerial mySerial4(8, 9);
SoftwareSerial mySerial5(10, 11);
SoftwareSerial mySerial6(12, 13);


const int keyPins[] = {36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49, 50, 51}; // button
const char keyLetters[] = {'A', 'D', 'E', 'I', 'J', 'M', 'N', 'P', 'Z', 'Y', 'V', 'U', 'T', 'S', 'R'}; // button with wrolds
const char words[][20] = {"VIJANAN", "SATTVA", "ZEN", "PRAJNA", "PRATITYASAMUTPADA", "DASEIN"}; // words
const int confirmPin = 44; // confirm
const int numKeys = sizeof(keyPins) / sizeof(keyPins[0]);
const int maxStringLength = 20;

char currentLetters[maxStringLength]; // A string that stores the pressed letter
int currentLetterIndex = 0; // index at which the letter is currently stored
bool confirmed = false;

char str[30];

void setup() {
  Serial.begin(9600);

    mySerial1.begin(9600);
    mySerial2.begin(9600);
    mySerial3.begin(9600);
    mySerial4.begin(9600);
    mySerial5.begin(9600);
    mySerial6.begin(9600);


  for (int i = 0; i < numKeys; i++) {
    pinMode(keyPins[i], INPUT_PULLUP);
  }
  pinMode(confirmPin, INPUT_PULLUP);
//  delay(2000);
  //Because the serial port screen will send 88 ff ff ff when booting, the serial port buffer must be cleared.
   while ( mySerial6.read() >= 0); //Clear serial port buffer
//    mySerial[5].print("page page1\xff\xff\xff"); //Send a command to jump to the main page
////  for (int i = 0; i < 6; i++) {
////    sprintf(str, "page page1\xff\xff\xff");
//    mySerial[5].print("page page1");
//    mySerial[5].write(0xFF);
//    mySerial[5].write(0xFF);
//    mySerial[5].write(0xFF);
//    delay(5000);
////    sprintf(str, "page page0\xff\xff\xff");
    mySerial1.print("page page1");
    mySerial1.write(0xFF);
    mySerial1.write(0xFF);
    mySerial1.write(0xFF);
    delay(2000);
    mySerial4.print("page page1");
    mySerial4.write(0xFF);
    mySerial4.write(0xFF);
    mySerial4.write(0xFF);
    delay(2000);
//////  }
}

void loop() {

  // Read key status
  for (int i = 0; i < numKeys; i++) {
    if (digitalRead(keyPins[i]) == LOW) {
      currentLetters[currentLetterIndex] = keyLetters[i];
      Serial.print(keyLetters[i]);
      currentLetterIndex++;
      while (digitalRead(keyPins[i]) == LOW); // wait for key realse
      delay(50); // Delay for a short period of time to eliminate key jitter
    }
  }

  // Check confirmation key status
  if (digitalRead(confirmPin) == LOW && !confirmed && currentLetterIndex > 0) {
    currentLetters[currentLetterIndex] = '\0'; // Add string terminator
    char closestWord[20];
    int minDifference = maxStringLength; // Set to the maximum possible difference value
    int minDifferenceIndex = -1; // Store word index with minimum difference
    for (int j = 0; j < sizeof(words) / sizeof(words[0]); j++) {
      int difference = compareWords(currentLetters, words[j]);
      if (difference < minDifference) {
        minDifference = difference;
        minDifferenceIndex = j;
      }
    }
    if (minDifferenceIndex != -1) {
      Serial.println("\nClosest word: ");
      Serial.println(words[minDifferenceIndex]);
      
    } else {
      Serial.println("\nError: No match found in known words.");
    }
    currentLetterIndex = 0; // Reset the index currently storing letters
    confirmed = true;
  } else if (digitalRead(confirmPin) == HIGH && confirmed) {
    confirmed = false;
  }
}

// Compare the differences between two letter sequences
int compareWords(const char* letters1, const char* letters2) {
  int difference = 0;
  for (int i = 0; i < strlen(letters1); i++) {
    bool found = false;
    for (int j = 0; j < strlen(letters2); j++) {
      if (letters1[i] == letters2[j]) {
        found = true;
        break;
      }
    }
    if (!found) {
      difference++;
    }
  }
  return difference;
}
