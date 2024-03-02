#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN  PB4

// Define the number of LEDs
#define LED_COUNT 1


struct RGBColor
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

RGBColor m_ledColor = {0, 0, 0};

void update() {
// Thanks to TinyNeoPixel for this code
#ifdef HELIOS_EMBEDDED
  // Assuming m_ledColor is a struct or class holding the RGB values.
  volatile uint8_t
    *ptr = (volatile uint8_t *)&m_ledColor, // Pointer directly to the m_ledColor variable.
    b = *ptr++, // First byte of the RGB structure.
    hi,         // PORT value with output bit set high.
    lo;         // PORT value with output bit set low.

  volatile uint16_t
    i = LED_COUNT * sizeof(RGBColor); // Loop counter
  // Define the port and pinMask for the LED pin
  volatile uint8_t* port = &PORTB; // Example for ATtiny85, adjust if using a different port
  uint8_t pinMask = (1 << LED_PIN); // Adjust PB14 to your specific pin
  // Setup the high and low values for the port.
  hi = *port | pinMask;
  lo = *port & ~pinMask;

 // 8 MHz(ish) AVRxt ---------------------------------------------------------
  #if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)
    // 8MHz specific timing.
    volatile uint8_t n1, n2 = 0;
    n1 = lo;
    if (b & 0x80) n1 = hi;

    // Dirty trick: RJMPs proceeding to the next instruction are used
    // to delay two clock cycles in one instruction word (rather than
    // using two NOPs).  This was necessary in order to squeeze the
    // loop down to exactly 64 words -- the maximum possible for a
    // relative branch.

    asm volatile(
     "_head8:"                    "\n\t" // Clk  Pseudocode
      // Bit 7:
      "st   %a[port], %[hi]"      "\n\t" // 1    PORT = hi
      "mov  %[n2]   , %[lo]"      "\n\t" // 1    n2   = lo
      "st   %a[port], %[n1]"      "\n\t" // 1    PORT = n1
      "rjmp .+0"                  "\n\t" // 2    nop nop
      "sbrc %[byte] , 6"          "\n\t" // 1-2  if (b & 0x40)
       "mov %[n2]   , %[hi]"      "\n\t" // 0-1   n2 = hi
      "st   %a[port], %[lo]"      "\n\t" // 1    PORT = lo
      "rjmp .+0"                  "\n\t" // 2    nop nop
      // Bit 6:
      "st   %a[port], %[hi]"      "\n\t" // 1    PORT = hi
      "mov  %[n1]   , %[lo]"      "\n\t" // 1    n1   = lo
      "st   %a[port], %[n2]"      "\n\t" // 1    PORT = n2
      "rjmp .+0"                  "\n\t" // 2    nop nop
      "sbrc %[byte] , 5"          "\n\t" // 1-2  if (b & 0x20)
       "mov %[n1]   , %[hi]"      "\n\t" // 0-1   n1 = hi
      "st   %a[port], %[lo]"      "\n\t" // 1    PORT = lo
      "rjmp .+0"                  "\n\t" // 2    nop nop
      // Bit 5:
      "st   %a[port], %[hi]"      "\n\t" // 1    PORT = hi
      "mov  %[n2]   , %[lo]"      "\n\t" // 1    n2   = lo
      "st   %a[port], %[n1]"      "\n\t" // 1    PORT = n1
      "rjmp .+0"                  "\n\t" // 2    nop nop
      "sbrc %[byte] , 4"          "\n\t" // 1-2  if (b & 0x10)
       "mov %[n2]   , %[hi]"      "\n\t" // 0-1   n2 = hi
      "st   %a[port], %[lo]"      "\n\t" // 1    PORT = lo
      "rjmp .+0"                  "\n\t" // 2    nop nop
      // Bit 4:
      "st   %a[port], %[hi]"      "\n\t" // 1    PORT = hi
      "mov  %[n1]   , %[lo]"      "\n\t" // 1    n1   = lo
      "st   %a[port], %[n2]"      "\n\t" // 1    PORT = n2
      "rjmp .+0"                  "\n\t" // 2    nop nop
      "sbrc %[byte] , 3"          "\n\t" // 1-2  if (b & 0x08)
       "mov %[n1]   , %[hi]"      "\n\t" // 0-1   n1 = hi
      "st   %a[port], %[lo]"      "\n\t" // 1    PORT = lo
      "rjmp .+0"                  "\n\t" // 2    nop nop
      // Bit 3:
      "st   %a[port], %[hi]"      "\n\t" // 1    PORT = hi
      "mov  %[n2]   , %[lo]"      "\n\t" // 1    n2   = lo
      "st   %a[port], %[n1]"      "\n\t" // 1    PORT = n1
      "rjmp .+0"                  "\n\t" // 2    nop nop
      "sbrc %[byte] , 2"          "\n\t" // 1-2  if (b & 0x04)
       "mov %[n2]   , %[hi]"      "\n\t" // 0-1   n2 = hi
      "st   %a[port], %[lo]"      "\n\t" // 1    PORT = lo
      "rjmp .+0"                  "\n\t" // 2    nop nop
      // Bit 2:
      "st   %a[port], %[hi]"      "\n\t" // 1    PORT = hi
      "mov  %[n1]   , %[lo]"      "\n\t" // 1    n1   = lo
      "st   %a[port], %[n2]"      "\n\t" // 1    PORT = n2
      "rjmp .+0"                  "\n\t" // 2    nop nop
      "sbrc %[byte] , 1"          "\n\t" // 1-2  if (b & 0x02)
       "mov %[n1]   , %[hi]"      "\n\t" // 0-1   n1 = hi
      "st   %a[port], %[lo]"      "\n\t" // 1    PORT = lo
      "rjmp .+0"                  "\n\t" // 2    nop nop
      // Bit 1:
      "st   %a[port], %[hi]"      "\n\t" // 1    PORT = hi
      "mov  %[n2]   , %[lo]"      "\n\t" // 1    n2   = lo
      "st   %a[port], %[n1]"      "\n\t" // 1    PORT = n1
      "rjmp .+0"                  "\n\t" // 2    nop nop
      "sbrc %[byte] , 0"          "\n\t" // 1-2  if (b & 0x01)
       "mov %[n2]   , %[hi]"      "\n\t" // 0-1   n2 = hi
      "st   %a[port], %[lo]"      "\n\t" // 1    PORT = lo
      "sbiw %[count], 1"          "\n\t" // 2    i-- (don't act on Z flag yet)
      // Bit 0:
      "st   %a[port], %[hi]"      "\n\t" // 1    PORT = hi
      "mov  %[n1]   , %[lo]"      "\n\t" // 1    n1   = lo
      "st   %a[port], %[n2]"      "\n\t" // 1    PORT = n2
      "ld   %[byte] , %a[ptr]+"   "\n\t" // 2    b = *ptr++
      "sbrc %[byte] , 7"          "\n\t" // 1-2  if (b & 0x80)
       "mov %[n1]   , %[hi]"      "\n\t" // 0-1   n1 = hi
      "st   %a[port], %[lo]"      "\n\t" // 1    PORT = lo
      "brne _head8"               "\n"   // 2    while(i) (Z flag set above)
    : [ptr]   "+e" (ptr),
      [byte]  "+r" (b),
      [n1]    "+r" (n1),
      [n2]    "+r" (n2),
      [count] "+w" (i)
    : [port]   "e" (port),
      [hi]     "r" (hi),
      [lo]     "r" (lo));

// 16 MHz(ish) AVRxt ------------------------------------------------------
#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)

      // 16MHz specific timing.
    volatile uint8_t next, bit;
    next = lo;
    bit = 8;

    asm volatile(
     "_head16:"                   "\n\t" // Clk  Pseudocode    (T =  0)
      "st   %a[port],  %[hi]"    "\n\t" // 1    PORT = hi     (T =  1)
      "nop"                      "\n\t" // 1    nop           (T =  2)
      "sbrc %[byte],  7"         "\n\t" // 1-2  if (b & 128)
       "mov  %[next], %[hi]"     "\n\t" // 0-1   next = hi    (T =  4)
      "dec  %[bit]"              "\n\t" // 1    bit--         (T =  5)
      "st   %a[port],  %[next]"  "\n\t" // 1    PORT = next   (T =  6)
      "nop"                      "\n\t" // 1    nop           (T =  7)
      "mov  %[next] ,  %[lo]"    "\n\t" // 1    next = lo     (T =  8)
      "breq _nextbyte16"          "\n\t" // 1-2  if (bit == 0) (from dec above)
      "rol  %[byte]"             "\n\t" // 1    b <<= 1       (T = 10)
      "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 12)
      "nop"                      "\n\t" // 1    nop           (T = 13)
      "st   %a[port],  %[lo]"    "\n\t" // 1    PORT = lo     (T = 14)
      "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 16)
      "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 18)
      "rjmp _head16"              "\n\t" // 2    -> _head16 (next bit out) (T=20)
     "_nextbyte16:"               "\n\t" //                    (T = 10)
      "ldi  %[bit]  ,  8"        "\n\t" // 1    bit = 8       (T = 11)
      "ld   %[byte] ,  %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 13)
      "st   %a[port], %[lo]"     "\n\t" // 1    PORT = lo     (T = 14)
      "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 16)
      "sbiw %[count], 1"         "\n\t" // 2    i--           (T = 18)
       "brne _head16"             "\n"   // 2    if (i != 0) -> _head16 (bit 0 of next byte.) (T=20)
    : [ptr]   "+e" (ptr),
      [byte]  "+r" (b),
      [bit]   "+d" (bit),
      [next]  "+r" (next),
      [count] "+w" (i)
    : [port]   "e" (port),
      [hi]     "r" (hi),
      [lo]     "r" (lo));

  #else
    #error "CPU SPEED NOT SUPPORTED"
  #endif
  // END AVR ----------------------------------------------------------------
#endif
}
int main(void) {
  DDRB |= (1 << LED_PIN);

  while (1) {
    m_ledColor = {255, 0, 0};
    update(); // Should turn the LED off
    _delay_ms(5);
    m_ledColor = {0, 0, 0};
    update(); // Should turn the LED off
    _delay_ms(8);
    m_ledColor = {0, 255, 0};
    update(); // Should turn the LED off
    _delay_ms(5);
    m_ledColor = {0, 0, 0};
    update(); // Should turn the LED off
    _delay_ms(8);
    m_ledColor = {0, 0, 255};
    update(); // Should turn the LED off
    _delay_ms(5);
    m_ledColor = {0, 0, 0};
    update(); // Should turn the LED off
    _delay_ms(8);
 }
}

