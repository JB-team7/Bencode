#include <map>
#include <string>

#define BENCODE_INT 0x69 // i<contents>e
#define BENCODE_BYTE_DELIM 0x3A // <length>:<contents> (length is encoded in base10 so 0 - 9)
#define BENCODE_LIST 0x6C // l<contents>e
#define BENCODE_DICT 0x64 // d<contents>e

typedef std::pair<int,void*> bencodepair;
typedef std::map<std::string,bencodepair >* bencodemap;
namespace bencode {
  bencodemap decode(const char *torrentPath);
  void print(bencodemap dict);
  bencodemap parseDict(void);
  void free(bencodemap dict);
  
  void *masterParse(int *type, int *length);


  unsigned concatenate(unsigned x, unsigned y);
}