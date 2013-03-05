#!/bin/bash

count=11
min=(0 55 160 225 320 640 1280 2600 5800 8000 10240)
max=(54 159 224 319 639 1279 2599 5799 7999 10239 65536)
value=(32 33 51 61 91 95 130 186 208 231 255)

while true
  do
    cat /sys/power/wait_for_fb_wake > /dev/null

    data=`cat /sys/class/sensors/light_sensor/raw_data`
    brightness=`cat /sys/class/backlight/panel/brightness`

    for index in `seq 0 $((count - 1))`
      do
        if (( $data >= ${min[$index]} && $data <= ${max[$index]} && ${value[$index]} != $brightness ))
          then
            echo $data ${value[$index]}
            echo ${value[$index]} > /sys/class/backlight/panel/brightness
        fi
      done

    sleep 5
  done

