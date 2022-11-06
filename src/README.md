# SlashDiablo Maphack Source Directory
This is the main source directory for new source files.

## BH Directory
Existing files from the BH directory are expected to be eventually moved into this directory. Files from this directory **MUST** not be moved to the BH directory. In order to move the files from the BH directory, such files need to match the consistency of the other files in this directory.

Source files in the BH directory may include the files in this directory, but not the other way around.

## Style Guide
All files in this directory **MUST** follow the Google C++ Style Guide, with the following exception(s):
- Source files for C++ **SHOULD** have the file extension `.cpp`.
- Header files that only provide a C++ interface **MUST** have the file extension `.hpp`.
