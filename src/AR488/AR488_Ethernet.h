#pragma once

#include <Arduino.h>

class EthernetStream : public Stream
{
public:
  EthernetStream(FILE* input, FILE* output);

  int    available();
  int    peek();
  int    read();
  void   flush();  //  placeholder to keep CI happy

  size_t write(const uint8_t data);
  size_t write(const uint8_t *buffer, size_t size);

private:
  FILE* input_file;
  FILE* output_file;
};

namespace ethernet
{
    Stream& begin();
} // namespace ethernet
