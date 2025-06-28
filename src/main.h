char str[256];


struct __zp Camera {
    unsigned long x;
    union {
        unsigned char xlow;
        unsigned short xmid;
        unsigned char xhigh;
    };
    unsigned long y;
    union {
        unsigned char low;
        unsigned short mid;
        unsigned char high;
    };
} Camera;


unsigned short layer_size;
unsigned char frame_timer[8];
#define chr_slot(a) (((0x10000 + layer_size)>>5) + a)




// main program control variable
unsigned char gamestate;


//unsigned short h;


__attribute__((section(".hiram"))) 
    unsigned char s_header_0, s_header_1, s_header_2, s_header_3, s_night;
//__attribute__((memory("ram")));