void palette_update_now(
    const unsigned char index, 
    const short color){

    // add index twice since each color is two bytes

    VERA.address = 0xFA00+(index<<1);
    VERA.address_hi = 1|VERA_INC_1;
    VERA.data0 = low_byte(color);
    VERA.data0 = high_byte(color);
}

void palette_update_set_now(
    unsigned char pal, 
    const short * set){

    pal &= 0x0f;
    // add index twice since each color is two bytes

    VERA.address = 0xFA00+(pal<<5);
    VERA.address_hi = 1|VERA_INC_1;
    for(char i = 0; i < 16; i++){
        VERA.data0 = low_byte(set[i]);
        VERA.data0 = high_byte(set[i]);
    }
}

unsigned short lerp_color(
    const short col1, 
    const short col2, 
    const unsigned char t){
        
    // ok so basically take two RGB444 colors and find a color in-between (using shift)
    //#define r1 tmp1 // -+
    //#define g1 tmp2 //  |
    //#define b1 tmp3 //  +-- these are zeropage, which is faster
    //#define r2 tmp4 //  |
    //#define g2 tmp5 //  |
    //#define b2 tmp6 // -+
    unsigned char r1,g1,b1,r2,g2,b2;
    
    unsigned char time,r_lerp,g_lerp,b_lerp;

    time = t&0x0f;
    if(time==0)return col1;
    if(time==0x0f)return col2;

    r1 = high_byte(col1)&0x0f; 
    g1 = low_byte(col1)>>4;
    b1 = low_byte(col1)&0x0f;

    r2 = high_byte(col2); 
    g2 = low_byte(col2)>>4;
    b2 = low_byte(col2)&0x0f;

    r_lerp = ( (r1*(0x0f-time)) + (r2*time) )>>4;
    g_lerp = ( (g1*(0x0f-time)) + (g2*time) )>>4;
    b_lerp = ( (b1*(0x0f-time)) + (b2*time) )>>4;

    return (r_lerp<<8) | (g_lerp<<4) | (b_lerp);
}