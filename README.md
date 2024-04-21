# Coalila
Coalila is an open source PCI bus accessing utility. pci-utils is a popular pci bus utility

on linux and windows. But I found some problem in my job. And I don't like the driver 

dropping mechanism in DirectIo because it is rootkit behavior. That is gray skill. 

BTW, pci-utils uses old way to scan bus devices. That way is for PC-AT compatible machine 

and could be obsoleted in future. So I build this toolset for windows.

Any feedback is welcome.

<BR>
[Warning]

To use this tool, please study PCI spec first. You should know what are you doing especially 

writing something to PCI bus. If something wrong duing writing or write to wrong address, 

your computer could be crazy. e.g. some PCI/PCIe device disappeared, 

To get your computer, you have to cold reset (power off then power on) computer.

OS reboot can't restore computer back.

<BR>
[Environment]

WindowSDK 10.0.20348 (WinSDK for server 2022)

WDK 10.0.20348 (WDK for server 2022)

VisualStudio 2019

<BR>
[Contact Me]

Author : Roy Wang (SmokingPC)

EMail : smokingpc@gmail.com

<BR>
<HR>

Coalila 是針對 PCI bus 資訊讀寫的小工具，原本這類型的工具以 pci-utils 為主，但它在windows

平台的版本很舊，並且使用舊型 PC-AT 的存取方式來處理(I/O Port Accessing)，並且沒有Thread-Safe保護。

對於I/O Port Accessing它利用了個灰色的技巧：drop legacy driver and install it at runtime.

這技巧原本是rootkit慣用的方式，我個人不是很愛用。

為了工作需要，我根據ACPI spec利用ECAM模式做了這支工具，它用memory mapping的方式取得ACPI MCFG 

table並映射進 memory space，如此一來存取PCI Bus資訊只需要直接讀寫這塊記憶體即可，另外我在 driver 

層也會進行Thread-Safe保護，避免可能的問題。

<BR>
[警告]

使用本工具前，請先研讀PCI Spec 3.0與PCIe Spec 5.0，請務必對你在讀寫的資料有清楚的認知。如果不小心

寫壞了某些資料有可能造成不可預期的問題，例如 PCI Device 消失等。如果遇到這種問題請 cold reset 電腦，

才能恢復正常，普通的 reboot 沒有效果。

<BR>
[開發環境]

WindowSDK 10.0.20348 (WinSDK for server 2022)

WDK 10.0.20348 (WDK for server 2022)

VisualStudio 2019

<BR>
[Contact Me]

Author : Roy Wang (SmokingPC)

EMail : smokingpc@gmail.com
