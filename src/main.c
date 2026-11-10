#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../include/chip8.h"

#define PIXEL_SIZE 10
#define GAME_W (64 * PIXEL_SIZE)
#define GAME_H (32 * PIXEL_SIZE)

bool load_rom(memory *m, const char *path)
{
    if (!path)
    {
        printf("Caminho da ROM não fornecido!\n");
        return false;
    }

    FILE *file = fopen(path, "rb");
    if (file)
    {
        // Lê os dados a partir de 0x200 (início padrão da memória de programas CHIP-8)
        size_t bytes_read = fread(&m->data[0x200], 1, 4096 - 0x200, file);
        printf("ROM carregada com sucesso: %s (%zu bytes)\n", path, bytes_read);
        fclose(file);
        return true;
    }
    else
    {
        printf("Erro ao abrir a ROM: %s\n", path);
        return false;
    }
}

int main(int argc, char *argv[])
{
    const char *rom_path = NULL;

    // Se o emulador foi chamado pelo Hub ou terminal com argumento de ROM
    if (argc > 1)
    {
        rom_path = argv[1];
    }
    else
    {
        // Fallback para execução standalone sem argumentos
        printf("Nenhuma ROM passada via argumento. Tentando roms/Pong.ch8...\n");
        rom_path = "roms/Pong.ch8";
    }

    // 1. Inicializa o SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_Log("Erro ao inicializar SDL: %s", SDL_GetError());
        return -1;
    }

    // 2. Cria a Janela
    char title[256];
    snprintf(title, sizeof(title), "CHIP-8 Emulator - %s", rom_path);

    SDL_Window *window = SDL_CreateWindow(title, GAME_W, GAME_H, 0);
    if (!window)
    {
        SDL_Log("Erro ao criar janela: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Centraliza a janela no monitor principal (índice 0)
    SDL_SetWindowPosition(window,
                          SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0));

    // 3. Cria o Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_Log("Erro ao criar renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // 4. Inicializa o estado do CHIP-8
    chip8 c;
    memory m;
    chip8_init(&c, &m);

    // Carrega a ROM
    if (!load_rom(&m, rom_path))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Aviso",
                                 "Não foi possível abrir a ROM informada. Verifique o caminho.", window);
    }

    bool quit = false;
    SDL_Event e;
    uint64_t last_timer_time = SDL_GetTicks();

    // Main Loop
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                quit = true;

            if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP)
            {
                uint8_t state = (e.type == SDL_EVENT_KEY_DOWN) ? 1 : 0;
                switch (e.key.key)
                {
                case SDLK_1: c.key[0x1] = state; break;
                case SDLK_2: c.key[0x2] = state; break;
                case SDLK_3: c.key[0x3] = state; break;
                case SDLK_4: c.key[0xC] = state; break;
                case SDLK_Q: c.key[0x4] = state; break;
                case SDLK_W: c.key[0x5] = state; break;
                case SDLK_E: c.key[0x6] = state; break;
                case SDLK_R: c.key[0xD] = state; break;
                case SDLK_A: c.key[0x7] = state; break;
                case SDLK_S: c.key[0x8] = state; break;
                case SDLK_D: c.key[0x9] = state; break;
                case SDLK_F: c.key[0xE] = state; break;
                case SDLK_Z: c.key[0xA] = state; break;
                case SDLK_X: c.key[0x0] = state; break;
                case SDLK_C: c.key[0xB] = state; break;
                case SDLK_V: c.key[0xF] = state; break;
                case SDLK_ESCAPE: quit = true; break;
                }
            }
        }

        uint64_t current_time = SDL_GetTicks();
        if (current_time - last_timer_time >= 16) // ~60 Hz
        {
            // 1. Executa a CPU (8 a 10 ciclos por frame)
            for (int i = 0; i < 10; i++)
            {
                chip_cycle(&c, &m);
            }

            // 2. Atualiza Timers
            if (c.delay_timer > 0)
                c.delay_timer--;
            if (c.sound_timer > 0)
                c.sound_timer--;

            // 3. Renderiza o display
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for (int y = 0; y < 32; y++)
            {
                for (int x = 0; x < 64; x++)
                {
                    if (c.display[x][y])
                    {
                        SDL_FRect rect = {
                            (float)(x * PIXEL_SIZE),
                            (float)(y * PIXEL_SIZE),
                            (float)PIXEL_SIZE,
                            (float)PIXEL_SIZE
                        };
                        SDL_RenderFillRect(renderer, &rect);
                    }
                }
            }
            SDL_RenderPresent(renderer);

            last_timer_time = current_time;
        }

        SDL_Delay(1); // Evita consumo excessivo de CPU
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}