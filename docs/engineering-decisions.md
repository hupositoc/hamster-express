# Engineering decision record

## Context

Delivery rewards and persisted progression must agree with the current gameplay state.

## Decision

Represent those interactions explicitly in gameplay code and document the expected runtime walkthrough.

## Consequences and evidence

Source inspection supports the architecture account; Unreal build, save compatibility and runtime behavior remain unverified in this environment.

See [architecture](architecture.md), [validation](validation.md) and the [case study](case-study.md) for implementation details and limits.
