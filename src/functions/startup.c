#define NT_ADR_L0(x,y) (0x0000+(x<<1)+(y<<7))
#define NT_ADR_L1(x,y) (0x4000+(x<<1)+(y<<7))



void configure_video(void){
    waitvsync();
    vera_layer_enable(0b00);
    vera_sprites_enable(0);
    

    VERA.control = (0 << 1); // DCSEL = 0
    VERA.display.hscale = 64;
    VERA.display.vscale = 64;
    VERA.display.border = 0x10;

    VERA.control = (1 << 1); // DCSEL = 1
    VERA.display.hstart = (0 >> 2);
    VERA.display.hstop = (636 >> 2);
    VERA.display.vstart = (56 >> 1);
    VERA.display.vstop = (424 >> 1);

    VERA.control = 0;



    VERA.layer0.config  = 0b10010010; // 64 wide(*2), 128 high // 16kb
    /*                      ||||||||
                            ||||||++-- Color Depth (0:1bpp, 1:2bpp, 2:4bpp, 3:8bpp)
                            ||||||
                            |||||+---- Bitmap Mode (0: off)
                            |||||
                            ||||+----- 256-color tile mode (1: enabled)
                            ||||
                            ||++------ Map Width (32 << x) tiles
                            ||
                            ++---------Map Height (32 << x) tiles
    */
    #define layer0_width (32<<((VERA.layer0.config&0b00110000)>>4))
    #define layer0_height (32<<((VERA.layer0.config&0b11000000)>>6))
    layer_size = ((layer0_width << 1)*layer0_height);

    VERA.layer0.mapbase = 0b10000000; // 0x10000, second bank of VRAM
    /*                      ||||||||
                            ++++++++-- Map Base Address (16:9)
    */

    VERA.layer0.tilebase= 0b00000000; // 0x00000, first bank of VRAM
    /*                      ||||||||
                            |||||||+-- Tile Width (0:8px, 1:16px)
                            |||||||
                            ||||||+--- Tile Height (0:8px, 1:16px)
                            ||||||
                            ++++++---- Tile Base Address (16:11)
    */



    VERA.layer1.config  = 0b01010010; // 64 wide(*2), 64 tall // 8kb
    /*                      ||||||||
                            ||||||++-- Color Depth (0:1bpp, 1:2bpp, 2:4bpp, 3:8bpp)
                            ||||||
                            |||||+---- Bitmap Mode (0: off)
                            |||||
                            ||||+----- 256-color tile mode (1: enabled)
                            ||||
                            ||++------ Map Width (32 << x) tiles
                            ||
                            ++---------Map Height (32 << x) tiles
    */
   #define layer1_width (32<<((VERA.layer1.config&0b00110000)>>4))
   #define layer1_height (32<<((VERA.layer1.config&0b11000000)>>6))
    layer_size += ((layer1_width << 1)*layer1_height);
    VERA.layer1.mapbase = 0b10100000; // 0x14000
    /*                      ||||||||
                            ++++++++-- Map Base Address (16:9)
    */

    VERA.layer1.tilebase= 0b01000000; // 0x08000
    /*                      ||||||||
                            |||||||+-- Tile Width (0:8px, 1:16px)
                            |||||||
                            ||||||+--- Tile Height (0:8px, 1:16px)
                            ||||||
                            ++++++---- Tile Base Address (16:11)
    */
    VERA.layer1.hscroll = 0x0000;
    VERA.layer1.vscroll = 0x0000;

    
    

    
    VERA.address = 0x0000;
    VERA.address_hi = 0|VERA_INC_1;

    
    // completely clear vram
    cx16_k_memory_fill(&VERA.data0,65535,0);
    VERA.data0 = 0;
    //cx16_k_memory_fill(&VERA.data0,layer_size,0);
    cx16_k_memory_fill(&VERA.data0,0xf9c0,0);
    
    
    
    waitvsync();
}