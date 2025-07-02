#define irq_vector (*(volatile unsigned short *)0x0314)
unsigned short* default_irq;


// Does exactly what it says.
void grab_irq_vector(){
    default_irq = irq_vector;
}

void custom_irq_handler(){
    //palette_update(0,0xfff);
    // if line irq was triggered, set new scroll values
    //if(VERA.irq_flags & VERA_IRQ_RASTER){
    //    irq_type_table[irq_type]();
    //    // toggle the flag
    //    VERA.irq_flags = VERA_IRQ_RASTER;
    //}
    //palette_update(0,background_color);
    //if(VERA.irq_flags & VERA_IRQ_VSYNC){

    blit_palette();
    __attribute__((leaf)) __asm__ volatile(
        "jmp ("xstr(default_irq)")\n"
    );

    //} else {
    //    __attribute__((leaf)) __asm__ volatile(
    //        // stack stuff
    //        "ply \n"
    //        "plx \n"
    //        "pla \n"
    //        "rti \n"
    //    );
    //}
}

void set_custom_irq(){
    // set the irq vector
    irq_vector = &custom_irq_handler; // 0x314
}

void clear_custom_irq(){
    // reset back to the default irq
    irq_vector = default_irq;
}