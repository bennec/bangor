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

#ifndef _BANGOR_CONTAINERS_BITSSTRINGS__HH_
#define _BANGOR_CONTAINERS_BITSSTRINGS__HH_

#include <array>
#include <vector>
#include <stdint.h>

namespace bangor
{

  namespace containers
  {
    class Node;

    /*!
     * \brief A bits strings collection, for the moment limited to 32 bits max
     *        strings.
     */
    class BitsStrings
    {

    public:

      /*!
       * \brief A bits strings collection iterator.
       */
      class Iterator
      {
      public:
	
	/*!
	 * \brief Initialize a bits strings iterator to a non existing
	 *        bits string.
	 */
	Iterator();
	
	/*!
	 * \brief Initialize a bits strings iterator from another bits strings
	 *        iterator.
	 *
	 * \param iterator Another bits strings iterator.
	 */
	Iterator(const Iterator& iterator);

	/*!
	 * \brief Release iterator resources.
	 */
	~Iterator();
	
	/*!
	 * \brief Dereference operator.
	 *
	 * No bounds checking is performed: deferencing collection end iterator
	 * has an undefined behaviour.
	 *
	 * \return The bits string the iterator points to.
	 */
	std::array<uint8_t, 4>& operator*();
	
	/*!
	 * \brief Pre-incrementation operator.
	 *
	 * \return The current iterator after forward to next collection bits
	 *         string.
	 */
	Iterator& operator++();
	
	/*!
	 * \brief Equality operator.
	 *
	 * \param right Right hand equality bits strings iterator.
	 * \param left  Left  hand equality bits strings iterator.
	 *
	 * \return Equality status: true when both iterators iterator point to
	 *         the same bits string of the same collection.
	 */
	friend bool operator==(const Iterator& right, const Iterator& left);

	/*!
	 * \brief Inequality operator.
	 *
	 * \param right Right hand inequality bits strings iterator.
	 * \param left  Left  hand inequality bits strings iterator.
	 *
	 * \return Inequality status: true when iterators point to different
	 *         bit strings.
	 */
	friend bool operator!=(const Iterator& right, const Iterator& left);

      private:
	
	// Bits strings collection is friend.
	friend class BitsStrings;

	/*!
	 * \brief Initialize a bits strings iterator from a bits strings 
	 *        collection.
	 *
	 * \param strings A bits strings collection.
	 */
	Iterator(const BitsStrings& strings);
	
	/*!
	 * \brief Forward to the next node containing a bits string.
	 */
	void forward();

	/*!
	 * \todo ASAP add a reference to the bits strings colection.
	 */
	
	/*!
	 * \var The bits string node the iterator points to.
	 */
	Node*              m_node;

	/*!
	 * \var The bits strings nodes whole left-hand side remains to be
	 * explored.
	 */
	std::vector<Node*> m_toBeEplored;
	
      };

      /*!
       * \brief Bits strings container operations error codes.
       */
      enum Error
      {
	E_NOERR = 0,
	E_NOMEM,
	E_BUSY,
	E_INVAL
      };

      /*!
       * \brief Default constructor.
       */
      BitsStrings();

      /*!
       * \brief Destructor.
       */
      virtual ~BitsStrings();

      /*!
       * \brief Tries to insert a bits string in host byte order into the
       *        container, max size is 32 bits.
       *
       *  \param[in] bits The bits string to insert into the container.
       *  \param[in] size The bits string size, it shall be in valid
       *                  [1, 32] interval.
       * 
       * \return     Execution status: BitsStrings::E_NOERR if
       *             successful.
       */
      Error insert(uint32_t bits, uint16_t size);

      /*!
       * \brief Tries to find the first container bits string that
       *        matches a bits string in host byte order.
       *
       *  \param[in] bits The bits string to match against container
       *                  bits strings.
       * 
       * \return     Execution status: BitsStrings::E_BUSY if a match is
       *             found, BitsStrings::E_NOERR otherwise.
       */
      Error match(uint32_t bits) const;
      
      /*!
       * \brief Return an iterator to the first bits string.
       * 
       * The iterator equals end() if the container is empty.
       * 
       * \return Iterator to the first bits string.
       */
      Iterator begin();
      
      /*!
       * \brief Return an iterator to none existing bits string.
       * 
       * \return First bitstring iterator.
       */
      Iterator end();

    private:

      /*!
       * \brief Tries to insert a bits string in network byte order into
       *        the container.
       *
       *  \param[in] bits The bits string to insert into the container,
       *                  in network byte order.
       *  \param[in] size The bits string size, it shall be in valid
       *                  [1, 32] interval.
       * 
       * \return     Execution status: BitsStrings::E_NOERR if
       *             successful.
       */
      Error insert(const std::array<uint8_t, 4>& bits, uint16_t size);

      /*!
       * \brief Tries to find the first container bits string that
       *        matches a bits string in network byte order.
       *
       *  \param[in] bits The bits string to match against container bits
       *                  strings, in network byte order.
       * 
       * \return     Execution status: BitsStrings::E_BUSY if a match is
       *             found, BitsStrings::E_NOERR otherwise.
       */
      Error match(const std::array<uint8_t, 4>& bits) const;

      /*!
       * \var Bits strings root node.
       */
      Node* m_root;
    };

  }

}
#endif
