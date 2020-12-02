#include <iostream>

#include "Drone.h"
#include "multicall.h"

/*
 *  . ............. ... ..... .............
 . . .         .   . . . .         .   .
 ... ....... . ... ... . ... ..... . ...
 .         . .   . .   .   . .   . . .
 ..... ... . ..... . ... ..... . . . ...
       . . . .       .         . . .   .
 ....... ... ... ... . ... ....... . ...
 .             . . . . . .   .       . .
 x<< v<<<x ... ... . ... ... ......... .
 v ^ v   ^ .       .   .   . .         .
 v ^ >>v ^ ........... . ... . v<< .....
 v ^   v ^ .         .   .     v ^ .
 v ^<<<x ^ ... v<< ..... . v<x<v ^ .....
 v       ^     v ^     . . v     ^     .
 v >>v . ^<<<x<v ^<x ..... v.... x .....
 v ^ v .           ^       v     ^
 v ^ x >>>>x>>>v . ^ v<x<<<< >>v ^ v<x<<
 v ^ v ^       v . ^ v       ^ v ^ v   ^
 x>^ >>^ . x<<<v . ^<< >>>>>>x v ^<<.. ^
         . v     .     ^       v     . ^
 ..... v<<<< >>>>v . o>^ ..... >>x>v . ^
 .   . v     ^   v .     . .       v   ^
 . ... >>x>v x . >>x>>>v . ..... v<< x>^
 .         v ^ . .     v       . v   ^ .
 . x<< v<x v ^ ... v<< >>v >>>>v v ..^ .
 . v ^ v ^ v ^     v ^   v ^   v v   ^
 v<< ^<< ^<< ^<<<<<x ^ ..x x . v x>v ^<<
 v             .     ^   v ^ . v   v   ^
 v ....>>>>>>x . ... ^<x v ^ ..x>v v x>^
 v     ^     v . . .   ^ v ^     v v ^
 >>x x>^ ... v ... . . ^ >>^ v<< v >>^ .
   v ^   . . v     . . ^ .   v ^ v     .
 . v ^ ... . v ....... ^ . v<< x >>v x<<
 . v ^ .     v       . ^   v   ^   v v ^
 . >>^ x<<<< >>v >>v . ^<<<x >>^ v<x v ^
 .     v   ^   v ^ v         ^   v   v ^
 ... v<< . ^<<<x x v >>>>v >>^ . >>v O ^
 .   v   .       ^ v ^   v ^   .   v   ^
 ....>>>>x>>>>>>>^ >>x ..>>x ......>>x>^
 */


void solve(const std::string& filename)
{
	const auto graph = Drone::buildGraph(filename);

	std::cout << "A: " << distStartToOxygen(graph) << std::endl;
	std::cout << "B: " << oxygenDistributionTime(graph) << std::endl;
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
