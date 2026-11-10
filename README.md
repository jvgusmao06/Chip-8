# CHIP-8 Emulator 👾

> Emulador do clássico sistema virtual **CHIP-8** escrito em **C** utilizando **SDL3**.

Este projeto funciona tanto como um emulador independente (*standalone*) quanto como um núcleo compatível com o launcher **[Central Emulators](https://github.com/jvgusmao06/central-emulators)**.

---

## 🎮 Controles (Teclado CHIP-8)

O teclado hexadecimal clássico de 16 teclas do CHIP-8 é remapeado no padrão moderno:

```
Teclado Original:           Teclado PC (QWERTY):
┌───┬───┬───┬───┐          ┌───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ C │          │ 1 │ 2 │ 3 │ 4 │
├───┼───┼───┼───┤          ├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ D │   --->   │ Q │ W │ E │ R │
├───┼───┼───┼───┤          ├───┼───┼───┼───┤
│ 7 │ 8 │ 9 │ E │          │ A │ S │ D │ F │
├───┼───┼───┼───┤          ├───┼───┼───┼───┤
│ A │ 0 │ B │ F │          │ Z │ X │ C │ V │
└───┴───┴───┴───┘          └───┴───┴───┴───┘
```
- Pressione **ESC** para sair do emulador a qualquer momento.

---

## 🚀 Como Executar

### 1. Integrado com o Central Emulators (Recomendado)
Abra a interface do **Central Emulators**, acesse a biblioteca do CHIP-8 e clique em **Jogar** na ROM desejada. O launcher chamará o emulador automaticamente com o caminho da ROM.

### 2. Linha de Comando (CLI)
Você pode passar o caminho de qualquer arquivo `.ch8` diretamente via terminal:

```bash
# No Windows
./exe/chip8.exe "roms/Pong.ch8"

# No Linux
./exe/chip8 "roms/Pong.ch8"
```

*(Caso execute sem parâmetros, o emulador tentará abrir a ROM padrão `roms/Pong.ch8` automaticamente)*.

---

## 🔨 Compilação

### Requisitos
- Compilador C (GCC, Clang ou MinGW para Windows).
- Biblioteca de desenvolvimento **SDL3** (`libsdl3-dev`).

### Compilando via Makefile
```bash
make
```

### Compilando Manualmente via GCC
```bash
# Linux
gcc -Wall -Wextra -O2 src/main.c src/chip8.c -Iinclude -lSDL3 -o exe/chip8

# Windows (MinGW)
x86_64-w64-mingw32-gcc -Wall -Wextra -O2 src/main.c src/chip8.c -Iinclude -lSDL3 -o exe/chip8.exe
```

---

## 📁 Estrutura do Repositório

```
Chip-8/
├── .github/workflows/ # Automação de CI/CD para compilar releases
├── exe/              # Binários compilados e distribuídos
│   └── chip8.exe
├── include/          # Cabeçalhos C (.h)
│   └── chip8.h
├── roms/             # ROMs de demonstração para testes
│   ├── Figures.ch8
│   └── Pong.ch8
├── src/              # Código-fonte da CPU e da Interface SDL3
│   ├── chip8.c
│   └── main.c
├── Makefile          # Script de automação de compilação
└── README.md
```
