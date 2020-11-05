# web_control_relay

## Wiring Diagram
#### ESP-01  <>  Pro Mini
#### RX    <<<<>>      3 (TX in software serial)
#### TX    <<<>>>      2 (RX in software serial)

### ESP-01 <> Power Supply
#### VCC <<<<>>>> 3V3
#### CH_PD <<<<>> 3V3
#### GND <<<<>>>> GND

### Relay <> Pro Mini
#### CH1 <<<<>> 14 (A0)
#### CH2 <<<<>> 15 (A1)
#### CH3 <<<<>> 16 (A2)
#### RGND <<<<>> GND

### Relay <> Power Supply
#### VCC <<<<>>>> 5V
#### GND <<<<>>>> GND