# Changelog

## [v0.2] - 2024-05-14

### 🏗 Refactor
- Migration complète de `str` de C vers C++
- Création des classes `Text`, `Column`, `Row`

### ✨ Features
- Support natif des strings UTF-8 (libunistring)
- Calcul de largeur visible (emoji, ANSI, etc.)
- Nettoyage ANSI (`str clean`)
- Découpage (`str split`)
- Répétition (`str repeat`)
- Affichage tabulaire (`str row`)
- Alignement automatique avec padding visuel

### 🛠 Build
- Structure propre `src/`, `include/`, `build/`
- Compilation vers `build/str`
- Installation POSIX-friendly dans `~/.local/bin`

