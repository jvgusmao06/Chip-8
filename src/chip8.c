#include "../include/chip8.h"
#include <stdio.h>
#include <stdlib.h>

#define INI_DATA 0x200

void chip8_init(chip8 *c, memory *m)
{
    // Clears V registers
    for (int i = 0; i < 16; i++)
        c->V[i] = 0;

    // Clears display
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 32; j++)
            c->display[i][j] = 0;
    }

    // Clears display
    for (int i = 80; i < 4096; i++)
        m->data[i] = 0;

    c->I = 0;
    c->SP = 0;

    uint8_t chip8_fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Loads memory with the fontset
    for (int i = 0; i < 80; i++)
    {
        m->data[i] = chip8_fontset[i];
    }

    // Loads PC with the inicial instruction address
    c->PC = INI_DATA;

    c->delay_timer = 0;
    c->sound_timer = 0;

    for(int i = 0; i < 16; i++){
        c->key[i] = 0;
    }
}

void chip_cycle(chip8 *c, memory *m)
{
    // Loads opcode with the instructions that is divided between PC and PC + 1
    uint16_t opcode = (m->data[c->PC] << 8) | m->data[c->PC + 1];

    // Increments PC
    c->PC += 2;

    // Presets most of the data
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t nn = (opcode & 0x00FF);
    uint16_t nnn = (opcode & 0x0FFF);

    // Compares opcode with all the known instructions
    switch (opcode & 0xF000)
    {
    case 0x000:
    { // 0xxx
        switch (opcode & 0x00FF)
        {
        case 0x00E0:
        { // 0xE0
            for (int i = 0; i < 64; i++)
            {
                for (int j = 0; j < 32; j++)
                {
                    c->display[i][j] = 0;
                }
            }
        }
        break;

        case 0x00EE:
        { // 0xEE
            c->SP--;
            c->PC = c->stack[c->SP];
        }
        break;
        }
    }
    break;

    case 0x1000:
    {
        c->PC = nnn;
    }
    break;

    case 0x2000:
    {
        c->stack[c->SP] = c->PC;
        c->SP++;
        c->PC = nnn;
    }
    break;

    case 0x3000:
    {
        if (c->V[x] == nn)
        {
            c->PC += 2;
        }
    }
    break;

    case 0x4000:
    {
        if (c->V[x] != nn)
        {
            c->PC += 2;
        }
    }
    break;

    case 0x5000:
    {
        if (c->V[x] == c->V[y])
        {
            c->PC += 2;
        }
    }
    break;

    case 0x6000:
    {
        c->V[x] = nn;
    }
    break;

    case 0x7000:
    {
        c->V[x] += nn;
    }
    break;

    case 0x8000:
    {
        switch (opcode & 0x000F)
        {
        case 0x0000:
        {
            c->V[x] = c->V[y];
        }
        break;

        case 0x0001:
        {
            c->V[x] |= c->V[y];
        }
        break;

        case 0x0002:
        {
            c->V[x] &= c->V[y];
        }
        break;

        case 0x0003:
        {
            c->V[x] ^= c->V[y];
        }
        break;

        case 0x0004:
        {
            uint16_t soma = c->V[x] + c->V[y];
            if (soma > 255)
            {
                c->V[15] = 1;
            }
            else
            {
                c->V[15] = 0;
            }
            c->V[x] = soma;
        }
        break;

        case 0x0005:
        {
            uint8_t vf = (c->V[y] <= c->V[x]) ? 1 : 0;
            c->V[x] -= c->V[y];
            c->V[15] = vf;
        }
        break;

        case 0x0006:
        {
            c->V[15] = c->V[y] & 0x01;
            c->V[x] = c->V[y] >> 1;
        }
        break;

        case 0x0007:
        {
            if (c->V[y] > c->V[x])
            {
                c->V[15] = 1;
            }
            else
            {
                c->V[15] = 0;
            }
            c->V[x] = c->V[y] - c->V[x];
        }
        break;

        case 0x000E:
        {
            c->V[15] = (c->V[y] >> 7) & 0x01;
            c->V[x] = c->V[y] << 1;
        }
        break;
        }
    }
    break;

    case 0x9000:
    {
        if (c->V[x] != c->V[y])
        {
            c->PC += 2;
        }
    }
    break;

    case 0xA000:
    {
        c->I = nnn;
    }
    break;

    case 0xB000:
    {
        c->PC = nnn + c->V[0];
    }
    break;

    case 0xC000:
    {
        uint8_t numero_aleatorio = rand() % 256;
        c->V[x] = numero_aleatorio & nn;
    }
    break;

    case 0xD000:
    {
        uint8_t x_pos = c->V[x] % 64;
        uint8_t y_pos = c->V[y] % 32;
        uint8_t altura = (opcode & 0x000F); // Quantas linhas o desenho tem

        c->V[15] = 0; // Reset da colisão

        for (int linha = 0; linha < altura; linha++)
        {
            if (y_pos + linha >= 32)
                break;

            // Pega o byte da memória (cada byte é uma linha de 8 pixels)
            uint8_t sprite_byte = m->data[c->I + linha];

            for (int coluna = 0; coluna < 8; coluna++)
            {
                if (x_pos + coluna >= 64)
                    break;

                // Se o bit atual do byte for 1...
                if ((sprite_byte & (0x80 >> coluna)) != 0)
                {

                    // Calcula a posição final garantindo que não saia da matriz
                    int final_x = (x_pos + coluna) % 64;
                    int final_y = (y_pos + linha) % 32;

                    // Checa colisão: se o pixel já era 1, VF vira 1
                    if (c->display[final_x][final_y] == 1)
                    {
                        c->V[0xF] = 1;
                    }

                    // Inverte o pixel (XOR)
                    c->display[final_x][final_y] ^= 1;
                }
            }
        }
    }
    break;

    case 0xE000:
    {
        switch (opcode & 0x00FF)
        {
        case 0x009E:
        {
            if (c->key[c->V[x]] == 1)
            {
                c->PC += 2;
            }
        }
        break;

        case 0x00A1:
        {
            if (c->key[c->V[x]] == 0)
            {
                c->PC += 2;
            }
        }
        break;
        }
    }
    break;

    case 0xF000:
    {
        switch (opcode & 0x00FF)
        {
        case 0x0007:
        {
            c->V[x] = c->delay_timer;
        }
        break;

        case 0x000A:
        {
            int pressed = 0;
            for (int i = 0; i < 16; i++)
            {
                if (c->key[i] != 0)
                {
                    c->V[x] = i;
                    pressed = 1;
                    break;
                }
            }
            if (!pressed)
            {
                c->PC -= 2;
            }
        }
        break;

        case 0x0015:
        {
            c->delay_timer = c->V[x];
        }
        break;

        case 0x0018:
        {
            c->sound_timer = c->V[x];
        }
        break;

        case 0x001E:
        {
            c->I += c->V[x];
            if (c->I > 0x0FFF)
            {
                c->V[0xF] = 1;
            }
            else
            {
                c->V[0xF] = 0;
            }
        }
        break;

        case 0x0029:
        {
            c->I = c->V[x] * 5;
        }
        break;

        case 0x0033:
        {
            m->data[c->I] = c->V[x] / 100;
            m->data[c->I + 1] = (c->V[x] / 10) % 10;
            m->data[c->I + 2] = c->V[x] % 10;
        }
        break;

        case 0x0055:
        {
            for (int i = 0; i <= x; i++)
            {
                m->data[c->I + i] = c->V[i];
            }

            c->I += x + 1;
        }
        break;

        case 0x0065:
        {
            for (int i = 0; i <= x; i++)
            {
                c->V[i] = m->data[c->I + i];
            }
            c->I += x + 1;
        }
        break;
        }
    }
    break;
    }
}