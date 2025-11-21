**Note from a Human:** This document was created with A.I. Proceed with caution.

---

# MultiBoot V1.05 for OS-9/6809 (Color Computer 3)

**Author:** Allen C. Huffman (OS-9 portion), Terry Todd (RS-DOS portion)  
**Copyright:** 1993, 1994 Sub-Etha Software

## Overview

MultiBoot is a utility for the Radio Shack Color Computer 3 running OS-9/6809. It allows users to install and manage MultiBoot on an existing bootable OS-9 disk, enabling selection of multiple boot files at startup. The program also incorporates RS-DOS code for compatibility and installation routines. MultiBoot does not create a bootable OS-9 disk; it requires one to already exist and then adds MultiBoot functionality to it.

## Features

- **MultiBoot Installation:**
  - Installs MultiBoot functionality onto an existing bootable OS-9 disk, supporting up to 16 boot files.
- **Boot File Selection:**
  - Reads directory entries, lets the user select which files are boot files, and prompts for descriptions.
- **Menu Delay Configuration:**
  - Allows setting a delay for the boot menu.
- **RS-DOS Compatibility:**
  - Installs RS-DOS MultiBoot loader code for cross-compatibility.
- **Edit Mode:**
  - Supports editing existing MultiBoot information (reordering boot files, changing delay).
- **Self-Installation:**
  - Can install MultiBoot code onto a disk if not already present.

## Data Structures

- **BF (BootFile):**
  - Stores a boot file's description and info (start/length).
- **MB (MultiBoot Sector):**
  - Contains menu delay, number of choices, and an array of boot file entries.

## How It Works

1. **Startup & Argument Parsing:**
   - Parses command-line arguments for device path and options (`-e` for edit, `-?` for help).
2. **Device Handling:**
   - Prompts for disk device if not specified.
   - Opens the disk for reading/writing.
3. **Boot Sector Check & Installation:**
   - Checks if MultiBoot is installed; if not, installs it by moving the OS-9 kernel, writing RS-DOS code, and updating allocation maps.
4. **Directory Scanning:**
   - Reads up to 50 directory entries, skipping hidden files.
   - Prompts user to select boot files and enter descriptions.
5. **Boot File Processing:**
   - Validates files (must be contiguous), stores metadata and description.
6. **Menu Delay Setting:**
   - Prompts for menu delay (default 10 seconds, range 1-30).
7. **MultiBoot Sector Update:**
   - Writes updated boot file info and menu delay to disk.
8. **Edit Mode:**
   - Allows changing menu delay and reordering boot files.
   - Prompts to save changes.

## Important Functions

- `OpenDevice()`
- `GetSides()`
- `SeekSector()`, `ReadBuffer()`, `WriteBuffer()`
- `Allocate()`
- `TracktoLSN()`, `Bitmap()`
- `BootCheck()`, `BootInstall()`
- `Process()`
- `Update()`
- `BootEdit()`

## Embedded Data

- **sec34_1:** First 20 bytes of original OS-9 kernel boot code.
- **mb_sec34_1:** RS-DOS DOS startup code (50 bytes), starting with ASCII "OS" signature, followed by 6809 machine code.
- **mb_sec33_15:** RS-DOS MultiBoot code V1.12 (512 bytes).

## User Interaction

- Prompts for device path, boot file selection, descriptions, and menu delay.
- Provides help and usage information.
- Handles errors gracefully (disk access, file fragmentation, allocation issues).

## Summary


MultiBoot V1.05 is a specialized utility for managing multi-boot OS-9 disks on the Color Computer 3. It combines OS-9 and RS-DOS compatibility, supports user-friendly boot file selection, and provides robust disk management features. The code is well-commented and modular, with clear separation between disk I/O, boot sector management, and user interaction.

---

Feel free to make corrections or add more details as needed.

## Functions and Structures Reference

### Structures

- **BF**
  - `char desc[27]`: Description of the boot file (padded and terminated).
  - `char info[5]`: Start and length info for the boot file.

- **MB**
  - `unsigned delay`: Menu delay value (in timer ticks).
  - `char choices`: Number of boot file choices.
  - `BF bf[MAXBOOTS]`: Array of boot file entries.

### Functions

- `main(int argc, char *argv[])`
  - **Parameters:**
    - `argc`: Number of command-line arguments.
    - `argv`: Array of argument strings.
  - **Return:** None (program entry point)
  - **Description:** Main program logic, handles user interaction, argument parsing, disk access, boot file selection, and menu configuration.

- `About(void)`
  - **Parameters:** None
  - **Return:** None (calls exit)
  - **Description:** Prints program information and usage message, then exits.

- `OpenDevice(unsigned mode)`
  - **Parameters:**
    - `mode`: File open mode (read/write flags).
  - **Return:** File descriptor (int) for the opened device
  - **Description:** Opens the disk device for reading/writing. Exits on failure.

- `GetSides(int fpath)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
  - **Return:** Number of disk sides (int)
  - **Description:** Determines the number of sides (heads) on the disk by reading the disk ID sector.

- `SeekSector(int fpath, int sector)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `sector`: Sector number to seek to.
  - **Return:** None
  - **Description:** Seeks to a specific sector on the disk. Exits on failure.

- `ReadBuffer(int fpath, char *buffer, int buflen)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `buffer`: Pointer to buffer to fill (modified inside function).
    - `buflen`: Number of bytes to read.
  - **Return:** None
  - **Description:** Reads a buffer from the disk into `buffer`. Exits on failure.

- `WriteBuffer(int fpath, char *buffer, int buflen)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `buffer`: Pointer to buffer to write.
    - `buflen`: Number of bytes to write.
  - **Return:** None
  - **Description:** Writes a buffer to the disk. Exits on failure.

- `Allocate(int fpath, int track, int sector, int sides, int range)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `track`: Track number.
    - `sector`: Sector number.
    - `sides`: Number of disk sides.
    - `range`: Number of sectors to allocate.
  - **Return:** None
  - **Description:** Allocates sectors in the disk bitmap, marking them as used.

- `TracktoLSN(int track, int sector, int sides)`
  - **Parameters:**
    - `track`: Track number.
    - `sector`: Sector number.
    - `sides`: Number of disk sides.
  - **Return:** Logical Sector Number (int)
  - **Description:** Converts track/sector/sides to Logical Sector Number (LSN).

- `Bitmap(int lsn, int *byte, int *bit)`
  - **Parameters:**
    - `lsn`: Logical Sector Number.
    - `byte`: Pointer to byte index (modified inside function).
    - `bit`: Pointer to bit mask (modified inside function).
  - **Return:** None
  - **Description:** Calculates byte/bit position in bitmap for a given LSN. `byte` and `bit` are output parameters.

- `BootCheck(int fpath, int sides)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `sides`: Number of disk sides.
  - **Return:** TRUE (boot exists) or FALSE (no boot present)
  - **Description:** Checks if MultiBoot is installed or if disk is bootable. Exits if not bootable.

- `BootInstall(int fpath, int sides)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `sides`: Number of disk sides.
  - **Return:** None
  - **Description:** Installs MultiBoot code onto the disk, moves kernel, writes loader, and updates allocation map.

- `Process(int fpath, long lsn, int item)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `lsn`: Logical Sector Number of file ID sector.
    - `item`: Index for boot file entry.
  - **Return:** TRUE (success) or FALSE (failure)
  - **Description:** Validates file (must be contiguous), stores boot file info and description in MultiBoot sector.

- `Update(int fpath, int sides)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `sides`: Number of disk sides.
  - **Return:** TRUE (success) or FALSE (failure)
  - **Description:** Writes updated MultiBoot info (delay, boot file entries) to disk.

- `BootEdit(int fpath, int sides)`
  - **Parameters:**
    - `fpath`: File descriptor for the disk device.
    - `sides`: Number of disk sides.
  - **Return:** TRUE (success) or FALSE (failure)
  - **Description:** Edits existing MultiBoot configuration (menu delay, boot file order), prompts to save changes.
