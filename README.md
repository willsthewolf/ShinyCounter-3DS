# ShinyCounter 3DS
---
A simple **on-screen encounter counter** for shiny hunting on a modded 3DS. It's a `.3gx` plugin for Luma3DS that draws the count on the bottom screen while you play, so you don't need to count on your phone.

### Features
- Counter shown on the bottom screen while you play
- Add / subtract with button combos
- Saved to the SD card on every change, so you keep it after resets and power-offs
- Set the value by hand or reset it to 0 from the plugin menu

### Controls
| Combo | Action |
|---|---|
| **L + Up** | +1 |
| **L + Down** | −1 |
| **L + Left** | Show / hide counter |
| **Select** | Open plugin menu (set value in hexadecimal, reset to 0, enable/disable counter) |

*The plugin menu is currently in Spanish.*

### Requirements
- A 3DS with **Luma3DS v13 or newer** (the plugin loader is built in)
- Works with cartridges and digital games

### Installation
1. Download `ShinyCounter.3gx` from [Releases](../../releases).
2. Copy it to your SD card at `luma/plugins/<TitleID>/`
   (or as `luma/plugins/default.3gx` to load it in every game). If you don't have a pluggins/titleID folder you can just create it inside luma.
3. Open the Rosalina menu (**L + Down + Select**), set **Plugin Loader** to **Enabled** and choose **Save settings**.
4. Start the game. The screen flashes blue when the plugin loads.

| Game | Title ID |
|---|---|
| Pokémon X | `0004000000055D00` |
| Pokémon Y | `0004000000055E00` |
| Pokémon Omega Ruby | `000400000011C400` |
| Pokémon Alpha Sapphire | `000400000011C500` |
| Pokémon Sun | `0004000000164800` |
| Pokémon Moon | `0004000000175E00` |
| Pokémon Ultra Sun | `00040000001B5000` |
| Pokémon Ultra Moon | `00040000001B5100` |

You can check any game's Title ID in FBI → Titles.

> Luma loads only **one** plugin per game. If you already use another plugin for that game, you'll have to choose one.

### Tested on
- Pokémon Ultra Sun (EUR, cartridge)

The plugin doesn't modify game memory, so it should work in other games too. Reports are welcome.

The count is stored in `shiny_counter.txt` at the root of the SD card.

### Building
Requires devkitPro (`3ds-dev`), `libctrpf` and `3gxtool`. Then run `make`. The repository also builds automatically with GitHub Actions on every push.


## Credits
- **Nanquitas**: [CTRPluginFramework](https://gitlab.com/thepixellizeross/ctrpluginframework)
- **PabloMK7**: [CTRPluginFramework-BlankTemplate](https://github.com/PabloMK7/CTRPluginFramework-BlankTemplate) and the Luma3DS plugin loader
- **ThePixellizerOSS**: CTRPluginFramework maintenance and packages
- **LumaTeam**: [Luma3DS](https://github.com/LumaTeam/Luma3DS)
- Made with help from Claude (Anthropic) / Hecho con ayuda de Claude (Anthropic)

## License
This project is licensed under the GNU General Public License v3.0 — see [LICENSE](LICENSE).

It uses CTRPluginFramework, which is distributed under its own license
(see the CTRPluginFramework repository).

