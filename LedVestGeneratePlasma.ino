// This sketch can genereate Plasma to the SD card if pin 2 is held low
//Otherwise it will play from the SD card. 


#include <Adafruit_NeoPixel.h>
#include <SD.h>


float movement = 0;
float movementFactor = 0.093;

float brightness = 50;
float size = 14;
float worldWidth = 40;
float worldHeight = 20;
int ledFactor = 4; //add spacing to leds. higher = finer

int minShade = -112;
int maxShade = 112;
float shade = 0.1;

unsigned long generateFrames = 100000;

//LED STRIP
const int DATAPIN = 6;
const int LEDCOUNT = 470;
//const int LEDCOUNT = 111;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDCOUNT, DATAPIN, NEO_GRB + NEO_KHZ400);

//SD CARD
File myFile;
const int chipSelect = 4;

//SD Card Info
Sd2Card card;
SdVolume volume;
SdFile root;


//Optimization Mathematics
float  sinShadePiRed, sinShadePiGreen, sinShadePiBlue = 0.0;
float cx = 20;
float cy = 10;
float dist = 0; 
int r,g,b;
uint32_t col = 0;
  
class LED
{
public:
      int x;
      int y;
      LED(int _x, int _y)
      {
        x=_x;
        y=_y;
      }
 };
  
//each line is a vertical on the vest  
LED leds[LEDCOUNT] = 
{
 //Front Right
 LED(0,1), LED(0, 2), LED(0,3), LED(0,4), LED(0,5), LED(0,6), LED(0,7), LED(0,8), LED(0,9), LED(0,10), //9
 LED(1, 13),LED(1,12),LED(1,11),LED(1,10),LED(1,9),LED(1,8),LED(1,7),LED(1,6),LED(1,5),LED(1,4),LED(1,3),LED(1,2),LED(1,1),LED(1,0), //14
 LED(2,0),LED(2,1), LED(2, 2), LED(2,3), LED(2,4), LED(2,5), LED(2,6), LED(2,7), LED(2,8), LED(2,9), LED(2,10),LED(2,11),LED(2,12),LED(2,13),LED(2,14),LED(2,15),//16
 
 LED(3, 17),LED(3, 16),LED(3, 15),LED(3, 14),LED(3, 13),LED(3,12),LED(3,11),LED(3,10),LED(3,9),LED(3,8),LED(3,7),LED(3,6),LED(3,5),LED(3,4),LED(3,3),LED(3,2),LED(3,1),LED(3,0),//18
 LED(4,0),LED(4,1), LED(4, 2), LED(4,3), LED(4,4), LED(4,5), LED(4,6), LED(4,7), LED(4,8), LED(4,9), LED(4,10),LED(4,11),LED(4,12),LED(4,13),LED(4,14),LED(4,15),LED(4,16),LED(4,17),//18
 LED(5, 17),LED(5, 16),LED(5, 15),LED(5, 14),LED(5, 13),LED(5,12),LED(5,11),LED(5,10),LED(5,9),LED(5,8),LED(5,7),LED(5,6),LED(5,5),LED(5,4),LED(5,3),LED(5,2),LED(5,1),LED(5,0),//18
 LED(6,1), LED(6, 2), LED(6,3), LED(6,4), LED(6,5), LED(6,6), LED(6,7), LED(6,8), LED(6,9), //9
 LED(7,8),LED(7,7),LED(7,6),LED(7,5),LED(7,4),LED(7,3),LED(7,2),LED(7,1),//8

 //BACK
 LED(8,1), LED(8, 2), LED(8,3), LED(8,4), LED(8,5), LED(8,6), LED(8,7), LED(8,8), 
 LED(9,9),LED(9,8),LED(9,7),LED(9,6),LED(9,5),LED(9,4),LED(9,3),LED(9,2),LED(9,1),
 LED(10,1), LED(10, 2), LED(10,3), LED(10,4), LED(10,5), LED(10,6), LED(10,7), LED(10,8), LED(10,9),LED(10,10),LED(10,11),
 
 LED(11, 18), LED(11, 17),LED(11, 16),LED(11, 15),LED(11, 14),LED(11, 13),LED(11,12),LED(11,11),LED(11,10),LED(11,9),LED(11,8),LED(11,7),LED(11,6),LED(11,5),LED(11,4),LED(11,3),LED(11,2),LED(11,1),
 LED(12,1), LED(12, 2), LED(12,3), LED(12,4), LED(12,5), LED(12,6), LED(12,7), LED(12,8), LED(12,9),LED(12,10),LED(12,11),LED(12,12), LED(12,13), LED(12,14), LED(12,15),LED(12,16),LED(12,17),LED(12,18),
 LED(13, 18),LED(13, 17),LED(13, 16),LED(13, 15),LED(13, 14),LED(13, 13),LED(13,12),LED(13,11),LED(13,10),LED(13,9),LED(13,8),LED(13,7),LED(13,6),LED(13,5),LED(13,4),LED(13,3),LED(13,2),LED(13,1),
 
 LED(14,1), LED(14, 2), LED(14,3), LED(14,4), LED(14,5), LED(14,6), LED(14,7), LED(14,8), LED(14,9),LED(14,10),LED(14,11),LED(14,12), LED(14,13), LED(14,14), LED(14,15),LED(14,16),LED(14,17),
 LED(15, 17),LED(15, 16),LED(15, 15),LED(15, 14),LED(15, 13),LED(15,12),LED(15,11),LED(15,10),LED(15,9),LED(15,8),LED(15,7),LED(15,6),LED(15,5),LED(15,4),LED(15,3),LED(15,2),LED(15,1),
 LED(16,1), LED(16, 2), LED(16,3), LED(16,4), LED(16,5), LED(16,6), LED(16,7), LED(16,8), LED(16,9),LED(16,10),LED(16,11),LED(16,12), LED(16,13), LED(16,14), LED(16,15),LED(16,16),LED(16,17),
 LED(17, 17),LED(17, 16),LED(17, 15),LED(17, 14),LED(17, 13),LED(17,12),LED(17,11),LED(17,10),LED(17,9),LED(17,8),LED(17,7),LED(17,6),LED(17,5),LED(17,4),LED(17,3),LED(17,2),LED(17,1),
 LED(18,1), LED(18, 2), LED(18,3), LED(18,4), LED(18,5), LED(18,6), LED(18,7), LED(18,8), LED(18,9),LED(18,10),LED(18,11),LED(18,12), LED(18,13), LED(18,14), LED(18,15),LED(18,16),LED(18,17),
 
 LED(19, 18), LED(19, 17),LED(19, 16),LED(19, 15),LED(19, 14),LED(19, 13),LED(19,12),LED(19,11),LED(19,10),LED(19,9),LED(19,8),LED(19,7),LED(19,6),LED(19,5),LED(19,4),LED(19,3),LED(19,2),LED(19,1),
 LED(20,1), LED(20, 2), LED(20,3), LED(20,4), LED(20,5), LED(20,6), LED(20,7), LED(20,8), LED(20,9),LED(20,10),LED(20,11),LED(20,12), LED(20,13), LED(20,14), LED(20,15),LED(20,16),LED(20,17),LED(20,18),
 LED(21, 18),LED(21, 17),LED(21, 16),LED(21, 15),LED(21, 14),LED(21, 13),LED(21,12),LED(21,11),LED(21,10),LED(21,9),LED(21,8),LED(21,7),LED(21,6),LED(21,5),LED(21,4),LED(21,3),LED(21,2),LED(21,1),
 
 LED(22,1), LED(22, 2), LED(22,3), LED(22,4), LED(22,5), LED(22,6), LED(22,7), LED(22,8), LED(22,9),LED(22,10),
 LED(23,9),LED(23,8),LED(23,7),LED(23,6),LED(23,5),LED(23,4),LED(23,3),LED(23,2),LED(23,1),
 LED(24,1), LED(24, 2), LED(24,3), LED(24,4), LED(24,5), LED(24,6), LED(24,7), LED(24,8), 
 
 //Front Left
 LED(25,8),LED(25,7),LED(25,6),LED(25,5),LED(25,4),LED(25,3),LED(25,2),LED(25,1),
 LED(26,1), LED(26, 2), LED(26,3), LED(26,4), LED(26,5), LED(26,6), LED(26,7), LED(26,8), LED(26,9), //9
 LED(27, 17),LED(27, 16),LED(27, 15),LED(27, 14),LED(27, 13),LED(27,12),LED(27,11),LED(27,10),LED(27,9),LED(27,8),LED(27,7),LED(27,6),LED(27,5),LED(27,4),LED(27,3),LED(27,2),LED(27,1),LED(27,0), //18
 LED(28,0),LED(28,1), LED(28, 2), LED(28,3), LED(28,4), LED(28,5), LED(28,6), LED(28,7), LED(28,8), LED(28,9), LED(28,10),LED(28,11),LED(28,12),LED(28,13),LED(28,14),LED(28,15),LED(28,16),LED(28,17), //18
 LED(29, 17),LED(29, 16),LED(29, 15),LED(29, 14),LED(29, 13),LED(29,12),LED(29,11),LED(29,10),LED(29,9),LED(29,8),LED(29,7),LED(29,6),LED(29,5),LED(29,4),LED(29,3),LED(29,2),LED(29,1),LED(29,0),//18
 LED(30,0),LED(30,1), LED(30, 2), LED(30,3), LED(30,4), LED(30,5), LED(30,6), LED(30,7), LED(30,8), LED(30,9), LED(30,10),LED(30,11),LED(30,12),LED(30,13),LED(30,14),LED(30,15), //16
 LED(31, 13),LED(31,12),LED(31,11),LED(31,10),LED(31,9),LED(31,8),LED(31,7),LED(31,6),LED(31,5),LED(31,4),LED(31,3),LED(31,2),LED(31,1),LED(31,0),//14
 LED(32,1), LED(32, 2), LED(32,3), LED(32,4), LED(32,5), LED(32,6), LED(32,7), LED(32,8), LED(32,9), LED(32,10) //9
  
};
 

void setup()
{
  Serial.begin(115200);
  strip.begin();
  
  SetIOPins();
  DiplayCardInfo();
  InitializeSD();
  
  if(digitalRead(2) == LOW)
  {
    DeletePreviousSDFile();
    GeneratePlasmaFile(generateFrames);
  }
  
}

void loop()
{
  if(digitalRead(2) == HIGH)
  {
    while(true)
    {
      PlayDataToVest(0);
    }
  }
}


void GeneratePlasmaFile(unsigned long frames)
{
  Serial.println("PARAMETERS:");
  Serial.print("brightness=");Serial.println(brightness);
  Serial.print("size=");Serial.println(size);
  Serial.print("movementFactor=");Serial.println(movementFactor);
  Serial.print("ledFactor=");Serial.println(ledFactor);
  Serial.print("generateFrames=");Serial.println(generateFrames);
  
  
  for(int i = 0; i<frames; i++)
  {
    myFile = SD.open("test.hex", FILE_WRITE);
    Plasma();
    Serial.println(i);
    
    digitalWrite(13, HIGH);
    delay(1);
    digitalWrite(13, LOW);
    
     myFile.close();
  }
  
  //ReadFile();
  digitalWrite(13, HIGH); //indicate the process has finished
}


void PlayDataToVest(int wait)
{
  Serial.println("Playing file.");
  myFile = SD.open("test.hex");
  
  while(myFile.available()) //untill there is no more data
  {
    for(int i = 0; i<LEDCOUNT; i++)//itterate each led
    {
      strip.setPixelColor(i,GetOneLedDataFromFile(myFile));
    }
    strip.show();
    FlashLed();
  }
  
  myFile.close();
}


void Plasma()
{

  for(int i = 0; i<LEDCOUNT; i++)
  {
    //get the xy co-ordinates and spread out by factor.
    LED c = leds[i];
    c.x*=ledFactor;
    c.y*=ledFactor;

    shade = (SinVerticle(c.x,c.y,size) //42ms
             + SinRotating(c.x,c.y,size)  //91ms
             + SinCircle(c.x,c.y, size))/3; //120ms
             //250ms
    
    //Optimization Mathematics
    sinShadePiRed = sin(shade*PI)*100; //21ms
    //sinShadePiGreen = sin(shade*PI+2)*100; //36ms
    sinShadePiBlue = sin(shade*PI+4)*100; //42ms
 
    r = map( sinShadePiRed, minShade, maxShade, 0, brightness);//2ms
    //g = map( sinShadePiGreen, minShade, maxShade, 0, brightness);//2ms
    b = map( sinShadePiBlue, minShade, maxShade, 0, brightness);//2ms
     
    //r = map( sin(shade*PI)*100, minShade, maxShade, 0, brightness);
    //g = map( sin(shade*PI+2*PI*sin(movement/23))*100, minShade, maxShade, 0, brightness);
    //b = map( sin(shade*PI+4*PI*sin(movement/20))*100, minShade, maxShade, 0, brightness);
    
    strip.setPixelColor(i,Color(r,g,b));//0.5ms
     
    WriteLedDataToSD(r,g,b); 
    
    }
  
    movement+=movementFactor;
    strip.show();
    

    //self correct mapping values to use maximum led resolution
    //if(sinShadePiRed < minShade) minShade = sinShadePiRed;
    //if(sinShadePiRed > maxShade) maxShade = sinShadePiRed;
  
    //Serial.println(minShade);
    //show the last frame/pixel
    /*
    Serial.print("Shade="); Serial.println(shade);
    Serial.print("r="); Serial.print(r); Serial.print("From="); Serial.println(sinShadePiRed);
    Serial.print("g="); Serial.print(g); Serial.print("From="); Serial.println(sinShadePiGreen);
    Serial.print("b="); Serial.print(b); Serial.print("From="); Serial.println(sinShadePiBlue);
    Serial.println();
    */
    //Serial.println(millis());
}

float SinVerticle(float x, float y, float s)
{
  return sin(x / s + movement);
}
 
float SinRotating(float x, float y, float s)
{
  return sin(  (x * sin(movement/33 ) + y * cos(movement/22) )  /size) ;
}
 
float SinCircle(float x, float y, float s)
{
  cx = worldWidth * sin(movement/10)*ledFactor;
  cy = worldHeight * cos(movement/10)*ledFactor;
  dist = sqrt(sq(cy-y) + sq(cx-x));
  return sin((dist/s ) + movement );
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}


void InitializeSD()
{
  Serial.print("Initializing SD card...");
  
  pinMode(53, OUTPUT);
   
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("SD Initialization Failed!");
    return;
  }
  
  Serial.println("SD Initialization OK.");
}

void DeletePreviousSDFile()
{
  //delete the current file
  if(SD.exists("test.hex"))
  {
    SD.remove("test.hex");
    Serial.println("Old File Deleted");
  }
}

void WriteLedDataToSD(uint8_t r,uint8_t g,uint8_t b)
{
  myFile.write(r);
  myFile.write(g);
  myFile.write(b);
}


uint32_t GetOneLedDataFromFile(File file)//gets 24 bits, return as 32
{
  uint32_t result;
  result = file.read();//msb
  result<<=8;
  result |= file.read();
  result<<=8;
  result |= file.read();//lsb
  return result;
}


void ReadFile()
{
  myFile = SD.open("test.hex");
  
  while(myFile.available()) //untill there is no more data
  {
    for(int i = 0; i<LEDCOUNT; i++)//itterate each led
    {
      Serial.println(GetOneLedDataFromFile(myFile));
    }
  }
  
  myFile.close();
}


void SetIOPins()
{  
  //to talk to the SD card, 10 on UNO, 53 on mega
  pinMode(53, OUTPUT);
  
  //protection, must earth pin 2 to start process
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);//pullup
  
  //led light indicator
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}






void DiplayCardInfo()
{

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }

  
  
 // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}


void FlashLed()
{
      digitalWrite(13, HIGH);
      delay(1);
      digitalWrite(13, LOW);
}

