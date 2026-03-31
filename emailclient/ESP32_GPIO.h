#ifndef _ESP32_GPIO_H
#define _ESP32_GPIO_H

// More info on the ESP32 is here:
// https://www.espboards.dev/esp32/esp32doit-devkit-v1/

// No boot sequence involvement
// No USB or JTAG conflicts
// No flash/PSRAM connections
// Freely assignable without issues
namespace SafePinsToUse
{
	enum GPIO
	{
		GPIO_18 = 0x12,
		GPIO_19 = 0x13,
		GPIO_21 = 0x15,
		GPIO_22 = 0x16,
		GPIO_23 = 0x17,
		GPIO_25 = 0x19,
		GPIO_26 = 0x1A,
		GPIO_27 = 0x1B,
		GPIO_32 = 0x20,
		GPIO_33 = 0x21
	};
}

namespace UseWithCautionPins
{
	enum GPIO
	{
		GPIO_01 = 0x01, // Connected to on-board USB-UART for uploading and logs; drives serial output at boot, 
						// so using as GPIO can disrupt programming or console
		GPIO_02 = 0x02, // If driven HIGH on reset (while IO0 is LOW), selects an unsupported SDIO boot mode, 
						// causing boot failure.
		GPIO_03 = 0x03, // Used for receiving data from USB-UART (programming); also pulled HIGH at boot for 
						// console communication, so using as GPIO can disrupt uploads.
		GPIO_04 = 0x04, // Sampled at reset for boot config; should not be driven at boot (affects boot mode timing).
		GPIO_05 = 0x05, // Must be HIGH during boot; if pulled LOW at reset, alters SDIO slave timing and may prevent normal boot.
		GPIO_12 = 0x0C, // Keep LOW during boot (internal PD); pulling HIGH at reset selects 1.8V flash mode, 
						// causing flash brownout if 3.3V flash is used.
		GPIO_13 = 0x0D, // Used for JTAG debugging (TCK); avoid using as GPIO if JTAG is needed.
		GPIO_14 = 0x0E, // Used for JTAG debugging (TMS); driving it as GPIO may interfere with 
						// JTAG or produce spurious signals at boot.
		GPIO_15 = 0x0F, // Keep HIGH during boot (internal PU); if LOW on reset, 
						// bootloader log is silenced and boot mode may change.
		GPIO_16 = 0x10, // Connected to internal PSRAM on PSRAM-enabled modules; not usable as GPIO on those modules.
		GPIO_17 = 0x11, // Connected to internal PSRAM on PSRAM-enabled modules; not usable as GPIO on those modules.
		GPIO_34 = 0x22, // Cannot be used as output (no drive capability); only suitable for analog/digital input.
		GPIO_35 = 0x23, // Cannot be used as output; only suitable for input.
		GPIO_36 = 0x24, // Cannot be used as output; only suitable for input (e.g., analog read).
		GPIO_39 = 0x27  // Cannot be used as output; only suitable for input.
	};
}

constexpr uint8_t GPIO_40{0x28};

#endif