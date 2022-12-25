#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char* argv[])
{
	struct indexed_number
	{
		long n;
		long idx;
	};
	std::vector<indexed_number> indexed_numbers;
	
	std::transform(std::istream_iterator<long>{std::cin},{},std::back_inserter(indexed_numbers),[i=0](auto value) mutable
	{
		return indexed_number{value,i++};
	});
		
	const long size = indexed_numbers.size();
	
	for(long j=0;j<size;++j)
	{
		long i=0;
		for(;i<size;++i)
		{
			if(indexed_numbers[i].idx==j) break;
		}
		
		const auto n = std::abs(indexed_numbers[i].n)%(size-1);
		const auto direction = indexed_numbers[i].n>0?1:-1;
		for(long j=0;j<n;++j)
		{
			auto prev = i+direction*j;
			auto target  = i+direction*(j+1);
			if(prev<0) prev+=size;
			if(target<0) target+=size;
			std::swap(indexed_numbers[prev%size],indexed_numbers[target%size]);
		}	
	}
	
	long s = 0;
	for(;s<size;++s)
	{
		if(indexed_numbers[s].n==0)
			break;
	}
	
	long sum = 0;
	for(long i=0;i<3;++i)
	{
		s+=1000;
		s%=indexed_numbers.size();
		sum+=indexed_numbers[s].n;
	}
	std::cout<<sum;
}
