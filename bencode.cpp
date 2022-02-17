#include <fstream>
#include "bencode.hpp"

#define BENCODE_END 0x65 // e
namespace bencode {
  FILE *file = NULL;

   /*
   * Parse and returns next int in bencode
   * @return integer parsed
   */
  unsigned int parseInt() {
    int c;
    int isFirst = 0;
    unsigned int tmp = 0;
    while((c = fgetc(file)) != BENCODE_END) {
      if(isFirst == 0) {
        tmp = c - '0';
        isFirst = -1;
      } else {
        tmp = concatenate(tmp, c - '0');
      }
    }
    return tmp;
  }

  /* Parses Bytes with given size
   * @param item where to copy contents to
   * @param size address of size
   */
  void parseByte(char **item, int *size) {
    if(!file) return;
    *item = (char*)calloc(1, *size);
    fread(*item, *size, 1, file);
  }

  void *masterParse(int *type, int *length) {
    if(!file) return NULL;
    switch(*type) {
      case BENCODE_DICT: {
        bencodemap dict = parseDict();
        return (void*)(dict);
        break;
      }
      case BENCODE_INT: {
        unsigned int value = parseInt();
        void *lt = malloc(sizeof(value));
        memcpy(lt, &value, sizeof(value));

        return lt;//(void*)&value;
        break;
      }
      case BENCODE_BYTE_DELIM: {
        char *value = NULL;
        parseByte(&value, length);
        return (void*)value;
        break;
      }
      case BENCODE_LIST: {
        printf("BENCODE_LIST\n");
        break;
      }
      default:
        break;
    }
    return NULL;
  }
  
  unsigned concatenate(unsigned x, unsigned y) {
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        
  }

  bencodemap parseDict(void) {
    bencodemap dict = new std::map<std::string,bencodepair >;
    int c;
    int length = 0;

    char *key = NULL;
    while((c = fgetc(file)) != BENCODE_END) {
     if(c >= 48 && c <= 57) {
      if(length == 0) {
        length = c - '0';
      } else {
        length = concatenate(length, c - '0');  
      }
     } else if(c != ':') { // only resets when needed...
       length = 0;
     }

      void *value = masterParse(&c, &length);
      if(value == NULL) continue;
      switch(c) {
        case BENCODE_BYTE_DELIM:
          length = 0;
          if(key == NULL) {
            key = (char*)value;
          } else {
            (*dict)[key] = bencodepair(BENCODE_BYTE_DELIM, value);
            ::free(key);
            key = NULL;
          }
          break;
        case BENCODE_INT:
          if(key) {
            (*dict)[key] = bencodepair(BENCODE_INT, value);
            ::free(key);
            key = NULL;
          }
          break;
        case BENCODE_DICT:
          if(key) {
            bencodemap *test = static_cast<bencodemap*>(value);
            (*dict)[key] = bencodepair(BENCODE_DICT, value);
            ::free(key);
            key = NULL;
          }
          break;
        default:
          printf("Deallocating...\n");
          if(key) ::free(key);
      }
    }
   
    return dict;
  }

  /* Prints contents of bencodemap.
   * @param dict the dictionary to print
   */
  void print(bencodemap dict) {
    for(const auto& item : *dict) {
      printf("'%s': ", item.first.c_str());
      switch(item.second.first) {
        case BENCODE_INT:
          printf("%u\n", *(unsigned int*)item.second.second);
          break;
        case BENCODE_BYTE_DELIM:
          printf("%s\n", (char*)item.second.second);
          break;
        case BENCODE_DICT:
          printf("[\n");
          print(static_cast<bencodemap>(item.second.second));
          printf("]\n");
          break;
      }
    }
  }

  /* Decodes contents of bencoded data.
   * @param torrentPath path of bencoded file
   * @return dictionary of contents
   */
  bencodemap decode(const char *torrentPath) {
    file = fopen(torrentPath, "rb");
    int c = fgetc(file); // NOTE: For now assuming file starts with a dictionary...
    void *content = masterParse(&c, NULL);
    if(file) fclose(file);
    return static_cast<bencodemap>(content);
  }

  /* Deallocates contents of bencodemap
   * @param dict dictionary to deallocate
   */
  void free(bencodemap dict) {
    for(const auto& item : *dict) {
      switch(item.second.first) {
         case BENCODE_INT:
          if(item.second.second) ::free(item.second.second);
          break;
        case BENCODE_BYTE_DELIM:
          if(item.second.second) ::free(item.second.second);
          break;
        case BENCODE_DICT:
          delete static_cast<bencodemap>(item.second.second);
          break;
      }
    }
    delete dict;
  }
}