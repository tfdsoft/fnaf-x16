#include "include.h"

unsigned char framecount;
unsigned char selection;

void change_selection(unsigned char select){
    if(selection != select){
        selection = select;
        zsm_rewind(1);
        zsm_play(1);
    }
}



void state_menu(){
    load_sfx("BLIP3",1);
    load_sfx("STATIC2",2);
    
    
    palette_update_set_now(2, title_palette);

    // freddy
    load_from_sd(0,"MENU4BPP.BIN",0x0000,2);
    hiram_load(2,"MENU.RLE",1);
    vram_unrle(2,NT_ADR_L0(0,0),1|VERA_INC_1);

    load_from_sd(0,"STATIC4BPP.BIN",0x8000,2);
    hiram_load(2,"STATIC.RLE",1);
    vram_unrle(1,NT_ADR_L1(0,0),1|VERA_INC_1);

    // title graphic
    load_sprite_from_sd(1,"FNAF",0); // 128 chr slots
    load_sprite_from_sd(1,"FNAF2",128); // 16 chr slots


    wavkit_setfile("DARKNESS.WAV");
    wavkit_setrate(PCM_8000HZ,0,0);
    wavkit_setloop(1);
    wavkit_setvol(12);
    wavkit_play();


    zsm_play(2);
    waitvsync();
    vera_layer_enable(0b11);
    vera_sprites_enable(1);
    while (1){
        waitvsync();
        palette_update_now(0x10,0x777);
        clear_sprites();
        poll_controller();
        

        update_timers();

        if((frame_timer[1])==0){
            vera_layer_enable(0b11);
            low_byte(VERA.layer1.hscroll) = rand8();
            high_byte(VERA.layer1.hscroll) = rand8();
            low_byte(VERA.layer1.vscroll) = (rand8()&7);
        } else {
            vera_layer_enable(0b01);
        }
        
        if((frame_timer[7])==0){
            switch(rand8()&0x1f){
                default:
                    high_byte(VERA.layer0.vscroll) = 0;
                    break;
                case 1:
                    high_byte(VERA.layer0.vscroll) = 1;
                    break;
                case 2:
                    high_byte(VERA.layer0.vscroll) = 2;
                    break;
                case 3:
                    high_byte(VERA.layer0.vscroll) = 3;
                    break;
            }
        }

        draw_sprite(chr_slot(0), 48, 16, SPR_P_1|SPR_Z_MID|SPR_W_64|SPR_H_64); // Five Nights at Freddy's X16
        draw_sprite(chr_slot(64), 48, 96, SPR_P_1|SPR_Z_MID|SPR_W_64|SPR_H_16); // New Game
        draw_sprite(chr_slot(80), 48, 116, SPR_P_1|SPR_Z_MID|SPR_W_64|SPR_H_16); // Continue
        draw_sprite(chr_slot(96), 48, 136, SPR_P_1|SPR_Z_MID|SPR_W_64|SPR_H_16); // 6th Nigh
        draw_sprite(chr_slot(128), 112, 136, SPR_P_1|SPR_Z_MID|SPR_W_16|SPR_H_16); // t
        draw_sprite(chr_slot(112), 48, 156, SPR_P_1|SPR_Z_MID|SPR_W_64|SPR_H_16); // Custom N
        draw_sprite(chr_slot(132), 112, 156, SPR_P_1|SPR_Z_MID|SPR_W_16|SPR_H_16); // ig
        draw_sprite(chr_slot(136), 128, 156, SPR_P_1|SPR_Z_MID|SPR_W_16|SPR_H_16); // ht
        

        if(mouse_in_window(48,96,64,16)){
            change_selection(0);
        }
        if(mouse_in_window(48,116,64,16)){
            change_selection(1);
        }
        if(mouse_in_window(48,136,64,16) ){
            change_selection(2);
        }
        if(mouse_in_window(48,156,64,16)){
            change_selection(3);
        }

        draw_sprite(chr_slot(140), 24, (96+(20*selection)), SPR_P_1|SPR_Z_MID|SPR_W_16|SPR_H_16); // >>



        wavkit_tick();
        zsm_fill_buffers();

        palette_update_now(0x10,0x000);


        RAM_BANK = SAVE_FILE_BANK;
        night += 1;
        myass99sixtyfour += 1;
    }
}




void state_game(){

    load_sfx("FAN",1);

    // office
    draw_image_from_sd(0,1,"OFFICE",NT_ADR_L0(0,0),51);
    load_sprite_from_sd(8,"FAN",0);

    // enable everything
    waitvsync();
    vera_layer_enable(0b01);
    vera_sprites_enable(1);

    while (1){
        waitvsync();
        clear_sprites();
        poll_controller();
        VERA.layer0.hscroll = Camera.xmid;

        update_timers();
        draw_sprite(chr_slot(((frame_timer[5]>>1)<<5)),(202-Camera.xmid),79,SPR_P_8|SPR_Z_MID|SPR_W_32|SPR_H_64);
        

        zsm_fill_buffers();


        framecount += 1;
        if(pad_b_new) {
            __attribute__((leaf)) __asm__ volatile(
                "stz $01\n"
                "loop:\n"
                "jsr $ffe4\n" // GETIN
                "bne loop\n"
                "sei\n"
                "jmp ($fffa)\n" // NMI
            ); 
        }


        // mouse in window stuff
        if(mouse_in_window(0,0,32,255)) Camera.xmid -= 2;
        if(mouse_in_window(32,0,32,255)) Camera.xmid -= 1;
        if(mouse_in_window((320-64),0,32,255)) Camera.xmid += 1;
        if(mouse_in_window((320-32),0,32,255)) Camera.xmid += 2;

        if(high_byte(Camera.xmid) > 0) Camera.xmid = 0;
        if(low_byte(Camera.xmid) > 80) Camera.xmid = 80;



        // freddy nose
        if(mouse_left_new && mouse_in_window((172-low_byte(Camera.xmid)),60,3,3)){
            load_sfx("PARTYFAVOR",2);
        }

        //VERA.layer0.hscroll += 1;
        
    }
}




int main(void){
    configure_video();
    cx16_k_mouse_config(-1,40,23);

    // change directories to the assets folder
    cbm_k_setnam("CD:ASSETS");
    cbm_k_setlfs(15,8,15);
    cbm_k_open();
    cbm_k_close(15);    

    // init music
    zsm_init_engine(ZSMKIT_BANK);
    zsmkit_setisr();

    wavkit_init_engine();
    
    while (1){
        switch (gamestate){
            case 0: state_menu(); 
        }
    }
    
    
}