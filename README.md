# Coalila
Coalila is an open source PCI bus accessing utility.

pci-utils is a popular pci bus utils on linux and windows.

But I found some problem in my job. And I don't like the driver 

dropping mechanism in DirectIo because it is rootkit behavior.

That is gray skill. BTW, pci-utils uses old way to scan bus devices.

That way is for PC-AT compatible machine and could be obsoleted in future.

So I build this toolset for windows.

Any feedback is welcome.


[Warning]

To use this tool, please study PCI spec first. You should know what are you doing especially writing something to PCI bus.

If something wrong duing writing or write to wrong address, your computer could be crazy.

e.g. some PCI/PCIe device disappeared, 

To get your computer, you have to cold reset (power off then power on) computer.

OS reboot can't restore computer back.



[Environment]

WindowSDK 10.0.20348 (WinSDK for server 2022)

WDK 10.0.20348 (WDK for server 2022)

VisualStudio 2019

[Contact Me]

Author : Roy Wang (SmokingPC)

EMail : smokingpc@gmail.com
