found an amazon 2015 fire tablet and decided to try installing lineageOS on it.


Most ROM flashing should take only two general steps:
1. rooting
2. flash lineageOS image

In my case it was:
1. rooting
2. flash lineageOS image
3. get it out of bootloop, and retry from step 1

## rooting

I had already rooted this tablet to install cyanogenmod (F to pay respects)

## flash lineageOS image

I found a lineageOS v14.1 built for fire tablet 2015 
https://forum.xda-developers.com/amazon-fire/orig-development/rom-lineage-14-1-t3962457

I managed to put the device into a boot loop, so now it gets more fun...
Restarting in recovery mode and clearing cache/factory reset did not fix it, so now I've got to find how to fix this.

After some googling, I found I can sideload the factory image from amazon, there's another xda thread that describes how to do that
https://forum.xda-developers.com/amazon-fire/general/unbrick-amazon-fire-7in-2015-5th-gen-t3285294

I downloaded v5.3.1.0 as recommended by the thread linked, and had no trouble starting the device in recovery mode, and pushing the bin file via `adb sideload`.

Waiting to see if it worked...YES, I got past the initial boot screen, and now it's booting into the fireOS.

I now need to root the tablet again.

## rooting (but actually this time)

The thread for rooting amazon tablet is found here https://forum.xda-developers.com/amazon-fire/development/wip-achieving-root-thread-t3238152

Blech, that thread seems to be for firmware <5.0.1 and I recovered back to 5.3.1.
But in the recovery thread it said 5.3.1 is rootable, so more searching leads to supertool https://forum.xda-developers.com/amazon-fire/development/amazon-fire-5th-gen-supertool-root-t3272695

Rooting for 5.3.1 is apparently now done via kingroot.  It failed on the first attempt for me, but I've read that you can just try it a couple times.

1 time
2 times
3 times
4 times (trying without usb connected)
5 times
6 times
(trying a reboot in between)
7 times kingroot doesn't get past the "verifying root status..." loader
(rebooting again)
8 times still not getting past first screen
(reboot?)
9 times still not getting past that, not sure what kingroot is doing

I'm going to bed for tonight, will pick this up again later.



