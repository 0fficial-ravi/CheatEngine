#include <Keyboard.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define Ent 0xB0
#define KEY_F12 0xCD
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

int okBtn = 6;  
int upBtn = 7;
int dwnBtn = 4;
int backBtn = 5;

int currPointer=0;
int maxPageItems=0;
String currentPage="main";
String prevPage="";
String masterPageList[]={"main","main/start/","main/config/","main/about/","main/start/cheatmode","/main/config/cheatconfig"};
String pageMain[]={"Start","Config","About"};
String pageStart[]={"Gta V","Max Payne","Vice City","San Andreas"};

String gameCheats[][5]={
  {"Gta V","LAWYERUP","TURTLE","TOOLUP","HIGHEX"},
  {"Max Payne","GetAllWeapons","GetInfiniteAmmo","GetHealth","GetMolotov"},
  {"Vice City","LEAVEMEALONE","NUTTERTOOLS","PROFESSIONALTOOLS","BIGBANG"},
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

String about[]={"Device Made By","    Raven","Date:","2024-7"};  //PUT ABOUT LATER

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
void displayCheatsConfig(String,String[][sizeof(gameCheats[0])/sizeof(gameCheats[0][0])]);
String findCheat(String,String);




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
      for (int j=0; j<maxPageItems; j++) 
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
  for (int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++) 
  {
    if(gameCheats[i][0].compareTo(gameName)==0)
    {
      for (int j=0; j<maxPageItems; j++) 
      {
      if(gameCheatsButtons[i][j].compareTo(button)==0)
        return gameCheats[i][j];
      }
    }
  }
  return "";
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
  else {
  //nothing
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
                                          lcd.print(cheat[i][currPointer-1]);
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
}


void moveCursor()
{
  if (digitalRead(upBtn) == HIGH ){
  if(currPointer>0){
  currPointer--;
  setPage();
  if(currentPage.compareTo(masterPageList[3])!=0)
  placeArrow();
  delay(300);
  }
  }
  if (digitalRead(dwnBtn) == HIGH){
  if(currPointer<maxPageItems-1){
  currPointer++;
  setPage();
  if(currentPage.compareTo(masterPageList[3])!=0)
  placeArrow();
  delay(300); 
  }
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
      buttonsInitialise();
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
         delay(300);
         break; 
        }
        else if (digitalRead(okBtn)==HIGH)
        {
         toFireCheat=findCheat(gameName,"1");
         if(toFireCheat.compareTo("")!=0)
         {
          launchCheat(toFireCheat);
         }
         else
         noCheatError();
         delay(300);
        }
         else if (digitalRead(upBtn)==HIGH)
        {
         toFireCheat=findCheat(gameName,"2");
         if(toFireCheat.compareTo("")!=0)
         {
          launchCheat(toFireCheat);
         }
         else
         noCheatError();
         delay(100);
        }
         else if (digitalRead(dwnBtn)==HIGH)
        {
         toFireCheat=findCheat(gameName,"3");
         if(toFireCheat.compareTo("")!=0)
         {
         launchCheat(toFireCheat);
         }
         else
         noCheatError();
         delay(100);
        }
         else if (digitalRead(backBtn)==HIGH)
        {
         toFireCheat=findCheat(gameName,"4");
         if(toFireCheat.compareTo("")!=0)
         {
         launchCheat(toFireCheat);
         }
         else
         noCheatError();
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
      buttonsInitialise();
      setPrevPage();

    }
    delay(300);
  }
  else if(digitalRead(backBtn)==HIGH)
  {
    setPrevPage();
    currentPage=prevPage;
    resetPointer();
    setPage();
    placeArrow();
    delay(300);
  }
}

void serialDisp()
{
  Serial.print("Current Pointer=");
  Serial.print(currPointer);
  Serial.print("\tMax Items=");
  Serial.print(maxPageItems);
  Serial.print("\tCurrent Page=");
  Serial.print(currentPage);
  Serial.print("\tPrev- Page=");
  Serial.println(currentPage);
}

void dispCheats()
{
  Serial.print("Max Page Items=\t");
  Serial.println(maxPageItems);

  Serial.println("---------Game cheats----------");
     for (int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++) 
      {
      for (int j=0; j<maxPageItems; j++) 
      {
        Serial.print(gameCheats[i][j]);
        Serial.print(" ");
      }
      Serial.println();
  }
  Serial.println("---------Game Buttons----------");
      for (int i=0;i<sizeof(gameCheats)/sizeof(gameCheats[0]);i++) 
      {
      for (int j=0;j<maxPageItems;j++) 
      {
        Serial.print(gameCheatsButtons[i][j]);
        Serial.print(" ");
      }
      Serial.println();
  }
}



void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0,arrow);
  lcd.createChar(1,cleanArrow);
  setPage();
  placeArrow();
  Serial.begin(9600);
}

void loop(){
  serialDisp();
  moveCursor();
  selectCursor();
}
