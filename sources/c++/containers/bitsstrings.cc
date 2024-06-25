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

#include <containers/bitsstrings.hh>
#include <new>

#if defined(linux)
# include <arpa/inet.h>
#endif

#include <vector>
#include <stdio.h>  // \todo: get rid of that !

namespace bangor
{
  namespace containers
  {
    class Node
    {
    public:
      Node() : m_isLeaf(false), m_value( {0x00, 0x00, 0x00, 0x00}), m_right(nullptr), m_left(nullptr)
	{
	}

      virtual ~Node()
	{
	  if (m_right)
	  {
	    delete m_right;
	  }

	  if (m_left)
	  {
	    delete m_left;
	  }
	}

      bool                   m_isLeaf;
      std::array<uint8_t, 4> m_value;
      Node*                  m_right;
      Node*                  m_left;
    };

    BitsStrings::BitsStrings() : m_root(new Node())
    {
    }

    BitsStrings::~BitsStrings()
    {
      // \todo: it leaks ! jenkins + valgrind + nonreg tests.
    }

    BitsStrings::Error BitsStrings::insert(uint32_t bits, uint16_t size)
    {
      uint32_t               beBits = htonl(bits); 
      std::array<uint8_t, 4> array = { 0x00, 0x00, 0x00, 0x00 };

      std::copy((uint8_t*)&beBits, (uint8_t*)(&beBits + 1), std::begin(array));
            
      return insert(array, size);
    }

    BitsStrings::Error BitsStrings::match(uint32_t bits) const
    {
      uint32_t               beBits = htonl(bits); 
      std::array<uint8_t, 4> array = { 0x00, 0x00, 0x00, 0x00 };

      std::copy(&beBits, &beBits + 1, std::begin(array));

      return match(array);
    }

    BitsStrings::Error BitsStrings::insert(const std::array<uint8_t, 4>& bits, uint16_t size)
    {
      Error error = E_NOERR;

      if (size >= 32)
      {
	error = E_INVAL;
      }
      else
      {
	uint32_t length = 0;
	Node*    node   = nullptr;
	Node*    next   = m_root;

	while (length < size && next != nullptr)
	{
	  uint32_t octet = length / 8;
	  uint8_t bit = 0x80 >> (length % 8);

	  node = next;

	  if (bits[octet] & bit)
	  {
	    next = node->m_right;
	  }
	  else
	  {
	    next = node->m_left;
	  }

	  if (next)
	  {
	    length++;
	  }
	}

	if (next)
	{
	  if (next->m_isLeaf)
	  {
	    error = E_BUSY;
	  }
	  else
	  {
	    node->m_isLeaf = true;
	    node->m_value = bits;
	  }
	}
	else
	{
	  for (; length < size; length++)
	  {
	    uint32_t index = length / 8;
	    uint8_t  bit   = 0x80 >> (length % 8);

	    next = new (std::nothrow) Node(); // \todo: manage bad allocation.

	    if (bits[index] & bit)
	    {
	      node->m_right = next;
	    }
	    else
	    {
	      node->m_left = next;
	    }

	    node = next;
	  }

	  node->m_isLeaf = true;
	  node->m_value = bits;
	}
      }

      return error;
    }

    BitsStrings::Error BitsStrings::match(const std::array<uint8_t, 4>& bits) const
    {
      Error    error  = E_NOERR;
      Node*    next   = m_root;
      uint32_t length = 0;

      while (error != E_BUSY && next != nullptr)
      {
	if (next->m_isLeaf)
	{
	  error = E_BUSY;
	}
	else
	{
	  uint32_t index = length / 8;
	  uint8_t  bit   = 0x80 >> (length % 8);

	  if (bits[index] & bit)
	  {
	    next = next->m_right;
	  }
	  else
	  {
	    next = next->m_left;
	  }

	  length++;
	}
      }
      return error;
    }

    BitsStrings::Iterator BitsStrings::begin()
    {
      Iterator iterator(*this);

      return iterator;
    }

    BitsStrings::Iterator BitsStrings::end()
    {
      Iterator iterator;

      return iterator;
    }
    
    BitsStrings::Iterator::Iterator() : m_node(nullptr)
    {
    }

    BitsStrings::Iterator::Iterator(const BitsStrings::Iterator& iterator)
      : m_node(iterator.m_node), m_toBeEplored(iterator.m_toBeEplored)
    {
    }

    BitsStrings::Iterator::~Iterator()
    {
    }

    BitsStrings::Iterator::Iterator(const BitsStrings& strings)
      : m_node(strings.m_root) 
    {
      m_toBeEplored.reserve(32);
      
      forward();
    }
    
    std::array<uint8_t, 4>& BitsStrings::Iterator::operator*()
    {
      return m_node->m_value;
    }

    BitsStrings::Iterator& BitsStrings::Iterator::operator++()
    {
      forward();

      return (*this);
    }

    void BitsStrings::Iterator::forward()
    {
      
      while (m_node != nullptr)
      {
	if (m_node->m_right != nullptr)
	{
	  if (m_node->m_left != nullptr)
	  {
	    m_toBeEplored.push_back(m_node);
	  }
	  
	  m_node = m_node->m_right;
	}
	else if (m_node->m_left != nullptr)
	{
	  m_node = m_node->m_left;
	}
	else if (m_toBeEplored.size() != 0)
	{
	  m_node = m_toBeEplored.back()->m_left;
	  
	  m_toBeEplored.pop_back();
	}
	else
	{
	  m_node = nullptr;
	}

	if (m_node != nullptr && m_node->m_isLeaf == true)
	{
	  break;
	}
      }
    }
        
    bool operator==(const BitsStrings::Iterator& right, const BitsStrings::Iterator& left)
    {
      return (right.m_node == left.m_node);
    }

    bool operator!=(const BitsStrings::Iterator& right, const BitsStrings::Iterator& left)
    {
      return (right.m_node != left.m_node);
    }
  }
  
}
