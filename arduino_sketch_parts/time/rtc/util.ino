
int bcdencode(char b){
  return (b / 10) * 16 + (b % 10);
}

int bcddecode(char b){
  return (b >> 4) * 10 + (b & 0x0f);
}
