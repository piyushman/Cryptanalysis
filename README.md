Cryptanalysis
===============

Decryption of Substitution Ciphers

Purpose
-------

Given a key length and the ciphertext, we need to find the plaintext from `Dictionary1.txt` and `Dictionary2.txt` files.

Algorithm
----

To decrypt the ciphertext

1. Divide the ciphertext in buckets with there being `keylength` number of buckets. (Each char in the ciphertext goes to the `mod keylength` bucket).
2. On each position of the key perform frequency analysis.
    + Calculate the index of coincidence.
       - We have the frequency of alphabets in the english language.
       - We get the frequency of alphabets in that bucket(key position).
       - We then shift the frequency of alphabets in the bucket 1,2...26 to get the maximum score. 
       - This gives us the most probable character in that position of the key.
3. We now have a probable key. Using this key we can find the most matching plaintext from `Dictionary 1` and `Dictionary 2` by using the `Levenshtein distance` algorithm.
4. This gives us the plaintext in mostcases.


Screenshots
-----------
![Example 1](/Screenshots/Screenshot1.PNG?raw=true "Example 1")

![Example 2](/Screenshots/Screenshot2.PNG?raw=true "Example 2")

![Example 3](/Screenshots/Screenshot3.PNG?raw=true "Example 3")


Improvements
------------

Plaintext which are combinations of words from Dictionary 2 are not decrypted correctly.
Need to redesign the algorithm or get a betterway to get all combinations of words in the Dictionary 2 file.
