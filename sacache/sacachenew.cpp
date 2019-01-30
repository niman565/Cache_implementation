#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// class for the direct cache that includes two functions read/write
class sacache{
	
	public:
		bool dirty;
		int tag, lru;
		char data[8];

	//initializing everything to 0
	sacache() {
		dirty = false;
		tag = 0;
		lru = 0;
		for (int i = 0; i < 8; i++) {
			data[i] = 0;
		}
	}
};

//read - returns the string represeantion of the nibble of data, hit, dirty bits
void read(sacache sa[8][8], int add, int index, bool hit, bool d) {
	

}
//write - updates the data and updates dirty to be correct
void write(sacache sa[8][8], int add, int index, int data, int t) {
	
	sa[index][t].data[add>>2] = data;
	sa[index][t].dirty = true;
	
}

//Main simulates a direct-mapped cache
int main(int argc, char** argv) {
	int lines = 4;
	const int spots = 64;	//cache lines = cache size/line bytes
	const int set = 8;
	int address, tag, data, func, index, place, lru, temp, big;
	bool dirty, hit;
	sacache sa[spots][set];

	int memory[65536];
	ifstream input(argv[1]);
	std::ofstream output;
	output.open("sa-output.txt");

	//gets the next line of stuff from the input file not sure about c++
	while (input >>hex>> address >> func >> data) {

		index = (address / lines) % spots;
		tag = address / spots;
		
		//increment lru for all parts of current block/set unless its a direct match
		for(int i = 0; i<set; i++){
			sa[index][i].lru++;
			//if tag is found set to 0 and hit to be true
			if(sa[index][i].tag == tag){
				sa[index][i].lru = 0;
				//hit = true;
			}
		}

		hit = true;
		if(temp == set)
				hit= false;
		
		dirty = sa[index][temp].dirty;
		//temp = biggest lru/the one we want to get rid of
		if(!hit){
			temp = 0;
			big = sa[index][0].lru;
			for(int j = 0; j < set; j++){
				if(sa[index][j].lru > big){
					temp = j;
					big = sa[index][j].lru;
				}
			}
			
			dirty = sa[index][temp].dirty;

			//if it is dirty
			if (sa[index][temp].dirty) {
				for (int i = 0; i < lines; i++) {
					//put data from cache into memory then continue on
					//place is a unique address in memory for every line
					place = sa[index][temp].tag*lines;
					(&memory[place])[i] = sa[index][temp].data[i];
				}
			}

			for(int i = 0; i< lines; i++){
				place = tag*lines;
				sa[index][temp].data[i] = (&memory[place])[i];
			}

			//reset things
			sa[index][temp].dirty = false;
			sa[index][temp].tag = tag;
			//hit = true;
			//dirty = true;
		} //if(!hit)

		//if hit is true, the data is already in the cache
		if (func == 0) { //func stands for 0x00/read or OxFF/write
			read(sa,address,index,hit,dirty);
			output << std::hex<<std::uppercase << setfill('0')<<setw(2)<<(int) (sa[index][temp].data[address>>2]&0xFF)<<' '<<hit<<' '<<dirty<<"\n";
		}
		else {
			write(sa, address, index, data, temp);
			
		}
	}
	
	input.close();
	return 0;
	
}
