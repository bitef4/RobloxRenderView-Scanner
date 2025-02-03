# Roblox RenderView Pattern Scanner

A C++ memory scanner to dynamically locate the RenderView address in Roblox using pattern matching. This avoids reliance on hardcoded offsets, making it resilient to game updates.

## Features
- **Pattern Scanning**: Dynamically finds `RenderJob(EarlyRendering;` in memory
- **Offset Calculation**: Derives RenderView address using `+0x1E8` offset
- **Caching**: Stores found addresses to optimize repeated scans
- **Memory Safety**: Checks region permissions before reading

## Prerequisites
- Windows OS (uses Windows API)
- C++ compiler (MSVC/g++)
- Roblox process access (read permissions)

## Code Overview (`main.cpp`)
- Uses `VirtualQueryEx` for memory region enumeration
- Scans for `RenderJob(EarlyRendering;` byte pattern
- Applies offset to calculate RenderView address
- Basic error handling for memory operations

## Limitations
- Requires manual Roblox PID injection (see code comments)
- Offset (`0x1E8`) may change with Roblox updates
- Windows-only implementation

## Validation
Verify the found address using:
1. **Cheat Engine**: Manually confirm pattern + offset
2. **Memory Inspection**: Check if `RenderView + offsets` points to valid Datamodel structures
3. **In-Game Testing**: Use the address for rendering modifications

