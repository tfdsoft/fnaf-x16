void savefile_generate(){
    RAM_BANK = SAVE_FILE_BANK;
    strcpy(save.header, "SAV");
    save.night = 1;

    cbm_k_setnam("@:SAVEFILE");
    cbm_k_setlfs(8,8,2);
    cx16_k_bsave((void*)0x400,(void*)0x800);
}

unsigned char savefile_peek(unsigned char* val){
    unsigned char oldbank = RAM_BANK;
    RAM_BANK = SAVE_FILE_BANK;
    unsigned char value = val;
    RAM_BANK = oldbank;
    return value;
}
/*
unsigned char savefile_poke(unsigned char* val, unsigned char byte){
    unsigned char oldbank = RAM_BANK;
    RAM_BANK = SAVE_FILE_BANK;
    low_byte(val);
    RAM_BANK = oldbank;
}*/