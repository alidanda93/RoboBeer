#!/usr/bin/env python


import cv2
import numpy as np

MIN_AREA = 5000
MAX_AREA = 1000000

cap = cv2.VideoCapture(0)

while True:
    _, img = cap.read()
    #img = cv2.flip(img,-1)
    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    (taille_y , taille_x,__) = np.shape(img)
    POSITION_TEXT_CENTER = (taille_x//20,taille_y//20)
    POSITION_TEXT_OBJET = (taille_x//20,2*(taille_y//20))
    POSITION_TEXT_OBJET2 = (taille_x//20,3*(taille_y//20))
    FONT_SIZE = 5
    FONT_THIKNESS = 10
    LINE_THICKNESS = 30
    RED_CANS=[]
    GREEN_CANS=[]
    CANS=[]
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
    red_mask_tot = red_mask + red_mask2
    #combine reds
    red = red_res+red_res2
    red = cv2.medianBlur(red,5)
    #Add the contours
    contourR, hierarchy = cv2.findContours(red_mask_tot.copy(),cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for j, contour in enumerate(contourR):
        epsilonR = 0.1 * cv2.arcLength(contour,True)
        approx = cv2.approxPolyDP(contour, epsilonR, True)
        if (len(approx)==4):
            #cv2.drawContours(red, contourR ,j, (255, 255, 0), 3)
            red_area = cv2.contourArea(contour)
            if (MIN_AREA<red_area<MAX_AREA ):
                cv2.drawContours(red, contourR ,j, (255, 0, 0), FONT_SIZE)
                M = cv2.moments(contour)
                cx = int(M['m10']/M['m00'])
                cy = int(M['m01']/M['m00'])
                x, y, w, h = cv2.boundingRect(contour)
                if (1.6 < (h/w) <1.8):      
                    cv2.rectangle(red,(x, y),(x+w, y+h),(0, 0, 255), LINE_THICKNESS)
                    cv2.drawMarker(red,(taille_x//2,taille_y//2),(255,0,0),cv2.MARKER_CROSS,LINE_THICKNESS,FONT_THIKNESS,FONT_SIZE)
                    cv2.drawMarker(red,(cx,cy),(255,0,0),cv2.MARKER_CROSS,LINE_THICKNESS,FONT_THIKNESS,FONT_SIZE)
                    CANS.append((red_area,cx,cy,(0,0,255)))


    ####GREEN####
    lower_green = np.array([35,25,10])
    upper_green = np.array([85,200,180])
    #green masks
    green_mask = cv2.inRange(hsv, lower_green, upper_green)
    green = cv2.bitwise_and(img,img, mask= green_mask)
    green = cv2.medianBlur(green,3)
    grey_green = cv2.cvtColor(green, cv2.COLOR_BGR2GRAY)
    _, threshold = cv2.threshold(grey_green, 100, 200, cv2.THRESH_BINARY)

    #Add the contours
    contourG , hierarchy = cv2.findContours(green_mask.copy(),cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)    
    contourN , hierarchy = cv2.findContours(threshold.copy(),cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)    
    for i, contour in enumerate(contourG):
        epsilonG = 0.1 * cv2.arcLength(contour,True)
        approx = cv2.approxPolyDP(contour, epsilonG, True)
        if (len(approx)==4):
            green_area = cv2.contourArea(contour)
            if (MIN_AREA<green_area < MAX_AREA ):
                cv2.drawContours(green, contourG ,i, (255, 0, 0), 3)
                M = cv2.moments(contour)
                cx = int(M['m10']/M['m00'])
                cy = int(M['m01']/M['m00'])
                x, y, w, h = cv2.boundingRect(contour)
                if (1.6 < (h/w) <1.8):
                    cv2.rectangle(green,(x, y),(x+w, y+h),(0, 0, 255), LINE_THICKNESS)
                    cv2.drawMarker(green,(taille_x//2,taille_y//2),(255,0,0),cv2.MARKER_CROSS,LINE_THICKNESS,FONT_THIKNESS,FONT_SIZE)
                    cv2.drawMarker(green,(cx,cy),(255,0,0),cv2.MARKER_CROSS,LINE_THICKNESS,FONT_THIKNESS,FONT_SIZE)
                    CANS.append((green_area,cx,cy,(0,255,0)))
  
    total = red + green
    enable=0
    action=0
    sens=0
    couleur=0
    try :
        (max_area,center_X,center_Y,color,distance)=max(CANS,key=lambda item:item[0])
        action = (0 if (taille_x//2-10<center_X<taille_x//2+10) else 1)  #0 pour AVANCER_ et 1 pour TOURNER_ 
        sens = (0 if (center_X<taille_x//2) else 1) # 0 pour tourner à GAUCHE_ et 1 pour la DROITE_ 
        couleur = (0 if (color==(0,0,255)) else 1)  # 0 pour ROUGE_ et 1 pour le VERTE_
        cv2.drawMarker(total,(center_X,center_Y),color,cv2.MARKER_CROSS,10,6,cv2.LINE_4)
        cv2.putText(total,str("X:"+str(center_X)+" Y:"+str(center_Y)),POSITION_TEXT_OBJET2,cv2.FONT_HERSHEY_SIMPLEX,FONT_SIZE,(255,255,255),FONT_THIKNESS,cv2.LINE_AA)
        cv2.putText(total,str("X:"+str(center_X)+" Y:"+str(center_Y)),POSITION_TEXT_OBJET2,cv2.FONT_HERSHEY_SIMPLEX,FONT_SIZE,(255,255,255),FONT_THIKNESS,cv2.LINE_AA)
        cv2.putText(total,couleur+action+sens,POSITION_TEXT_OBJET,cv2.FONT_HERSHEY_SIMPLEX,FONT_SIZE,(255,255,255),FONT_THIKNESS,cv2.LINE_AA)
        enable=1
    except:
        pass

    cv2.imshow('total',total)
    trame = bytearray([1,0,0,1,0,0,enable,action,sens,couleur,1,0,0,1,0,0])
    list_bytes=list(trame)
    print(str(list_bytes)+'\r')
    

    key = cv2.waitKey(1)
    if key == 27:
        break
