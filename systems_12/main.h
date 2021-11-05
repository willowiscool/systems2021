struct pop_entry {
	int year;
	int population;
	char boro[15];
};
int readCSV();
//struct pop_entry[] readData();
void displayData(int count, struct pop_entry data[]);

struct pop_entry getInput();
void addData();
void updateData();
