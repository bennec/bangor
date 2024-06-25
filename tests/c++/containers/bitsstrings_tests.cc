/**********************************************************************************
 * MIT License                                                                    *
 *                                                                                *
 * Copyright © 2024 by Christophe Benne, this file is part of BANGOR software.    *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 * ********************************************************************************/

#include <gtest/gtest.h>
#include <containers/bitsstrings.hh>

namespace bangor
{
  namespace containers
  {
    TEST(Bitsstrings, Iterator01)
    {
      std::array<uint8_t, 4> D0_8 = { 0xD0, 0x00, 0x00, 0x00};
      std::array<uint8_t, 4> D189 = { 0xD1, 0x80, 0x00, 0x00};
      BitsStrings            bits;
      BitsStrings::Iterator  iter;
      
      
      EXPECT_EQ  (bits.insert(0xD0000000, 8), BitsStrings::E_NOERR);
      
      iter = bits.begin();
      EXPECT_TRUE(iter   != bits.end());
      EXPECT_TRUE(*iter  == D0_8);
      EXPECT_TRUE(++iter == bits.end());
      
      EXPECT_EQ(bits.insert(0xD1800000, 9), BitsStrings::E_NOERR);
      iter = bits.begin();
      EXPECT_TRUE(iter   != bits.end());
      EXPECT_TRUE(*iter  == D189);      
      EXPECT_TRUE(++iter != bits.end());
      EXPECT_TRUE(*iter  == D0_8);      
      EXPECT_TRUE(++iter == bits.end());
      
      
      EXPECT_EQ(bits.insert(0xD1000000, 9), BitsStrings::E_NOERR);
    }
    
  }

}

int main(int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
