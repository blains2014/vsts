# OSOTRUXE (SnoopyChu) — 5-Layer Timbre Morph & Selective FX VST3

A Windows **VST3** instrument plugin designed for FL Studio that blends up to **5 audio samples** into one unified sound with per-sample ADSR, polarity inversion, selective FX routing, and a 7-module FX rack.

## Features
- **5 Sample Slots:** Load `.wav`, `.aif`, `.flac`, or `.mp3` files per slot with individual **Level**, **Pitch**, **ADSR** (`Attack`, `Decay`, `Sustain`, `Release`), **Polarity Invert (Ø)**, and **FX Send** toggle.
- **Blend / Mesh Controls:** `Timbre` (spectral layer weighting), `Color` (bipolar tilt), `Shape` (global ADSR contour scaling), and `Movement` (staggered LFO layer modulation).
- **Selective 7-Module FX Rack:** Processes only the samples with **FX Send** enabled:
  - **EQ** (Low, Mid, High)
  - **Compressor** (Threshold, Ratio, Attack, Release)
  - **Saturation** (Drive, Mix)
  - **Distortion** (Drive, Tone, Mix)
  - **Phaser** (Rate, Depth, Feedback, Mix)
  - **Delay** (Time, Feedback, Mix)
  - **Reverb** (Size, Damping, Width, Mix)

## Easiest Way to Get the Installable `.vst3` / `.exe` (No Local Setup)
1. Create a free GitHub repository and upload the contents of this folder.
2. Go to the **Actions** tab and run **Build Windows VST3 & Installer**.
3. Download the `OSOTRUXE-Windows-VST3` artifact — it contains both `OSOTRUXE.vst3` and `OSOTRUXE_Windows_Installer.exe`.

## Building Locally on Windows
1. Install **CMake** (3.22+) and **Visual Studio 2022** (with "Desktop development with C++").
2. Open PowerShell in this folder and run:
   ```powershell
   cmake -B build -G "Visual Studio 17 2022" -A x64
   cmake --build build --config Release --target OSOTRUXE_VST3
   ```
3. Copy `build\OSOTRUXE_artefacts\Release\VST3\OSOTRUXE.vst3` to `C:\Program Files\Common Files\VST3\`.

## Installing in FL Studio
1. Run `OSOTRUXE_Windows_Installer.exe` (or copy `OSOTRUXE.vst3` to `C:\Program Files\Common Files\VST3\`).
2. In FL Studio, go to **Options → Manage Plugins → Find installed plugins**.
3. Favorite **OSOTRUXE** and load it in the Channel Rack.
