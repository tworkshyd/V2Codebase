
T-works Foundation
Ventilator Project
Display Board Firmware
PCB Rev: Display Board v2.2
Repo link: https://github.com/tworkshyd/V2Codebase.git



11-aug-2021
	02.49pm:	Clock configured to 16Mhz, tested clock out at PE7, also tested the delay routine by toggling a port pin @ 1Hz and check the waveform on DSO.

12-aug-2021
	09:00am:	with '#define F_CPU   8000000' settings,
			Observed waveforms on DSO, one port pin toggle in while loop resulting ito a waveform with 160nsecs pulse <--> 2.66MHz
			Also observed _delayms(1000) ... yeilding to 1.01Hz


