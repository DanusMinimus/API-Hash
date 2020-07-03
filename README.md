# IAT-Api-Hash
A simple C project which I developed, it leverages API hashing to call and lookup functions within the IAT:

* Look up the IAT and load it entirely to a linked list
* Hash each API within the IAT using a CRC32 Hash
* Locate all hashed APIs within the list by brute forcing the IAT
* Locate and call hashed CRC32 API identifiers
* Create generic WINAPI calls using macros

# TODO
* Add EAT support for extra obfuscation
