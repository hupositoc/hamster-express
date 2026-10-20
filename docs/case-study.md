# Engineering case study: Hamster Express

## 1. The task and operating context

Explore a complete small gameplay loop: control a hamster, pick up cargo, deliver it to a goal, receive a reward and retain progress between sessions.

A hobby project built around Unreal Engine C++. It is separate from marine testing and is presented as a prototype rather than a released commercial game.

## 2. The problem before the tool

A game idea becomes useful engineering practice only when its pieces cooperate: movement, interaction state, world layout, feedback, rewards and saved progress. Building those pieces separately without a connected loop would leave a visual scene rather than a playable prototype.

## 3. The intervention

Use distinct actors for the character, cargo and delivery goal; keep completion/reward logic in the game mode; render messages through the HUD; and serialize player transform and delivery progress through a SaveGame object. Build a simple world at runtime to reduce dependence on external art during the technical prototype stage.

### Input → decision → output

1. **Input:** Move / interact.
2. **Processing:** Carry cargo.
3. **Interpretation:** Complete delivery.
4. **Output:** Reward / save progress.

This workflow is illustrated in the README and is mapped to actual source entry points in [architecture](architecture.md).

## 4. Work enabled by the implementation

- Exercise the full pick-up, travel, deliver and reward sequence in one codebase.
- Explore player movement, sprinting and interaction through explicit C++ classes.
- Persist player location, coins and delivery-completion state through manual save/load and completion-triggered saving.

The repository contains the connected gameplay classes, a +100 coin completion rule and save/load paths. Source inspection confirms those mechanisms; playability, performance and packaged-build reliability have not been measured in this environment.

## 5. Technical choices and tradeoffs

### Finish a small loop before expanding content

One delivery with persistent consequences provides a clearer engineering milestone than a large scene without working interactions.

### Centralize reward rules

The completion guard prevents the same objective from granting repeated rewards through overlap callbacks.

### Use placeholder art deliberately

Runtime primitives keep the technical loop inspectable. They are not presented as final character production art or animation.

## 6. Engineering contribution

Implement cargo handling, delivery rewards and save/load behavior in Unreal C++. The linked architecture document identifies implementation entry points; validation records distinguish source inspection, executable checks and hardware-dependent behavior.

## 7. Evidence and scope

Gameplay class relationships, the duplicate-completion guard, reward value and saved fields were inspected from C++ source. Editor, packaging and runtime tests remain separate tasks.

- The prototype needs Unreal Engine 5.8 and a supported Windows C++ environment.
- No runtime footage, frame-time benchmark or packaged-build test is included.
- Loading resets carrying state; recovery of an in-progress delivery is not a complete inventory system.
- Rendering settings are configuration, not evidence of visual quality or measured GPU performance.

## 8. Next measurable milestone

- [ ] Record a real gameplay capture after an editor build.
- [ ] Test duplicate overlap, save/load and interrupted-delivery cases.
- [ ] Replace placeholders with documented, redistributable art while preserving the loop.

## Before and after the intervention

| Before | Implementation | Work enabled and evidence |
|---|---|---|
| A delivery-game idea needed an executable interaction and progression model. | Implement cargo handling, delivery rewards and save/load behavior in Unreal C++. | The prototype provides source-level material for examining gameplay state and persistence; a packaged play session has not been verified here. |
