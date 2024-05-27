#include "wl_definitions.h"
#include "kshomeWdTask.h"
#include "Arduino.h"

/*******************************************************/
#include "WDT.h"
#define AON_WDT_Enable                      (0)
#if AON_WDT_Enable == 0
    #define RUN_CALLBACK_IF_WATCHDOG_BARKS  (0)
#endif

WDT wd(AON_WDT_Enable);
/******************************************************/

void kshomeWDApp:: deviceReboot()
{
  wd.init(wdHoldupTime_s*1000);// in ms
  Serial.println("Watch dog called");
  wd.start();
}

WS_SINT32 kshomeWDApp::initWdApp( WS_UINT32 timeOutSec, char * name)
{
  WS_SINT32 idAvailable = -1; 
  
  for (int i = 0; i< MAX_WD_APP_SUPPORTED; i++){
    if ( wdList[i].wdAppId == 0 ) {
      wdList[i].remaining_s = timeOutSec;
      wdList[i].timeout_s = timeOutSec;
      if(name != NULL){
        strncpy(&wdList[i].name[0], name, MAX_NAME_SIZE);
      }
      wdList[i].wdAppId = i+1;
      idAvailable = wdList[i].wdAppId;
      break; 
    }              
  }   
  return idAvailable;
}

bool kshomeWDApp::resetWatchDog(WS_UINT32 wdId)
{
  bool isReseted = false;

  for (int i = 0; i< MAX_WD_APP_SUPPORTED; i++){
    if ( wdList[i].wdAppId == wdId ) {
      isReseted = true;
      wdList[i].remaining_s = wdList[i].timeout_s;
      break;
    }
  }    
  return isReseted;
  Serial.println("Watch dog called ");
}

void kshomeWDApp:: kshomeWdLooper() // every Second call
{
  //Serial.println("WD looper called");
  for (int i = 0; i< MAX_WD_APP_SUPPORTED; i++){
      if ( wdList[i].wdAppId != 0 ) {
       // Serial.println("WD app Created");
        if(wdList[i].isWDStarted == 1) {
          Serial.println("WD app Started");
          if (wdList[i].remaining_s - 1 <= 0){
            // App Crashed ! trigger the HW WD to reboot the device
            Serial.println("device rebooting");
            deviceReboot();
            break;
          }
          else{
            wdList[i].remaining_s = wdList[i].remaining_s- 1;
            Serial.print("WD remainig value :  ");
            Serial.println(wdList[i].remaining_s);
            Serial.print("WD timeout value :  ");
            Serial.println(wdList[i].timeout_s);
            Serial.print("WD ID  :  ");
            Serial.println(wdList[i].wdAppId);
          }
        }
      }
    } 
}

bool kshomeWDApp::startWdApp(WS_UINT32 wdId)
{
  bool isStarted = false;
  
  for (int i = 0; i< MAX_WD_APP_SUPPORTED; i++){
    if ( wdList[i].wdAppId == wdId ) {
      wdList[i].remaining_s = wdList[i].timeout_s;       
      isStarted = true;
      wdList[i].isWDStarted = 1;
      break;
    }
  }   
  return isStarted;  
}

bool kshomeWDApp::stopWdApp(WS_UINT32 wdId)
{
  bool isStopped = false;
  
  for (int i = 0; i< MAX_WD_APP_SUPPORTED; i++){
    if ( wdList[i].wdAppId == wdId ) {
      isStopped = true;
      wdList[i].isWDStarted = 0;
      break;
    }
  }   
  return isStopped; 
}