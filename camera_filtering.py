import cv2
import numpy as np
import os

cap = cv2.VideoCapture(0)

while True:
    _, img = cap.read()

    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

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
        cv2.rectangle(red,(x, y),(x+w, y+h),(0, 0, 255), 2)

    #lower green
    lower_green = np.array([36,10,10])
    upper_green = np.array([86,255,255])
    #red masks
    green_mask = cv2.inRange(hsv, lower_green, upper_green)
    green = cv2.bitwise_and(img,img, mask= green_mask)
    #Add the contours
    contours = cv2.findContours(green_mask.copy(),cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)[-2]
    if len(contours) > 0:
        green_area = max(contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(green_area)
        cv2.rectangle(green,(x, y),(x+w, y+h),(0, 255, 0), 2)



    cv2.imshow('red',red)
    cv2.imshow('green',green)


    key = cv2.waitKey(1)
    if key == 27:
        break
