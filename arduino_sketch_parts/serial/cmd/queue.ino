char chrbuf_isempty(){
  if (chrbuf_frontptr == chrbuf_backptr){
    return true;
  }
  return false;
}

char chrbuf_isfull(){
  if (((chrbuf_frontptr + 1) % 16) == chrbuf_backptr){
    return true;
  }
  return false;
}

void chrbuf_enqueue(unsigned int c){
  if (((chrbuf_frontptr + 1) % 16) != chrbuf_backptr){
    chrbuf[chrbuf_frontptr] = c;
    chrbuf_frontptr = (chrbuf_frontptr+1) % 16;
  }
}

unsigned int chrbuf_dequeue(){
  if (chrbuf_frontptr != chrbuf_backptr){
    unsigned int t = chrbuf[chrbuf_backptr];
    chrbuf_backptr = (chrbuf_backptr + 1) % 16;
    return t;
  }
}
