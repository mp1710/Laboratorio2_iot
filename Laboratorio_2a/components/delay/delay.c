#include "delay.h"
#include "esp_rom_sys.h"

// Esta funcion crea un retardo en milisegundos utilizando 
// la función esp_rom_delay_us() que se encuentra en el archivo 
// esp_rom_sys.h, no retorna nada
void delay_ms(uint32_t ms)
{
    // 1 ms = 1000 us
    esp_rom_delay_us(ms * 1000);
}

// Esta funcion crea un retardo en segundos utilizando la misma 
// funcion anterior, solo que cambiando las unidades de tiempo, no retorna nada.
void delay_s(uint32_t s)
{
    // 1 s = 1,000,000 us
    esp_rom_delay_us(s * 1000000);
}
