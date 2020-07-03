# API Hasher
A simple C project which I developed, it leverages API hashing to call and lookup functions within the IAT and the EAT(Kinda):

* Look up the IAT and load it entirely to a linked list
* Hash each API within the IAT using a CRC32 Hash
* Locate all hashed APIs within the list by brute forcing the IAT
* Locate and call hashed CRC32 API identifiers
* Look up the EAT to locate APIs using a CRC32 hash
* Create generic WINAPI calls using macros

# TODO
* Obfuscate all string references
* Add full EAT support - Currently locates hashes within the EAT and then resolves them using GetProcAddress
