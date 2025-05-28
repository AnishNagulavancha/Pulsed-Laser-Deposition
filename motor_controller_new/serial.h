#define BEGINNING               (0)
#define SMALL_RING_SIZE         (16)
#define LARGE_RING_SIZE         (100)

// These two are for 8MHz
#define SET_UCA0_115200         {UCA0BRW = 4;UCA0MCTLW = 0x5551;UCA1BRW = 4;UCA1MCTLW = 0x5551;}
#define SET_UCA0_9600           {UCA0BRW = 52;UCA0MCTLW = 0x4911;UCA1BRW = 52;UCA1MCTLW = 0x4911;}

void Init_Serial_UCA(void);
void out_character(char character);

void USCI_A0_transmit(void);
void USCI_A1_transmit(void);
void clearProcessBuff(volatile char* pb, volatile unsigned int* pb_index, volatile unsigned int* pb_buffered);
void loadRingtoPB(volatile unsigned int* rx_wr, unsigned int* rx_rd, volatile char* Rx_Process, volatile char* Rx_Ring, volatile unsigned int* pb_index, volatile unsigned int* pb_buffered);
void loadRingtoPB_0(void);
void loadRingtoPB_1(void);
void clearProcessBuff_0(void);
void clearProcessBuff_1(void);
void serialInterrupt(volatile unsigned int* rx_wr, volatile char Rx_Ring[], volatile char Tx[], volatile unsigned int* tx_index, volatile unsigned short * txbuf, volatile unsigned short * txbuf_other, volatile unsigned short ucaiv, volatile unsigned short * ucaie, volatile unsigned short rxbuf);
void SerialProcess(void);
void HandleCommands(void);
int stoi(char* str, int len);
void performCommand(volatile char* rx_process, volatile char * tx, void (*send)(void));
void itoa(volatile unsigned long long n, volatile char s[]);
void reverse(volatile char s[]);
//#define CLEARPB0                        {clearProcessBuff(USB0_Char_Rx_Process, &pb0_index, &pb0_buffered);}
//#define CLEARPB1                        {clearProcessBuff(USB1_Char_Rx_Process, &pb1_index, &pb1_buffered);}
//#define LOADPB0                         {loadRingtoPB(&usb0_rx_wr, &usb0_rx_rd, USB0_Char_Rx_Process, USB0_Char_Rx_Ring, &pb0_index, &pb0_buffered);}
//#define LOADPB1                         {loadRingtoPB(&usb1_rx_wr, &usb1_rx_rd, USB1_Char_Rx_Process, USB1_Char_Rx_Ring, &pb1_index, &pb1_buffered);}