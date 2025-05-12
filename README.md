
# 🚧 str 🚧 (not ready for production)

`C` programm to easily handle `utf8` strings:
- `str width <string>` → Returns visual width of `string` (number of terminal `columns` really used to display it)
- `str height <string>` → Returns `string` lines count (⚠️ not related to terminal display)
- `str clean <string>` → Returns only visible chars of `string` (removes all `ANSI escape codes`)
- `str split <string> <separator>` → Returns each splitted `part` into a new line
- `str repeat <count> <string> <opt:separator>` → Returns a merged `string`
- `str row <strings> <opt:separator>` → Merges N multiline strings row‑by‑row, inserting `separator` between columns

🚧 Work in progress 🚧
    - `str box --size <opt:width:-fit> <opt:height:-fit> <opt:align:-top_left> --color <opt:border_color:-NONE> -- <string>`
    - `str position <top|mid|bot> <fly|tiret|arrow> <string>`
    - `str results <int:passed> <int:fallback> <int:failed> <opt:accepted_fail_count>`

