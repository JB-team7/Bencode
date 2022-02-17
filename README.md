# Bencode
Easy to use and lightning fast utility to print and parse bencoded data

# Tutorial
You can input a path of your bencoded file and it'll return a 'bencodemap' 
  (bencodemap decoded = bencode::decode("test.torrent");).
  
      bencodemap is a std::map with the key being a std::string and the value being a <int,void*> std::pair.
      'int' in the pair represents the value type:
          - BENCODE_INT (unsigned int)
          - BENCODE_BYTE_DELIM (char*)
          - BENCODE_DICT (bencodemap)
      'void*' in the pair represents the value (cast it to the type you need):
          - BENCODE_INT *(unsigned int*)"value"
          - BENCODE_BYTE_DELIM (char*)"value
          - BENCODE_DICT static_cast<bencodemap>("value")
  
      C-Strings and Integers are mallocated so make sure to use 'bencode::free(*bencodemap*);' method
      after you are done using the dictionary.
  
# Screenshots
![Screen Shot 2022-02-16 at 7 53 46 PM](https://user-images.githubusercontent.com/63203414/154389484-a33b6d96-9f5a-4d65-909a-76fea2de21fd.png)
  
![Screen Shot 2022-02-16 at 7 54 02 PM](https://user-images.githubusercontent.com/63203414/154390840-778c1ba2-7a89-42e7-b1c0-49e12197274a.png)


