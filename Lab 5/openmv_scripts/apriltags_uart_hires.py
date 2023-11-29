# Find Small Apriltags
#
# This script shows off how to use blob tracking as a pre-filter to
# finding Apriltags in the image using blob tracking to find the
# area of where the tag is first and then calling find_apriltags
# on that blob.

# Note, this script works well assuming most parts of the image do not
# pass the thresholding test... otherwise, you don't get a distance
# benefit.

import sensor, image, time, math, omv

#adding UART capability
import pyb, ustruct
uart = pyb.UART(3, 115200, timeout_char = 50)

#say hello
uart.write("Hej, verden!!\n")

# Set the thresholds to find a white object (i.e. tag border)
thresholds = (160, 255)

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
if omv.board_type() == "H7": sensor.set_framesize(sensor.VGA)
elif omv.board_type() == "M7": sensor.set_framesize(sensor.VGA)
else: raise Exception("You need a more powerful OpenMV Cam to run this script")
sensor.skip_frames(time = 100) # increase this to let the auto methods run for longer
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

# The apriltag code supports up to 6 tag families which can be processed at the same time.
# Returned tag objects will have their tag family and id within the tag family.
tag_families = 0
tag_families |= image.TAG16H5 # comment out to disable this family
#tag_families |= image.TAG25H7 # comment out to disable this family
#tag_families |= image.TAG25H9 # comment out to disable this family
#tag_families |= image.TAG36H10 # comment out to disable this family
#tag_families |= image.TAG36H11 # comment out to disable this family (default family)
#tag_families |= image.ARTOOLKIT # comment out to disable this family

while(True):
    clock.tick()
    img = sensor.snapshot()

    # First, we find blobs that may be candidates for tags.
    box_list = []

    # AprilTags may fail due to not having enough ram given the image size being passed.
    tag_list = []

#    print("snapshot")

###########
# TODO:
# make thresholds correspond to size of actual tags
# check on merging or not
#############

    for blob in img.find_blobs([thresholds], pixels_threshold=100, area_threshold=800, merge=False):
        # Next we look for a tag in an ROI that corresponds to the blob.
        # For VGA resolution, a typical tag will be ~40 pixels across, ~60 if angled to 45 deg
        w = 64
        h = 64

        x = blob.x()
        y = blob.y()

        box_list.append((x, y, w, h)) # We'll draw these later.

        # Since we constrict the roi size apriltags shouldn't run out of ram.
        # But, if it does we handle it...
        try:
            tag_list.extend(img.find_apriltags(roi=(x,y,w,h), families=tag_families))
        except (MemoryError): # Don't catch all exceptions otherwise you can't stop the script.
            print("memory overflow:")
            pass

    for b in box_list:
        img.draw_rectangle(b)
    # Now print out the found tags
    for tag in tag_list:
        img.draw_rectangle(tag.rect())
        img.draw_cross(tag.cx(), tag.cy())
        for c in tag.corners():
            img.draw_circle(c[0], c[1], 5)
        print("Tag:", tag.cx(), tag.cy(), tag.rotation(), tag.id())
        #uart.write(str(tag.id()))
        uart.write(ustruct.pack("<bbhhhb", 255, 0, tag.id(), tag.cx(), tag.cy(), 170))
