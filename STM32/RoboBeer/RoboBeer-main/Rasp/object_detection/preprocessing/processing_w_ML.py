from ctypes import sizeof
import cv2
import numpy as np
import tensorflow_hub as hub
import tensorflow as tf

# Apply image detector on a batch of image.
detector = hub.load("https://tfhub.dev/tensorflow/efficientdet/lite0/detection/1")
img = cv2.imread('test.jpeg')
img = img[np.newaxis, ...]
image_tensor = tf.convert_to_tensor(img)
boxes, scores, classes, num_detections = detector(img)
print(boxes)
print(scores)
print(classes)
print(num_detections)
#print(np.shape(img))
