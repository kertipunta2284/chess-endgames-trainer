# Chess Endgame Trainer ♟️

[English](#english) | [Русский](#русский)

---

## English

A random chess position generator focused on endgame training. This program helps you sharpen your endgame skills by creating random, legal positions with a specific configuration of pieces provided in text format.

### 🚀 Features
* Generates random endgames (King and pawns, Rook endgames, Bishop endgames, etc.).
* Guarantees position legality (kings are not under attack out of turn, no pawns on the 1st or 8th ranks).
* Outputs positions to the console using text graphics and exports them in FEN format.
* Automatically generates a ready-to-use Lichess URL to open the exact FEN position in the board editor.

### ⚙️ Position Configuration
The program searches for any `.txt` file in its folder to read the piece configuration. 

* **If a `.txt` file is found**: The program parses it automatically.
* **If no `.txt` file is found**: The program will prompt you to enter the configuration manually in the terminal.

#### Format Rules:
* Use standard FEN piece letters: white pieces are **UPPERCASE** (`R`, `N`, `B`, `Q`, `P`), black pieces are **lowercase** (`r`, `n`, `b`, `q`, `p`).
* **Do not include kings** — they are generated automatically.
* **No spaces inside the piece set!** Write all pieces together as a single word. 
* Use exactly **one space** at the end to separate the piece set from the active color (`w` for White, `b` for Black).

*Examples of valid text files/inputs:*
```text
Rp w
BN w
QQQQqqqq b
```

### 🛠️ Requirements & Compilation
The code is cross-platform. So, whatever, Skyler White. 

To use the program, you just need to compile and link two source files: `main.c` and `funcs.c`. Quick guide for compiling the project on Linux/macOS:
```bash
gcc main.c funcs.c -o main.out
```

### 💻 Usage
Linux/macOS:
```bash
./main.out
```

### 📝 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Русский

Генератор случайных шахматных позиций, ориентированный на тренировку эндшпиля. Программа помогает оттачивать навыки разыгрывания окончаний, создавая случайные легальные позиции с заданной конфигурацией фигур в текстовом формате.

### 🚀 Возможности
* Генерация случайных эндшпилей (король и пешки, ладейные окончания, слоновые окончания и т. д.).
* Гарантия легальности позиций (король не находится под шахом, если сейчас не его ход; нет пешек на 1-й и 8-й горизонталях).
* Вывод позиций в консоль с помощью текстовой графики и экспорт в формат FEN.
* Автоматическая генерация готовой ссылки для Lichess, которая сразу открывает созданную FEN-позицию в редакторе доски.

### ⚙️ Конфигурация позиций
Программа ищет любой файл с расширением `.txt` в своей папке, чтобы считать настройки фигур.

* **Если `.txt` файл найден**: Программа автоматически распознает его и берет данные оттуда.
* **Если файла нет**: Программа запросит ввод конфигурации прямо в терминале.

#### Правила формата:
* Используйте стандартные буквы фигур из FEN-нотации: белые фигуры — **ЗАГЛАВНЫЕ** (`R`, `N`, `B`, `Q`, `P`), черные — **строчные** (`r`, `n`, `b`, `q`, `p`).
* **Королей указывать не нужно** — они всегда добавляются на доску автоматически.
* **Никаких пробелов внутри набора фигур!** Пишите все фигуры слитно, одним словом.
* Используйте ровно **один пробел** в конце, чтобы отделить набор фигур от очереди хода: `w` (белые) или `b` (черные).

*Примеры корректных файлов/ввода:*
```text
Rp w
BN w
QQQQqqqq b
```

### 🛠️ Требования и компиляция
Код кроссплатформенный. Код кроссплатформенный. Альцгеймер.

Чтобы запустить программу, вам нужно просто скомпилировать и объединить два файла: `main.c` и `funcs.c`. Быстрый гайд по сборке для Linux/macOS:
```bash
gcc main.c funcs.c -o main.out
```

### 💻 Запуск
Linux/macOS:
```bash
./main.out
```

### 📝 Лицензия
Этот проект распространяется под лицензией MIT — подробности см. в файле [LICENSE](LICENSE).

