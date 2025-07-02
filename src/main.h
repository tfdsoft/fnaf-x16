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
unsigned char gamestate=1;

unsigned char night=1;

//unsigned short h;

/*
__attribute__((section(".hiram"))) struct {
    unsigned char header[4];
    
} save;
*/
struct __save {
    char header[4];
    unsigned char night;
};
#define save (*(struct __save *)0x400)



    
//__attribute__((memory("ram")));