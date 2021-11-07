struct pop_entry {
	int year;
	int population;
	char boro[15];
};
int readCSV();
//int readData(struct pop_entry *data);
void displayData(int count, struct pop_entry data[]);

struct pop_entry getInput();
int addData();
int updateData();
