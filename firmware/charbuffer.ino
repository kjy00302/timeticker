unsigned int charbuffer_buf[32] = {0,};
unsigned char charbuffer_readptr = 0;
unsigned char charbuffer_writeptr = 0;

char charbuffer_isfull() {
  if (((charbuffer_writeptr + 1) % 32) == charbuffer_readptr) {
    return true;
  }
  return false;
}

char charbuffer_isempty() {
  if (charbuffer_writeptr == charbuffer_readptr) {
    return true;
  }
  return false;
}

void charbuffer_enqueue(unsigned int chr) {
  if (charbuffer_isfull()) {
    return;
  }
  charbuffer_buf[charbuffer_writeptr] = chr;
  charbuffer_writeptr = (charbuffer_writeptr + 1) % 32;
}

unsigned int charbuffer_dequeue() {
  if (charbuffer_isempty()) {
    return;
  }
  unsigned int t = charbuffer_buf[charbuffer_readptr];
  charbuffer_readptr = (charbuffer_readptr + 1) % 32;
  return t;
}

void charbuffer_reset() {
  charbuffer_readptr = 0;
  charbuffer_writeptr = 0;
}
