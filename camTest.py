import numpy as np
import cv2
from matplotlib import pyplot as plt

left = cv2.VideoCapture(0)
right = cv2.VideoCapture(1)

while(True):
    # Capture frame-by-frame
    leftRead, left_frame = left.read() 
    rightRead, right_frame = right.read() 
    #disparity = None
    
    # convert the RGB to greyscale
    if leftRead & rightRead:
        Lgrey = cv2.cvtColor(left_frame,cv2.COLOR_BGR2GRAY)
        Rgrey = cv2.cvtColor(right_frame,cv2.COLOR_BGR2GRAY)
       # rB, rG, rR = cv2.split(right_frame)
       # lB, lG, lR = cv2.split(left_frame)

    # create the disparity map
        stereo = cv2.StereoBM_create(16,7)
        disparity = stereo.compute(Lgrey, Rgrey)
        output = np.fliplr(disparity/255)
       # rDisp = stereo.compute(lR,rR)
       # gDisp = stereo.compute(lG,rG)
       # bDisp = stereo.compute(lB,rB)
       # colors = cv2.merge((bDisp,gDisp,rDisp))

    # Display the resulting frame
        #cv2.imshow('disparity',output)
        cv2.imshow('zoom', cv2.pyrUp(output))
       # cv2.imshow('BGR', colors)
       # cv2.imwrite('test.png', disparity)
       # cv2.imshow('right',Rgrey)
       # cv2.imshow('left',Lgrey)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    
   

# When everything is done, release the capture
left.release()
right.release()
cv2.destroyAllWindows()