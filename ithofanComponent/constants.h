#pragma once

#include <stdint.h>

/*	Type of transfers */
#define CC1101_WRITE_BURST						0x40
#define CC1101_READ_SINGLE						0x80
#define CC1101_READ_BURST						0xC0

/*	Type of register */
#define CC1101_CONFIG_REGISTER					CC1101_READ_SINGLE
#define CC1101_STATUS_REGISTER					CC1101_READ_BURST

/*	PATABLE & FIFO's */
#define CC1101_PATABLE							0x3E	// PATABLE address
#define CC1101_TXFIFO							0x3F	// TX FIFO address
#define CC1101_RXFIFO							0x3F	// RX FIFO address
#define CC1101_PA_LowPower						0x60
#define CC1101_PA_LongDistance					0xC0

enum class CommandStrobe : uint8_t {
    sres = 0x30, // Reset CC1101 chip
    sfstxon = 0x31, // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1). If in RX (with CCA): Go to a wait state where only the synthesizer is running (for quick RX / TX turnaround).
    sxoff = 0x32, // Turn off crystal oscillator
    scal = 0x33, // Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without setting manual calibration mode (MCSM0.FS_AUTOCAL=0)
    srx = 0x34, // Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1
    stx = 0x35, // In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled: Only go to TX if channel is clear
    sidle = 0x36, // Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
    swor = 0x38, // Start automatic RX polling sequence (Wake-on-Radio) as described in Section 19.5 if WORCTRL.RC_PD=0
    spwd = 0x39, // Enter power down mode when CSn goes high
    sfrx = 0x3A, // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
    sftx = 0x3B, // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
    sworrst = 0x3C, // Reset real time clock to Event1 value
    snop = 0x3D, // No operation. May be used to get access to the chip status byte
};

enum class ConfigRegister : uint8_t {
    iocfg2 = 0x00, // GDO2 Output Pin Configuration
    iocfg1 = 0x01, // GDO1 Output Pin Configuration
    iocfg0 = 0x02, // GDO0 Output Pin Configuration
    fifothr = 0x03, // RX FIFO and TX FIFO Thresholds
    sync1 = 0x04, // Sync Word, High Byte
    sync0 = 0x05, // Sync Word, Low Byte
    pktlen = 0x06, // Packet Length
    pktctrl1 = 0x07, // Packet Automation Control
    pktctrl0 = 0x08, // Packet Automation Control
    addr = 0x09, // Device Address
    channr = 0x0A, // Channel Number
    fsctrl1 = 0x0B, // Frequency Synthesizer Control
    fsctrl0 = 0x0C, // Frequency Synthesizer Control
    freq2 = 0x0D, // Frequency Control Word, High Byte
    freq1 = 0x0E, // Frequency Control Word, Middle Byte
    freq0 = 0x0F, // Frequency Control Word, Low Byte
    mdmcfg4 = 0x10, // Modem Configuration
    mdmcfg3 = 0x11, // Modem Configuration
    mdmcfg2 = 0x12, // Modem Configuration
    mdmcfg1 = 0x13, // Modem Configuration
    mdmcfg0 = 0x14, // Modem Configuration
    deviatn = 0x15, // Modem Deviation Setting
    mcsm2 = 0x16, // Main Radio Control State Machine Configuration
    mcsm1 = 0x17, // Main Radio Control State Machine Configuration
    mcsm0 = 0x18, // Main Radio Control State Machine Configuration
    foccfg = 0x19, // Frequency Offset Compensation Configuration
    bscfg = 0x1A, // Bit Synchronization Configuration
    agcctrl2 = 0x1B, // AGC Control
    agcctrl1 = 0x1C, // AGC Control
    agcctrl0 = 0x1D, // AGC Control
    worevt1 = 0x1E, // High Byte Event0 Timeout
    worevt0 = 0x1F, // Low Byte Event0 Timeout
    worctrl = 0x20, // Wake On Radio Control
    frend1 = 0x21, // Front End RX Configuration
    frend0 = 0x22, // Front End TX Configuration
    fscal3 = 0x23, // Frequency Synthesizer Calibration
    fscal2 = 0x24, // Frequency Synthesizer Calibration
    fscal1 = 0x25, // Frequency Synthesizer Calibration
    fscal0 = 0x26, // Frequency Synthesizer Calibration
    rcctrl1 = 0x27, // RC Oscillator Configuration
    rcctrl0 = 0x28, // RC Oscillator Configuration
    fstest = 0x29, // Frequency Synthesizer Calibration Control
    ptest = 0x2A, // Production Test
    agctest = 0x2B, // AGC Test
    test2 = 0x2C, // Various Test Settings
    test1 = 0x2D, // Various Test Settings
    test0 = 0x2E, // Various Test Settings
};

enum class StatusRegister : uint8_t {
    partnum = 0x30, // Chip ID
    version = 0x31, // Chip ID
    freqest = 0x32, // Frequency Offset Estimate from Demodulator
    lqi = 0x33, // Demodulator Estimate for Link Quality
    rssi = 0x34, // Received Signal Strength Indication
    marcstate = 0x35, // Main Radio Control State Machine State
    wortime1 = 0x36, // High Byte of WOR Time
    wortime0 = 0x37, // Low Byte of WOR Time
    pktstatus = 0x38, // Current GDOx Status and Packet Status
    vco_vc_dac = 0x39, // Current Setting from PLL Calibration Module
    txbytes = 0x3A, // Underflow and Number of Bytes
    rxbytes = 0x3B, // Overflow and Number of Bytes
    rcctrl1_status = 0x3C, // Last RC Oscillator Calibration Result
    rcctrl0_status = 0x3D // Last RC Oscillator Calibration Result
};

/* Bit fields in the chip status byte */
#define CC1101_STATUS_CHIP_RDYn_BM              0x80
#define CC1101_STATUS_STATE_BM                  0x70
#define CC1101_STATUS_FIFO_BYTES_AVAILABLE_BM   0x0F

/* Masks to retrieve status bit */
#define CC1101_BITS_TX_FIFO_UNDERFLOW			0x80
#define CC1101_BITS_RX_BYTES_IN_FIFO			0x7F
#define CC1101_BITS_MARCSTATE					0x1F

enum class MarcState : uint8_t {
	sleep = 0x00,
	idle = 0x01,
	xoff = 0x02,
	vcoon_mc = 0x03,
	regon_mc = 0x04,
	mancal = 0x05,
	vcoon = 0x06,
	regon = 0x07,
	startcal = 0x08,
	bwboost = 0x09,
	fs_lock = 0x0A,
	ifadcon = 0x0B,
	endcal = 0x0C,
	rx = 0x0D,
	rx_end = 0x0E,
	rx_rst = 0x0F,
	txrx_switch = 0x10,
	rxfifo_overflow = 0x11,
	fstxon = 0x12,
	tx = 0x13,
	tx_end = 0x14,
	rxtx_switch = 0x15,
	txfifo_underflow = 0x16
};

enum class ChipState : uint8_t {
	mask = 0x70,
	idle = 0x00,
	rx = 0x10,
	tx = 0x20,
	fstxon = 0x30,
	calibrate = 0x40,
	settling = 0x50,
	rx_overflow = 0x60,
	tx_underflow = 0x70	
};
