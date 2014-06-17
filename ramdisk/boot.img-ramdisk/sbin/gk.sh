#!/sbin/busybox sh

mount -o remount,rw /system
/sbin/busybox mount -t rootfs -o remount,rw rootfs

pm disable com.sec.knox.seandroid
setenforce 0

echo 2 > /sys/devices/system/cpu/sched_mc_power_savings
echo 1 > /sys/kernel/dyn_fsync/Dyn_fsync_active

/system/bin/setprop pm.sleep_mode 1
/system/bin/setprop ro.ril.disable.power.collapse 0
/system/bin/setprop ro.telephony.call_ring.delay 1000

ln -s /res/synapse/uci /sbin/uci
/sbin/uci

/sbin/busybox mount -t rootfs -o remount,ro rootfs
mount -o remount,ro /system
