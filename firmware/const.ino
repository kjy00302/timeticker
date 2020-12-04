const unsigned int STR_NOW[] PROGMEM = {0xc9c0, 0xae08, 0xc740, 0x20, 0}; // 지금은
const unsigned int STR_IPNIDA[] PROGMEM = {0xc785, 0xb2c8, 0xb2e4, 0}; // 입니다
const unsigned int STR_TEMP[] PROGMEM = {0xc628, 0xb3c4, 0xb294, 0x20, 0}; // 온도는
const unsigned int STR_HUMI[] PROGMEM = {0xc2b5, 0xb3c4, 0xb294, 0x20, 0}; // 습도는

const unsigned int* const PROGMEM CONST_STRING[] = {
  STR_NOW,
  STR_IPNIDA,
  STR_TEMP,
  STR_HUMI
};


const unsigned int PROGMEM CONST_HOUR[][3] = {
  {0xc5f4, 0xb450}, // 열두
  {0xd55c}, // 한
  {0xb450}, // 두
  {0xc138}, // 세
  {0xb124}, // 네
  {0xb2e4, 0xc12f}, // 다섯
  {0xc5ec, 0xc12f}, // 여섯
  {0xc77c, 0xacf1}, // 일곱
  {0xc5ec, 0xb35f}, // 여덟
  {0xc544, 0xd649}, // 아홉
  {0xc5f4}, // 열
  {0xc5f4, 0xd55c} // 열한
};


const unsigned int PROGMEM CONST_DIGIT[] = {
  0xc77c, // 일
  0xc774, // 이
  0xc0bc, // 삼
  0xc0ac, // 사
  0xc624, // 오
  0xc721, // 육
  0xce60, // 칠
  0xd314, // 팔
  0xad6c //구
};
