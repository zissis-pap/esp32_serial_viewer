const uint8_t bluetooth_greeting[] ="\
WELCOME TO ESP SERIAL VIEWER\n\
Software version: 0.15.3\n------------------------------------------------------------\n\n\
Enter 'esp help' to see all available options";

const uint8_t help_message[] = "\
ESP SERIAL VIEWER MANUAL\n\n\
------ ESP EXE COMMANDS ------\n\n\
* esp exe read: enables reading the uart\n\
* esp exe list: lists the contents of the SD\n\n\
------ ESP SET COMMANDS ------\n\n\
* esp set baud x: sets uart baud rate to x option\n\
        1: 115200,  8bits, 1 stop bit, no parity\n\
        2: 115200,  8bits, 1 stop bit, even parity\n\
        3: 57600,   8bits, 1 stop bit, no parity\n\
        4: 57600,   8bits, 1 stop bit, even parity\n\
        5: 9600,    8bits, 1 stop bit, no parity\n\
        6: 9600,    8bits, 1 stop bit, even parity";