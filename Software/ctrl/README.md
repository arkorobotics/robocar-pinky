Pinky Control
=======

Pinky's Control Software is a work in progress...

## Configure Jetson

```
Change '/sys/bus/i2c/devices/i2c-1/bus_clk_rate' from 100000 to 400000
```

## Build

```
$ cd robocar-pinky/Software/ctrl
$ make
```

## Run
```
$ cd robocar-pinky/Software/ctrl/build
$ ./ctrl
```

