#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>

int main(int argc, char* argv[])
{
	std::unordered_map<char,int> digits
	{
		{'2',2},
		{'1',1},
		{'0',0},
		{'-',-1},
		{'=',-2}
	};
	
	std::unordered_map<int,char> inv_digits
	{
		{2,'2'},
		{1,'1'},
		{0,'0'},
		{-1,'-'},
		{-2,'='}
	};
	
	const auto from_snafu = [&](const auto& s)
	{
		long long n=0, base = 5, current = 1;
		for(int i=s.size()-1;i>=0;--i)
		{
			const auto c = s[i];
			n+=digits[c]*current;
			current*=base;
		}
		return n;
	};
	
	const auto to_snafu = [&](long long n)
	{
		std::string result;
		int carry = 0;
		
		do
		{
			auto digit = n%5 + carry;
			carry = 0;
			if(digit>2)
			{
				digit-=5;
				carry = 1;
			}
			result = inv_digits[digit] + result;
		}
		while(n/=5);
		
		return carry!=0?'1'+result:result;
	};
	
	const auto sum = std::transform_reduce(std::istream_iterator<std::string>{std::cin},{},0ll,std::plus<>{},from_snafu);
	std::cout<<to_snafu(sum);
	
	return 0;
}
