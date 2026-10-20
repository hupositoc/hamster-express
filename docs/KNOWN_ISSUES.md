# Known Issues — v0.2.0

- Production art assets are not yet embedded; the playable prototype uses engine primitives assembled into stylized forms.
- The cart is rigidly attached rather than using final suspension/rope simulation.
- No authored animation set yet; CharacterMovement provides locomotion but the placeholder mesh does not animate.
- The runtime world is generated in code to guarantee source portability; the vertical slice will move major art to authored Unreal assets.
- This package has not been compiled inside this ChatGPT execution environment because Unreal Engine binaries are not installed here. It is prepared for UE 5.8 source compilation on Windows.
