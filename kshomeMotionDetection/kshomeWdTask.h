#ifndef __KHSOME_WD_TASK
#define __KHSOME_WD_TASK


#include "userDef.h"

#define MAX_WD_APP_SUPPORTED  10
#define MAX_NAME_SIZE 32
#define DEFAULT_HOLD_TIME_MS 100

struct wdParam{
  char name[MAX_NAME_SIZE];
  WS_UINT32 wdAppId;
  WS_UINT32 timeout_s;
  WS_UINT32 remaining_s;
  bool isWDStarted;
};

class kshomeWDApp{
  private:
    wdParam wdList[MAX_WD_APP_SUPPORTED]; 
    WS_UINT32 wdHoldupTime_s = 1; // time to wait after wd trigger for reboot.
 
    void initWdApp(){     
      ws_memset(wdList, 0, (sizeof(wdParam)*MAX_WD_APP_SUPPORTED) );
    }
    void deviceReboot();

  public:  
    kshomeWDApp(){ 
      initWdApp();         
    }
    
    kshomeWDApp(WS_UINT32 holdTime){ 
      initWdApp();
      wdHoldupTime_s = holdTime;     
    }

  bool resetWatchDog(WS_UINT32 wdId);
  WS_SINT32 initWdApp(WS_UINT32 timeOutSec, char * name);
  void  kshomeWdLooper() ;
  bool startWdApp(WS_UINT32 wdId);
  bool stopWdApp(WS_UINT32 wdId);
};


#endif //kshomeWdTask.h