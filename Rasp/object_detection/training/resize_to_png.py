from PIL import Image 
import glob, os

i=0
for infile in glob.glob("*.JPEG"):
    file, ext = os.path.splitext(infile)
    im = Image.open(infile)
    rgb_im = im.convert('RGB')
    rgb_im_small = rgb_im.resize([512,512])
    rgb_im_small.save( str(i) + ".png", "PNG")
    i += 1


for infile in glob.glob("*.JPG"):
    file, ext = os.path.splitext(infile)
    im = Image.open(infile)
    rgb_im = im.convert('RGB')
    rgb_im_small = rgb_im.resize([512,512])
    rgb_im_small.save( str(i) + ".png", "PNG")
    i += 1
