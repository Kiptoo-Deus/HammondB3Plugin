Hammond B3 Plugin Requirements
Overview
The Hammond B3 Plugin emulates the iconic Hammond B3 organ, featuring tonewheel synthesis, drawbar controls, vibrato/chorus, Leslie speaker effects, and key click/percussion transients.
Features

Tonewheel Synthesis: DSP-based generation of 91 tonewheels with sine-like waves.
Drawbars: Nine adjustable drawbars (16', 5⅓', 8', 4', 2⅔', 2', 1⅗', 1⅓', 1') for harmonic control.
Vibrato/Chorus: DSP-based V1-V3 (vibrato) and C1-C3 (chorus) effects.
Leslie Speaker: Convolution-based emulation using sampled impulse responses for horn and rotor, with speed control (stop, slow, fast).
Key Click/Percussion: Sample-based transients for key click and percussion (2nd/3rd harmonic, normal/soft, fast/slow decay).
GUI: Intuitive interface with sliders, selectors, and preset management.
Formats: VST3 and AU, compatible with major DAWs (Ableton Live, Logic Pro, Reaper).

Technical Approach

DSP: Used for tonewheel synthesis and vibrato/chorus to ensure low latency and flexibility.
Samples: Used for Leslie impulse responses and key click/percussion transients for realism.
Performance: Target <5ms latency, <10% CPU usage on a 2.5 GHz quad-core CPU.
Platforms: macOS 10.15+, Windows 10+.

Sample Usage

Leslie IRs: High-quality impulse responses for horn and rotor at different speeds.
Key Click/Percussion: Short samples for mechanical transients, triggered on note-on events.

