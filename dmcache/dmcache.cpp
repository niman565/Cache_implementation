#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// class for the direct cache that includes two functions read/write
class dmcache{
	
	public:
		bool dirty;
		int tag;
		char data[8];

	//initializing everything to 0
	dmcache() {
		dirty = false;
		tag = 0;
		for (int i = 0; i < 8; i++) {
			data[i] = 0;
		}
	}
};

//read - returns the string represeantion of the nibble of data, hit, dirty bits
void read(dmcache dm[32], int add, int index, bool hit, bool d) {
	

}
//write - updates the data and updates dirty to be correct
void write(dmcache dm[32], int add, int index, int data) {
	dm[index].data[add&7] = data;
	dm[index].dirty = true;
}

//Main simulates a direct-mapped cache
int main(int argc, char** argv) {
	int lines = 8;
	const int spots = 32;	//cache lines = cache size/line bytes
	int address, tag, data, func, index, place;
	bool dirty, hit;
	dmcache dm[spots];
	int memory[65536];
	ifstream input(argv[1]);
	std::ofstream output;
	output.open("dm-output.txt");

	//gets the next line of stuff from the input file not sure about c++
	while (input >>hex>> address >> func >> data) {
		//cout<<hex<<address<<func<<data<<endl;		
		index = (address / lines) % spots;
		tag = address / spots;
		dirty = dm[index].dirty;

		//find hit or miss
		if ( (dm[index].tag == tag) ) { //if it finds a match then its a hit
			hit = true;
		}
		else {
			hit = false;
		}

		//if hit = false / the line is not in the cache
		if (!hit) {
			//if its dirty need to write cache to memory
			if (dm[index].dirty) {
				for (int i = 0; i < lines; i++) {
					//put data from cache into memory then continue on
					//place is a unique address in memory for every line
					place = dm[index].tag*lines;
					(&memory[place])[i] = dm[index].data[i];
				}
			}
			//if the cache is not dirty
			for (int i = 0; i < lines; i++) {
				//put the data from memory directly into cache
				place = tag*lines;
				dm[index].data[i] = (&memory[place])[i];
			}
			//reset things
			dm[index].dirty = false;
			dm[index].tag = tag;

		} //if(!hit)

		//if hit is true, the data is already in the cache
		if (func == 0) { //func stands for 0x00/read or OxFF/write
			read(dm,address,index,hit,dirty);
			output << std::hex<<std::uppercase << setfill('0')<<setw(2)<<(int) (dm[index].data			[address&7]&0xFF)<<' '<<hit<<' '<<dirty<<"\n";
			
		}
		else {
			write(dm, address, index, data);
		}
	}
	
	input.close();
	return 0;
	
}
