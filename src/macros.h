#define fourth_byte(a) *((unsigned char*)&a+3)
#define third_byte(a) *((unsigned char*)&a+2)
#define high_byte(a) *((unsigned char*)&a+1)
#define low_byte(a) *((unsigned char*)&a)
// for getting or modifying just 1 byte of a short

#define high_word(a) *((unsigned short*)&a+1)
#define low_word(a) *((unsigned short*)&a)
// for getting or modifying just 1 word of a long

//#define mid_word(a) ((unsigned short)(high_byte(a)+(third_byte(a)<<8)))


//#define fast_sub(a,b) (mid_word(a)-mid_word(b))

//#define short_no_subpixel(a) *((unsigned short*)&a)
// unsigned long, but only the middle 2 bytes ()

#undef RAM_BANK
#define RAM_BANK (*(volatile unsigned char *)0x00)
#undef BANK_RAM
#define BANK_RAM ((unsigned char *)0xA000)


//#define METATILE_CORNER ((unsigned short *)0xA000)

//#define NT_ADR_L0(x,y) (0x000+(y<<6)+(x<<1))
//#define NT_ADR_L1(x,y) (0x800+(y<<6)+(x<<1))


#define xstr(s) str(s)
#define str(s) #s

struct __r {
    unsigned short r0;
    unsigned short r1;
    unsigned short r2;
    unsigned short r3;
    unsigned short r4;
    unsigned short r5;
    unsigned short r6;
    unsigned short r7;
    unsigned short r8;
    unsigned short r9;
    unsigned short r10;
    unsigned short r11;
    unsigned short r12;
    unsigned short r13;
    unsigned short r14;
    unsigned short r15;
};
#define R (*(volatile struct __r *)0x02)







// stolen from tfdsoft/famidash/LIB/headers/nesdash.s
// written by alexmush
// holy fuck i am a genius
#define do_if_flag_common(func, opcode) do { \
__asm__("j" opcode " %s", __LINE__); \
do func while(0); \
    __asm__("%s:", __LINE__); \
} while(0)

#define do_if_c_set(func) do_if_flag_common(func, "cc")
#define do_if_c_clr(func) do_if_flag_common(func, "cs")
#define do_if_z_set(func) do_if_flag_common(func, "ne")
#define do_if_z_clr(func) do_if_flag_common(func, "eq")
#define do_if_v_set(func) do_if_flag_common(func, "vc")
#define do_if_v_clr(func) do_if_flag_common(func, "vs")
#define do_if_n_set(func) do_if_flag_common(func, "pl")
#define do_if_n_clr(func) do_if_flag_common(func, "mi")

// aliases
#define do_if_equal(func) do_if_z_set(func)
#define do_if_zero(func) do_if_z_set(func)
#define do_if_not_equal(func) do_if_z_clr(func)
#define do_if_not_zero(func) do_if_z_clr(func)
#define do_if_carry(func) do_if_c_set(func)
#define do_if_borrow(func) do_if_c_clr(func)
#define do_if_negative(func) do_if_n_set(func)
#define do_if_bit7_set(func) do_if_n_set(func)
#define do_if_positive(func) do_if_n_clr(func) 
#define do_if_bit7_clr(func) do_if_n_clr(func)