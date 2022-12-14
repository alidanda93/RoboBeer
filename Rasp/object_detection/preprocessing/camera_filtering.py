import cv2
import numpy as np
import serial
import time

# serial = serial.Serial ("/dev/ttyS0", 115200)

def get_variable_name(variable):
    globals_dict = globals()
    return [var_name for var_name in globals_dict if globals_dict[var_name] is variable]

def drawContours(screen):
    if (get_variable_name(screen)[0]=='red'):
        cv2.rectangle(screen,(x, y),(x+w, y+h),(0, 0, 255), 2)
        cv2.drawMarker(screen,(taille_x//2,taille_y//2),(255,0,0),cv2.MARKER_CROSS,10,6,cv2.LINE_4)
        cv2.putText(screen,str("Center at "+"X:"+str(taille_x/2)+" Y:"+str(taille_y/2)),POSITION_TEXT_CENTER,cv2.FONT_HERSHEY_SIMPLEX,FONT_SIZE,(0,0,255),FONT_THIKNESS,cv2.LINE_AA)
        cv2.drawMarker(screen,(x+w//2,y+h//2),(0,0,255),cv2.MARKER_CROSS,10,6,cv2.LINE_4)
        cv2.putText(screen,str("Object at "+"X:"+str(x+w/2)+" Y:"+str(y+h/2)+" w:" +str(w)+" h:"+str(h)),POSITION_TEXT_OBJET,cv2.FONT_HERSHEY_SIMPLEX,FONT_SIZE,(0,0,255),FONT_THIKNESS,cv2.LINE_AA)
        return str("X:"+str(x+w/2)+" Y:"+str(y+h/2)+" w:" +str(w)+" h:"+str(h))
    
    elif (get_variable_name(screen)[0]=='green'):
        cv2.rectangle(green,(x, y),(x+w, y+h),(0, 255, 0), 2)
        cv2.drawMarker(green,(taille_x//2,taille_y//2),(255,0,0),cv2.MARKER_CROSS,10,6,cv2.LINE_4)
        cv2.putText(green,str("Center at "+"X:"+str(taille_x/2)+" Y:"+str(taille_y/2)),POSITION_TEXT_CENTER,cv2.FONT_HERSHEY_SIMPLEX,FONT_SIZE,(0,0,255),FONT_THIKNESS,cv2.LINE_AA)
        cv2.drawMarker(green,(x+w//2,y+h//2),(0,255,0),cv2.MARKER_CROSS,10,6,cv2.LINE_4)
        cv2.putText(green,str("Object at "+"X:"+str(x+w/2)+" Y:"+str(y+h/2)+" w:" +str(w)+" h:"+str(h)),POSITION_TEXT_OBJET,cv2.FONT_HERSHEY_SIMPLEX,FONT_SIZE,(0,0,255),FONT_THIKNESS,cv2.LINE_AA)
        return str("X:"+str(x+w/2)+" Y:"+str(y+h/2)+" w:" +str(w)+" h:"+str(h))

    else:
        return -1

def Red_or_Green(red_surface,green_surface):
    if red_surface>green_surface : 
        
        serial.write()

cap = cv2.VideoCapture(0)

while True:
    _, img = cap.read()

    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    (taille_y , taille_x,__) = np.shape(img)
    POSITION_TEXT_CENTER = (taille_x//10,taille_y//10)
    POSITION_TEXT_OBJET = (taille_x//10,2*(taille_y//10))
    FONT_SIZE = 1
    FONT_THIKNESS = 2
    ####RED####

    #lower red
    lower_red = np.array([0,50,50])
    upper_red = np.array([10,255,255])
    #upper red
    lower_red2 = np.array([170,50,50])
    upper_red2 = np.array([180,255,255])
    #red masks
    red_mask = cv2.inRange(hsv, lower_red, upper_red)
    red_res = cv2.bitwise_and(img,img, mask= red_mask)
    red_mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
    red_res2 = cv2.bitwise_and(img,img, mask= red_mask2)
    #combine reds
    red = red_res+red_res2
    #Add the contours
    contours = cv2.findContours(red_mask2.copy(),cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)[-2]
    if len(contours) > 0:
        red_area = max(contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(red_area)
        if (1.6 < (h/w) <1.8):drawContours(red)

    ####GREEN####

    #lower green
    lower_green = np.array([36,30,10])
    upper_green = np.array([86,255,255])
    #green masks
    green_mask = cv2.inRange(hsv, lower_green, upper_green)
    green = cv2.bitwise_and(img,img, mask= green_mask)
    #Add the contours
    contours = cv2.findContours(green_mask.copy(),cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)[-2]
    if len(contours) > 0:
        green_area = max(contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(green_area)
        if (1.6 < (h/w) <1.8): drawContours(green)


    cv2.imshow('red',red)
    cv2.imwrite('red.png',red)
    cv2.imshow('green',green)
    cv2.imwrite('green.png',green)


    key = cv2.waitKey(1)
    if key == 27:
        break
