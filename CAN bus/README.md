Task for 'Redes de comunicação automotivas'

KitCAN - 16F / LabTools - Mosaico Didactic Division / 'Manual Kit CAN-16F rev2.pdf'

The idea was to connect 3 devices using CAN bus. Each device was sending 1 analog value from trimpot and reading the value from the other 2 devices. Each display was showing the address of each node with the respective analog value.

Some initial tests was performed to verify the correct working of each module.

The first video I tested the SPI communication between the PIC and the CAN controller IC at my home. The 'C' directive is for my LCD.
https://youtu.be/9wIS63e8tIY

The last video I transferred the same code to 3 independent modules where I set different CAN address by a DIP switch 
can address = 0x0220 | (Value from DIPswitch & 0x0F) . The 'E' directive is used for LCD of Mosaico  kit.
https://youtu.be/UmrB_xfkr3w

'CAN T2 Ricardo de Freitas.pdf' has calculations for CAN bus setup.
