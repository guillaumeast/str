# 🚀 str

[![C++](https://img.shields.io/badge/language-C++17-blue)](https://en.cppreference.com/)
[![Build](https://img.shields.io/badge/build-clang++-orange)](https://clang.llvm.org/)
[![Terminal CLI](https://img.shields.io/badge/type-CLI-lightgrey)](https://en.wikipedia.org/wiki/Command-line_interface)
[![Platform: Unix](https://img.shields.io/badge/platform-Unix-darkgreen)](https://en.wikipedia.org/wiki/Unix)
[![Status: v0.2](https://img.shields.io/badge/status-v0.2-green)](#️️️️changelog)
[![License: MIT](https://img.shields.io/badge/license-MIT-white)](./LICENSE)

> 🧠 **`str`** is a `terminal`-friendly, `unicode`-`ansi`-aware `string` manipulation `CLI` — minimal like `cut` or `sed`, but `C++`-powered, `ANSI`/`escape`-safe, and fully `Unicode`-aware (including `wide` and `combined` emojis).

---

## ✨ Features

- `UTF-8` aware (`libunistring`)
- `ANSI`/`escape`-safe (clean, pad, align)
- Column renderer (`str row`)
- `Visual width` handling (emojis, CJK, etc.)
- `POSIX` pipe & `shell`-friendly
- Minimal dependencies
- Fast, portable, single-binary

---

## 🚀 Commands available

| Command             | Description                            |
|---------------------|----------------------------------------|
| `str width <string>`         | Get visual `width` of a string           |
| `str height <string>`        | Get number of `lines` (`\n` count + 1)                   |
| `str clean <string>`         | Remove `ANSI escape sequences`           |
| `str split  <string> <sep>`   | Split a `string` using a `separator`       |
| `str repeat <n> <string> [separator]` | Repeat a `string` with optional `separator` |
| `str row <separator> <col1> <col2> ...` | Render multiple `multiline strings` into one `row` |

> `Display width` is measured in `columns`, of `characters` or `strings`, when output to a device that uses `non-proportional fonts`.

> Note that for some rarely used characters the actual `fonts` or `terminal emulators` can use a different `width`. There is no mechanism for communicating the `display width` of `characters` across a Unix pseudo-terminal (`tty`).
> Also, there are scripts with complex rendering, like the `Indic` scripts. For these scripts, there is no such concept as `non-proportional fonts`. Therefore the results of these functions usually work fine on most scripts and on most `characters` but can fail to represent the actual `display width`.

> 📚 See [libunistring documentation – Display width <uniwidth.h>](https://www.gnu.org/software/libunistring/manual/libunistring.html#uniwidth_002eh) for details on how display width is computed.

---

## 🛠️ Build and Install

```sh
make           # compile to build/str
make install   # copy binary to ~/.local/bin/str
make uninstall # remove binary from ~/.local/bin
```

Your binary will be available as `str` anywhere in your shell if `~/.local/bin` is in your `$PATH`.

---

## 🧪 Examples

<details>
    <summary>str width</summary>
    
```bash
red="\033[31m"                    # ANSI escape code for red
reset="\033[0m"                   # ANSI escape code to reset colors

str width "1234"                 # output → "4"
str width "1🛑4"                 # output → "4"
str width "${red}1${reset}🛑4"   # output → "4"
```

</details>

<details>
    <summary>str height</summary>
    
```sh
str height "1\n2\n3"             # output → "3"
```

</details>

<details>
    <summary>str clean</summary>
    
```sh
red="\033[31m"                    # ANSI escape code for red
reset="\033[0m"                   # ANSI escape code to reset colors

str clean "${red}Red${reset}"   # output → "Red"
```

</details>

<details>
    <summary>str split</summary>
    
```sh
str split "a-b-c" "-"            # output → "a\nb\nc"
```

</details>

<details>
    <summary>str repeat</summary>
    
```sh
str repeat 3 "foo" "-"           # output → "foo-foo-foo"
```

</details>

<details>
    <summary>str row</summary>

```sh
# Ansi colors:
red="\033[31m"                    # ANSI escape code for red
reset="\033[0m"                   # ANSI escape code to reset colors

# and wild combined emojis glyphes
regular_emoji="✓"                # width → 1 (1 byte)
wide_emoji="🛑"                  # width → 2 (2 bytes)
combined_emoji="⚠️"              # width → 1 (n bytes)

# Are complex to format in pure sh POSIX scripts
str row "|" "1${wide_emoji}4\n1234" "1${combined_emoji} 4\n1234" "1${red}23${reset}4" "1234\n1234"

# But are easy to render instantly in C++ ⚡️
# -----------Expected output-----------
1🛑4|1⚠️4|1234|1234            # First "23" should render in red color but I can't show it in this `md` file
1234|1234|1234|1234
```

</details>

---

## 📁 Project structure

```
str/
├── include/           # public headers
│   ├── Text.hpp
│   ├── Column.hpp
│   └── types.hpp
├── src/               # C++ source files
│   ├── main.cpp
│   ├── Text.cpp
│   └── Column.cpp
├── build/             # build output
├── Makefile
├── LICENSE
└── README.md
```

---

## 📦 Dependencies

- [`libunistring`](https://www.gnu.org/software/libunistring/)
- C++17 compiler (`clang++` or `g++`)

---

## 📋 Changelog

### [v0.2] – 2024-05-14

- 🏗 Migrated full codebase from C to modern C++
- ✨ Added `Text`, `Column`, and `Row` classes
- ✨ New CLI commands: `clean`, `width`, `height`, `split`, `repeat`, `row`
- ✅ Unicode and ANSI-aware padding and width calculation
- 📦 Structure: `src/`, `include/`, `build/`, `Makefile`

---

## 🧑 Author

Made by [@guillaumeast](https://github.com/guillaumeast) with ❤️

MIT License
