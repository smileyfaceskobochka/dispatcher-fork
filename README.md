# Модель диспетчера задач операционной системы

Программная модель диспетчера задач для лабораторной работы по дисциплине "Операционные системы"

Руководство пользователя: [HTML](https://alirzaev.github.io/dispatcher/user-manual), [PDF](https://alirzaev.github.io/dispatcher/user-manual.pdf)

Загрузки: https://github.com/alirzaev/dispatcher/releases/latest

# Это форк

Форк [alirzaev/dispatcher](https://github.com/alirzaev/dispatcher) (оригинальный
репозиторий архивирован и больше не поддерживается). Цель форка - собрать проект
на современной системе: Botan 3, C++20, GCC 13+.

Алгоритмы диспетчеризации и формат файла задания **не изменены**. Изменены только
сборочные зависимости и несовместимые с новыми компиляторами части сторонних
библиотек.

Проверено на Arch Linux / CachyOS (GCC 16, CMake 4, Qt 5.15, Botan 3.13).
Windows и macOS в этом форке **не проверялись** - для них по-прежнему проще
использовать готовые сборки из [релизов оригинала](https://github.com/alirzaev/dispatcher/releases/latest).

## Что изменено

- **Botan 3 вместо 2.x.** В `qtutils/cryptography.h` переписаны вызовы, удалённые
  в Botan 3: `HashFunction::create` → `create_or_throw`,
  `get_cipher_mode(name, ENCRYPTION)` → `Cipher_Mode::create_or_throw(name, Cipher_Dir::Encryption)`,
  `process()` теперь принимает `span`, а не `std::string`.

- **`cmake/Modules/FindBotan.cmake`.** Botan 3 убрал umbrella-заголовок
  `botan/botan.h`, поэтому исходный поиск ничего не находил. Теперь ищется
  `botan/cipher_mode.h` в `botan-3`/`botan-2`.

- **C++20 вместо C++17** во всех таргетах - Botan 3 требует C++20.

- **`3rdparty/tl/optional.hpp`.** `optional<T&>::emplace()` вызывал
  несуществующий `construct()`. Код не используется (в проекте только
  `optional` со значениями), но GCC 13+ отвергает его через `-Wtemplate-body`.
  Переписано на привязку ссылки.

- **`tests/CMakeLists.txt`.** Catch2 2.12.2 считает `MINSIGSTKSZ` константой
  времени компиляции, что перестало быть верным начиная с glibc 2.34.
  Определён `CATCH_CONFIG_NO_POSIX_SIGNALS`.

## Формат файла задания не изменён

Файлы `.ejson` шифруются AES-256/CBC/PKCS7 ключом из SHA-256 от Ф. И. О.
студента, перед шифротекстом записывается 16-байтовый вектор инициализации.
Формат не изменился, поэтому файлы, сохранённые этой сборкой, читаются
исходной программой, и наоборот. Совместимость проверена расшифровкой
сторонней реализацией (openssl).

# Структура проекта

- qtutils - Библиотека со вспомогательными функциями

- schedulers - Библиотека с алгоритмами работы диспетчера задач

- tests - Тесты

- generator - Библиотека для генерации заданий

- dispatcher - Программная модель с графическим интерфейсом

- taskbuilder - Конструктор заданий

- widgets - Библиотека с UI-компонентами

# Файл задания

Структуру файла задания можно найти [здесь](docs/TASK.md)

# Сборка приложений

## Необходимые компоненты

### Windows

- CMake 3.10 или новее

- Qt 5.12 или новее

- Botan (криптографическая библиотека) 3.x

- Visual Studio 2017 или новее со следующими компонентами:

  - Разработка классических приложений на C++

  - Debugging Tools for Windows

vcpkg:

```
vcpkg install qt5-base botan
```

> В этом форке требуется Botan 3 и C++20. Если vcpkg ставит Botan 2,
> соберите Botan 3 из исходников и укажите префикс через
> `-DCMAKE_PREFIX_PATH`. Форк на Windows не проверялся.

### macOS

- Минимальная версия macOS - 10.13

- CMake 3.10 или новее

- Qt 5.14 или новее

- Botan (криптографическая библиотека) 3.x

- Xcode 10 (Command Line Tools)

Homebrew:

```sh
brew install cmake qt botan
```

> В этом форке требуется Botan 3 и C++20. Форк на macOS не проверялся.

### Linux (Arch / CachyOS)

- CMake 3.10 или новее

- Qt 5.12 или новее

- Botan (криптографическая библиотека) **3.x**

- g++ 10 или новее с поддержкой C++20

```sh
sudo pacman -S --needed base-devel cmake ninja qt5-base botan
```

### Linux (Ubuntu / Debian)

- Минимальная версия Ubuntu - 20.04

- CMake 3.10 или новее

- Qt 5.12 или новее

- Botan (криптографическая библиотека) **3.x**

- g++ 10 или новее с поддержкой C++20

```sh
sudo apt install build-essential cmake ninja-build qtbase5-dev
```

> В дистрибутивах, где в репозитории лежит только Botan 2, требуется сборка
> Botan 3 из исходников в отдельный префикс:
>
> ```sh
> git clone --depth 1 --branch 3.13.0 https://github.com/randombit/botan
> cmake -S botan -B botan/build -GNinja -DCMAKE_BUILD_TYPE=Release \
>       -DCMAKE_INSTALL_PREFIX="$HOME/.local/botan3"
> cmake --build botan/build && cmake --install botan/build
> ```
>
> ```sh
> cmake -S . -B build -GNinja -DCMAKE_PREFIX_PATH="$HOME/.local/botan3"
> ```

## Используемые сторонние библиотеки

- [Catch2 2.12.2](https://github.com/catchorg/Catch2)
- [nlohmann::json 3.7.3](https://github.com/nlohmann/json)
- [Mapbox Variant 1.1.6](https://github.com/mapbox/variant)
- [TartanLlama optional 1.0.0](https://github.com/TartanLlama/optional)

## Используемые сторонние шрифты

- [Microsoft Cascadia Code 1911.21](https://github.com/microsoft/cascadia-code)

## Сборка

Проект собирается штатными средствами Qt: либо открываем `CMkaeLists.txt` через QtCreator, либо
собираем с помощью CMake:

```sh
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH="<Qt root dir>/lib/cmake" -DDISPATCHER_DEBUG=1 ..
make
```

`DISPATCHER_DEBUG=1` - включение дополнительной отладочной информации.

Вариант с Ninja (проверенный на Linux):

```sh
cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Получаем три исполняемых файла:

- `build/dispatcher/dispatcher` - программная модель диспетчера (то, что нужно для лабораторной работы)

- `build/taskbuilder/taskbuilder` - конструктор заданий

- `build/tests/tests` - тесты алгоритмов

Тесты можно запустить отдельно:

```sh
./build/tests/tests
```

При первом запуске программа спрашивает Ф. И. О. студента. Оно используется как
ключ шифрования файлов задания, поэтому менять его после начала работы нельзя:
сохранённые с другим именем файлы не откроются.

# Сборка руководства пользователя

## Необходимые компоненты

- Asciidoctor 2.0 или новее

- Asciidoctor PDF 1.5 или новее (для сборки руководства в PDF)

- Pandoc 2.9 или новее (для сборки руководства в другие форматы)

## Сборка

```sh
cd docs/user-manual

# HTML

asciidoctor -o index.html index.adoc

# PDF

asciidoctor-pdf -a pdf-theme=theme.yml -o user-manual.pdf index.adoc

# Другие форматы (fb2, epub, docx и пр.)

asciidoctor -b docbook5 -o user-manual.docbook index.adoc
pandoc -f docbook -t <output format> -o <output file> index.docbook
```

# В случае возникновения проблем

Задания, которые генерируются самой программой, сохраняются в файл `dispatcher.json` во временной папке пользователя.
Открыть данную папку в стандартном файловом менеджере можно через меню программной модели:
"Справка" > "Устранение неполадок" > "Открыть временную папку".
При составлении [issue](https://github.com/alirzaev/dispatcher/issues), пожалуйста, прикрепите этот файл тоже.
