detector = hub.load("https://tfhub.dev/tensorflow/efficientdet/lite0/detection/1")



boxes, scores, classes, num_detections = detector(image_tensor)