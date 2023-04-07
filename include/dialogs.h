const uint8_t bluetooth_greeting[] ="\
WELCOME TO ESP SERIAL VIEWER\n\
Software version: 0.17.0\n------------------------------------------------------------\n\n\
Enter 'esp help' to see all available options";

const uint8_t help_message[] = "\
ESP SERIAL VIEWER MANUAL\n\n\
------ ESP EXECUTION COMMANDS ------\n\n\
* esp read: enables reading the uart\n\
* esp stop: stops reading the uart\n\
* esp list: lists the contents of the SD\n\
* esp save <file>: starts reading and saves to the specified file\n\ 
* esp open <file>: opens the specified file\n\n\
------ ESP SETTING COMMANDS ------\n\n\
* esp set baud x: sets uart baud rate to x option\n\
        1: 115200,  8bits, 1 stop bit, no parity\n\
        2: 115200,  8bits, 1 stop bit, even parity\n\
        3: 57600,   8bits, 1 stop bit, no parity\n\
        4: 57600,   8bits, 1 stop bit, even parity\n\
        5: 9600,    8bits, 1 stop bit, no parity\n\
        6: 9600,    8bits, 1 stop bit, even parity";