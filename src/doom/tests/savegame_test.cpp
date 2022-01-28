#include "catch.hpp"
#include <array>

extern "C" {
#include "doomtype.h"
#include "p_saveg.h"

byte  saveg_read8_in_stream   (FILE* stream, boolean* error);
extern short saveg_read16_in_stream  (FILE* stream, boolean* error);
extern int   saveg_read32_in_stream  (FILE* stream, boolean* error);
extern void  saveg_write16_in_stream (FILE* stream, short value, boolean* error);
extern void  saveg_write32_in_stream (FILE* stream, int value  , boolean* error);
}

static_assert(sizeof(boolean) == sizeof(byte));

// ------------------ 8 BIT SCENARIOS ------------------
// write
SCENARIO("Writing 8 bits in a file", "[write]") {
  GIVEN("An empty file")
  {
    std::array<byte, 2> buf{};
    FILE *test_file = fmemopen(buf.data(), 2*sizeof(byte), "a");
    setbuf(test_file, NULL);

    AND_GIVEN("A value to write")
    {
      byte value = 0x43;

      AND_GIVEN("No prior error")
      {
        boolean err = false;

        WHEN("Writing 8 bits in the stream")
        {
          saveg_write8_in_stream(test_file, value, &err);
          THEN("There is no error after the write")
          {
            CHECK(err == false);
            AND_THEN("The file as been written into with the correct value")
            {
              INFO("Bytes: " << Catch::rangeToString(buf));
              CHECK(buf[0] == value);
            }
          }
          fclose(test_file);
        }
      }

      AND_GIVEN("A prior error")
      {
        boolean err = true;
        WHEN("Writing 8 bits in the stream")
        {
          saveg_write8_in_stream(test_file, value, &err);
          fclose(test_file);
          THEN("There is still an error after writing")
          {
            CHECK(err == true);
            AND_THEN("The file has been written into with the correct value")
            {
              CHECK(buf[0] == value);
            }
          }
        }
      }
    }
  }
}

SCENARIO("Writing 8 bits in a file fails", "[write]") {
  GIVEN("A file with an error")
  {
    std::array<byte, 2> buf{0x13,0x37};
    FILE *test_file = fmemopen(buf.data(), 2*sizeof(byte), "r");
    setbuf(test_file, NULL);

    AND_GIVEN("No prior error")
    {
      boolean err = false;
      WHEN("Writing 8 bits in the stream")
      {
        saveg_write8_in_stream(test_file, 0x42, &err);
        auto error_from_file = ferror(test_file);
        THEN("There is an error after the write")
        {
          CHECK(error_from_file != 0);
          CHECK(err == true);
          AND_THEN("The file has not been modified")
          {
            CHECK(buf[0] == 0x13);
            CHECK(buf[1] == 0x37);
          }
        }

        fclose(test_file);
      }
    }
  }
}

// Read
SCENARIO("Reading 8 bits in a file", "[read]") {
  GIVEN("A file")
  {
    byte buf[1] = { 0xA3 };
    FILE *test_file = fmemopen(buf, sizeof(byte), "r");
    AND_GIVEN("A null byte variable")
    {
      byte result = NULL;
      AND_GIVEN("No prior error")
      {
        boolean err = false;
        WHEN("Reading 8 bits in the stream")
        {
          result = saveg_read8_in_stream(test_file, &err);
          fclose(test_file);
          THEN("There is no error after reading")
          {
            CHECK(err == false);
            AND_THEN("The byte variable has been modified")
            {
              CHECK(result == (byte)0xA3);
            }
          }
        }
      }

      AND_GIVEN("A prior error")
      {
        boolean err = true;
        WHEN("Reading 8 bits in the stream")
        {
          result = saveg_read8_in_stream(test_file, &err);
          fclose(test_file);
          THEN("There is an error after the reading")
          {
            CHECK(err == true);
            AND_THEN("The byte variable has been modified")
            {
              CHECK(result == (byte)0xA3);
            }
          }
        }
      }
    }
  }
}

SCENARIO("Reading 8 bits in a file with an error", "[read]") {
  GIVEN("A file")
  {
    std::array<byte, 2> buf{0x42, 0x52};
    FILE *test_file = fmemopen(buf.data(), 2*sizeof(byte), "a");
    setbuf(test_file, NULL);

    AND_GIVEN("A null byte variable")
    {
      byte result = 0;
      AND_GIVEN("No prior error")
      {
        boolean err = false;
        WHEN("Reading 8 bits in the stream")
        {
          result = saveg_read8_in_stream(test_file, &err);
          THEN("There is an error after the reading")
          {
            CHECK(ferror(test_file) != 0);
            CHECK(err == true);
            AND_THEN("The byte variable has not been modified")
            {
              CHECK(result == static_cast<byte>(-1));
            }
          }
        }
      }
    }
    fclose(test_file);
  }
}
// ------------------ 8 BIT SCENARIOS ------------------