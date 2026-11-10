# CHIP-8 Emulator Core 👾

> Núcleo emulador do clássico sistema virtual **CHIP-8** escrito em **C** utilizando **SDL3**.

Este projeto funciona tanto como um emulador standalone via linha de comando quanto como um núcleo compatível com o launcher **[Central Emulators](https://github.com/jvgusmao06/central-emulators)**.

---

## 🎮 Controles (Teclado CHIP-8)

O teclado hexadecimal clássico de 16 teclas do CHIP-8 é remapeado no padrão moderno de PC:

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
- Pressione **ESC** para encerrar o emulador a qualquer momento.

---

## 🚀 Como Executar

O emulador não embute ROMs nem binários proprietários no repositório; o caminho da ROM deve ser fornecido via argumento CLI.

### 1. Integrado com o Central Emulators (Recomendado)
Abra a interface do **Central Emulators**, navegue até a biblioteca do CHIP-8, selecione sua pasta de ROMs e clique em **Jogar** no título desejado. O Hub chamará o emulador automaticamente passando o caminho do arquivo selecionado.

### 2. Linha de Comando (CLI Standalone)
Você pode executar o emulador compilado passando o caminho de qualquer arquivo de ROM `.ch8`:

```bash
# No Windows
./exe/chip8.exe "C:/Caminho/Para/SeuJogo.ch8"

# No Linux
./exe/chip8 "/home/usuario/roms/SeuJogo.ch8"
```

Se nenhum argumento for passado, o executável exibirá a sintaxe correta de uso no terminal:
```text
Uso: ./chip8 <caminho_da_rom.ch8>
Dica: Use o launcher 'Central Emulators' para escolher e iniciar suas ROMs.
```

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
├── .github/workflows/ # Automação de CI/CD para compilar releases no GitHub
├── include/          # Cabeçalhos C (.h)
│   └── chip8.h
├── src/              # Código-fonte da CPU e da Interface SDL3
│   ├── chip8.c
│   └── main.c
├── Makefile          # Script de automação de compilação
├── .gitignore        # Ignora binários locais (exe/) e ROMs
└── README.md
```

> **Nota:** Arquivos compilados em `exe/` e arquivos de jogos em `roms/` são mantidos fora do controle de versão por boas práticas e distribuídos via [GitHub Releases](https://github.com/jvgusmao06/Chip-8/releases).
