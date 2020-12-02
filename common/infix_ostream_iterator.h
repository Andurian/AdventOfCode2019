#ifndef CSAR_IAB_INFIX_OSTREAM_ITERATOR
#define CSAR_IAB_INFIX_OSTREAM_ITERATOR

#include <algorithm>
#include <iterator>
#include <iostream>

template<typename TChar = char, typename TTraits = std::char_traits<TChar>>
class infix_ostream_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
public:
    using char_type = TChar;
    using traits_type = TTraits;
    using ostream_type = std::basic_ostream<TChar, TTraits>;

    infix_ostream_iterator(ostream_type & os, const TChar * delimiter)
        : m_os{ &os }, m_delimiter{ delimiter }, m_isFirstElement{ true }
    {
        // empty
    }

    infix_ostream_iterator(ostream_type & os) : infix_ostream_iterator{ os, nullptr } {}

    template<typename T>
    infix_ostream_iterator & operator=(T const & item)
    {
        if (!m_isFirstElement && m_delimiter != nullptr)
        {
            *m_os << m_delimiter;
        }
        *m_os << item;
        m_isFirstElement = false;
        return *this;
    }

    infix_ostream_iterator & operator*() { return *this; }

    infix_ostream_iterator & operator++() { return *this; }

    infix_ostream_iterator & operator++(int) { return *this; }

private:
    std::basic_ostream<TChar, TTraits> * m_os;
    const TChar * m_delimiter;
    bool m_isFirstElement;
};

using char_infix_ostream_iterator = infix_ostream_iterator<>;

template<typename TContainer>
void printCommaSeparatedLine(const TContainer & c)
{
    std::copy(c.begin(), c.end(), char_infix_ostream_iterator(std::cout, ", "));
    std::cout << std::endl;
}

#endif //! defined(CSAR_IAB_INFIX_OSTREAM_ITERATOR)