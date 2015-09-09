# UB-ANC
Open Testbed Platform for Drones

===================================

Steps needed for using serial port to connect to USRP E310:

1) sudo adduser $USER dialout (screen /dev/ttyUSB0 115200)

2) logout

3) screen /dev/ttyUSB0 115200

-----------------------------------

To use USRP E310 folder on your host:

1) sudo adduser $USER fuse

2) logout

3) sshfs -o allow_root root@128.205.187.244:/ usrp-folder-host

-----------------------------------

Steps needed to port Qt5 to USRP E310:

1) git clone  https://github.com/qtproject/qtbase

2) cd qtbase

3) git checkout stable

4) mkdir build

5) cd build

6) ../configure {with custom parameters in the folder}

7) make -j4

8) make install

-----------------------------------

Steps needed to port QtSerialPort to USRP E310:


1) git clone  https://github.com/qtproject/qtserialport

2) cd qtserial

3) git checkout stable

4) mkdir build

5) cd build

6) /path-to-sdk/sysroots/armv7ahf-vfp-neon-oe-linux-gnueabi/usr/local/Qt-5.6.0/bin/qmake ..

7) make

8) make install

-----------------------------------

Steps for Volk benchmark testing on USRP E310:

1) pip install numpy

2) pip install matplotlib

3) cd /path-to-gnuradio/examples/volk_benchmark

4) vim volk_test_funcs.py 

5) replace all scipy with numpy

6) read the README file for doing different tests.

For the current CPU speed one can dynamically watch this change in real time using:

sudo watch -n 1  cat /sys/devices/system/cpu/cpu*/cpufreq/cpuinfo_cur_freq

To see the maximum CPU speed, use:

cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_max_freq 
