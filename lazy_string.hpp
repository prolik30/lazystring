#ifndef LAZY_STRING_HPP_124325432
#define LAZY_STRING_HPP_124325432

#include <cstddef>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <algorithm>
#include <string>

namespace std_utils
{
	using boost::shared_array;
	using boost::shared_ptr;
	using boost::make_shared;
	using std::char_traits;

	template <class charT, class traits = char_traits<charT> >
	class lazy_basic_string
	{
	private:
		class proxy;

	public:
		typedef traits  			traits_type;
		typedef charT 				value_type;
		typedef charT& 				reference;
		typedef const charT& 		const_reference;
		typedef charT* 				pointer;
		typedef const charT* 		const_pointer;
		typedef ptrdiff_t 			difference_type;
		typedef size_t 				size_type;

		explicit lazy_basic_string() 
		{
			shared_array <value_type> temp(new value_type[1]);
			traits::assign(temp[0], value_type());
			size_ = 0;
			pointer_ = shared_array<value_type>(temp);
		}

		lazy_basic_string(value_type const str[])
		{
			if(str != 0)
			{
				size_type size_temp = traits::length(str);
				shared_array <value_type> temp(new value_type[size_temp + 1]);
				traits::copy(temp.get(), str, size_temp);
				traits::assign(temp[size_temp], value_type());
				size_ = size_temp;
				pointer_ = shared_array<value_type>(temp);
			}	
		}

		explicit lazy_basic_string(size_type const amount, value_type symbol) 
		{
			shared_array <value_type> temp(new value_type[amount+1]);
			traits::assign(temp.get(), amount, symbol);
			traits::assign(temp[amount], value_type());
			size_ = amount;
			pointer_ = shared_array<charT>(temp);
		}

		lazy_basic_string(lazy_basic_string const &another_lbs) : pointer_(another_lbs.pointer_), size_(another_lbs.size_)
		{}

		~lazy_basic_string() 
		{}

		void swap(lazy_basic_string<value_type, traits_type> &another_lbs)
		{
			std::swap(size_, another_lbs.size_);
			pointer_.swap(another_lbs.pointer_);
		}

		lazy_basic_string<value_type, traits_type>& operator=(const lazy_basic_string<value_type, traits_type> &another_lbs)
		{
			lazy_basic_string<value_type, traits_type>(another_lbs).swap(*this);
			return *this;
		}

		lazy_basic_string<value_type, traits_type>& operator=(const charT *str)
		{
			lazy_basic_string<value_type, traits_type>(str).swap(*this);
			return *this;
		}

		lazy_basic_string<value_type, traits_type>& operator=(const charT symbol)
		{
			lazy_basic_string<value_type, traits_type>(1, symbol).swap(*this);
			return *this;
		}


		void clear()
		{
			pointer_.reset();
			size_ = 0;
		}

		size_type size() const
		{ return size_; }

		bool empty() const
		{ return (size_ == 0); }

		const value_type* c_str() const
		{ return pointer_.get(); }

		lazy_basic_string<value_type, traits_type> & operator+=(lazy_basic_string<value_type, traits_type> const &another_lbs)
		{
			shared_array <value_type> temp(new value_type[size_ + another_lbs.size() + 1]);
			for(size_type i = 0; i < size_; ++i)
				traits::assign(temp[i], pointer_[i]);

			for(value_type i = 0; i < (value_type)another_lbs.size(); ++i)
				traits::assign(temp[i + size_], another_lbs.pointer_[i]); 			

			traits::assign(temp[size_ + another_lbs.size()], value_type());

			pointer_ = shared_array<value_type>(temp);
			size_ = size_ + another_lbs.size();
			return *this;
		}	

		lazy_basic_string<value_type, traits_type> &operator+=(value_type const str[])
		{
			*this += lazy_basic_string<value_type, traits_type>(str);
			return *this;
		}

		lazy_basic_string<value_type, traits_type> & operator+=(const value_type symbol)
		{
			*this += lazy_basic_string<value_type, traits_type>(1, symbol);
			return *this;

		}		

		lazy_basic_string<value_type, traits_type> operator+(value_type const symbol) const
		{
			lazy_basic_string<value_type, traits_type> temp(*this);	
			temp += symbol;
			return temp;
		}

		lazy_basic_string<value_type, traits_type> operator+(value_type const str[]) const
		{
			lazy_basic_string<value_type, traits_type> temp(*this);	
			temp += str;
			return temp;
		}

		lazy_basic_string<value_type, traits_type> operator+(lazy_basic_string<value_type, traits_type> const &another_lbs) const
		{
			lazy_basic_string<value_type, traits_type> temp(*this);	
			temp += another_lbs;
			return temp;	
		}

		bool operator==(lazy_basic_string<value_type, traits_type> const &another_lbs) const
		{
			return (size_ == another_lbs.size_ && traits::compare(pointer_.get(), another_lbs.pointer_.get(), size_) == 0);
		}

		bool operator==(value_type const str[]) const
		{
			return (*this == lazy_basic_string<value_type, traits_type>(str));
		}

		bool operator!=(lazy_basic_string<value_type, traits_type> const &another_lbs)
		{
			return !(*this == another_lbs);
		}

		bool operator!=(value_type const str[])
		{
			return !(*this == str);
		}

		bool operator<(lazy_basic_string<value_type, traits_type> const &another_lbs) const
		{
			return traits::compare(pointer_.get(), another_lbs.pointer_.get(), (size_ < another_lbs.size()) ? size_ : another_lbs.size()) < 0;
		}

		bool operator<(value_type const str[])
		{
			return *this < lazy_basic_string<value_type, traits>(str);
		}

		bool operator>(lazy_basic_string<value_type, traits_type> const &another_lbs) const
		{
			return !(*this < another_lbs) && *this != another_lbs;
		}

		bool operator>(value_type const str[])
		{
			return !(*this < str) && *this != str;
		}

		bool operator<=(lazy_basic_string<value_type, traits_type> const &another_lbs) const
		{
			return !(*this > another_lbs);
		}

		bool operator<=(value_type const str[])
		{
			return !(*this > str);
		}

		bool operator>=(lazy_basic_string<value_type, traits_type> const &another_lbs) const
		{
			return !(*this < another_lbs);
		}

		bool operator>=(value_type const str[])
		{
			return !(*this < str);
		}

		value_type const &operator[](size_type index) const
		{ return pointer_[index]; }

		proxy operator[](size_t index)
		{ return proxy(this, index); }

	private:
		shared_array <value_type> pointer_;
		size_type size_;

		value_type* sys_c_str() const
		{ return pointer_.get(); }

		bool unique() const
		{ return pointer_.unique(); }

		class proxy 
		{
		public:
			proxy(lazy_basic_string<charT, traits> *str, size_t pos) : lbstr_(str), pos_(pos)
			{}

			operator value_type() 
			{ return (lbstr_->c_str())[pos_]; }

			operator value_type&() const
			{ 
				if(!lbstr_->unique())
					*lbstr_ = lazy_basic_string<value_type, traits>(lbstr_->sys_c_str());

				return (lbstr_->sys_c_str())[pos_]; 
			}

			proxy(const proxy &another) : lbstr_(another.lbstr_), pos_(another.pos_)
			{}

			proxy& operator=(const charT &symbol)
			{
				if(!lbstr_->unique())
					*lbstr_ = lazy_basic_string<value_type, traits>(lbstr_->sys_c_str());

				lbstr_->sys_c_str()[pos_] = symbol;
				return *this; 
			}

			proxy& operator=(const proxy &another)
			{
				charT temp = another;
				*this = temp;
				return *this;
			}

		private:
			lazy_basic_string<charT, traits> *lbstr_;
			size_t pos_;
		};
	};

	template <class charT, class traits> 
	bool operator==(charT const str[], lazy_basic_string<charT, traits> const &lbs)
	{ return lbs == str; }

	template <class charT, class traits> 
	bool operator!=(charT const str[], lazy_basic_string<charT, traits> &lbs)
	{ return lbs != str; }

	template <class charT, class traits> 
	bool operator<=(charT const str[], lazy_basic_string<charT, traits> &lbs)
	{ return lbs >= str; }

	template <class charT, class traits> 
	bool operator>=(charT const str[], lazy_basic_string<charT, traits> &lbs)
	{ return lbs <= str; }

	template <class charT, class traits> 
	bool operator<(charT const str[], lazy_basic_string<charT, traits> &lbs)
	{ return lbs > str; }

	template <class charT, class traits> 
	bool operator>(charT const str[], lazy_basic_string<charT, traits> &lbs)
	{ return lbs < str; }

	template <class charT, class traits> 
	lazy_basic_string<charT, traits> operator+(charT const str[], lazy_basic_string<charT, traits> const &lbs) 
	{ 
		lazy_basic_string<charT, traits> temp(str);	
		temp += lbs;
		return temp; 
	}

	template <class charT, class traits> 
	lazy_basic_string<charT, traits> operator+(charT const symbol, lazy_basic_string<charT, traits> const &lbs) 
	{ 
		lazy_basic_string<charT, traits> temp(1, symbol);	
		temp += lbs;
		return temp; 
	}
	
	template <class charT, class traits>
	void swap(lazy_basic_string<charT, traits> &first_lbs, lazy_basic_string<charT, traits> &second_lbs)
	{ first_lbs.swap(second_lbs); } 

	typedef lazy_basic_string<char> 	lazy_string;
	typedef lazy_basic_string<wchar_t>  lazy_wstring; 

	namespace details
    {
        struct ci_char_traits : public std::char_traits<char>
        {
            static bool eq(char c1, char c2)
            { return std::toupper(c1) == std::toupper(c2); }  
            
            static bool lt(char c1, char c2)
            { return std::toupper(c1) < std::toupper(c2); }
            
            static int compare(const char* s1, const char* s2, size_t n) 
            {
                while(n-- != 0) 
                {
                    if(std::toupper(*s1) < std::toupper(*s2)) 
                    	return -1;
                    if(std::toupper(*s1) > std::toupper(*s2)) 
                    	return 1;
                    ++s1; ++s2;
                }
            return 0;
            }

            static const char* find(const char* s, int n, char a) 
            {
                const char ua = std::toupper(a);
                while(n-- != 0) 
                {
                    if(std::toupper(*s) == ua)
                        return s;
                    s++;
                }
                return 0;
            }
        };    
	}

	typedef lazy_basic_string<char, details::ci_char_traits> lazy_istring;
	
}

#endif