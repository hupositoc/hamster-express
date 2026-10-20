# Hamster Express — Technical Decisions v0.2.0

## Engine: Unreal Engine 5.8

The original brief prioritized integrated-graphics scalability. The updated product direction explicitly allows rebuilding from scratch and prioritizes visual charm on the owner's development laptop over low-end compatibility.

Selected stack:
- Unreal Engine 5.8
- C++ runtime gameplay core
- DirectX 12 / Shader Model 6
- Lumen dynamic GI + reflections
- Hardware ray tracing when available
- Virtual Shadow Maps
- TSR anti-aliasing/upscaling path
- Runtime-generated stylized prototype world so the source project can bootstrap without binary content dependencies

Target development PC:
- Intel Core i7-14700HX
- NVIDIA GeForce RTX 4060 Laptop GPU (8 GB class dedicated VRAM)
- 32 GB RAM
- DirectX 12

Target presentation:
- 1080p, 60 FPS goal for the vertical slice
- 640x360 windowed default retained for discreet office/windowed use; Alt+Enter toggles fullscreen
- High/Epic-quality stylized rendering, not photorealism

## Visual Direction

"Premium stylized miniature adventure": rounded hamster forms, warm materials, readable silhouettes, soft global illumination, believable depth, high-quality animation later, and a diorama-like world.

The hamster remains intentionally stylized rather than using strand-based realistic fur. The final production character should use authored mesh + rig + facial shapes; the current v0.2.0 C++ assembled character is a technical placeholder only.

## Why not Godot for this direction?

Godot remains attractive for small builds and open-source workflows, but the revised goal is maximum attractive real-time rendering on one RTX-class PC. Unreal's integrated Lumen, VSM, Nanite ecosystem, animation tooling and worldbuilding pipeline better match that goal.

## Why not Unity 6.3 LTS?

Unity is viable and has a strong asset ecosystem. However, matching the same out-of-box dynamic GI, shadowing, worldbuilding and high-end rendering pipeline would require more render-pipeline/asset decisions. Unreal is selected because visual fidelity is now a primary constraint.
