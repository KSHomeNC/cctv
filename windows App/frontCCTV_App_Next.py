import cv2
import os
from datetime import datetime
import time
win_app = True
RTSP_URL = 'rtsp://192.168.0.42:554'
dir_path = ' '
default_path = ' '
os.environ['OPENCV_FFMPEG_CAPTURE_OPTIONS'] = 'rtsp_transport;udp' # Use tcp instead of udp if stream is unstable
if win_app:
    default_path = 'D:/KSHome'
    dir_pth = 'D:/KSHome/capture/cctvFront/'
else:
    default_path = '/media/pi/Seagate'
    dir_pth = '/media/pi/Seagate/KSHome/capture/cctvFront/'


def getSotrageDst():
    fullPath = ' '
    if os.path.isdir(default_path):
        if not os.path.isdir(dir_pth):
            os.mkdir(dir_pth)       
        subDir = dir_pth + str(datetime.now().strftime("%m_%d_%y"))
        if not os.path.isdir(subDir):
            os.mkdir(subDir)
        fullPath = subDir
    return fullPath


cap = cv2.VideoCapture(RTSP_URL, cv2.CAP_FFMPEG)
if not cap.isOpened():
    print('Cannot open RTSP stream')
    time.sleep(10)
    exit(-1)

video_codec = cv2.VideoWriter_fourcc(*'XVID')

            
while True:    
    t = datetime.now()
    sDay = t.day
    dirPath = getSotrageDst()      
    while True:       
        frame_width = int(cap.get(3))
        frame_height = int(cap.get(4))
        fps = 30
        st = datetime.now()
        sHour=t.hour
        fName = st.strftime("%H_%M_%S")
        fName = dirPath +'/'+ fName+'.mp4'
        print(fName)
        video_output = cv2.VideoWriter(fName, video_codec, fps, (frame_width, frame_height))
        while True:
            ret, frame = cap.read()

            if ret == True:
                video_output.write(frame)
            #else:
            #    break
            t = datetime.now()
            if t.hour != sHour:
                print('current time')
                print(t.hour)
                print('file Creation time')
                print(sHour)
                break
        
        print('Video was saved!')
        video_output.release()
        t = datetime.now()
        if t.day != sDay:
            break

cap.release()
video_output.release()