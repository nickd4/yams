## Yet Another Machine Simulator (YAMS)

See project homepage https://www.niksula.hut.fi/~buenos/yams.html. I, `nickd4`,
have created this repository so that I can include `yams` as a submodule in my
projects. Files from https://www.niksula.hut.fi/~buenos/dist/yams-1.4.2.tar.gz.

### Introduction

YAMS is a machine simulator which emulates the MIPS32 architecture CPU close enough (should be fully compliant, but we can't claim that it is) to allow cross compilation with standard MIPS32 compiler back-ends. YAMS also provides a very simple device interface to the simulated memory, disks, network interfaces, terminals and a real-time clock. There is also support for pluggable I/O devices. Pluggable devices are separate programs that implement the functionality of one or more devices and communicate with YAMS over a network or Unix domain socket.

Many features of YAMS are configurable. For example the number of CPUs can be configured. When the number of CPUs is more than one, YAMS simulates an SMP machine. The devices are also configurable. For example various delays for disks, terminals and network interfaces can be set.

YAMS also provides a hardware console which can be used to debug programs. The hardware console can be used to set breakpoints and dump the contents of registers, TLBs and memory. The memory dumping functionality also contains a disassembler.

### Performance

The purpose of YAMS is to provide a very simple yet realistic simulated hardware platform for educational purposes. High performace (i.e. high clock speed) was not a factor in its implementation, so a normal slowdown factor between host clock speed and simulator clock speed is in the order of 500, resulting in simulator clock speeds of only a few megaherz.

So if you are looking for a fast MIPS emulator/simulator, then YAMS is not for you.
