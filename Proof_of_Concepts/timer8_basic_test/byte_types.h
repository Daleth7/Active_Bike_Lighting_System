#ifndef TYPE_ALIASES_RELATEDTOBYTESIZESSSS__________
#define TYPE_ALIASES_RELATEDTOBYTESIZESSSS__________

#include <cstdint>

    // Alias only up to double word since SAMD21 runs on a 32-bit architecture
typedef std::uint8_t    byte;
typedef std::uint16_t   word;
typedef std::uint32_t   dword;
typedef word            size_type;
typedef dword           big_size_type;

#endif