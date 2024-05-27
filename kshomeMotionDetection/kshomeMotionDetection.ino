/*

 Example guide:
 https://www.amebaiot.com/en/amebapro2-amb82-mini-arduino-video-motion-mp4/

 For recommended setting to achieve better video quality, please refer to our Ameba FAQ: https://forum.amebaiot.com/t/welcome-to-ameba-faq/1748

 This example acts as a Security System based on Motion Detection, which would start to record a 
 30 seconds long MP4 video everytime motion is detected. (Alarm function could be initiated as well, but on default disabled)
 */
//#include "WDT.h"
#include "AmebaFatFS.h"
#include "setDateNTime.h"
#include "cloudService.h"
#include "KHomeTask.h"
#include "commandHandler.h"
#include "kshomeMotionDetection.h"
#include "kshomeWdTask.h"
//#include "kshomeConfig.h"
//#include "cctvApp.h"


#include "StreamIO.h"
#include "VideoStream.h"
#include "AudioStream.h"
#include "AudioEncoder.h"
#include "MP4Recording.h"
#include "RTSP.h"
#include "MotionDetection.h"
#include "VideoStreamOverlay.h"
#include "consoleNet.h"

#define CHANNELVID 0  // Channel for RTSP streaming & MP4 recording
#define CHANNELMD  3  // RGB format video for motion detection only avaliable on channel 3



ws_TCB wsTaskDec[]={
  //taskId,     taskHandler,    tick,         lifetime, isRunnable, isStarted
    {0,     osdTask,          1000/TIME_TICK,   0,            0,         0},  // per Sec
    {1,     mdTask,           500/TIME_TICK,    0,            0,         0},  //Per 500mSec  
    {2,     commandTask,      1000/TIME_TICK,   0,            0,         0}, // per Sec
    {3,     clonsolNetHandler,1000/TIME_TICK,   0,            0,         0}, // per Sec
    {4,     wdTask,           3000/TIME_TICK,   0,            0,         0}, // per Sec
};
#define MAX_TASK  (sizeof(wsTaskDec)/sizeof(ws_TCB))


AmebaFatFS fs;
kshomeDnT datenTime;

char ssid[] = "subhSpec"; // your network SSID (name)
char pass[] = "pawan@158";     // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;

bool motionDetected = false;
int motionCount = 0;
int silenceTimeSec = 60;
int timelapseSec = silenceTimeSec;

AudioSetting configA(0);
VideoSetting configV(VIDEO_FHD, CAM_FPS, VIDEO_H264, 0);
VideoSetting configMD(VIDEO_VGA, 10, VIDEO_RGB, 0);     // Low resolution RGB video for motion detection
Audio audio;
AAC aac;
RTSP rtsp;
MotionDetection MD;
StreamIO videoStreamerMD(1, 1); // 1 Input RGB Video -> 1 Output MD
StreamIO audioStreamer(1, 1);   // 1 Input Audio -> 1 Output AAC
StreamIO avMixStreamer(2, 1);   // 2 Input Video + Audio -> 2 Output MP4 + RTSP


kshomeWDApp wdApp(10);

void getConf()
{
  #if 0
  App_Conf conf;

  readConfig(&conf);
  Serial.println(conf.ssid);
  Serial.println(conf.pass);
  Serial.println(conf.cloudAdd);
  Serial.println(conf.cloudPort);
  Serial.println(conf.motionCount);
  Serial.println(conf.silenceTimeSec);
  Serial.println(conf.burzer);

  strlcpy(conf.ssid, "testWifi",sizeof("testWifi"));  
  writeConfig(&conf); 
  #endif
}

void cctvInit(){
    // Configure camera video channel with video format information
    Camera.configVideoChannel(CHANNELVID, configV);
    Camera.configVideoChannel(CHANNELMD, configMD);
    Camera.videoInit();

    // Configure audio peripheral for audio data output
    audio.configAudio(configA);
    audio.begin();
    // Configure AAC audio encoder
    aac.configAudio(configA);
    aac.begin();

    // Configure RTSP with identical video format information and enable audio streaming
    rtsp.configVideo(configV);
    rtsp.configAudio(configA, CODEC_AAC);
    rtsp.begin();

    // Configure motion detection for low resolution RGB video stream
    MD.configVideo(configMD);
    MD.begin();
    MD.setDetectionMask(mask);

    // Configure MP4 with identical video format information
    // Configure MP4 recording settings
    //mp4.configVideo(configV);
    //mp4.configAudio(configA, CODEC_AAC);
   // mp4.setRecordingDuration(60*60); // 1 hr = 60*60
   // mp4.setRecordingFileCount(1);

    // Configure StreamIO object to stream data from audio channel to AAC encoder
    audioStreamer.registerInput(audio);
    audioStreamer.registerOutput(aac);
    if (audioStreamer.begin() != 0) {
      Serial.println("StreamIO link start failed");
    }

    // Configure StreamIO object to stream data from video channel and AAC encoder to MP4 recording
    avMixStreamer.registerInput1(Camera.getStream(CHANNELVID));
    avMixStreamer.registerInput2(aac);
    avMixStreamer.registerOutput1(rtsp);
    //avMixStreamer.registerOutput2(mp4);
    if (avMixStreamer.begin() != 0) {
        Serial.println("StreamIO link start failed");
    }

    // Start data stream from video channel
    Camera.channelBegin(CHANNELVID);

    // Configure StreamIO object to stream data from low res video channel to motion detection
    videoStreamerMD.registerInput(Camera.getStream(CHANNELMD));
    videoStreamerMD.setStackSize();
    videoStreamerMD.registerOutput(MD);
    if (videoStreamerMD.begin() != 0) {
      Serial.println("StreamIO link start failed");
    }

    // Start data stream from low resolution video channel
    Camera.channelBegin(CHANNELMD);
    
    // Configure OSD for drawing on video stream
    OSD.configVideo(CHANNELVID, configV);
    OSD.configVideo(CHANNELVID, configV);    
    OSD.begin();
}

void motionDetection(void )
{
 
   // Motion detection results is expressed as an array
    // With 0 or 1 in each element indicating presence of motion
    // Iterate through all elements to check for motion
    // and calculate largest rectangle containing motion
    if (motionDetected == false){
      std::vector<MotionDetectionResult> md_results = MD.getResult();
        
      if (MD.getResultCount() > 0) {
          digitalWrite(GREEN_LED, HIGH);
          motionCount++;
          Serial.print("Motion detection : ");
          Serial.println(motionCount);
          if(motionCount==3){  
            tone(BUZZER_PIN, 1000, 500);
            motionDetected = true;
            digitalWrite(GREEN_LED, HIGH);
            timelapseSec = silenceTimeSec;
            motionCount=0;
          }
      } 
      else{
        motionCount = 0;
        digitalWrite(GREEN_LED, LOW); // GREEN LED turn off when no motion detected
      }
    }
    else{
         timelapseSec--;
         if ( timelapseSec == 0)
         {
            digitalWrite(GREEN_LED, LOW); // GREEN LED turn off when no motion detected 
            motionDetected = false;
         }
    }   
  
}

void updateOSD(void)
{
  OSD.createBitmap(CHANNELVID); 
  char *osdText = datenTime.getDateNTimeStr();
  OSD.drawText(CHANNELVID, 500,1000, osdText  ,COLOR_GREEN);
  OSD.update(CHANNELVID);  
}


void setup() {
    Serial.begin(115200);
    
    // GPIO Initialization
    pinMode(GREEN_LED, OUTPUT);

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        digitalWrite(GREEN_LED, HIGH);
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);

        // wait 2 seconds for connection:
        delay(1000);
        digitalWrite(GREEN_LED, LOW);
        delay(1000);
    }
   /* Synch RTC with NTP server*/
   datenTime.begin();
   datenTime.synchRTCtoNtpServer();   
   Serial.println(datenTime.getDateNTimeStr());
   closeNetInit();
   registerTasks(wsTaskDec,MAX_TASK); 
 
   /*start SD card file system */
   fs.begin();

   cctvInit();    
   // example to crete wd task for each services
   //int32_t id =  wdApp.initWdApp(10, "test wd");
   //Serial.print( " Wd App Id *********************************************************************************************************");
   //Serial.println(id);
   //wdApp.startWdApp(id);
}

void commandTask()
{
  checkSerialCommand();
}

void mdTask (void )
{
  motionDetection();
}

void osdTask (void)
{
  updateOSD();
}

void wdTask()
{
  wdApp.kshomeWdLooper() ;
}
void loop() {
  startTask();    
  while (1)
  {    
    taskScheduler();
    taskRunner();
    delay(TIME_TICK);     
 }//while
}
