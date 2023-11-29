# Find Small Apriltags
#
# This script shows off how to use blob tracking as a pre-filter to
# finding Apriltags in the image using blob tracking to find the
# area of where the tag is first and then calling find_apriltags
# on that blob.

# Note, this script works well assuming most parts of the image do not
# pass the thresholding test... otherwise, you don't get a distance
# benefit.

import sensor, image, time, math, struct, omv

#adding UART capability -- edit UART() for your choice of UART
import pyb, ustruct
uart = pyb.UART(3, 115200, timeout_char = 50)

#say hello
uart.write("Hej, verden!!\n")

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
if omv.board_type() == "H7": sensor.set_framesize(sensor.QQVGA)
elif omv.board_type() == "M7": sensor.set_framesize(sensor.QQVGA)
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


def checksum(data):
    checksum = 0
    for i in range(0, len(data), 2):
        checksum += ((data[i+1] & 0xFF) << 8) | ((data[i+0] & 0xFF) << 0)
    return checksum & 0xFFFF

def to_object_block_format(tag):
    angle = int((tag.rotation() * 180) / math.pi)
    temp = struct.pack("<hhhhhh", tag.id(), tag.cx(), tag.cy(), tag.w(), tag.h(), angle)
    return struct.pack("<bbh12sb", 0xFF, 0x55, checksum(temp), temp, 0xAA)


while(True):
    clock.tick()
    img = sensor.snapshot()
    tag_list = img.find_apriltags(families=image.TAG16H5) # default TAG36H11 family

    # Now print out the found tags
    for tag in tag_list:
        img.draw_rectangle(tag.rect())
        img.draw_cross(tag.cx(), tag.cy())
        for c in tag.corners():
            img.draw_circle(c[0], c[1], 5)
        print("Tag:", tag.id(), tag.cx(), tag.cy(), tag.rotation())
        #uart.write(str(tag.id()))
        #angle = int((tag.rotation() * 180) // math.pi)
        #uart.write(ustruct.pack("<bbhhhhhb", 255, 0, tag.id(), tag.cx(), tag.cy(), tag.w(), tag.h(), angle, 170))
        data_buf = to_object_block_format(tag)
        uart.write(data_buf)
