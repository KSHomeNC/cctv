#include <WiFi.h>
#include "cloudService.h"


#define serverAdd "192.168.1.56"

WiFiClient cloudClient;
unsigned int serverPort = 1234;
#define bLen 1500//128*1024
char buff[bLen];
int sendFileToCloud(const char* filename, AmebaFatFS *fs)
{
  char path[128]; 
  int retVal = -1;
  
  printf("sending File \"%s\"  to cloud \r\n", filename);
  sprintf(path, "%s%s", fs->getRootPath(), filename);
  File file = fs->open(path);
  Serial.println(file);
  if (file){
    if (cloudClient.connect(serverAdd,serverPort))
    {
      if(cloudClient.write(filename)){
        delay(100);
        long fSize = file.size();
        long len = fSize;
        Serial.print("File size: ");
        Serial.println(fSize);
        if(len ==  0){// just remove the empty file
          fs->remove(filename);     
        }
        while(len/(bLen)){
          Serial.print("Bytes read from File");
          Serial.println(file.read(buff, bLen));        
          //file.read(buff, bLen);
          len-= bLen;
          if(!cloudClient.write(buff, bLen)){      
            Serial.println("File Write to cloud failed");
            break;
          }                   
          Serial.print("Remaining File size: ");
          Serial.println(len);  
        }
        if(len > 0 && len < bLen){      
          Serial.print("Last pack of Bytes read from File:  ");
          int l = file.read(buff, len);          
          Serial.println(l);     
          if(!cloudClient.write(buff, l)){      
             Serial.println("File Write to cloud failed");
            }
            else{                 
              retVal = 0;              
            }
          }
      }//file name
      cloudClient.stop();
    }//connect
    file.close();
    if(retVal==0){
      fs->remove(filename); 
    }
  }
  return retVal;
}
