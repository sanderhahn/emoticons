#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char* name;
  int start;
  int end;
} range;

range ranges[] = {
  {"Emoticons", 0x1F600, 0x1F64F},
  {"Miscellaneous Symbols and Pictographs", 0x1F300, 0x1F5FF},
  {"Supplemental Symbols and Pictographs", 0x1F900, 0x1F9FF},
  {"Transport and Map Symbols", 0x1F680, 0x1F6FF},
  {"Diversity Modifiers", 0x1F3FB, 0x1F3FF},
  {"Dingbats", 0x2700, 0x27BF},
  {"Miscellaneous", 0x2600, 0x26FF},
};

//  U+0000 ..   U+007F  1 byte   0xxx xxxx
//  U+0080 ..   U+07FF  2 bytes  110x xxxx   10xx xxxx
//  U+0800 ..   U+FFFF  3 bytes  1110 xxxx   10xx xxxx   10xx xxxx
// U+10000 .. U+10FFFF  4 bytes  1111 0xxx   10xx xxxx   10xx xxxx   10xx xxxx

char buf[] = {0,0,0,0,0};
int utf8out(char* buf, int ch) {
  if(ch <= 0x7F) {
    buf[0] = ch;
    buf[1] = 0;
    return 1;
  } else if(ch <= 0x7FF) {
    buf[0] = (ch >> 6) | 0xC0;
    buf[1] = (ch & 0x3F) | 0x80;
    buf[2] = 0;
    return 2;
  } else if(ch <= 0xFFFF) {
    buf[0] = (ch >> 12) | 0xE0;
    buf[1] = ((ch >> 6) & 0x3F) | 0x80;
    buf[2] = (ch & 0x3F) | 0x80;
    buf[3] = 0;
    return 3;
  } else if(ch <= 0x10FFFF) {
    buf[0] = (ch >> 18) | 0xF0;
    buf[1] = ((ch >> 12) & 0x3F) | 0x80;
    buf[2] = ((ch >> 6) & 0x3F) | 0x80;
    buf[3] = (ch & 0x3F) | 0x80;
    buf[4] = 0;
    return 4;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  for(int i = 0; i < sizeof(ranges) / sizeof(range); i++) {
    range range = ranges[i];
    printf("## %s\n\n", range.name);
    for(int emoticon = range.start; emoticon <= range.end; emoticon++) {
      int s = utf8out(buf, emoticon);
      printf("`");
      for(int i = 0; i < s; i++) {
        printf("%02x", buf[i] & 0xff);
        if(i != s - 1) {
          printf(" ");
        }
      }
      printf("` ");
      if(s == 0 || fwrite(buf, 1, s, stdout) != s) {
        exit(1);
      }
      printf("\n\n");
    }
  }
  return 0;
}
