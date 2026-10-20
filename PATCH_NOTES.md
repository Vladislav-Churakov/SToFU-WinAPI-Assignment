# Binary patch notes

## What was changed

The original executable contains a conditional jump before a call through the import address table to `UCRTBASE.DLL::abort`.

| Property | Value |
|---|---|
| Ghidra virtual address of jump | `0x1400295AC` |
| PE image base (Ghidra) | `0x140000000` |
| RVA | `0x295AC` |
| File offset (Ghidra Byte Source Offset) | `0x69AC` |
| Original bytes | `74 07` (`JZ +7`) |
| Patched bytes | `EB 07` (`JMP +7`) |
| Modified byte | at file offset `0x69AC`, `74` -> `EB` |

The conditional branch falls through to `CALL abort` for the matched message `Random number is: 2`. Changing `JZ` to `JMP` keeps the same target `0x1400295B5` and always skips `CALL abort`. The call remains in the binary but is bypassed on this path.

To reproduce: import the **original** EXE in Ghidra, inspect `0x1400295AC`, and note the byte source offset. Open a **copy** of the original EXE in HxD, go to offset `0x69AC` in hexadecimal (from start of file), verify `74 07`, overwrite `74` with `EB`, and save. Do not insert bytes or change file length. Reimport the modified EXE to see `JMP` in the listing.

## Verification of provided binaries

The EXEs are both 723,968 bytes long and differ at exactly **one byte** (file offset `0x69AC`).

- Original: `74 07 FF 15 BC 90 0A 00`
- Patched: `EB 07 FF 15 BC 90 0A 00`

The original application source is unmodified; any rebuild will lose the binary patch unless it is reapplied to the new binary using the new build's instruction address and file offset.
