const uint8_t bluetooth_greeting[] ="\
WELCOME TO ESP SERIAL VIEWER\n\
Software version: 0.13.0\n------------------------------------------------------------\n\n\
Input 'esp help' to see all available options\n";

const uint8_t help_message[] = "\
ESP SERIAL VIEWER MANUAL\n\n\
--- ESP SET COMMANDS ---\n\n\
* esp set baud x: sets uart baud rate to x option\n\
        1: 115200,  8bits, 1 stop bit, no parity\n\
        2: 115200,  8bits, 1 stop bit, even parity\n\
        3: 57600,   8bits, 1 stop bit, no parity\n\
        4: 57600,   8bits, 1 stop bit, even parity\n\
        5: 9600,    8bits, 1 stop bit, no parity\n\
        6: 9600,    8bits, 1 stop bit, even parity\n\ 
";