unsigned char rand8(){
    volatile unsigned char rand;
    unsigned long seed = cx16_k_entropy_get();
    rand ^= low_byte(seed);
    rand ^= high_byte(seed);
    //rand ^= third_byte(seed);
    return rand;
}