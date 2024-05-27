#include <WiFiUdp.h>
#include <arduino.h>
#include "consoleNet.h"
#include "commandHandler.h"
char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back
int localPort =1234;
WiFiUDP Udp;



//void deviceReboot();

void closeNetInit()
{
  Udp.begin(localPort);  
}
void clonsolNetHandler(void)
{
  int packetSize = Udp.parsePacket();
  if (packetSize) {
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remoteIp = Udp.remoteIP();
      Serial.print(remoteIp);
      Serial.print(", port ");
      Serial.println(Udp.remotePort());

      // read the packet into packetBufffer
      int len = Udp.read(packetBuffer, 255);
      if (len > 0) {
          packetBuffer[len] = 0;
          if(String(packetBuffer) == "RSET")
          {
            //deviceReboot();
          }
      }
      
      Serial.println("Contents:");
      Serial.println(packetBuffer);    
  }
}


