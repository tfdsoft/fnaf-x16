unsigned char sprite_offset;

#define SPR_8BPP 0x8000

#define SPR_HF 0x0001
#define SPR_VF 0x0002
#define SPR_Z_OFF 0x0000
#define SPR_Z_LOW 0x0004
#define SPR_Z_MID 0x0008
#define SPR_Z_HI 0x000C
#define SPR_W_8 0x0000
#define SPR_W_16 0x1000
#define SPR_W_32 0x2000
#define SPR_W_64 0x3000
#define SPR_H_8 0x0000
#define SPR_H_16 0x4000
#define SPR_H_32 0x8000
#define SPR_H_64 0xC000

#define SPR_P_0 0x0000
#define SPR_P_1 0x0100
#define SPR_P_2 0x0200
#define SPR_P_3 0x0300
#define SPR_P_4 0x0400
#define SPR_P_5 0x0500
#define SPR_P_6 0x0600
#define SPR_P_7 0x0700
#define SPR_P_8 0x0800
#define SPR_P_9 0x0900
#define SPR_P_A 0x0a00
#define SPR_P_B 0x0b00
#define SPR_P_C 0x0c00
#define SPR_P_D 0x0d00
#define SPR_P_E 0x0e00
#define SPR_P_F 0x0f00


void draw_sprite(
    unsigned short chr_address,
    unsigned short x,
    unsigned short y,
    unsigned short attr
){
    // don't wanna keep drawing if we hit the limit
    if(sprite_offset&0x80) return;

    VERA.address = 0xFC00 + (sprite_offset<<3);
    VERA.address_hi = 1|VERA_INC_1;

    VERA.data0 = low_byte(chr_address);
    VERA.data0 = high_byte(chr_address);
    VERA.data0 = low_byte(x);
    VERA.data0 = high_byte(x);
    VERA.data0 = low_byte(y);
    VERA.data0 = high_byte(y);
    VERA.data0 = low_byte(attr);
    VERA.data0 = high_byte(attr);
    sprite_offset++;
}

void clear_sprites(){
    VERA.address = 0xFC00;
    VERA.address_hi = 1|VERA_INC_1;
    VERA.data0 = 0xcd; // end of usable vram (mouse goes here)
    VERA.data0 = 0x0f;

    VERA.address = 0xFC07; // set sprite 0 to 8x8, palette 0
    VERA.data0 = 0x00;

    sprite_offset = 1; // leave space for the mouse
    VERA.address = 0xFC00 + 6 + (((unsigned short)sprite_offset)<<3); // first attr byte
    VERA.address_hi = 1|VERA_INC_8;

    // tmp variables are in zeropage so it *should* be faster
    for(unsigned char tmp = sprite_offset; tmp < 128; ++tmp){
        __attribute__((leaf)) __asm__ volatile ("stz $9f23");
    }

    //draw_sprite(0xFAE|SPR_8BPP, 224, 0, 0|SPR_Z_HI|SPR_W_32|SPR_H_32);
}





void update_timers(){
    for (char tmp = 0; tmp < sizeof(frame_timer); tmp++){
        frame_timer[tmp]++;
        if(frame_timer[tmp] > tmp) frame_timer[tmp] = 0;
    }
}