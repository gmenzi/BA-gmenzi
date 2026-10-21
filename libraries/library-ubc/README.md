# polyCMUT module library

Use this when another board should treat the finished PPG PCB as one component. The part name in KiCad is **polyCMUT**. Send the whole `library` folder. The PPG KiCad project stays the source for changing the module itself.

## Add it in KiCad

1. **Preferences → Manage Symbol Libraries → Add existing.** Choose `polyCMUT.kicad_sym`. Set the nickname to `polyCMUT`.
2. **Preferences → Manage Footprint Libraries → Add existing.** Choose `polyCMUT.pretty`. Set the nickname to `polyCMUT`.
3. Place symbol `polyCMUT:polyCMUT`. The footprint is already `polyCMUT:polyCMUT`.

## What the footprint is

The fabrication outline is on **F.Fab**, including the center opening. It is not on Edge.Cuts, so placing the module does not cut the host board.

Host lands are 1.0 × 0.9 mm and sit entirely outside the module. There is a 0.2 mm gap between the module edge and each land, so the land does not cover the castellated hole. The solder joint is the fillet on the outside, which an iron can remove. Adjacent lands are 0.6 mm apart. The silk dot marks pin 1, **GND**.

| Pins | On the symbol, top to bottom | Net on the module |
|------|------------------------------|-------------------|
| 1 | Right | GND |
| 2–9 | Right | CH1–CH8 |
| 10–12 | Left | NC1–NC3, plated castellations with no internal net |

The footprint has no mounting holes, so placing the module does not drill the host board.

Keep other parts outside the courtyard. The module is 16.0 × 16.0 mm and 0.8 mm thick.
