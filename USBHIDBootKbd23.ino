// Modified from https://github.com/felis/USB_Host_Shield_2.0/blob/master/examples/HID/USBHIDBootKbd/USBHIDBootKbd.ino
// Modified to print the ASCII hex codes of the keys pressed, and to handle some of the non-printable keys.

#include <hidboot.h>
#include <usbhub.h>
#include <Keyboard.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#define UHS_HID_BOOT_KEY_ESC 0x29
#define UHS_HID_BOOT_KEY_DEL 0x4C
#define UHS_HID_BOOT_KEY_BACKSPACE 0x2A
#define UHS_HID_BOOT_KEY_HOME 0x4A
#define UHS_HID_BOOT_KEY_PAGE_UP 0x4B
#define UHS_HID_BOOT_KEY_END 0x4D
#define UHS_HID_BOOT_KEY_PAGE_DN 0x4E
#define UHS_HID_BOOT_KEY_TAB 0x2B
#define UHS_HID_BOOT_KEY_CAPS 0x39
#define UHS_HID_BOOT_KEY_SCROLL 0x46

#define NUM_KEYPAD_NUMLOCK 0x53
#define NUM_KEYPAD_SLASH 0x54
#define NUM_KEYPAD_STAR 0x55
#define NUM_KEYPAD_MINUS 0x56
#define NUM_KEYPAD_SEVEN 0x5F
#define NUM_KEYPAD_EIGHT 0x60
#define NUM_KEYPAD_NINE 0x61
#define NUM_KEYPAD_PLUS 0x57
#define NUM_KEYPAD_FOUR 0x5C
#define NUM_KEYPAD_FIVE 0x5D
#define NUM_KEYPAD_SIX 0x5E
#define NUM_KEYPAD_BACK 0x2A
#define NUM_KEYPAD_ONE 0x59
#define NUM_KEYPAD_TWO 0x5A
#define NUM_KEYPAD_THREE 0x5B
#define NUM_KEYPAD_ENTER 0x58
#define NUM_KEYPAD_ZERO 0x62
#define NUM_KEYPAD_DOT 0x63


class KbdRptParser : public KeyboardReportParser
{
    uint32_t clickTime;
    uint32_t stopTime;

    void ExecKey(uint8_t mod, uint8_t key);
protected:
    virtual uint8_t HandleLockingKeys(USBHID* hid, uint8_t key);
    virtual void OnKeyDown(uint8_t mod, uint8_t key);
    virtual void OnKeyUp(uint8_t mod, uint8_t key);
    virtual void OnKeyPressed(uint8_t key);
};

void KbdRptParser::ExecKey(uint8_t m, uint8_t key)
{
    //Serial.print(" >");
    PrintHex<uint8_t>(key, 0x80);
    //Serial.print("< ");
    Serial.print(" -- ");
    switch (key)
    {
        case NUM_KEYPAD_NUMLOCK:
            Serial.println("NUM_KEYPAD_NUMLOCK ");
            break;
        case NUM_KEYPAD_SLASH:
            Serial.println("NUM_KEYPAD_SLASH ");
            break;
        case NUM_KEYPAD_STAR:
            Serial.println("NUM_KEYPAD_STAR ");
            break;
        case NUM_KEYPAD_MINUS:
            Serial.println("NUM_KEYPAD_MINUS ");
            break;

        case NUM_KEYPAD_SEVEN:
            Serial.println("NUM_KEYPAD_SEVEN ");
            break;
        case NUM_KEYPAD_EIGHT:
            Serial.println("NUM_KEYPAD_EIGHT ");
            break;
        case NUM_KEYPAD_NINE:
            Serial.println("NUM_KEYPAD_NINE ");
            break;
        case NUM_KEYPAD_PLUS:
            Serial.println("NUM_KEYPAD_PLUS ");
            break;

        case NUM_KEYPAD_FOUR:
            Serial.println("NUM_KEYPAD_FOUR ");
            break;
        case NUM_KEYPAD_FIVE:
            Serial.println("NUM_KEYPAD_FIVE ");
            break;
        case NUM_KEYPAD_SIX:
            Serial.println("NUM_KEYPAD_SIX ");
            break;
        case NUM_KEYPAD_BACK:
            Serial.println("NUM_KEYPAD_BACK ");
            break;

        case NUM_KEYPAD_ONE:
            Serial.println("NUM_KEYPAD_ONE ");
            break;
        case NUM_KEYPAD_TWO:
            Serial.println("NUM_KEYPAD_TWO ");
            break;
        case NUM_KEYPAD_THREE:
            Serial.println("NUM_KEYPAD_THREE ");
            break;
        case NUM_KEYPAD_ENTER:
            Serial.println("NUM_KEYPAD_ENTER ");
            break;


        case NUM_KEYPAD_ZERO:
            Serial.println("NUM_KEYPAD_ZERO ");
            break;
        case NUM_KEYPAD_DOT:
            Serial.println("NUM_KEYPAD_DOT ");
            break;
    }
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
    clickTime = millis();
    Serial.print("Down: ");
    Serial.println(clickTime, DEC);
}

uint8_t KbdRptParser::HandleLockingKeys(USBHID* hid, uint8_t key) {
    return 0;
};

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
    stopTime = millis();
    Serial.print("Up: ");
    Serial.println(stopTime, DEC);
    uint32_t diffTime = stopTime-clickTime;
    Serial.print("Diff: ");
    Serial.println(diffTime, DEC);
    if(diffTime > 500) {
        Serial.println("Long Click");
    } else if(diffTime < 150) {
        Serial.println("Short Click");
    }

    ExecKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
    Serial.print("ASCII: ");
    Serial.println(key, HEX);
};

USB Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);

uint32_t next_time;

KbdRptParser Prs;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay(200);

    next_time = millis() + 5000;
    //UsbDEBUGlvl = 0xff;

    HidKeyboard.SetReportParser(0, (HIDReportParser *)&Prs);
}

void loop()
{
    Usb.Task();
}
