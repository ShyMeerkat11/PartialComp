# SPI with the Arduino as the master device and the OpenMV Cam as the slave.
#
# Please wire up your OpenMV Cam to your Arduino like this:
#
# OpenMV Cam Master Out Slave In (P0) - Arduino Uno MOSI (11)
# OpenMV Cam Master In Slave Out (P1) - Arduino Uno MISO (12)
# OpenMV Cam Serial Clock        (P2) - Arduino Uno SCK  (13)
# OpenMV Cam Slave Select        (P3) - Arduino Uno SS   (10)
# OpenMV Cam Ground                   - Arduino Ground

import pyb, ustruct, time, sensor, image, math

#text = "Hello World Gavin and Andrew\n"
#data = ustruct.pack("<bi%ds" % len(text), 85, len(text), text) # 85 is a sync char.


# READ ME!!!
#
# Please understand that when your OpenMV Cam is not the SPI master it may miss responding to
# sending data as a SPI slave no matter if you call "spi.send()" in an interrupt callback or in the
# main loop below. Because of this you must absolutely design your communications protocol such
# that if the slave device (the OpenMV Cam) doesn't call "spi.send()" in time that garbage data
# read from the SPI peripheral by the master (the Arduino) is tossed. To accomplish this we use a
# sync character of 85 (binary 01010101) which the Arduino will look for as the first byte read.
# If it doesn't see this then it aborts the SPI transaction and will try again. Second, in order to
# clear out the SPI peripheral state we always send a multiple of four bytes and an extra four zero
# bytes to make sure that the SPI peripheral doesn't hold any stale data which could be 85. Note
# that the OpenMV Cam will miss the "spi.send()" window randomly because it has to service
# interrupts. Interrupts will happen a lot more while connected to your PC.

# The hardware SPI bus for your OpenMV Cam is always SPI bus 2.
# polarity = 0 -> clock is idle low.
# phase = 0 -> sample data on rising clock edge, output data on falling clock edge.
spi = pyb.SPI(2, pyb.SPI.SLAVE, polarity=0, phase=0, firstbit=pyb.SPI.MSB, bits = 8)


threshold_index = 1 # 0 for red, 1 for green, 2 for blue, 3 for yellow

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [#(30, 100, 15, 127, 15, 127), # generic_red_thresholds
              #(30, 100, -64, -8, -32, 32), # generic_green_thresholds
              (0, 30, 0, 64, -128, 0),      # Blue
              (40, 100, -35, 35, 25, 127)]
data = "Hello"
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 5000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()
# NSS callback.
def nss_callback(line):
    global spi
    try:
        spi.send(data, timeout=40)
        print(data)
        #SPI.send(send, recv=None, timeout=5000)
    except OSError as err:
        pass # Don't care about errors - so pass.
        # Note that there are 3 possible errors. A timeout error, a general purpose error, or
        # a busy error. The error codes are 116, 5, 16 respectively for "err.arg[0]".

# Configure NSS/CS in IRQ mode to send data when requested by the master.
pyb.ExtInt(pyb.Pin("P3"), pyb.ExtInt.IRQ_FALLING, pyb.Pin.PULL_DOWN, nss_callback)

while(True):
    clock.tick()
    #print(data)
    img = sensor.snapshot()
    foundObject = False
    for blob in img.find_blobs([thresholds[threshold_index]], pixels_threshold=200, area_threshold=200, merge=True):
        # These values depend on the blob not being circular - otherwise they will be shaky.
        if blob.convexity() > 0.8 and blob.h() > 50:
            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,255,0))
            img.draw_line(blob.minor_axis_line(), color=(0,0,255))
            # These values are stable all the time.
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            x = blob.cxf()
            y = blob.cyf()
            h = blob.h()
            w = blob.w()
            #Use blob.code() to determine which color its detecting
            xInch = ((2.8*25.4*240)/(h*2.4))/25.4
            yInch = (y-120)/h
            zInch = (x-160)/w
            dTheta = math.degrees(math.atan(zInch/xInch))
            #Change "Yellow" to color variable
            #Parse with Spaces
            print("Color", "Yellow", "Distance", xInch, "Height", yInch, "Angle", dTheta)
            #string_in_string = "Shepherd {} is on duty.".format(shepherd)
            data = "Color Yellow Distance {} Height {} Angle {} \0".format(xInch, yInch, dTheta)
            #data = "Color {} Distance {} Height {} Angle {} \0".format(color, Inch, yInch, dTheta)
            foundObject = True
            #print(data)
        else:
            #data = "else statemnt \0"
            #print(data)
            #Append to string of data to get ready to send
            #temp = temp.append(x,y,theta)
    #clear the data?
    #what happens if we are in the loop and spi gets called?
    #keep a temp at the end of the loop incase this happens
    #data = temp
        # Note - the blob rotation is unique to 0-180 only.
            img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
    #print(clock.fps())
    if foundObject == False:
        data = "No Objects Found\0"
    foundObject = False

###################################################################################################
# Arduino Code
###################################################################################################
#
# #include <SPI.h>
# #define SS_PIN 10
# #define BAUD_RATE 19200
# #define CHAR_BUF 128
#
# void setup() {
#   pinMode(SS_PIN, OUTPUT);
#   Serial.begin(BAUD_RATE);
#   SPI.begin();
#   SPI.setBitOrder(MSBFIRST);
#   SPI.setClockDivider(SPI_CLOCK_DIV16);
#   SPI.setDataMode(SPI_MODE0);
#   delay(1000); // Give the OpenMV Cam time to bootup.
# }
#
# void loop() {
#   int32_t len = 0;
#   char buff[CHAR_BUF] = {0};
#   digitalWrite(SS_PIN, LOW);
#   delay(1); // Give the OpenMV Cam some time to setup to send data.
#
#   if(SPI.transfer(1) == 85) { // saw sync char?
#     SPI.transfer(&len, 4); // get length
#     if (len) {
#       SPI.transfer(&buff, min(len, CHAR_BUF));
#       len -= min(len, CHAR_BUF);
#     }
#     while (len--) SPI.transfer(0); // eat any remaining bytes
#   }
#
#   digitalWrite(SS_PIN, HIGH);
#   Serial.print(buff);
#   delay(1); // Don't loop to quickly.
# }
