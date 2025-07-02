void hiram_load(char header, const char* fileptr, const unsigned char bank){
    cbm_k_setnam(fileptr);
    cbm_k_setlfs(0,8,header);
    RAM_BANK = bank;
    cbm_k_load(0,0xa000);
}

void load_from_sd(char header, const char* fileptr, const unsigned short addr, const unsigned char page){
    cbm_k_setnam(fileptr);
    cbm_k_setlfs(0,8,header);
    cbm_k_load(page,addr);
}

void vram_unrle(
    const unsigned char palette, 
    const unsigned short address, 
    const unsigned char page){

    unsigned short rle_value;
    unsigned char rle_run;
    unsigned char * rle_ptr, unrle_ptr;

    VERA.address = address;
    VERA.address_hi = page;

    rle_ptr=BANK_RAM;
    low_byte(rle_value) = rle_ptr[0]; 
    high_byte(rle_value) = (rle_ptr[1] & 0x0f) + (palette<<4);
    rle_run = (rle_ptr[1] >> 4); 
    
    rle_ptr += 2;
    do {
        while(rle_run > 0) {
            VERA.data0 = low_byte(rle_value);
            VERA.data0 = high_byte(rle_value);
            rle_run--;
        }
        //wait();
        low_byte(rle_value) = rle_ptr[0]; 
        high_byte(rle_value) = (rle_ptr[1] & 0x0f) + (palette<<4);
        rle_run = rle_ptr[1] >> 4; 
        rle_ptr += 2;
    } while(rle_run > 0);
    return;
}

void load_sprite_from_sd(const char palette, const char * filename, const unsigned char chr){

    // load graphics
    strcpy(str,filename);
    strcat(str,".BIN");
    load_from_sd(0,str,(chr_slot(chr)<<5),3);

    // load palette
    strcpy(str,filename);
    strcat(str,"-PALETTE.BIN");
    load_from_sd(0,str,(((void*)0x600) + (palette<<5)),0);
}

void draw_image_from_sd(const bool layer, const char palette, const char * filename, const unsigned short mapaddr, const unsigned char width){
    unsigned char i = 0, j = 0, tmp;

    // load tiles
    strcpy(str,filename);
    strcat(str,"T.DAT");
    load_from_sd(2,str,(0x0000 + (layer<<15)),2);

    // load map
    strcpy(str,filename);
    strcat(str,"M.DAT");
    cbm_k_setnam(str);
    cbm_k_setlfs(9,8,2);
    cbm_k_open();
    cbm_k_chkin(9);
    VERA.address = mapaddr;
    VERA.address_hi = 1|VERA_INC_1;
    while (!(cbm_k_readst() & 0b01000000)){ // while not end of file
        VERA.data0 = cbm_k_acptr();
        tmp = cbm_k_acptr();
        VERA.data0 = (tmp + (palette << 4));
        i++;
        if (i >= width){
            i=0;
            j++;
            VERA.address = (mapaddr + (j << 7));
            VERA.address_hi = 1|VERA_INC_1;
        }
        
    }
    cbm_k_close(9);

    // load palette
    strcpy(str,filename);
    strcat(str,"P.DAT");
    load_from_sd(2,str,(0x600 + (palette<<5)),0);
}

void load_song(const char* fileptr){
    char str[33];
    strcpy(str, fileptr);
    strcat(str, ".ZSM");

    RAM_BANK = ZSMKIT_BANK;
    zsm_close(0);

    //zsm_setmem(0, BANK_RAM, ZSM_RAM_BANK_0);
    zsm_setfile(0, str);
    zsm_loadpcm(0, 0xa000, ZSM_PCM_BANK_MUSIC);
    //zsm_play(0);

    //zsm_setatten(0,0);
}
void load_sfx(const char* fileptr, const char prio){
    char str[33];
    strcpy(str, fileptr);
    strcat(str, ".SFX.ZSM");

    RAM_BANK = ZSMKIT_BANK;
    zsm_close(prio);

    //zsm_setmem(1, BANK_RAM, ZSM_RAM_BANK_0);
    zsm_setfile(prio, str);
    zsm_loadpcm(prio, 0xa000, ZSM_PCM_BANK_SFX);
    //zsm_play(prio);
}