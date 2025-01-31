#include <Keyboard.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define Ent 0xB0
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte arrow[] = {
  B00000,
  B00100,
  B00110,
  B11111,
  B00110,
  B00100,
  B00000,
  B00000
};
byte cleanArrow[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte buttonArrows[] = {
  B00100,
  B01010,
  B11111,
  B00000,
  B00000,
  B11111,
  B01010,
  B00100
};
byte buttonArrowsUp[] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B11111,
  B01010,
  B00100
};

byte buttonArrowsDwn[] = {
  B00100,
  B01010,
  B11111,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100
};
int okBtn = 6;  
int upBtn = 7;
int dwnBtn = 4;
int backBtn = 5;

int currPointer=0;
int maxPageItems=0;
int buttonValue=0;
 
String currentPage="main";
String prevPage="";
String masterPageList[]={"main","main/start/","main/config/","main/about/","main/start/cheatmode","/main/config/cheatconfig","/main/config/cheatconfig/buttonConfig"};
String pageMain[]={"Start","Config","About"};
String pageStart[]={"Gta V","Max Payne","Vice City","San Andreas"};

String gameCheats[][5]={
  {"Gta V","LAWYERUP","TURTLE","TOOLUP","HIGHEX"},
  {"Max Payne","GetAllWeapons","GetInfiniteAmmo","GetHealth","GetMolotov"},
  {"Vice City","LEAVEMEALONE","NUTTERTOOLS","THUGSTOOLS","BIGBANG"},
  {"San Andreas","TURNDOWNTHEHEAT","HESOYAM","LXGIWYL","CPKTNWT"}
};
String gameAttributes[][3]={
  {"Gta V","~","no"},
  {"Max Payne","0xCD","yes"},
  {"Vice City"," ","no"},
  {"San Andreas"," ","no"}
};

char consoleKey=' ';
String consoleToggle="no";
String gameNameForConfig="";
String gameCheatsButtons[sizeof(gameCheats)/sizeof(gameCheats[0])][sizeof(gameCheats[0])/sizeof(gameCheats[0][0])];

String about[]={"Device Made By","Ravi","Date:","2024-7"};

void moveCursor();
void selectCursor();
void setMaxPageItems();
void setPage();
void displayPage(String []);
void placeArrow();
void clearArrowPlace();
void resetPointer();
void cheatStartNotification(String);
void cheatStopNotification(String);
void setPrevPage();
void buttonsInitialise();
void dispCheats();
void noCheatError();
void setConsoleKey();
void launchCheat(String);
int dispButton(String);
void displayCheatsConfig(String,String[][sizeof(gameCheats[0])/sizeof(gameCheats[0][0])]);
String findCheat(String,String);
void displayButton();
void resetGameName();
void setButton(String,String);

void welcome()
{
delay(1000);
lcd.setCursor(3,0);
lcd.print("Welcome to");
delay(1000);
lcd.setCursor(0,1);
lcd.print("CheatEngine v1.0");
delay(2000);
lcd.clear();
}
void resetGameName()
{
gameNameForConfig="";
}
void setButton(String gameName,String cheatName)
{
    for (int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++) 
     {
      if(gameCheats[i][0].compareTo(gameName)==0)
        {
        for (int j=1; j<sizeof(gameCheats[0])/sizeof(gameCheats[0][0]); j++) 
          {
          if(gameCheats[i][j].compareTo(cheatName)==0)
            gameCheatsButtons[i][j]=String(buttonValue);
          }
        }
      }
}
void displayButton()
{
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print(buttonValue);
  lcd.setCursor(7,0);
  lcd.write(2);
}
void resetPointer()
{
  currPointer=0;
}
void placeArrow()
{
  clearArrowPlace();
  lcd.setCursor(0,currPointer%2);
  lcd.write(0);
}
void clearArrowPlace()
{
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(0,1);
  lcd.write(1);
}
void buttonsInitialise()
{
   for (int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++) 
  {
      for (int j=0; j<sizeof(gameCheats[0])/sizeof(gameCheats[0][0]); j++) 
      {
        if(j==0)                                        //game name->[i][0]
          gameCheatsButtons[i][j]=gameCheats[i][j];
        else if (j <= 4 && j > 0)                       //first four cheats=1,2,3,4
          gameCheatsButtons[i][j]=String(j);
        else
          gameCheatsButtons[i][j] = String(0);          //rest cheats =0
      }
  }
}

void cheatStartNotification(String gameName)
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(gameName);
  lcd.setCursor(1, 1);
  lcd.print("Mode Activated");
  delay(1000);
  lcd.clear();
}

void cheatStopNotification(String gameName)
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(gameName);
  lcd.setCursor(0, 1);
  lcd.print("Mode De-Activated");
  delay(1000);
  lcd.clear();
}

void noCheatError()
{
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("No Cheat");
  lcd.setCursor(2,1);
  lcd.print("Assigned ):");
  delay(500);
  lcd.clear();
}

String findCheat(String gameName,String button)
{
  if(button.compareTo("-1")!=0)
    {
    for (int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++) 
     {
      if(gameCheats[i][0].compareTo(gameName)==0)
        {
        for (int j=0; j<sizeof(gameCheats[0])/sizeof(gameCheats[0][0]); j++) 
          {
          if(gameCheatsButtons[i][j].compareTo(button)==0)
          return gameCheats[i][j];
          }
        }
      }
  return "";
  }
  else if(button.compareTo("-1")==0)
  {
     for (int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++) 
      {
        if(gameCheats[i][0].compareTo(gameName)==0)
          {
            return gameCheats[i][currPointer+1];
          }
        }
      return "";
  }
}
int dispButton(String cheatName)
{
  for(int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++)
  {
   if(gameCheats[i][0].compareTo(gameNameForConfig)==0)
   {
    for(int j=1;j<=sizeof(gameCheats[0])/sizeof(gameCheats[0][0]);j++)
    {
      if(gameCheats[i][j].compareTo(cheatName)==0)
      return gameCheatsButtons[i][j].toInt();
    }
   }
  }
  return 0;
}

void launchCheat(String cheatName)
{
            lcd.setCursor(0,0);
            lcd.print(cheatName);
            lcd.setCursor(2,1);
            lcd.print("Activated");
            if(consoleKey!=' '){
            Keyboard.write(consoleKey);
            delay(100);
            }
            Keyboard.print(cheatName);
            delay(100);
            Keyboard.press(Ent);
            delay(100);
            Keyboard.release(Ent);
            delay(100);
            if (consoleToggle.compareTo("yes") == 0)
            {
              Keyboard.write(consoleKey);
              delay(100);
            }
            lcd.clear();
}

void setConsoleKey(String gameName)
{
  for(int i=0;i<(sizeof(gameAttributes)/sizeof(gameAttributes[0]));i++)
    {
      if(gameAttributes[i][0].compareTo(gameName)==0)
      {
            if(gameAttributes[i][1].compareTo("~")==0)
              consoleKey='~';
            else if (gameAttributes[i][1].compareTo("0xCD")==0)
              consoleKey=0xCD;
            else if(gameAttributes[i][1].compareTo(" ")==0)
             consoleKey=' ';
            consoleToggle=gameAttributes[i][2];
      } 
    }
}

void setMaxPageItems()
{
  if(currentPage.compareTo(masterPageList[0])==0)//main
  {
    maxPageItems=sizeof(pageMain)/sizeof(pageMain[0]);
  }
  else if (currentPage.compareTo(masterPageList[1])==0) {  //main/start/
    maxPageItems=sizeof(pageStart)/sizeof(pageStart[0]);
  } 
  else if (currentPage.compareTo(masterPageList[2])==0) { //main/start/
    maxPageItems=sizeof(pageStart)/sizeof(pageStart[0]);
  }
   else if (currentPage.compareTo(masterPageList[3])==0) { //main/about/
    maxPageItems=sizeof(about)/sizeof(about[0]);
  }
  else if (currentPage.compareTo(masterPageList[4])==0) { //main/start/cheatmode/
    maxPageItems=sizeof(gameCheats[0])/sizeof(gameCheats[0][0]);
  }
  else if (currentPage.compareTo(masterPageList[5])==0) { //main/config/cheatconfig/
    maxPageItems=(sizeof(gameCheats[0])/sizeof(gameCheats[0][0]))-1;
  }
}

void setPage()
{
  if(currentPage.compareTo(masterPageList[0])==0)//main
  {
  setMaxPageItems();
  displayPage(pageMain);
  }
  else if(currentPage.compareTo(masterPageList[1])==0)//main/start/
  {
    setMaxPageItems();
    displayPage(pageStart);
  }
  else if(currentPage.compareTo(masterPageList[2])==0)//main/config/
  {
    setMaxPageItems();
    displayPage(pageStart);
  }
   else if(currentPage.compareTo(masterPageList[3])==0)//main/about/
  {
    setMaxPageItems();
    displayPage(about);
  }
   else if(currentPage.compareTo(masterPageList[4])==0)//main/start/cheatmode
  {
    setMaxPageItems();
  }
   else if(currentPage.compareTo(masterPageList[5])==0)//main/config/cheatconfig
  {
    setMaxPageItems();
    displayCheatsConfig(gameNameForConfig,gameCheats);
  }
   else if(currentPage.compareTo(masterPageList[6])==0)//main/config/cheatconfig/buttonConfig
  {
    displayButton();
  }
}
void displayCheatsConfig(String gameName,String cheat[][sizeof(gameCheats[0])/sizeof(gameCheats[0][0])])
{
for(int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++)
  {
    if(gameCheats[i][0].compareTo(gameName)==0)
      {
                                lcd.clear();
                                if(maxPageItems%2!=0)
                                  {
                                          if((currPointer+1)==maxPageItems)
                                          {
                                          lcd.setCursor(1,0);
                                          lcd.print(cheat[i][currPointer+1]);
                                          }
                                          else if((currPointer+1)%2!=0){
                                          lcd.setCursor(1,0);
                                          lcd.print(cheat[i][currPointer+1]);
                                          lcd.setCursor(1,1);
                                          lcd.print(cheat[i][currPointer+2]);
                                          }
                                          else if((currPointer+1)%2==0)
                                          {
                                          lcd.setCursor(1,0);
                                          lcd.print(cheat[i][currPointer]);
                                          lcd.setCursor(1,1);
                                          lcd.print(cheat[i][currPointer+1]);
                                          }
                                  }
                                else if(maxPageItems%2==0)
                                  {
                                  if((currPointer+1)%2!=0)
                                  {
                                    lcd.setCursor(1,0);
                                    lcd.print(cheat[i][currPointer+1]);
                                    lcd.setCursor(1,1);
                                    lcd.print(cheat[i][currPointer+2]);
                                  }
                                  else if((currPointer+1)%2==0)
                                    {
                                    lcd.setCursor(1,0);
                                    lcd.print(cheat[i][currPointer]);
                                    lcd.setCursor(1,1);
                                    lcd.print(cheat[i][currPointer+1]);
                                    }
                                 }
                                 break;
      }
  }
}

void displayPage(String pagearray[])
{
  lcd.clear();
  if(maxPageItems%2!=0)
  {
    if((currPointer+1)==maxPageItems)
    {
      lcd.setCursor(1,0);
      lcd.print(pagearray[currPointer]);
    }
    else if(currPointer%2==0){
      lcd.setCursor(1,0);
      lcd.print(pagearray[currPointer]);
      lcd.setCursor(1,1);
      lcd.print(pagearray[currPointer+1]);
    }
    else if(currPointer%2!=0)
    {
      lcd.setCursor(1,0);
      lcd.print(pagearray[currPointer-1]);
      lcd.setCursor(1,1);
      lcd.print(pagearray[currPointer]);
    }
  }
  else if(maxPageItems%2==0)
  {
    if(currPointer%2==0){
      lcd.setCursor(1,0);
      lcd.print(pagearray[currPointer]);
      lcd.setCursor(1,1);
      lcd.print(pagearray[currPointer+1]);
  }
   else if(currPointer%2!=0)
    {
      lcd.setCursor(1,0);
      lcd.print(pagearray[currPointer-1]);
      lcd.setCursor(1,1);
      lcd.print(pagearray[currPointer]);
    }
  }
}

void setPrevPage()
{
 if(currentPage.compareTo(masterPageList[1])==0||currentPage.compareTo(masterPageList[2])==0||currentPage.compareTo(masterPageList[3])==0)
    prevPage=masterPageList[0];
  else if (currentPage.compareTo(masterPageList[4])==0)
    prevPage=masterPageList[1];
  else if (currentPage.compareTo(masterPageList[5])==0)
    prevPage=masterPageList[2];
  else if (currentPage.compareTo(masterPageList[6])==0)
    prevPage=masterPageList[5];
}


void moveCursor()
{
  if (digitalRead(upBtn) == HIGH )
  {
    if(currPointer>0 && currentPage.compareTo(masterPageList[6])!=0)
      {
      currPointer--;
      setPage();
      if(currentPage.compareTo(masterPageList[3])!=0 && currentPage.compareTo(masterPageList[6])!=0)
      placeArrow();
      }
      if(currentPage.compareTo(masterPageList[6])==0)
      {
            if(buttonValue>1)
            {
              buttonValue--;
              setPage();
              lcd.setCursor(7,0);
              lcd.write(3);
            }
      }
      delay(300);
  }
  if (digitalRead(dwnBtn) == HIGH)
    {
    if(currPointer<maxPageItems-1 && currentPage.compareTo(masterPageList[6])!=0)
      {
      currPointer++;
      setPage();
      if(currentPage.compareTo(masterPageList[3])!=0 && currentPage.compareTo(masterPageList[6])!=0)
      placeArrow();
      }
      if(currentPage.compareTo(masterPageList[6])==0)
            {
            if(buttonValue<4){
            buttonValue++;
            setPage();
            lcd.setCursor(7,0);
            lcd.write(4);
            }
      }
      delay(300); 
    }
}
void selectCursor()
{
  if(digitalRead(okBtn)==HIGH)
  {
    if(currentPage.compareTo(masterPageList[0])==0)//main
    {                                                                         //page change
      switch(currPointer)
      {
        case 0: resetPointer();
                currentPage=masterPageList[1]; //main/start/
                setPage();                                                            //case 0-start
                placeArrow();                                                         //case 1-config
                break;                                                                //case 2-about                                                                                   
        case 1: resetPointer();
                currentPage=masterPageList[2];  //main/config/
                setPage();
                placeArrow();
                break;
        case 2: resetPointer();    
                currentPage=masterPageList[3]; //main/about/
                setPage();
                break;
      }
    }
    else if (currentPage.compareTo(masterPageList[1])==0)//main/start
    {
      String toFireCheat="";
      String gameName=pageStart[currPointer];
      currentPage=masterPageList[4];  //main/start/cheatmode/
      clearArrowPlace();
      setPage();
      setPrevPage();
      setConsoleKey(gameName);
      cheatStartNotification(gameName);
      while(1) 
      {
        if(digitalRead(okBtn)==HIGH && digitalRead(backBtn)==HIGH)
        {
         cheatStopNotification(gameName);
         currentPage=prevPage;
         setPage();
         placeArrow();
         delay(100);
         break; 
        }
        else if (digitalRead(okBtn)==HIGH)
        {
         toFireCheat=findCheat(gameName,"1");
         if(toFireCheat.compareTo("")!=0)
         {
          launchCheat(toFireCheat);
         }
         else if(toFireCheat.compareTo("")==0){
         noCheatError();
         }
         delay(100);
        }
         else if (digitalRead(upBtn)==HIGH)
        {
         toFireCheat=findCheat(gameName,"2");
         if(toFireCheat.compareTo("")!=0)
         {
          launchCheat(toFireCheat);
         }
        else if(toFireCheat.compareTo("")==0){
         noCheatError();
         }
         delay(100);
        }
         else if (digitalRead(dwnBtn)==HIGH)
        {
         toFireCheat=findCheat(gameName,"3");
         if(toFireCheat.compareTo("")!=0)
         {
         launchCheat(toFireCheat);
         }
         else if(toFireCheat.compareTo("")==0){
         noCheatError();
         }
         delay(100);
        }
         else if (digitalRead(backBtn)==HIGH)
        {
         toFireCheat=findCheat(gameName,"4");
         if(toFireCheat.compareTo("")!=0)
         {
         launchCheat(toFireCheat);
         }
        else if(toFireCheat.compareTo("")==0){
         noCheatError();
         }
         delay(100);
        }
      }
    }
    else if (currentPage.compareTo(masterPageList[2])==0)//main/config
    {
      gameNameForConfig=pageStart[currPointer];
      currentPage=masterPageList[5];  //main/start/cheatmode/
      resetPointer();
      clearArrowPlace();
      setPage();
      placeArrow();
      setPrevPage();
    }
     else if (currentPage.compareTo(masterPageList[5])==0)//main/config/cheatconfig
    {
      clearArrowPlace();
      currentPage=masterPageList[6];
      String cheatValue=findCheat(gameNameForConfig,"-1");
      buttonValue=dispButton(cheatValue);
      setPrevPage();  
      setPage();
      while(1) 
      {
        moveCursor();    
        if(digitalRead(backBtn)==HIGH)
        {
          currentPage=prevPage;
          setPage();
          placeArrow();
          delay(300);
          break;
        }
        else if(digitalRead(okBtn)==HIGH)
        {
          setButton(gameNameForConfig,cheatValue);
          lcd.setCursor(7,0);
          lcd.write(2);
          delay(300);
        }       
        }
  }
  delay(300);
  }
  else if(digitalRead(backBtn)==HIGH)
  {
    if(currentPage.compareTo(masterPageList[6])!=0)
      resetGameName();
    setPrevPage();
    currentPage=prevPage;
    resetPointer();
    setPage();
    placeArrow();
    delay(300);
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0,arrow);
  lcd.createChar(1,cleanArrow);
  lcd.createChar(2,buttonArrows);
  lcd.createChar(3,buttonArrowsUp);
  lcd.createChar(4,buttonArrowsDwn);
  welcome();
  buttonsInitialise();
  setPage();
  placeArrow();
}

void loop(){
  moveCursor();
  selectCursor();
}
