// MIT License

// Copyright (c) 2024 Mikael Lund aka Wombat
// streaming additions by usersniper, 2025

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// ZSMKit wrapper for playback with LLVM-MOS C/C++

#include <cx16.h>
#include <stdint.h>
#include "zsmkit.h"

#define RAM_BANK (*(volatile unsigned char *)0x00)


// ----------------------------------------
// MAKE V1 WORK WITH THIS WHEN YOU GET BACK
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ----------------------------------------





#define ZSM_ADDR 0x8700
#define ZSM_INIT_ENGINE ZSM_ADDR + 0x00
#define ZSM_TICK ZSM_ADDR + 0x03
#define ZSM_PLAY ZSM_ADDR + 0x06
#define ZSM_STOP ZSM_ADDR + 0x09
#define ZSM_REWIND ZSM_ADDR + 0x0C
#define ZSM_CLOSE ZSM_ADDR + 0x0F
#define ZSM_FILL_BUFFERS ZSM_ADDR + 0x12
#define ZSM_SETLFS ZSM_ADDR + 0x15
#define ZSM_SETFILE ZSM_ADDR + 0x18
#define ZSM_LOADPCM ZSM_ADDR + 0x1B
#define ZSM_SETMEM ZSM_ADDR + 0x1E
#define ZSM_SETATTEN ZSM_ADDR + 0x21
#define ZSM_SETCB ZSM_ADDR + 0x24
#define ZSM_CLEARCB ZSM_ADDR + 0x27
#define ZSM_GETSTATE ZSM_ADDR + 0x2A
#define ZSM_SETRATE ZSM_ADDR + 0x2D
#define ZSM_GETRATE ZSM_ADDR + 0x30
#define ZSM_SETLOOP ZSM_ADDR + 0x33
#define ZSM_OPMATTEN ZSM_ADDR + 0x36
#define ZSM_PSGATTEN ZSM_ADDR + 0x39
#define ZSM_PCMATTEN ZSM_ADDR + 0x3C
#define ZSM_SET_INT_RATE ZSM_ADDR + 0x3F
#define ZCM_SETMEM ZSM_ADDR + 0x4B
#define ZCM_PLAY ZSM_ADDR + 0x4E
#define ZCM_STOP ZSM_ADDR + 0x51
#define ZSMKIT_SETISR ZSM_ADDR + 0x54
#define ZSMKIT_CLEARISR ZSM_ADDR + 0x57

#define xstr(s) str(s)
#define str(s) #s

// Inputs: .A = RAM bank to assign to ZSMKit
void zsm_init_engine(const uint8_t bank) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_INIT_ENGINE) "\n" ::"a"(bank)
      : "x", "y", "p");
}

// Inputs: .X = priority, .A .Y = memory location (lo hi), $00 = RAM bank
void zsm_setmem(const uint8_t priority, const uint16_t addr,
                const uint8_t bank) {
  RAM_BANK = bank;
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_SETMEM) "\n" ::"x"(priority), "a"((uint8_t)(addr & 0xFF)),
      "y"((uint8_t)((addr >> 8) & 0xFF))
      : "p");
}

// Inputs: .A = 0 (tick music data and PCM)
//         .A = 1 (tick PCM only)
//         .A = 2 (tick music data only)
void zsm_tick(const uint8_t what) {
  __attribute__((leaf)) __asm__ volatile("jsr " xstr(ZSM_TICK) "\n" ::"a"(what)
                                         : "x", "y", "p");
}

void zsm_play(const uint8_t priority) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_PLAY) "\n" ::"x"(priority)
      : "a", "y", "p");
}

void zsm_stop(const uint8_t priority) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_STOP) "\n" ::"x"(priority)
      : "a", "y", "p");
}

void zsm_rewind(const uint8_t priority) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_REWIND) "\n" ::"x"(priority)
      : "a", "y", "p");
}

void zsm_close(const uint8_t priority) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_CLOSE) "\n" ::"x"(priority)
      : "a", "y", "p");
}

// Inputs: .X = priority, .A = attenuation value
void zsm_setatten(const uint8_t priority, const uint8_t attenuation) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_SETATTEN) "\n" ::"x"(priority), "a"(attenuation)
      : "y", "p");
}

// Inputs: .X = priority, .A .Y = pointer to callback, $00 = RAM bank
void zsm_setcb(const uint8_t priority, const uint16_t callback,
               const uint8_t bank) {
  RAM_BANK = bank;
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_SETCB) "\n" ::"x"(priority), "a"((uint8_t)(callback)),
      "y"((uint8_t)(callback >> 8))
      : "p");
}

void zsm_clearcb(const uint8_t priority) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_CLEARCB) "\n" ::"x"(priority)
      : "a", "y", "p");
}

// Inputs: .X = priority
// Outputs: .C = playing, Z = not playable, .A .Y = (lo hi) loop counter
struct ZsmState zsm_getstate(const uint8_t priority) {
  struct ZsmState state;
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_GETSTATE) "\n"
                                "php\n"    // P -> stack
                                "ta%2\n"   // A -> X -> loopcnt_lo
                                "pla\n"    // P -> A
                                "pha\n"    // A(=P) -> stack
                                "lsr\n"    // extract zero
                                "and #1\n" // A = 1 if Z is set
                                "sta %3\n" // A -> r -> not_playable
                                "plp\n"    // restore P
                                "lda #0\n" // extract carry
                                "adc #0\n" // C -> A -> playing
      : "=a"(state.playing), "=y"(state.loopcnt_hi), "=x"(state.loopcnt_lo),
        "=r"(state.not_playable)
      : "x"(priority)
      : "p");
  return state;
}

// Inputs: .X = priority, .A .Y = (lo hi) new tick rate
// Outputs: none
void zsm_setrate(const uint8_t priority, const uint16_t rate) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_SETRATE) "\n" ::"x"(priority),
      "a"((uint8_t)(rate & 0xFF)), "y"((uint8_t)((rate >> 8) & 0xFF))
      : "p");
}

// Inputs: .X = priority
// Outputs: A .Y = (lo hi) tick rate
uint16_t zsm_getrate(const uint8_t priority) {
  uint8_t result_hi, result_lo;
  __attribute__((leaf)) __asm__ volatile("jsr " xstr(ZSM_GETRATE) "\n"
                                         : "=a"(result_lo), "=y"(result_hi)
                                         : "x"(priority)
                                         : "p");
  return (((uint16_t)result_hi) << 8) | result_lo;
}

// Inputs: .X = priority, .C = whether to loop
void zsm_setloop(const uint8_t priority, const bool loop) {
  __attribute__((leaf)) __asm__ volatile(
      "jsr " xstr(ZSM_SETLOOP) "\n" ::"x"(priority), "c"(loop)
      : "a", "y", "p");
}





// Inputs: .X = priority, .A .Y = memory location (lo hi), $00 = RAM bank
void zsm_setfile(const uint8_t priority, const uint16_t addr){
    __attribute__((leaf)) __asm__ volatile(
        "jsr " xstr(ZSM_SETFILE) "\n" ::
        "x"(priority),
        "a"((uint8_t)(addr & 0xFF)),
        "y"((uint8_t)((addr >> 8) & 0xFF))
        : "p"
    );
}


// Inputs: .X = priority, .A .Y = memory location (lo hi), $00 = RAM bank
// Outputs: .A .Y = next memory location after end of load, $00 = RAM bank
uint16_t zsm_loadpcm(const uint8_t priority, const uint16_t addr, const uint8_t bank){
    uint8_t address_lo, address_hi;
    RAM_BANK = bank;
    __attribute__((leaf)) __asm__(
        "jsr " xstr(ZSM_LOADPCM)"\n"
        : "=a"(address_lo), "=y"(address_hi)
        : "x"(priority), "a"((uint8_t)(addr & 0xFF)), "y"((uint8_t)((addr >> 8) & 0xFF))
        : "p"
    );
    return (address_hi << 8) + address_lo;
} 

void zsm_fill_buffers() {
    __attribute__((leaf)) __asm__ volatile (
        "jsr " xstr(ZSM_FILL_BUFFERS) "\n"
        ::: "a","x","y","p"
    );
}


void zsmkit_setisr(){
    __attribute__((leaf)) __asm__ volatile (
        "jsr " xstr(ZSMKIT_SETISR) "\n"
        ::: "a","x","y","p"
    );
}
void zsmkit_clearisr(){
    __attribute__((leaf)) __asm__ volatile (
        "jsr " xstr(ZSMKIT_CLEARISR) "\n"
        ::: "a","x","y","p"
    );
}


__attribute__((section(".zsm_section"),used)) 
    static const uint8_t zsmkitinc[] = {
        #embed "../lib/ZSMKIT-8700.BIN"
    };