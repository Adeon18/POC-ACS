# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Ade0n\Desktop\lab\AAAAAAAAAAAAAAA\psoc_5-2-muliak_trush\Lab_2_7SEG_3.cydsn\Lab_2_7SEG_3.cyprj
# Date: Thu, 25 Nov 2021 17:21:50 GMT
#set_units -time ns
create_clock -name {CyRouted1} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyWCO} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/wco}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyECO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/eco}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {SPIM_IntClock} -source [get_pins {ClockBlock/hfclk}] -edges {1 3 7} [list [get_pins {ClockBlock/udb_div_1}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/hfclk}] -edges {1 60001 120001} [list [get_pins {ClockBlock/udb_div_3}]]


# Component constraints for C:\Users\Ade0n\Desktop\lab\AAAAAAAAAAAAAAA\psoc_5-2-muliak_trush\Lab_2_7SEG_3.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Ade0n\Desktop\lab\AAAAAAAAAAAAAAA\psoc_5-2-muliak_trush\Lab_2_7SEG_3.cydsn\Lab_2_7SEG_3.cyprj
# Date: Thu, 25 Nov 2021 17:21:48 GMT
