
# CosmOS hostid

Each CosmOS node has a hostid.  The device that provides is `hostid0`.  The hostid device initializes itself at startup from the `rand0` device, which in turn initalizes itself from the `rtc0` device.

 