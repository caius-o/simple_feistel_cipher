## Simple Feistel Cipher

The cryptography module at university taught me all about the use of cryptography in computing.
We covered all the topics related to the use of cryptography including symmetric and asymmetric encryption/decryption,
hashing (message digest), hash based message authentication code (HMAC) & digital signatures.

We covered how to implement basic ciphers in python. However, as i began to learn the C programming language, I wanted to 
take things further and implement the cipher in C resulting in this.

This cipher is **NOT** secure, it operates on 16 bit blocks using an 8 bit key. The cipher can be made more secure by increasing the key & block 
size however the code does not account for varying key sizes or block sizes. Furthermore, the cipher itself is also very simple and can be broken via brute-forcing.
This is just an exercise to improve my cryptographic knowledge and develop my programming skills and is posted for educational purposes only.

If you're interested in testing the code for fun or educational purposes, the program opens a text file called "test.txt" and creates an encrypted version
of the file. It then immediately opens the encrypted file and applies the decryption process on it and outputs the decrypted text in its own file; fully
demonstrating the encryption and decryption process leaving artefacts to examine.

You will need a C/C++ compiler for your system (GCC, MSVC, CLANG, etc).
