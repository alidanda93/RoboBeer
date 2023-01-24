# Copyright 2021 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""Main script to run the object detection routine."""
import argparse
import sys
import time
import tensorflow as tf
import cv2
from skimage.measure import label, regionprops
import numpy as np



def predict(imgs,interpreter,input_details,output_details):
    interpreter.set_tensor(input_details[0]['index'], imgs)
    interpreter.invoke()
    output_data = [interpreter.get_tensor(output_details[0]['index']),interpreter.get_tensor(output_details[1]['index'])]
    return output_data

def draw_predictions(imgs,interpreter,input_details,output_details,rotations=4,pred_mode='mean'):
    all_green_preds = []
    all_red_preds = []
    for i in range(1,rotations+1):
        
        rot_imgs= np.rot90(imgs,i)
        preds = predict(rot_imgs[np.newaxis,...],interpreter,input_details,output_details)
        
        green_preds,red_preds = preds
        green_preds = np.rot90(np.squeeze(green_preds),-i)
        red_preds = np.rot90(np.squeeze(red_preds),-i)
        
        all_green_preds.append(green_preds)
        all_red_preds.append(red_preds)
    
    
    all_green_preds = np.stack(all_green_preds,-1)
    all_red_preds = np.stack(all_red_preds,-1)
    if pred_mode=='mean':
        all_green_preds = np.mean(all_green_preds,-1)
        all_red_preds = np.mean(all_red_preds,-1)
    else:
        all_green_preds = np.min(all_green_preds,-1)
        all_red_preds = np.min(all_red_preds,-1)
        
    props_green = regionprops(np.round(all_green_preds).astype(int))
    props_red = regionprops(np.round(all_red_preds).astype(int))
    for prop in props_green:
        x,y,h,w = prop.bbox
        cv2.rectangle(imgs, (y, x), (w, h), (255, 0, 0), 2)
        cv2.putText(imgs, 'Canette verte', (y, x-5), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (36,255,12), 2)
    
    for prop in props_red:
        x,y,h,w = prop.bbox
        cv2.rectangle(imgs, (y, x), (w, h), (255, 0, 0), 2)
        cv2.putText(imgs, 'Canette rouge', (y, x-5), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,36,12), 2)
        




def run(model: str, camera_id: int, width: int, height: int, num_threads: int,
        enable_edgetpu: bool) -> None:


    
  # Variables to calculate FPS
  counter, fps = 0, 0
  start_time = time.time()
  fps_avg_frame_count = 10
  left_margin = 15
  row_size = 10
  font_size = 1
  text_color = (0, 0, 255)
  font_thickness = 1
  
  interpreter = tf.lite.Interpreter('tflite/model.tflite')
  interpreter.allocate_tensors()
  input_details = interpreter.get_input_details()
  output_details = interpreter.get_output_details()

  
  # Start capturing video input from the camera
  cap = cv2.VideoCapture(camera_id)
  cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
  cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)



  # Continuously capture images from the camera and run inference
  while cap.isOpened():
    success, image = cap.read()
    if not success:
      sys.exit(
          'ERROR: Unable to read from webcam. Please verify your webcam settings.'
      )

    counter += 1
# =============================================================================
#     image = cv2.flip(image, 1)
# =============================================================================

    # Convert the image from BGR to RGB as required by the TFLite model.
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    image = cv2.resize(image,(256,256))
    image = image.astype('float32')
    
    
    # Create a TensorImage object from the RGB image.
    draw_predictions(image,interpreter,input_details,output_details)
    image = image.astype(np.uint8)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)


    # Calculate the FPS
    if counter % fps_avg_frame_count == 0:
      end_time = time.time()
      fps = fps_avg_frame_count / (end_time - start_time)
      start_time = time.time()

    # Show the FPS
    fps_text = 'FPS = {:.1f}'.format(fps)
    text_location = (left_margin, row_size)
    cv2.putText(image, fps_text, text_location, cv2.FONT_HERSHEY_PLAIN,
                font_size, text_color, font_thickness)

    # Stop the program if the ESC key is pressed.
    if cv2.waitKey(1) == 27:
      break
    cv2.imshow('object_detector', image)

  cap.release()
  cv2.destroyAllWindows()


def main():
  parser = argparse.ArgumentParser(
      formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  parser.add_argument(
      '--model',
      help='Path of the object detection model.',
      required=False,
      default='detect_with_metadata.tflite')
  parser.add_argument(
      '--cameraId', help='Id of camera.', required=False, type=int, default=0)
  parser.add_argument(
      '--frameWidth',
      help='Width of frame to capture from camera.',
      required=False,
      type=int,
      default=256)
  parser.add_argument(
      '--frameHeight',
      help='Height of frame to capture from camera.',
      required=False,
      type=int,
      default=256)
  parser.add_argument(
      '--numThreads',
      help='Number of CPU threads to run the model.',
      required=False,
      type=int,
      default=8)
  parser.add_argument(
      '--enableEdgeTPU',
      help='Whether to run the model on EdgeTPU.',
      action='store_true',
      required=False,
      default=False)
  args = parser.parse_args()

  run(args.model, int(args.cameraId), args.frameWidth, args.frameHeight,
      int(args.numThreads), bool(args.enableEdgeTPU))


if __name__ == '__main__':
  main()
