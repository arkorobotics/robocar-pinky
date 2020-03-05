Pinky Control Software
=======

## Configure Jetson I2C Bus
```
Update the I2C-A device clock rate by changing '/sys/bus/i2c/devices/i2c-1/bus_clk_rate' from 100000 to 400000
```

## Build
```
$ cd robocar-pinky/Software/ctrl
$ make
```

## Run
```
$ cd robocar-pinky/Software/ctrl
$ ./ctrl.sh
```

