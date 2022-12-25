#include <advent.hpp>

#include <array>
#include <iostream>
#include <unordered_set>

#include <cstddef>

using namespace advent;

int main(int argc, char* argv[])
{
	std::unordered_set<point2d> visited;
	std::array<point2d,2> snake{};
	
	char d;
	int n;
	while(std::cin>>d>>n)
	{
		while(n--)
		{
			switch(d)
			{
				case 'U': snake[0].y-=1; break;
				case 'D': snake[0].y+=1; break;
				case 'L': snake[0].x-=1; break;
				case 'R': snake[0].x+=1; break;
			}
			
			for(std::size_t i=1;i<snake.size();++i)
			{
				if(std::abs(snake[i-1].x-snake[i].x)>1 || std::abs(snake[i-1].y-snake[i].y)>1)
				{
					if(snake[i].x<snake[i-1].x) ++snake[i].x;
					else if(snake[i].x>snake[i-1].x) --snake[i].x;
					if(snake[i].y<snake[i-1].y) ++snake[i].y;
					else if(snake[i].y>snake[i-1].y) --snake[i].y;
				}
			}
			visited.insert(snake.back());
		}
	}
	
	std::cout<<visited.size();
	return 0;
}
